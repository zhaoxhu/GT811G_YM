#include "gwd_eth_loop_detect.h"
#if ( RPU_MODULE_LOG_SUPPORT == MODULE_YES )
#include "gw_log.h"
#endif
#include "gwd_eth_loop_detect_realize.h"
#include "app_alarm_ctc.h"
//#define TEST
#if 1
#define ALARM_INTERVAL		(3)
#define	CLEAR_INTERVAL		(10)
#define LOOP_AGING_TIME		(3)
/*add by zhaoxh ����·���ƴ򿪣�������·ʱdown �� wake up �˿ڣ������澯*/
int flag_port_up = 0;
typedef struct addr
{
	unsigned char da[GWD_MACADDR_LEN];
	unsigned char sa[GWD_MACADDR_LEN];
}eth_head_t;

typedef struct
{
	gw_uint32 tpid			:16;
	gw_uint32 vlan_id_h		:4;
	gw_uint32 Priority		:3;
	gw_uint32 CFI			:1;
	gw_uint32 vlan_id_l		:4;
	gw_uint32 vlan_id_m		:4;
}tag_t;


typedef struct {
    gw_uint16 tpid;
    gw_uint16 tag;
} __attribute__((packed)) gwd_vlan_hdr_t;

typedef struct
{
	gw_uint8	lpbMask;	//�˿��Ƿ����˻�·, 1:��·,0:����
	gw_uint8	lpbDown;	//�˿��Ƿ���Ϊ��·�ر�,1:�ر�,0:����
	gw_uint16	slpCnt;		//�˿ڻ�·�رպ��������
	gw_uint16	wakeCnt;	//�˿ڻ�·�رպ��Զ��������Ĵ���
	gw_uint16	clearCnt;	//�˿��Զ���������û�в鵽��·�Ĵ���
	gw_uint16	alarm_count;//�˿ڸ澯�������
	gw_uint8	lpb_down_forever;	//�˿���·����down ��
	gw_uint8	alarm_flag;	//�澯��־����Ҫ�澯��1������Ҫ�澯��0
	gw_uint8	wake_flag;	//���ѱ�־����Ҫ������1������Ҫ������0
}__attribute__((packed))loop_back_status_t;

typedef struct
{
	gw_uint8				valid_flag;
	loop_back_status_t		loop_back_status;
	loop_back_port_info_t	port_infor;
	gw_uint8				extern_flag;		//��·��Ϣ��־����־�Ƿ������չ�Ļ�·��Ϣ��0��������չ��Ϣ��1��������չ��Ϣ
	loop_back_port_info_t	extern_port_infor;	//��չ�˿���Ϣ(�Զ˶˿�)
	gw_uint16				aging_count;		//�ϻ�ʱ�����
}__attribute__((packed))loop_back_ports_infor_t;

typedef struct	loop_back_infor_s
{
	gw_node node;
	gw_uint16	vlan_id;
	gw_uint8	ports_num;
	loop_back_ports_infor_t	ports_infor[GWD_UNI_PORT_NUM];
}__attribute__((packed))loop_back_infor_t;


#endif

//#define TEST

#if 1

#ifndef ENABLE
#define ENABLE	1
#endif

#ifndef DISABLE
#define DISABLE	0
#endif


extern void mib_uni_loop_back_alarm_raise(cs_uint8 brd,cs_uint8 port);
extern void mib_uni_loop_back_alarm_clear(cs_uint8 brd,cs_uint8 port);



#define TPID	(0x8100)

static gw_list	g_loop_back_infor_list;

//�ڲ�ʵ��

gwd_status
loop_back_control_pkt_proc(
	GWD_IN	gw_uint8	*pkt,
	GWD_IN	gw_uint16	len
	);

gwd_status
loop_back_control_pkt_parse(
	GWD_IN	gw_uint8				*pkt,
	GWD_IN	gw_uint16				len,
	GWD_OUT	loop_control_config_t	*config
	);

gwd_status
loop_back_detect_config_set(
	GWD_IN	loop_control_config_t	*config
	);

gwd_status
loop_back_detect_config_cmp_cur(
	GWD_IN	loop_control_config_t	*config
	);

gwd_status
loop_back_detect_config_get(
	GWD_OUT	loop_control_config_t *config
	);

gwd_status
loop_back_detect_pkt_proc_enable(
	GWD_IN	int enable
	);

gw_uint8
gwd_loop_back_detect_pkt_handler(
	GWD_IN	gw_uint8		*pkt,
	GWD_IN	gw_uint16		pkt_len,
	GWD_IN	gwd_port_id_t	port_id
	);

gwd_status
gwd_loop_back_detect_pkt_parser(
	GWD_IN	gw_uint8		*pkt,
	GWD_IN	gw_uint16		pkt_len,
	GWD_IN	gwd_port_id_t	port_id
	);

gwd_status
gwd_loop_back_detect_pkt_get_port_infor(
	GWD_IN	gw_uint8					*pkt,
	GWD_IN	gw_uint16					pkt_len,
	GWD_IN	gwd_port_id_t				port_id,
	GWD_OUT	gw_uint16					*vlan_id,
	GWD_OUT	loop_back_port_info_t 		*port_infor,
	GWD_OUT	loop_back_port_info_t 		*extern_port_infor
	);

gwd_status
loop_back_detect_pkt_into_cpu_enable(
	GWD_IN	int enable
	);

gwd_status
ifm_eth_set_slot(
	GWD_IN_OUT	gw_uint32 *ulIfIndex,
	GWD_IN		gw_uint32 slot
	);

gw_uint32
ifm_eth_get_slot(
	GWD_IN	gw_uint32 ulIfIndex
	);

gwd_status
ifm_eth_set_port(
	GWD_IN_OUT	gw_uint32 *ulIfIndex,
	GWD_IN		gw_uint32 port
	);

gw_uint32
ifm_eth_get_port(
	GWD_IN	gw_uint32 ulIfIndex
	);

gwd_status	loop_list_node_ports_infor_update(
	GWD_IN	loop_back_ports_infor_t	*ports_infor,
	GWD_IN	gw_uint16				aging_time,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor
	);

gwd_status	loop_list_node_ports_infor_add(
	GWD_IN	loop_back_ports_infor_t	*ports_infor,
	GWD_IN	gw_uint16				aging_time,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor
	);

void 
loop_outside_port_admin_oper_proc(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_ports_infor_t		*ports_infor
	);

void 
loop_port_admin_control(
	GWD_IN	loop_control_config_t	*config,
	GWD_IN	loop_back_ports_infor_t	*ports_infor
	);

void 
loop_port_wake_up(
	GWD_IN	loop_control_config_t	*config,
	GWD_IN	loop_back_ports_infor_t	*ports_infor
);

void 
loop_port_alarm_periodically(
	GWD_IN	loop_control_config_t	*config,
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_ports_infor_t	*ports_infor
);

void 
loop_port_status_clear_proc(
	GWD_IN	loop_control_config_t	*config,
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_ports_infor_t	*ports_infor	
);

gwd_status
loop_back_status_list_add_node(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor
	);

gwd_status
loop_back_status_list_port_control(
	GWD_IN	loop_control_config_t	*config
	);

gwd_status
loop_back_status_list_update(
	void
	);

gwd_status
loop_back_status_list_reset_port(
	void
	);

gwd_status
loop_back_status_list_clear(
	void
	);

void
loop_back_status_list_show(
	void
	);

gwd_status loop_back_status_list_led_proc(
	void
	);

gwd_status
loop_back_detect_task_eable(
	GWD_IN	int enable
	);

void
loop_detect_task(
	GWD_IN	void *data
	);

gwd_status
loop_back_detect_port_status_proc_enable(
	GWD_IN	int enable
);

gwd_status
loop_back_test_port(
	GWD_IN	gw_uint16			vlan_detect,
	GWD_IN	gwd_port_id_t		port_id,
	GWD_IN	gwd_sdl_vlan_mode_t vlan_mode,
	GWD_IN	gw_uint16			vlan_def,
	GWD_IN	gw_uint16			*vlan_cfg,
	GWD_IN	gw_uint16			vlan_num
	);

gwd_status
loop_back_test_pkt_send(
	GWD_IN	loop_control_config_t	*config
	);

gwd_status
loop_back_mac_check_proc(
	GWD_IN		loop_control_config_t	*config
	);

gwd_status
loop_back_detect_test_vlan_port(
	GWD_IN	gwd_port_id_t	port_id,
	GWD_IN	gw_uint16		vlan_id
	);

gwd_status
loop_back_detect_pkt_build(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_port_info_t 	*port_infor,
	GWD_OUT	gwd_loop_detect_frame_t	*loop_detect_frame
	);

gwd_status
gwd_loop_back_detect_frame_dest_mac_get(
	GWD_OUT	gwd_mac_t *mac
	);

gwd_status
gwd_loop_back_detect_frame_src_mac_get(
	GWD_OUT	gwd_mac_t *mac
	);

gwd_status
loop_back_port_vlan_proc(
	GWD_IN		gwd_port_id_t	port_id,
	GWD_IN		gw_uint16		vlan_id
	);

gwd_status
eth_pkt_tag_num_get(
	GWD_IN		gw_uint8	*eth_pkt,
	GWD_IN		gw_uint16	len,
	GWD_OUT		gw_uint16	*tag_num
	);

gwd_status
loop_back_occur_proc(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor
);

gwd_status
loop_back_cancel_proc(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor
);

gwd_status gwd_port_vlan_check(
	GWD_IN	gwd_port_id_t	port_id,
	GWD_IN	gw_uint16		vlan
	);

//��ȡonu�˿���Ϣ
gwd_status
onu_port_infor_get(
	GWD_IN	gwd_port_id_t 			port_id,
	GWD_OUT	loop_back_port_info_t	*port_infor
	);

gwd_status
loop_back_port_control(
	GWD_IN	gwd_port_id_t	port_id
	);
#endif

extern int gwd_loopdetect_default_config(void);

#if 1
/*�ڲ�ʵ��*/
gwd_status
loop_back_control_pkt_proc(
	GWD_IN	gw_uint8	*pkt,
	GWD_IN	gw_uint16	len
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;

	loop_control_config_t loop_control_config;
	//��ڹ�����
	if(NULL == pkt)
	{
		return GWD_E_ERROR;
	}
	if(len <= 0)
	{
		return GWD_E_ERROR;
	}

	//��·����oam���Ľ���
	loop_back_control_pkt_parse(pkt, len, &loop_control_config);

	//����onu��·���ģ�鹦��
	loop_back_detect_config_enable(&loop_control_config);

	return ret;
}



//��·oam���Ľ���
gwd_status
loop_back_control_pkt_parse(
	GWD_IN	gw_uint8				*pkt,
	GWD_IN	gw_uint16				len,
	GWD_OUT	loop_control_config_t	*config
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;

	OAM_ONU_LPB_DETECT_FRAME *loop_back_oam_frame = NULL;

	//��ڹ�����
	if(NULL == pkt)
	{
		return GWD_E_ERROR;
	}
	if(len <= 0)
	{
		return GWD_E_ERROR;
	}
	if(NULL == config)
	{
		return GWD_E_ERROR;
	}

	loop_back_oam_frame = (OAM_ONU_LPB_DETECT_FRAME *)pkt;

	config->enable = loop_back_oam_frame->enable;
	config->vid = ntohs(loop_back_oam_frame->vid);
	config->interval = ntohs(loop_back_oam_frame->interval);
	config->policy = ntohs(loop_back_oam_frame->policy);
	memcpy(config->smac, loop_back_oam_frame->smac, GWD_MACADDR_LEN);
	if(LPB_OLD_VER_LEN == len)
	{
		config->waitforwakeup = 3;
		config->maxwakeup = 3;
	}
	else
	{
		config->waitforwakeup = ntohs(loop_back_oam_frame->waitforwakeup);
		config->maxwakeup = ntohs(loop_back_oam_frame->maxwakeup);
	}
	config->alarm_interval = ALARM_INTERVAL;
	config->clear_interval = CLEAR_INTERVAL;
	config->opposite_aging_time = LOOP_AGING_TIME;

	#ifdef TEST
	gwd_printf("config->enable :%u\n", config->enable);
	gwd_printf("config->vid :%u\n", config->vid);
	gwd_printf("config->interval :%u\n", config->interval);
	gwd_printf("config->policy :%u\n", config->policy);
	gwd_printf("config->waitforwakeup :%u\n", config->waitforwakeup);
	gwd_printf("config->maxwakeup :%u\n", config->maxwakeup);

	gwd_printf("config->smac : %02x:%02x:%02x:%02x:%02x:%02x\n",	config->smac[0],
																config->smac[1],
																config->smac[2],
																config->smac[3],
																config->smac[4],
																config->smac[5]);
	#endif

	return ret;
}


//���û�·����Ƿ�ʹ��
gwd_status
loop_back_detect_config_enable(
	GWD_IN	loop_control_config_t *config
	)
{

	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	int enable_set = DISABLE;

	if(config->enable!= 0)
	{
		enable_set = ENABLE;
	}
	else
	{
		enable_set = DISABLE;
	}

	//���÷����仯���ȹرջ�·���ܣ������´���
	if(GWD_E_OK != loop_back_detect_config_cmp_cur(config))
	{
		//Olt��·������Ϣ����
		loop_back_detect_config_set(config);
		
		//ע��/ ж�ػ�·��⴦����
		loop_back_detect_pkt_proc_enable(DISABLE);

		//��/ ������·��ⱨ�Ľ���cpu
		loop_back_detect_pkt_into_cpu_enable(DISABLE);

		//����/ �رջ�·��������߳�
		loop_back_detect_task_eable(DISABLE);

		//��·���˿�״̬����
		loop_back_detect_port_status_proc_enable(DISABLE);

		if(ENABLE == enable_set)
		{
			//ע��/ ж�ػ�·��⴦����
			loop_back_detect_pkt_proc_enable(enable_set);

			//��/ ������·��ⱨ�Ľ���cpu
			loop_back_detect_pkt_into_cpu_enable(enable_set);

			//����/ �رջ�·��������߳�
			loop_back_detect_task_eable(enable_set);

			//��·���˿�״̬����
			loop_back_detect_port_status_proc_enable(enable_set);
		}	
	}
	else
	{
		//����δ�䣬��������
	}

	return GWD_E_OK;
}



//Olt��·������Ϣ����
static loop_control_config_t g_loop_control_config;
gwd_status
loop_back_detect_config_set(
	GWD_IN	loop_control_config_t	*config
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;
	//��ڹ�����
	if(NULL == config)
	{
		return GWD_E_ERROR;
	}

	memcpy(&g_loop_control_config, config, sizeof(loop_control_config_t));

	return ret;
}

gwd_status
loop_back_detect_config_cmp_cur(
	GWD_IN	loop_control_config_t	*config
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;
	//��ڹ�����
	if(NULL == config)
	{
		return GWD_E_ERROR;
	}

	if(0 == memcmp(&g_loop_control_config, config, sizeof(loop_control_config_t)))
	{
		ret = GWD_E_OK;
	}
	else
	{
		ret = GWD_E_ERROR;
	}

	return ret;
}


//Olt��·������Ϣ��ȡ
gwd_status
loop_back_detect_config_get(
	GWD_OUT	loop_control_config_t *config
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;
	//��ڹ�����
	if(NULL == config)
	{
		return GWD_E_ERROR;
	}

	memcpy(config, &g_loop_control_config, sizeof(loop_control_config_t));

	return ret;
}


//ע��/ ж�ػ�·��⴦����
gwd_status
loop_back_detect_pkt_proc_enable(
	GWD_IN	int enable
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	int enable_set = DISABLE;
	static int enable_current = DISABLE;

	//״̬���
	if(enable != 0)
	{
		enable_set = ENABLE;
	}
	else
	{
		enable_set = DISABLE;
	}
	if(enable_set != enable_current)
	{
		//������״̬
		enable_current = enable_set;

		if(ENABLE == enable_set)
		{
			//ע�ỷ·��⴦����
			if(GWD_E_OK != gwd_loop_detect_pkt_handler_reg(gwd_loop_back_detect_pkt_handler))
			{
				gwd_printf("gwd_loop_detect_pkt_handler_reg failed!\n");
			}
			
			if(GWD_E_OK != gwd_loop_detect_pkt_parser_reg(gwd_loop_back_detect_pkt_parser))
			{
				gwd_printf("gwd_loop_detect_pkt_parser_reg failed!\n");
			}
		}
		else
		{
		    //todo:ͬ��
			// ж�ػ�·��⴦����
			gwd_loop_detect_pkt_handler_reg(NULL);
			gwd_loop_detect_pkt_parser_reg(NULL);
		}
	}

	return GWD_E_OK;
}




gw_uint8
gwd_loop_back_detect_pkt_handler(
	GWD_IN	gw_uint8		*pkt,
	GWD_IN	gw_uint16		pkt_len,
	GWD_IN	gwd_port_id_t	port_id
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif

	gw_uint16 vlan_id = 0;
	loop_back_port_info_t port_infor;
	loop_back_port_info_t extern_port_infor;

	//�ӱ�������ȡ�˿���Ϣ
	gwd_loop_back_detect_pkt_get_port_infor(pkt, pkt_len, port_id, &vlan_id, &port_infor, &extern_port_infor);

	if(GWD_E_OK != gwd_port_vlan_check(port_infor.port_id, vlan_id))	//���vlan �Ϸ���
	{
		gwd_sdl_vlan_mode_t vlan_mode;
		gw_uint16 vlan_def;
		gw_uint16 vlan[PER_PORT_MAX_VLAN] = {0};
		gw_uint16 vlan_num = 0;
		
		gwd_onu_port_vlan_get(port_id, &vlan_mode, &vlan_def, vlan, &vlan_num);

		vlan_id = vlan_def;
	}

	//��ȡ�˿���Ϣ
	if(GWD_E_OK != onu_port_infor_get(port_id, &port_infor))
	{
		gwd_printf("onu_port_infor_get failed\n");
	}
	
	//��·�澯
	loop_back_occur_proc(vlan_id, &port_infor, &extern_port_infor);

	//���滷·״̬
	loop_back_status_list_add_node(vlan_id, &port_infor, &extern_port_infor);
	
	return 1;
}

gwd_status
gwd_loop_back_detect_pkt_parser(
	GWD_IN	gw_uint8		*pkt,
	GWD_IN	gw_uint16		pkt_len,
	GWD_IN	gwd_port_id_t	port_id
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_ERROR;
	gwd_loop_detect_frame_data_t *pdata = NULL;
	gw_uint16 tag_num = 0;

	eth_pkt_tag_num_get(pkt, pkt_len, &tag_num);
	pdata = (gwd_loop_detect_frame_data_t *)(pkt+sizeof(eth_head_t)+tag_num*sizeof(gwd_vlan_hdr_t));

	if(ntohs(pdata->eth_type) == GWD_ETH_TYPE_LOOP_DETECT &&
		ntohs(pdata->loop_flag) == GWD_LOOP_DETECT_CHECK)
	{
		ret = GWD_E_OK;
	}
	else
	{
		ret = GWD_E_ERROR;
	}

	return ret;
}

gwd_status
gwd_loop_back_detect_pkt_get_port_infor(
	GWD_IN	gw_uint8					*pkt,
	GWD_IN	gw_uint16					pkt_len,
	GWD_IN	gwd_port_id_t				port_id,
	GWD_OUT	gw_uint16					*vlan_id,
	GWD_OUT	loop_back_port_info_t 		*port_infor,
	GWD_OUT	loop_back_port_info_t 		*extern_port_infor
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	//��ڹ�����
	if(NULL == pkt)
	{
		return GWD_E_ERROR;
	}
	if(NULL == vlan_id)
	{
		return GWD_E_ERROR;
	}
	if(NULL == port_infor)
	{
		return GWD_E_ERROR;
	}
	if(NULL == extern_port_infor)
	{
		return GWD_E_ERROR;
	}

	gwd_loop_detect_frame_t *pFrame = NULL;
	gwd_loop_detect_frame_data_t *pdata = NULL;
	gw_uint8	opposite_mac[GWD_MACADDR_LEN] = {0};
	gw_uint32	onu_if_index = 0;
	gw_uint32	opposite_slot = 0;
	gw_uint32	opposite_port = 0;
	gw_uint8	opposite_olt_mac[GWD_MACADDR_LEN] = {0};
	gw_uint16 tag_num = 0;

	eth_pkt_tag_num_get(pkt, pkt_len, &tag_num);

	pFrame = (gwd_loop_detect_frame_t *)pkt;
	pdata = (gwd_loop_detect_frame_data_t *)(pkt+sizeof(eth_head_t)+tag_num*sizeof(gwd_vlan_hdr_t));

	//����vlan
	*vlan_id =  ntohs(pdata->onu_vid);

	memset(port_infor, 0, sizeof(loop_back_port_info_t));
	memset(extern_port_infor, 0, sizeof(loop_back_port_info_t));

	onu_if_index = ntohl(pdata->onu_if_index);
	opposite_slot = ifm_eth_get_slot(onu_if_index);
	opposite_port = ifm_eth_get_port(onu_if_index);
	memcpy(opposite_olt_mac, pFrame->src_mac, GWD_MACADDR_LEN);

	/*���ñ��˶˿���Ϣ*/
	//���ö˿ں�
	port_infor->port_id =port_id;

	//���ò�λ��
	port_infor->slot_id =1;


	/*���öԶ˶˿���Ϣ*/
	//���ö˿ں�
	extern_port_infor->port_id = (opposite_port&0xff);

	//���ò�λ��
	extern_port_infor->slot_id = (opposite_slot&0xff);

	//����onu ��λ��
	extern_port_infor->onu_slot = pdata->onu_location[1];

	//����onu pon�ں�
	extern_port_infor->onu_pon = pdata->onu_location[2];

	//����onu �߼���·��ʶ
	extern_port_infor->onu_llid = pdata->onu_location[3];

	//����onu ����
	extern_port_infor->onu_type = pdata->onu_type;

	//����onu mac
	memcpy(extern_port_infor->onu_mac, opposite_mac, sizeof(extern_port_infor->onu_mac));

	//����olt ����
	extern_port_infor->olt_type = pdata->olt_type;

	//����olt mac
	memcpy(extern_port_infor->olt_mac, opposite_olt_mac, GWD_MACADDR_LEN);

	return GWD_E_OK;
}



//��/ ������·��ⱨ�Ľ���cpu
gwd_status
loop_back_detect_pkt_into_cpu_enable(
	GWD_IN	int enable
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	int enable_set = DISABLE;
	static int enable_current = DISABLE;

	//״̬���
	if(enable != 0)
	{
		enable_set = ENABLE;
	}
	else
	{
		enable_set = DISABLE;
	}
	if(enable_set != enable_current)
	{
		//������״̬
		enable_current = enable_set;

		if(ENABLE == enable_set)
		{
			gwd_loop_detect_pkt_dst_set(GWD_UP_STREAM, GWD_DST_CPU);
			gwd_loop_detect_pkt_dst_set(GWD_DOWN_STREAM, GWD_DST_CPU);
		}
		else
		{
			gwd_loop_detect_pkt_dst_set(GWD_UP_STREAM, GWD_DST_DROP);
			gwd_loop_detect_pkt_dst_set(GWD_DOWN_STREAM, GWD_DST_DROP);
		}
	}

	return GWD_E_OK;
}

gwd_status
ifm_eth_set_slot(
	GWD_IN_OUT	gw_uint32 *ulIfIndex,
	GWD_IN		gw_uint32 slot
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	//��ڹ�����
	if(NULL == ulIfIndex)
	{
		return GWD_E_ERROR;
	}

	onu_if_index_t unIfIndex;

	unIfIndex.ulPhyIfIndex = (*ulIfIndex);
	unIfIndex.phy_slot_port.slot = (slot&0x1f);
	*ulIfIndex = unIfIndex.ulPhyIfIndex;

    return GWD_E_OK;
}

gw_uint32
ifm_eth_get_slot(
	GWD_IN	gw_uint32 ulIfIndex
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
    gw_uint32 ulSlot = 0;

	onu_if_index_t unIfIndex;
	unIfIndex.ulPhyIfIndex = ulIfIndex;

	ulSlot = unIfIndex.phy_slot_port.slot;

    return ulSlot;
}

gwd_status
ifm_eth_set_port(
	GWD_IN_OUT	gw_uint32 *ulIfIndex,
	GWD_IN		gw_uint32 port
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	//��ڹ�����
	if(NULL == ulIfIndex)
	{
		return GWD_E_ERROR;
	}

	onu_if_index_t unIfIndex;

	unIfIndex.ulPhyIfIndex = (*ulIfIndex);
	unIfIndex.phy_slot_port.port = (port&0x3f);
	*ulIfIndex = unIfIndex.ulPhyIfIndex;

    return GWD_E_OK;
}

gw_uint32
ifm_eth_get_port(
	GWD_IN	gw_uint32 ulIfIndex
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
    gw_uint32 ulPort = 0;

	onu_if_index_t unIfIndex;
	unIfIndex.ulPhyIfIndex = ulIfIndex;

	ulPort = unIfIndex.phy_slot_port.port;


    return ulPort;
}

gwd_status	loop_list_node_ports_infor_update(
	GWD_IN	loop_back_ports_infor_t	*ports_infor,
	GWD_IN	gw_uint16				aging_time,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;
	
	//��ڹ�����
	if((NULL == ports_infor)
		|| (NULL == port_infor))
	{
		ret = GWD_E_ERROR;
		goto end;
	}

	ports_infor->port_infor = *port_infor;
	if(extern_port_infor != NULL)
	{
		ports_infor->extern_flag = 1;
		ports_infor->extern_port_infor = *extern_port_infor;
		ports_infor->aging_count = aging_time;
	}
	else
	{
		if(ports_infor->aging_count <= 0)
		{
			ports_infor->extern_flag = 0;
			memset(&(ports_infor->extern_port_infor), 0, sizeof(loop_back_port_info_t));
		}
		else
		{
			ports_infor->aging_count--;
		}

	}

	ports_infor->loop_back_status.lpbMask = 1;
	
end:
	return ret;
}

gwd_status	loop_list_node_ports_infor_add(
	GWD_IN	loop_back_ports_infor_t	*ports_infor,
	GWD_IN	gw_uint16				aging_time,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;
	
	//��ڹ�����
	if((NULL == ports_infor)
		|| (NULL == port_infor))
	{
		ret = GWD_E_ERROR;
		goto end;
	}

	memset(ports_infor, 0, sizeof(loop_back_ports_infor_t));
	ports_infor->valid_flag = 1;
	ports_infor->port_infor = *port_infor;
	ports_infor->loop_back_status.lpbMask = 1;
	
	if(extern_port_infor != NULL)
	{
		ports_infor->extern_flag = 1;
		ports_infor->extern_port_infor = *extern_port_infor;
		ports_infor->aging_count = aging_time;
	}
	else
	{
		ports_infor->extern_flag = 0;
		ports_infor->aging_count = 0;
	}
	
end:
	return ret;

}

/*�����ⲿ�Զ˿�up/down ����������û��Զ˿�admin���й������������·��Ϣ*/
void 
loop_outside_port_admin_oper_proc(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_ports_infor_t		*ports_infor
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	gwd_sdl_port_admin_t admin_set;
	gwd_sdl_port_admin_t admin_cur;			

	gwd_port_admin_get(ports_infor->port_infor.port_id, &admin_cur);

	if(ports_infor->loop_back_status.lpbDown != 0)
	{
		admin_set = GWD_SDL_PORT_ADMIN_DOWN;
	}
	else
	{
		admin_set = GWD_SDL_PORT_ADMIN_UP;
	}

	if(admin_cur != admin_set)
	{
	    cs_printf("cancel loop  zhaoxh :12334234\n");
		loop_back_cancel_proc(vlan_id, &(ports_infor->port_infor), &(ports_infor->extern_port_infor));
		//��ջ�·�˿ڵ�������Ϣ
		memset(ports_infor, 0, sizeof(loop_back_ports_infor_t));
	}
}

void 
loop_port_admin_control(
	GWD_IN	loop_control_config_t	*config,
	GWD_IN	loop_back_ports_infor_t	*ports_infor
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	//�˿��Ƿ��⵽������·
	if(ports_infor->loop_back_status.lpbMask != 0)	//��
	{
		//��·����
		if(config->policy != 0)
		{
			//down ���˿�
			flag_port_up = 1;
			if(GWD_E_OK == gwd_port_admin_set(ports_infor->port_infor.port_id, GWD_SDL_PORT_ADMIN_DOWN))
			{
				//��¼�˿�down ��
				ports_infor->loop_back_status.lpbDown = 1;
				/*��ֹ����cancel loop���ڲ�׼����down�˿�ʱ����ж�����*/
				ports_infor->loop_back_status.clearCnt = 0;
			}
			else
			{
				gwd_printf("gwd_port_admin_set failed, in %s, line :%d\n", __func__, __LINE__);
			}
		}
		else
		{
			//do nothing
		}
	}
	else
	{
		//do nothing
	}
}

void 
loop_port_wake_up(
	GWD_IN	loop_control_config_t	*config,
	GWD_IN	loop_back_ports_infor_t	*ports_infor
)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	//�˿����޻��ѻ���δ���﻽�Ѵ������ֵ���Ѷ˿�
	if(((gw_uint16)(-1) == config->maxwakeup)
		|| (ports_infor->loop_back_status.wakeCnt < config->maxwakeup))
	{
		if(config->waitforwakeup <= ports_infor->loop_back_status.slpCnt)
		{
		    
			//���Ѷ˿�
			flag_port_up = 1;
			if(GWD_E_OK == gwd_port_admin_set(ports_infor->port_infor.port_id, GWD_SDL_PORT_ADMIN_UP))
			{
			    cs_printf("************up ok\n");
				//��¼�˿�״̬
				ports_infor->loop_back_status.lpbDown = 0;
				
				//�ȴ���������
				ports_infor->loop_back_status.slpCnt = 0;	

				//�˿ڻ��Ѵ����Լ�1
				ports_infor->loop_back_status.wakeCnt++;
			}
			else
			{
				gwd_printf("gwd_port_admin_set failed, in %s, line :%d\n", __func__, __LINE__);
			}
		}
		else
		{
			//�ȴ������Լ�1
			ports_infor->loop_back_status.slpCnt++;
		}
		
	}
	else
	{
		//do nothing
	}
	
}

void 
loop_port_alarm_periodically(
	GWD_IN	loop_control_config_t	*config,
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_ports_infor_t	*ports_infor
)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	//�����Ը澯
	if( (gw_uint16)(-1) == config->alarm_interval)
	{
		//�澯
		if(GWD_E_OK != loop_back_occur_proc(vlan_id, &(ports_infor->port_infor), &(ports_infor->extern_port_infor)))
		{
			gwd_printf("loop_back_occur_proc failed !\n");
		}

		//�澯��������
		ports_infor->loop_back_status.alarm_count = 0;
	}
	else
	{
		//�澯�����Լ�1
		ports_infor->loop_back_status.alarm_count++;
	}
}

void 
loop_port_status_clear_proc(
	GWD_IN	loop_control_config_t	*config,
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_ports_infor_t	*ports_infor	
)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
    	if(ports_infor->loop_back_status.lpbDown == 0)
    	{
            	//�˿��Ƿ��⵽������·
            	if(ports_infor->loop_back_status.lpbMask != 0)	//��
            	{
            		//�˿ڿ�����û�м�⵽��·��������
            		ports_infor->loop_back_status.clearCnt = 0;
            	}
            	else
            	{
            	    #ifdef TEST
            	    cs_printf("clear_interval = %d\n",ports_infor->loop_back_status.clearCnt);
					#endif
					//�˿ڿ�����û�м�⵽��·�����Ƿ�ﵽ����
            		if(config->clear_interval <= ports_infor->loop_back_status.clearCnt)	//��
            		{
            			#if ( RPU_MODULE_LOG_SUPPORT == MODULE_YES )
            			gw_syslog(GW_LOG_LEVEL_MAJOR, "Interface  eth%d/%d no loop found in vlan %d for %d intervals,clear status\n",
            											ports_infor->port_infor.slot_id, ports_infor->port_infor.port_id, vlan_id, ports_infor->loop_back_status.clearCnt);
            			#endif
            			
            			//�����·�澯
            			 #ifdef TEST
            			cs_printf("zhaoxh cancel:321123\n");
						cs_printf("clear_interval = %d\n",ports_infor->loop_back_status.clearCnt);
                        #endif
						flag_port_up = 0;
						loop_back_cancel_proc(vlan_id, &(ports_infor->port_infor), &(ports_infor->extern_port_infor));
            			//��ջ�·�˿ڵ�������Ϣ
            			memset(ports_infor, 0, sizeof(loop_back_ports_infor_t));
            		}
            		else	//��
            		{
            			//�˿ڿ�����û�м�⵽��·�����Լ�1
            			ports_infor->loop_back_status.clearCnt++;
            		}
            	}
          }
	
}

gwd_status
loop_back_status_list_add_node(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;
	gw_uint8 port_id = 0;
	gw_uint16 index = 0;
	loop_back_ports_infor_t *ports_infor = NULL;
	//��ڹ�����
	if(NULL == port_infor)
	{
		return GWD_E_ERROR;
	}
	//extern_port_infor ����ΪNULL

	loop_back_status_list_lock();
	
	port_id = port_infor->port_id;
	index = port_id - 1;
	gw_uint16 aging_time = 0;
	loop_control_config_t config;
	loop_back_detect_config_get(&config);
	aging_time = config.opposite_aging_time;
	//��黷·vlan �Ƿ�������б�
	loop_back_infor_t *node = NULL;
	node = (loop_back_infor_t *)gw_lst_find(&g_loop_back_infor_list, (gw_uint32)&vlan_id);
	
	if(NULL != node)	//vlan�������б�
	{
		gw_uint8 ports_num = 0;

		ports_num = node->ports_num;
		//���˿ںϷ���
		if(index < ports_num)	//�˿ںϷ�
		{
			ports_infor = &(node->ports_infor[index]);
			//���˿��Ƿ����
			if(ports_infor->valid_flag != 0)		//�˿ڴ���
			{
				//�˿���Ϣ�ϻ�
				loop_list_node_ports_infor_update(ports_infor, aging_time, port_infor, extern_port_infor);
			}
			else	//�˿ڲ�����
			{
				//���˿���Ϣ���뻷·vlan
				loop_list_node_ports_infor_add(ports_infor, aging_time, port_infor, extern_port_infor);
			}

		}
		else	//�˿ڲ��Ϸ�
		{
			//����ʧ��
			ret = GWD_E_ERROR;
			goto end;
		}
	}
	else	//vlan ���������б�
	{
		int ports_num = 0;
		gwd_uni_port_num_get(&ports_num);
		if(ports_num <= index)
		{
			//����ʧ��
			ret = GWD_E_ERROR;
			goto end;
		}
		
		//�����µĽڵ�
		loop_back_infor_t *loop_back_node = NULL;

		loop_back_node = (loop_back_infor_t *)gwd_malloc(sizeof(loop_back_infor_t));
		if(NULL == loop_back_node)
		{
			gwd_printf("in %s, gwd_malloc failed!\n", __func__);
			ret = GWD_E_ERROR;
			goto end;
		}
		memset(loop_back_node, 0, sizeof(loop_back_infor_t));

		ports_infor = &(loop_back_node->ports_infor[index]);
		//���ýڵ���Ϣ
		loop_back_node->vlan_id = vlan_id;
		loop_back_node->ports_num = sizeof(loop_back_node->ports_infor) / sizeof(loop_back_node->ports_infor[0]);
		loop_list_node_ports_infor_add(ports_infor, aging_time, port_infor, extern_port_infor);

		//���ڵ�����б�
		gw_lst_add(&g_loop_back_infor_list, (gw_node *)loop_back_node);

		//���ش�ӡ��·��Ϣ
		#if ( RPU_MODULE_LOG_SUPPORT == MODULE_YES )
		{
			char *olt_type = NULL;
			extern char onu_type[];
			
			switch(port_infor->olt_type)
			{
				case GWD_OLT_GFA6100:
					olt_type = "GFA6100";
					break;
				case GWD_OLT_GFA6700:
					olt_type = "GFA6700";
					break;
				case GWD_OLT_GFA6900:
					olt_type = "GFA6700";
					break;
				default:
					olt_type = "unknown olt";
					break;
			}
			
			gw_syslog(GW_LOG_LEVEL_MAJOR, "Interface  eth%d/%d loop[%s(%02x%02x.%02x%02x.%02x%02x)%d/%d %s(%02x%02x.%02x%02x.%02x%02x)V(%d)]\n",
							port_infor->slot_id, port_infor->port_id, olt_type, port_infor->olt_mac[0], port_infor->olt_mac[1],
							port_infor->olt_mac[2], port_infor->olt_mac[3], port_infor->olt_mac[4], port_infor->olt_mac[5], 
							port_infor->onu_slot, port_infor->onu_pon, onu_type, port_infor->onu_mac[0], port_infor->onu_mac[1],
							port_infor->onu_mac[2], port_infor->onu_mac[3], port_infor->onu_mac[4], port_infor->onu_mac[5], vlan_id);
		}
		#endif

	}

end:
	loop_back_status_list_unlock();
	return ret;
}

gwd_status
loop_back_status_list_port_control(
	GWD_IN	loop_control_config_t	*config
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	if(NULL == config)
	{
		return GWD_E_ERROR;
	}

	loop_back_status_list_lock();
	
	//������·��Ϣ�б�
	loop_back_infor_t *node = NULL;

	gw_lst_scan(&g_loop_back_infor_list, node, loop_back_infor_t *)
	{
		int i = 0;
		gw_uint16 vlan_id = 0;
		gw_uint8 ports_num = 0;
		loop_back_ports_infor_t	*ports_infor = NULL;

		vlan_id = node->vlan_id;
		ports_num = node->ports_num;
		ports_infor = node->ports_infor;

		//�����˿�
		for(i=0; i<ports_num; i++,ports_infor++)
		{

			//���˿���Ϣ�Ƿ�������б�
			if(ports_infor->valid_flag != 0)	//�������б�
			{
				//�����ⲿ�Զ˿�up/down ����������û��Զ˿�admin���й������������·��Ϣ
				loop_outside_port_admin_oper_proc(vlan_id,ports_infor);
				if(0 == ports_infor->valid_flag)
				{
					continue;
				}

				//��·�˿ڿ���
				loop_port_admin_control(config, ports_infor);

				//���ڸ澯
				loop_port_alarm_periodically(config, vlan_id, ports_infor);

				//���Ѷ˿�
				loop_port_wake_up(config, ports_infor);

				//�����·����
				loop_port_status_clear_proc(config, vlan_id, ports_infor);
				if(0 == ports_infor->valid_flag)
				{
					continue;
				}
				
				//ÿ�δ�����ɺ�·״̬���㣬���ֵ���ڻ�·��������1
			
			ports_infor->loop_back_status.lpbMask = 0;	
			}
			else							//���������б�
			{
				//��ѯ��һ���˿�
				continue;
			}
		}
	}

	loop_back_status_list_unlock();
	return GWD_E_OK;
}

gwd_status
loop_back_status_list_update(
	void
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	loop_back_infor_t *node = NULL;
	loop_back_infor_t *node_pre = NULL;

	loop_back_status_list_lock();
	node = (loop_back_infor_t *)gw_lst_first(&g_loop_back_infor_list);
	while(NULL != node)
	{
		int i = 0;
		loop_back_ports_infor_t *ports_infor = NULL;
		int ports_num  = 0;
		gw_uint8 delete_flag = 0;

		ports_num = node->ports_num;
		ports_infor = node->ports_infor;

		//vlan �ڵ����ж˿���Ϣ����Ч������Ϊɾ��
		delete_flag = 1;
		for(i=0; i<ports_num; i++)
		{
			if(0 != ports_infor[i].valid_flag)
			{
				delete_flag = 0;
				break;
			}
			else
			{
				continue;
			}
		}

		//ɾ����ɾ����ǵĽڵ�
		if(1 == delete_flag)
		{
			loop_back_infor_t *node_temp = NULL;
			node_temp = node;
			
			//node ������һ�ڵ�
			node = (loop_back_infor_t *)gw_lst_next((gw_node *)node);

			//ɾ��
			gw_lst_delete(&g_loop_back_infor_list, (gw_node *)node_temp);
			gwd_free(node_temp);
			node_temp = NULL;
		}
		else
		{
			 node_pre = node;
			 
			//node ������һ�ڵ�
			node = (loop_back_infor_t *)gw_lst_next((gw_node *)node);
		}

	}

	loop_back_status_list_unlock();
	
	return GWD_E_OK;
}


gwd_status
loop_back_status_list_reset_port(
	void
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	loop_back_infor_t *node = NULL;

	loop_back_status_list_lock();
	gw_lst_scan(&g_loop_back_infor_list, node, loop_back_infor_t *)
	{
		loop_back_ports_infor_t *ports_infor = NULL;
		int ports_num  = 0;
		int i = 0;

		ports_num = node->ports_num;
		ports_infor = node->ports_infor;

		for(i=0; i<ports_num; i++)
		{
			loop_back_ports_infor_t *port_infor = NULL;

			port_infor = &(ports_infor[i]);

			if(0 != port_infor->valid_flag)
			{
				if(0 != port_infor->loop_back_status.lpbDown)
				{
					//���Ѷ˿�
					gwd_port_admin_set(port_infor->port_infor.port_id, GWD_SDL_PORT_ADMIN_UP);
				}
			}
		}

	}

	loop_back_status_list_unlock();
	
	return GWD_E_OK;
}

gwd_status
loop_back_status_list_clear(
	void
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	loop_back_infor_t *node = NULL;
	
	loop_back_status_list_lock();
	
	while(NULL != (node = (loop_back_infor_t *)gw_lst_get(&g_loop_back_infor_list)))
	{
		gwd_free(node);
	}

	loop_back_status_list_unlock();
	
	return GWD_E_OK;
}

void
loop_back_status_list_show(
	void
	)
{
	//��������
	loop_back_infor_t *node = NULL;
	gw_uint32 count = 0;

	loop_back_status_list_lock();
	
	count = gw_lst_count(&g_loop_back_infor_list);

	gwd_printf("list count :%d\n", count);
	gw_lst_scan(&g_loop_back_infor_list, node, loop_back_infor_t *)
	{
		gwd_printf("node->vlan_id :%d, node->ports_num :%d\n", node->vlan_id, node->ports_num);

		int i = 0;
		loop_back_ports_infor_t *ports_infor = NULL;
		int ports_num  = 0;

		ports_num = node->ports_num;
		ports_infor = node->ports_infor;
		for(i=0; i<ports_num; i++)
		{
			gwd_printf("ports_infor[%d].valid_flag :%d\n", i, ports_infor[i].valid_flag);
		}
	}

	loop_back_status_list_unlock();
	return;
}
gwd_status loop_back_status_list_led_proc(
	void
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	loop_back_status_list_lock();
	if(NULL == gw_lst_first(&g_loop_back_infor_list))	//�б����޼�¼�������ڻ�·
	{
	    flag_port_up = 0;
		loop_back_led_proc(GWD_LOOP_CNACEL);
	}
	else
	{
		//do nothing
	}

	loop_back_status_list_unlock();
	return GWD_E_OK;
}


//����/ �رջ�·��������߳�
gwd_status
loop_back_detect_task_eable(
	GWD_IN	int enable
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;
	int enable_set = DISABLE;
	static int enable_current = DISABLE;

	//״̬���
	if(enable != 0)
	{
		enable_set = ENABLE;
	}
	else
	{
		enable_set = DISABLE;
	}
	if(enable_set != enable_current)
	{
		//������״̬
		enable_current = enable_set;

		if(ENABLE == enable_set)
		{
			#ifdef TEST
			gwd_printf("***loop_detect_task***\n");
			#endif
			//������·�������
			gwd_loop_detect_thread_create(loop_detect_task);
		}
		else
		{
			//�رջ�·�������
			gwd_loop_detect_thread_delete();
		}
	}
	return ret;
}


void
loop_detect_task(
	GWD_IN	void *data
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif

	while(1)
	{
		loop_control_config_t loop_control_config;

		//��ȡ��·��������
		loop_back_detect_config_get(&loop_control_config);

		//���Ͳ��Ա���
		loop_back_test_pkt_send(&loop_control_config);

		//��ʱ
		gwd_thread_delay(loop_control_config.interval*1000/2);

		//��鴦��·mac
		loop_back_mac_check_proc(&loop_control_config);
		

		//��·����
		loop_back_status_list_port_control(&loop_control_config);
		
		
		//����vlan ��·��Ϣ
		loop_back_status_list_update();
		

		//��·led �ƴ���
		loop_back_status_list_led_proc();
		

		//��ʱ
		gwd_thread_delay(loop_control_config.interval*1000/2);
	}

}

gwd_status
loop_back_detect_port_status_proc_enable(
	GWD_IN	int enable
)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif

	int enable_set = DISABLE;
	static int enable_current = DISABLE;

	//״̬���
	if(enable != 0)
	{
		enable_set = ENABLE;
	}
	else
	{
		enable_set = DISABLE;
	}

	if(enable_set != enable_current)
	{
		//������״̬
		enable_current = enable_set;

		if(DISABLE == enable_set)
		{
			loop_back_status_list_reset_port();
			loop_back_status_list_clear();
			loop_back_status_list_led_proc();
		}
	}

	return GWD_E_OK;
}

gwd_status
loop_back_test_port(
	GWD_IN	gw_uint16			vlan_detect,
	GWD_IN	gwd_port_id_t		port_id,
	GWD_IN	gwd_sdl_vlan_mode_t vlan_mode,
	GWD_IN	gw_uint16			vlan_def,
	GWD_IN	gw_uint16			*vlan_cfg,
	GWD_IN	gw_uint16			vlan_num
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	gw_uint16 vlan_test = 0;

	if(GWD_SDL_VLAN_MODE_TRANSPARENT == vlan_mode)
	{
		vlan_test = vlan_detect;
		loop_back_detect_test_vlan_port(port_id, vlan_test);
	}
	else
	{
		if(0 == vlan_detect)	//��������vlan
		{
			int i = 0;

			vlan_test = vlan_def;
			loop_back_detect_test_vlan_port(port_id, vlan_test);
			
			for(i=0; i<vlan_num; i++)
			{
				vlan_test = vlan_cfg[i];
				loop_back_detect_test_vlan_port(port_id, vlan_test);
			}
		}
		else
		{
			if(vlan_def == vlan_detect)
			{
				vlan_test = vlan_def;
				loop_back_detect_test_vlan_port(port_id, vlan_test);
			}

			int i = 0;
			for(i=0; i<vlan_num; i++)
			{
				if(vlan_cfg[i] == vlan_detect)
				{
					vlan_test = vlan_cfg[i];
					loop_back_detect_test_vlan_port(port_id, vlan_test);
					break;
				}
			}
		}
	}

	return GWD_E_OK;
}

gwd_status
loop_back_test_pkt_send(
	GWD_IN	loop_control_config_t	*config
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	//��ڹ�����
	if(NULL == config)
	{
		return GWD_E_ERROR;
	}
	gw_uint16 vlan_cfg = 0;
	int port_num = 0;
	int i = 0;

	vlan_cfg = config->vid;
	gwd_uni_port_num_get(&port_num);

	for(i=0; i<port_num; i++)	//�����˿�
	{
		gwd_port_id_t port_id;
		gwd_sdl_vlan_mode_t vlan_mode;
		gw_uint16 vlan_def;
		gw_uint16 vlan[PER_PORT_MAX_VLAN] = {0};
		gw_uint16 vlan_num = 0;
		
		port_id = CS_UNI_PORT_ID1 + (port_num - 1 - i);

		gwd_onu_port_vlan_get(port_id, &vlan_mode, &vlan_def, vlan, &vlan_num);
		loop_back_test_port(config->vid, port_id, vlan_mode, vlan_def, vlan, vlan_num);
	}

	return GWD_E_OK;
}

gwd_status
loop_back_mac_check_proc(
	GWD_IN		loop_control_config_t	*config
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	//��ڹ�����
	if(NULL == config)
	{
		return GWD_E_ERROR;
	}

	int uni_port_num = 0;
	int i = 0;
	int vlan_cfg = 0;
	char mac_cfg[GWD_MACADDR_LEN] = {0};

	vlan_cfg = config->vid;
	memcpy(mac_cfg, config->smac, GWD_MACADDR_LEN);

	//��ȡ�˿�����
	gwd_uni_port_num_get(&uni_port_num);

	for(i=0; i<uni_port_num; i++)	//�����˿�
	{
		gwd_port_id_t port_id;
		gw_uint16 vlan[PER_PORT_MAX_VLAN] = {0};
		gw_uint16 vlan_num = 0;
		gwd_mac_t gw_mac;
		int j = 0;

		port_id = CS_UNI_PORT_ID1 + i;
		memcpy(gw_mac.addr, mac_cfg, GWD_MACADDR_LEN);
		gwd_fdb_port_mac_get_vlan(port_id, gw_mac, vlan, &vlan_num);
		for(j=0; j<vlan_num; j++)
		{
			gw_uint16 vlan_loop = vlan[j];
			if(0 == vlan_cfg)	//��������vlan
			{
				//do nothing
			}
			else if(vlan_loop == vlan_cfg)	//����ָ��vlan
			{
				//do nothing
			}
			else	//����������������vlan
			{
				continue;
			}

			loop_back_port_vlan_proc(port_id, vlan_loop);
		}
	}

	return GWD_E_OK;
}


gwd_status
loop_back_detect_test_vlan_port(
	GWD_IN	gwd_port_id_t	port_id,
	GWD_IN	gw_uint16		vlan_id
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;
	loop_back_port_info_t	port_infor;
	gwd_loop_detect_frame_t loop_detect_frame;

	//��ȡ�˿���Ϣ
	if(GWD_E_OK != (ret = onu_port_infor_get(port_id, &port_infor)))
	{
		gwd_printf("onu_port_infor_get failed, ret :%d\n", ret);
		return ret;
	}

	//������·��ⱨ��
	if(GWD_E_OK != (ret = loop_back_detect_pkt_build(vlan_id, &port_infor, &loop_detect_frame)))
	{
		gwd_printf("loop_back_detect_pkt_build failed, ret :%d\n", ret);
		return ret;
	}

	//���ͻ�·���Ա���
	if(GWD_E_OK != (ret = loop_back_detect_pkt_send(port_id, &loop_detect_frame)))
	{
		gwd_printf("loop_back_detect_pkt_send failed, ret :%d\n", ret);
		return ret;
	}

	return GWD_E_OK;
}

gwd_status
loop_back_detect_pkt_build(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_port_info_t 	*port_infor,
	GWD_OUT	gwd_loop_detect_frame_t	*loop_detect_frame
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	//��ڹ�����
	if(NULL == port_infor)
	{
		return GWD_E_ERROR;
	}
	if(NULL == loop_detect_frame)
	{
		return GWD_E_ERROR;
	}

	gwd_mac_t dest_mac;
	gwd_mac_t src_mac;
	gw_uint32	onu_if_index = 0;

	gwd_loop_back_detect_frame_dest_mac_get(&dest_mac);
	gwd_loop_back_detect_frame_src_mac_get(&src_mac);

	#if (CPU_PKT_TO_SPECIFIC_UNI_PORT)
	ifm_eth_set_slot(&onu_if_index, port_infor->slot_id);
	ifm_eth_set_port(&onu_if_index, port_infor->port_id);
	#endif

	//����Ŀ��mac
	memcpy(loop_detect_frame->dest_mac, dest_mac.addr, GWD_MACADDR_LEN);

	//����Դmac
	memcpy(loop_detect_frame->src_mac, src_mac.addr, GWD_MACADDR_LEN);

	//����TPID
	loop_detect_frame->tpid = htons(TPID);

	//����vlan id
	loop_detect_frame->vid = htons(vlan_id);

	//������̫������ (ip ����)
	loop_detect_frame->eth_type = htons(GWD_ETH_TYPE_LOOP_DETECT);

	//���û�·��־
	loop_detect_frame->loop_flag = htons(GWD_LOOP_DETECT_CHECK);

	//����olt ����
	loop_detect_frame->olt_type = port_infor->olt_type;

	//����onu ����
	loop_detect_frame->onu_type = port_infor->onu_type;

	//����onu λ��
	loop_detect_frame->onu_location[0] = 0;
	loop_detect_frame->onu_location[1] = port_infor->onu_slot;
	loop_detect_frame->onu_location[2] = port_infor->onu_pon;
	loop_detect_frame->onu_location[3] = port_infor->onu_llid;

	//����onu mac
	memcpy(loop_detect_frame->onu_mac, port_infor->onu_mac, GWD_MACADDR_LEN);
	
	//����onu ��vlan id
	loop_detect_frame->onu_vid = htons(vlan_id&0xfff);

	loop_detect_frame->onu_if_index = htonl(onu_if_index);

	return GWD_E_OK;
}

static gw_uint8 loop_detect_dest_mac[GWD_MACADDR_LEN] = {0x00, 0x0F, 0xE9, 0x04, 0x8E, 0xDF};
gwd_status
gwd_loop_back_detect_frame_dest_mac_get(
	GWD_OUT	gwd_mac_t *mac
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	memcpy(mac->addr, loop_detect_dest_mac, GWD_MACADDR_LEN);

	return GWD_E_OK;
}

gwd_status
gwd_loop_back_detect_frame_src_mac_get(
	GWD_OUT	gwd_mac_t *mac
	)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	loop_control_config_t config;

	loop_back_detect_config_get(&config);

	memcpy(mac->addr, config.smac, GWD_MACADDR_LEN);
	return GWD_E_OK;
}

gwd_status
loop_back_port_vlan_proc(
	GWD_IN		gwd_port_id_t	port_id,
	GWD_IN		gw_uint16		vlan_id
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	loop_back_port_info_t	port_infor;

	//��ȡ�˿���Ϣ
	onu_port_infor_get(port_id, &port_infor);

	if(GWD_E_OK != gwd_port_vlan_check(port_infor.port_id, vlan_id))
	{
		gwd_sdl_vlan_mode_t vlan_mode;
		gw_uint16 vlan_def;
		gw_uint16 vlan[PER_PORT_MAX_VLAN] = {0};
		gw_uint16 vlan_num = 0;
		
		gwd_onu_port_vlan_get(port_id, &vlan_mode, &vlan_def, vlan, &vlan_num);

		vlan_id = vlan_def;
	}

	//���˿���Ϣ�����б�
	loop_back_status_list_add_node(vlan_id, &port_infor, NULL);

	return GWD_E_OK;
}

gwd_status
eth_pkt_tag_num_get(
	GWD_IN		gw_uint8	*eth_pkt,
	GWD_IN		gw_uint16	len,
	GWD_OUT		gw_uint16	*tag_num
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	//��ڹ�����
	if(NULL == eth_pkt)
	{
		return GWD_E_ERROR;
	}

	if(NULL == tag_num)
	{
		return GWD_E_OK;
	}

	gw_uint8 *posation = NULL;
	tag_t *tag = NULL;
	gw_uint16 count = 0;

	posation = (gw_uint8 *)eth_pkt;
	//������̫��ͷ
	posation += sizeof(eth_head_t);

	//����vlan tag
	tag = (tag_t *)posation;
	count = 0;
	while(0x8100 == ntohs(tag->tpid))
	{
		tag++;
		count++;
	}

	*tag_num = count;

	return GWD_E_OK;
}

gwd_status
loop_back_occur_proc(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor

)
{
	static int first_call = 1;
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	gwd_loop_status_t loop_status;

	loop_status = GWD_LOOP_OCCUR;
	//�ϱ��澯
	gwd_loop_back_alarm(vlan_id, port_infor, extern_port_infor, loop_status);

	//mib_uni_loop_back_alarm_raise(port_infor->slot_id,port_infor->port_id);
	if(first_call)
	{
		int portId = 0;
		for(portId = CS_UNI_PORT_ID1;portId <= UNI_PORT_MAX;portId++)
		{
			ctc_oam_set_alarm_config(CTC_PORT_ALARM,portId,0x304,CTC_ALARM_ON);
		}
		first_call = 0;
	}
	loop_back_led_proc(loop_status);
	ctc_oam_build_alarm_event(CTC_PORT_ALARM, port_infor->port_id, 0x304, CTC_ALARM_RAISE, (cs_uint64)0);

	//led �ƴ���
	loop_back_led_proc(loop_status);

	#if 0
	//���������·���ƣ���رջ�·�˿�
	loop_back_port_control(port_infor->port_id);
	#endif
	
	return GWD_E_OK;
}

gwd_status
loop_back_cancel_proc(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor

)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	//����澯
	gwd_loop_back_alarm(vlan_id, port_infor, extern_port_infor, GWD_LOOP_CNACEL);

	//mib_uni_loop_back_alarm_clear(port_infor->slot_id,port_infor->port_id);
	ctc_oam_build_alarm_event(CTC_PORT_ALARM, port_infor->port_id, 0x304, CTC_ALARM_CLEAR, (cs_uint64)0);
	return GWD_E_OK;
}

static gw_int32 vlan_compare(void *data, gw_uint32 key)
{
	loop_back_infor_t *pentry = (loop_back_infor_t*)data;
	gw_uint16 *vlan = (gw_uint16 *)key;

	if(pentry->vlan_id == *vlan)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void
loop_back_detect_init(
	void
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	loop_control_config_t config;
	memset(&config, 0, sizeof(loop_control_config_t));
	
	loop_back_detect_config_set(&config);

	loop_back_status_list_lock_init();

	gw_lst_init(&g_loop_back_infor_list, vlan_compare);
	
	gwd_loopdetect_default_config();
	return;
}

gwd_status gwd_port_vlan_check(
	GWD_IN	gwd_port_id_t	port_id,
	GWD_IN	gw_uint16		vlan
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_ERROR;
	gwd_sdl_vlan_mode_t vlan_mode;
	gw_uint16 vlan_def;
	gw_uint16 vlan_cfg[PER_PORT_MAX_VLAN] = {0};
	gw_uint16 vlan_num = 0;
	
	gwd_onu_port_vlan_get(port_id, &vlan_mode, &vlan_def, vlan_cfg, &vlan_num);

	if(GWD_SDL_VLAN_MODE_TRANSPARENT == vlan_mode)
	{
		ret = GWD_E_OK;
	}
	else
	{
		if(vlan == vlan_def)
		{
			ret = GWD_E_OK;
		}
		else
		{
			int i = 0;
			for(i=0; i<vlan_num; i++)
			{
				if(vlan == vlan_cfg[i])
				{
					ret = GWD_E_OK;
					break;
				}
			}
		}
	}
	
	return ret;
}


//��ȡonu�˿���Ϣ
gwd_status
onu_port_infor_get(
	GWD_IN	gwd_port_id_t 			port_id,
	GWD_OUT	loop_back_port_info_t	*port_infor
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	//��ڹ�����
	if(NULL == port_infor)
	{
		return GWD_E_ERROR;
	}
	
	gwd_mac_t olt_mac;
	gwd_olt_type_t olt_type;
	gw_uint32 pon_slot;
	gw_uint32 pon_port;
	gw_uint16 onu_llid;
	gwd_mac_t onu_mac;
	
	gwd_olt_mac_get(&olt_mac);
	gwd_onu_mac_get(&onu_mac);
#ifdef	HAVE_LOOP_DETECT_ALARM_BASE_OLT
	if(olt_inform_onu_local_infor_enable_get())
	{
		onu_local_info_t onu_local_info;
		olt_inform_onu_local_infor_get(&onu_local_info);
		olt_type = onu_local_info.ucOltType;
		pon_slot = onu_local_info.ucOltSlot;
		pon_port = onu_local_info.ucOltPort;
		onu_llid = onu_local_info.ucOnuId;
	}
	else
	{
		gwd_olt_type_get(&olt_type);
		gwd_pon_slot_port_get(&pon_slot, &pon_port);
		gwd_mpcp_llid_get(&onu_llid);
	}
#else
	gwd_olt_type_get(&olt_type);
	gwd_pon_slot_port_get(&pon_slot, &pon_port);
	gwd_mpcp_llid_get(&onu_llid);
#endif

	port_infor->port_id = port_id;
	port_infor->slot_id = 1;
	port_infor->onu_slot = pon_slot;
	port_infor->onu_pon = pon_port;
	port_infor->onu_llid = onu_llid;
	port_infor->onu_type = gwd_onu_product_id_get();
	memcpy(port_infor->onu_mac, onu_mac.addr, GWD_MACADDR_LEN);
	port_infor->olt_type = olt_type;
	memcpy(port_infor->olt_mac, olt_mac.addr, GWD_MACADDR_LEN);
	
	return GWD_E_OK;
}

gwd_status
loop_back_port_control(
	GWD_IN	gwd_port_id_t	port_id
	)
{
	loop_control_config_t loop_control_config;

	//��ȡ��·��������
	loop_back_detect_config_get(&loop_control_config);
	if(loop_control_config.policy != 0)
	{
		gwd_port_admin_set(port_id, GWD_SDL_PORT_ADMIN_DOWN);
	}

	return GWD_E_OK;
}

int gwd_loopdetect_default_config(void)
{
/*default send CTC loop alarm ,if olt perform "loop-detection enable" ,send gwd loop alarm*/
		loop_control_config_t config;
		unsigned char mac[GWD_MACADDR_LEN] ={0x00,0x0f,0xe9,0x07,0x80,0x11};
		config.enable = 1;
		config.vid = 0;  
		config.interval = 1;
		config.policy = 1;
		memcpy(config.smac, mac, GWD_MACADDR_LEN);                                                                                             

		config.waitforwakeup = 60;
		
		config.maxwakeup = (gw_uint16)(-1);
		
		config.alarm_interval = (gw_uint16)(-1);
		config.clear_interval = 10;
		config.opposite_aging_time = 3;
		
	
#ifdef TEST
		cs_printf("config->enable :%u\n", config.enable);
		cs_printf("config->vid :%u\n", config.vid);
		cs_printf("config->interval :%u\n", config.interval);
		cs_printf("config->policy :%u\n", config.policy);
		cs_printf("config->waitforwakeup :%u\n", config.waitforwakeup);
		cs_printf("config->maxwakeup :%u\n", config.maxwakeup);
	
		cs_printf("config->smac : %02x:%02x:%02x:%02x:%02x:%02x\n", config.smac[0],
																	config.smac[1],
																	config.smac[2],
																	config.smac[3],
																	config.smac[4],
																	config.smac[5]);
#endif
	loop_back_detect_config_enable(&config);
	return 0;
}

#endif

/*end*/


