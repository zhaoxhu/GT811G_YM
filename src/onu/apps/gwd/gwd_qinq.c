#include "gwd_qinq.h"
#ifdef HAVE_QINQ
#include "sdl_vlan.h"
#ifdef HAVE_QINQ_CFG_RESTORE
#include "gwd_flash_oper.h"
#endif
#include "sdl_vlan_util.h"

qinq_list_t vlan_qinq_list_running;
qinq_list_t *pVlan_qinq_list_running = &vlan_qinq_list_running;
qinq_list_t vlan_qinq_list_config;
qinq_list_t *pVlan_qinq_list_config = &vlan_qinq_list_config;

extern int vlan_entry_add_cmp(vlan_qinq_infor_t vlan_entry1, vlan_qinq_infor_t vlan_entry2)
{
	if(vlan_entry1.old_vid != vlan_entry2.old_vid)
	{
		return 0;
	}

	if(vlan_entry1.direction != vlan_entry2.direction)
	{
		return 0;
	}

	return 1;
}

extern int vlan_entry_del_cmp(vlan_qinq_infor_t vlan_entry1, vlan_qinq_infor_t vlan_entry2)
{
	if(vlan_entry1.old_vid != vlan_entry2.old_vid)
	{
		return 0;
	}

	if(vlan_entry1.direction != vlan_entry2.direction)
	{
		return 0;
	}

	if(vlan_entry1.new_vid != vlan_entry2.new_vid)
	{
		return 0;
	}

	if(vlan_entry1.action != vlan_entry2.action)
	{
		return 0;
	}

	return 1;
}

extern cs_status vlan_qinq_table_search(qinq_list_t *list, vlan_qinq_infor_t vlan_entry, int (*vlan_entry_cmp)(vlan_qinq_infor_t, vlan_qinq_infor_t), int *found)
{
	//入口参数检查，通过则继续处理，否则推出并返回错误
	if(NULL == list)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	
	if(NULL == vlan_entry_cmp)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	if(NULL == found)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	
	//遍历链表, 用比较函数比较
	qinq_link_t link = NULL;

	*found = 0;
	for(link = list->head; NULL != link; link = link->next)
	{
		//根据查找结果设置标志位，退出
		if(1 == (*vlan_entry_cmp)(link->elem, vlan_entry))
		{
			*found = 1;
			break;
		}
		else
		{
			//do nothing
		}
	}
	
	return CS_E_OK;
}

extern cs_status vlan_qinq_sw_table_init()
{

	qinq_list_t *list = pVlan_qinq_list_running;
	//入口参数检查，通过则继续处理，否则推出并返回错误
	if(NULL == list)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	//设置链表的长度为0，并设置头指针为空，设置尾指针为空
	list->head = NULL;
	list->tail = NULL;
	
	return CS_E_OK;
}

extern cs_status vlan_qinq_sw_table_add(qinq_list_t *list, vlan_qinq_infor_t vlan_entry)
{
	//入口参数检查，通过则继续处理，否则推出并返回错误
	if(NULL == list)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	
	//查找链表，确定元素是否可以添加
	int found = 0;
	if(CS_E_OK == vlan_qinq_table_search(list, vlan_entry, vlan_entry_add_cmp, &found))
	{
		//没有找到冲突的元素，可以添加
		if(0 == found)
		{
			//申请节点空间，加入元素
			qinq_link_t link = NULL;
			link = (qinq_node_t *)malloc(sizeof(qinq_node_t));
			if(NULL == link)
			{
				cs_printf("in %s\n", __func__);
				cs_printf("malloc failed\n");
				return CS_E_ERROR;
			}
			memcpy(&(link->elem), &vlan_entry, sizeof(vlan_qinq_infor_t));		

			//将节点加入链表
			link->next = NULL;
			if(NULL == list->head)
			{
				list->head = link;
				list->tail = list->head;
			}
			else
			{
				list->tail->next = link;
				list->tail = link;
			}
		}
		else
		{
			cs_printf("vlan entry conflict!\n");
			return CS_E_ERROR;
		}
	}
	else
	{
		cs_printf("vlan_qinq_table_search err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);		
		return CS_E_ERROR;
	}

	
	return CS_E_OK;
}
extern cs_status vlan_qinq_sw_table_del(qinq_list_t *list, vlan_qinq_infor_t vlan_entry)
{
	//入口规则检查
	if(NULL == list)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	//查找链表，确定元素是否可以删除
	qinq_link_t link = NULL;
	qinq_link_t link_previou = NULL;

	for(link=list->head; NULL!=link; link_previou=link, link=link->next)
	{
		//如果可以删除，则删除
		if(1 == vlan_entry_del_cmp(link->elem, vlan_entry))
		{
			if(list->head == list->tail)
			{
				list->head = NULL;
				list->tail = NULL;
			}
			else
			{
				if(list->head == link)
				{
					list->head = link->next;
				}
				else if(list->tail == link)
				{
					list->tail = link_previou;
					list->tail->next = NULL;
				}
				else
				{
					link_previou->next = link->next;
				}
			}
			free(link);
			link = NULL;
			return CS_E_OK;
		}
		else
		{
			continue;
		}
	}
		
		//否则返回错误，并退出
	cs_printf("vlan entry not found !");
	return CS_E_ERROR;
}

extern cs_status vlan_qinq_sw_table_dump(qinq_list_t *list)
{
	//入口规则检查
	if(NULL == list)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	
	//打印提示信息
	cs_printf("%s\t %s\t %s\t %s\t %s\t %s\t %s\t %s\n", "count", "qualifier", "old-vid", "direction", "prio-source", "new-vid", "vlan-type", "action");
	//遍历链表
	qinq_link_t link = NULL;
	vlan_qinq_infor_t vlan_entry;
	int count = 0;
	char qualifier[10]= {0};
	int old_vid = 0;
	char direction[10] = {0};
	char prio_source[15] = {0};
	int new_vid = 0;
	int vlan_type = 0;
	char action[15] = {0};

	for(link = list->head, count = 1; NULL != link; link = link->next, count++)
	{
		//打印元素信息
		vlan_entry = link->elem;
		
		strcpy(qualifier, "VLAN");
		old_vid = vlan_entry.old_vid;
		if(DOWN == vlan_entry.direction)
		{
			strcpy(direction, "DOWN");
		}
		else
		{
			strcpy(direction, "UP");
		}

		strcpy(prio_source, "ORIGINAL");

		new_vid = vlan_entry.new_vid;
		vlan_type = 0x8100;
		if(EXCHANGE == vlan_entry.action)
		{
			strcpy(action, "EXCHANGE");
		}
		else if(ATTACH == vlan_entry.action)
		{
			strcpy(action, "ATTACH");
		}
		else
		{
			
		}
		cs_printf("%d\t %s\t\t %d\t\t %s\t\t %s\t %d\t\t 0x%x\t\t %s\n", count, qualifier, old_vid, direction, prio_source, new_vid, vlan_type, action);
	}
		
	return CS_E_OK;
}

extern cs_status vlan_qinq_sw_table_get_length(qinq_list_t *list, int *length)
{
	//入口规则检查
	if(NULL == list)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	if(NULL == length)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	//获取链表长度
	qinq_link_t link = NULL;
	int count = 0;
	for(link=list->head; NULL!=link; link=link->next)
	{
		count++;
	}
	*length = count;
	
	return CS_E_OK;
}



extern cs_status vlan_qinq_sw_table_update_hw(qinq_list_t *list)
{
	cs_status ret = CS_E_OK;
	//入口规则检查
	if(NULL == list)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	qinq_link_t link = NULL;
	vlan_qinq_infor_t vlan_entry;

	for(link=list->head; NULL!=link; link=link->next)
	{
		vlan_entry = link->elem;
		if(CS_E_OK == vlan_qinq_hw_table_add(vlan_entry))
		{
			//do nothing	
		}
		else
		{
			cs_printf("in %s\n", __func__);
			cs_printf("vlan_qinq_hw_table_add err!\n");
			ret = CS_E_ERROR;
		}
		
	}

	return ret;
}

extern cs_status vlan_qinq_hw_table_add(vlan_qinq_infor_t vlan_entry)
{
	cs_port_id_t port_id;
	int oper_count = 0;
	cs_sdl_vlan_oper_t vlan_oper[__VLAN_RULE_PER_PORT_MAX];
	QINQ_DIRECTION direction;
	QINQ_ACTION action;
	int old_vid = 0;
	int new_vid = 0;

	//获取配置
	direction = vlan_entry.direction;
	action = vlan_entry.action;
	old_vid = vlan_entry.old_vid;
	new_vid = vlan_entry.new_vid;
	
	switch(direction)
	{
		case UP:
			port_id = CS_UNI_PORT_ID1;
			vlan_oper[oper_count].vlan_id = old_vid;
			vlan_oper[oper_count].op_vid= new_vid;
			break;
		case DOWN:
			port_id = CS_PON_PORT_ID;
			vlan_oper[oper_count].vlan_id = old_vid;
			vlan_oper[oper_count].op_vid= new_vid;
			break;
		default:
			cs_printf("in %s\n", __func__);
			cs_printf("direction wrong :%d\n", direction);
			return CS_E_ERROR;
			break;
	}
		
	switch(action)
	{
		case EXCHANGE:
			vlan_oper[oper_count].vlan_cmd = SDL_VLAN_CMD_SWAP;
			break;
		case ATTACH:
			if(UP == direction)
			{
				vlan_oper[oper_count].vlan_cmd = SDL_VLAN_CMD_PUSH;
			}
			else
			{
				vlan_oper[oper_count].vlan_cmd = SDL_VLAN_CMD_POP;
			}
			
			break;
		default:
			cs_printf("in %s\n", __func__);
			cs_printf("direction wrong :%d\n", direction);
			return CS_E_ERROR;
			break;
	}
	oper_count = 1;

	//将配置更新到硬件
	if(CS_E_OK != (vlan_member_add(port_id, oper_count, vlan_oper)))
	{
		cs_printf("in %s\n", __func__);
		cs_printf("vlan_member_add err, port_id :%d\n", port_id);
		return CS_E_ERROR;
	}
	else
	{
		//执行成功，不做处理
	}
	
	return CS_E_OK;
}


extern cs_status vlan_qinq_hw_table_del(vlan_qinq_infor_t vlan_entry)
{
	cs_port_id_t port_id;
	int oper_count = 0;
	cs_uint16 vlan_oper[__VLAN_RULE_PER_PORT_MAX];
	QINQ_DIRECTION direction;
	int old_vid = 0;
	
	oper_count = 0;
	direction = vlan_entry.direction;
	old_vid = vlan_entry.old_vid;
	switch(direction)
	{
		case UP:
			port_id = CS_UNI_PORT_ID1;
			vlan_oper[oper_count] = old_vid;
			break;
		case DOWN:
			port_id = CS_PON_PORT_ID;
			vlan_oper[oper_count] = old_vid;
			break;
		default:
			cs_printf("in %s\n", __func__);
			cs_printf("direction wrong :%d\n", direction);
			return CS_E_ERROR;
			break;
	}
	oper_count = 1;

	//将配置更新到硬件
	if(CS_E_OK != (vlan_member_del(port_id, oper_count, vlan_oper)))
	{
		cs_printf("in %s\n", __func__);
		cs_printf("vlan_member_del err, port_id :%d\n", port_id);
		return CS_E_ERROR;
	}
	else
	{
		//执行成功，不做处理
	}

	return CS_E_OK;
}

extern cs_status vlan_qinq_sw_table_clear(qinq_list_t *list)
{
	//入口规则检查
	if(NULL == list)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	if(NULL != list->head)
	{
		qinq_link_t link_previous = NULL;
		qinq_link_t link = NULL;

		link = list->head;
		do
		{
			link_previous = link;
			link = link->next;
			
			free(link_previous);
		}while(NULL != link);

		//设置链表头
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		//链表为空
	}
	return CS_E_OK;
}

extern cs_status vlan_qinq_hw_table_clear(qinq_list_t *list)
{
	cs_status ret = CS_E_OK;
	//入口规则检查
	if(NULL == list)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	qinq_link_t link = NULL;
	vlan_qinq_infor_t vlan_entry;

	for(link=list->head; NULL!=link; link=link->next)
	{
		vlan_entry = link->elem;
		if(CS_E_OK == vlan_qinq_hw_table_del(vlan_entry))
		{
			//do nothing	
		}
		else
		{
			cs_printf("in %s\n", __func__);
			cs_printf("vlan_qinq_hw_table_del err!\n");
			ret = CS_E_ERROR;
		}
		
	}

	return ret;
}



extern cs_status vlan_qinq_hw_port_dump(cs_port_id_t port_id)
{
	cs_status ret = 0;
	cs_status state = CS_E_OK;
	cs_uint16 vlan_num = 0;
	cs_sdl_vlan_oper_t vlanid_list[__VLAN_RULE_PER_PORT_MAX];
	int i = 0;
	int old_vid = 0;
	int new_vid = 0;
	char *vlan_oper_str[] = {"nop", "push", "pop", "swap", "l2"};
	int vlan_oper = 0;
	state = vlan_member_get(port_id, &vlan_num, vlanid_list);
	if(CS_E_OK == state)
	{
		for(i=0; i<vlan_num; i++)
		{
			old_vid = vlanid_list[i].vlan_id;
			new_vid = vlanid_list[i].op_vid;
			vlan_oper = vlanid_list[i].vlan_cmd;
			cs_printf("old_vid :%d, new_vid :%d, vlan_oper :%s\n", old_vid, new_vid, vlan_oper_str[vlan_oper]);
		}
	}
	else
	{
		
	}
	
	return ret;
}

extern cs_status vlan_qinq_hw_table_dump(void)
{
	cs_printf("\ndebug information from hardware:\n");
	cs_printf("------------------------------------------------------------------\n");
	cs_printf("up :\n");
	vlan_qinq_hw_port_dump(CS_UNI_PORT_ID1);
	cs_printf("down :\n");
	vlan_qinq_hw_port_dump(CS_PON_PORT_ID);
	cs_printf("------------------------------------------------------------------\n");
	return CS_E_OK;
}

extern cs_status vlan_qinq_table_cpy(qinq_list_t *list_dest, qinq_list_t *list_src)
{
	cs_status ret = CS_E_OK;
	//入口规则检查
	if(NULL == list_dest)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	if(NULL == list_src)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	//遍历链表
	qinq_link_t link = NULL;
	for(link=list_src->head; NULL!=link; link=link->next)
	{
		//将原链表的每个元素加入目的链表
		if(CS_E_OK != vlan_qinq_sw_table_add(list_dest, link->elem))
		{
			cs_printf("in %s\n", __func__);
			cs_printf("vlan_qinq_sw_table_add err!\n");
			ret = CS_E_ERROR;
			break;	
		}
		else
		{	
			//do nothing
		}
	}
		
	return ret;
}

extern cs_status vlan_qinq_table_recover(qinq_list_t *list)
{
	if(CS_E_OK != vlan_qinq_table_cpy(pVlan_qinq_list_running, list))
	{
		cs_printf("vlan_qinq_table_recover err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	
	if(CS_E_OK != vlan_qinq_sw_table_update_hw(pVlan_qinq_list_running))
	{
		cs_printf("vlan_qinq_table_recover err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	
	return CS_E_OK;
}

extern cs_status vlan_qinq_sw_table_exchange_search_new_vid_by_old_vid_and_direction(qinq_list_t *list, int old_vid, QINQ_DIRECTION direction, int *new_vid)
{
	//入口规则检查
	if((NULL == list)||(NULL == new_vid))
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	cs_status ret = CS_E_ERROR;
	//遍历链表
	qinq_link_t link = NULL;
	vlan_qinq_infor_t vlan_entry;
	int count = 0;
	
	for(link = list->head, count = 1; NULL != link; link = link->next, count++)
	{
		//打印元素信息
		vlan_entry = link->elem;

		if((EXCHANGE == vlan_entry.action)&&(vlan_entry.old_vid == old_vid)&&(vlan_entry.direction == direction))
		{
			*new_vid = vlan_entry.new_vid;
			ret = CS_E_OK;
		}
	}
		
	return ret;
}

extern void pop_vlan(cs_uint8 * in,cs_uint8 * out,cs_uint32 * len);
extern void push_vlan(cs_uint16 vlan, cs_uint8 *in, cs_uint8 *out,cs_uint32* len);
extern cs_status pkt_qinq_vlan_translate(cs_pkt_t *pkt)
{
	cs_status ret = CS_E_OK;
	
	int old_vid = 0;
	QINQ_DIRECTION direction;
	int new_vid = 0;
	cs_uint8 *eth_pkt = NULL;
	cs_uint32 eth_len = 0;
	cs_uint16 src_port = 0;
	
	eth_pkt = pkt->data + pkt->offset;
	eth_len = pkt->len;
	src_port = pkt->port;
	old_vid = pkt->svlan;
	if(src_port == CS_PON_PORT_ID)
	{
		direction = DOWN;
	}
	else
	{
		direction = UP;
	}
	
	if(CS_E_OK == vlan_qinq_sw_table_exchange_search_new_vid_by_old_vid_and_direction(pVlan_qinq_list_running, old_vid, direction, &new_vid))
	{
		pop_vlan(eth_pkt, eth_pkt, &eth_len);
		push_vlan(new_vid, eth_pkt, eth_pkt, &eth_len);
		pkt->len = eth_len;
		
		ret = CS_E_OK;
	}
	else
	{
		ret = CS_E_ERROR;
	}

	return ret;
}

#ifdef HAVE_QINQ_CFG_RESTORE
//tlv
extern int vlan_qinq_table_tlv_infor_get(int *len, char **value, int *free_need)
{
	int ret = 0;
	//入口规则检查
	if(NULL == len)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		goto err;
	}
	else
	{
		*len = 0;
	}
	if(NULL == value)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		goto err;
	}
	else
	{
		*value = NULL;
	}
	if(NULL == free_need)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		goto err;
	}
	else
	{
		*free_need = 0;
	}
	
	
	//获取申请空间的大小
	int size = 0;
	qinq_list_t *list = pVlan_qinq_list_running;
	int length = 0;
	if(CS_E_OK != vlan_qinq_sw_table_get_length(list, &length))
	{
		goto err;
	}

	if(0 == length)
	{
		goto end;
	}
	size = length * sizeof(vlan_qinq_infor_t);
	
	//申请空间
	vlan_qinq_infor_t *pElem = NULL;
	if(NULL == (pElem = (vlan_qinq_infor_t *)iros_malloc(IROS_MID_APP, size)))
	{
		goto err;
	}

	//遍历链表，复制信息到指定的空间
	qinq_link_t link = NULL;
	vlan_qinq_infor_t elem;
	vlan_qinq_infor_t *pElem_offset = pElem;
	for(link=list->head; NULL!=link; link=link->next)
	{
		elem = link->elem;
		memcpy(pElem_offset, &elem, sizeof(elem));
		pElem_offset++;
	}
	
	//设置对外输出的变量值
	*len = size;
	*value = (char *)pElem;
	*free_need = 1;
err:
	ret = -1;
end:
	return ret;
}

extern int vlan_qinq_table_tlv_infor_handle(int len, char *data, int opcode)
{
	int ret = 0;
	//入口规则检查
	if(0 == len)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		ret = -1;
		goto end;
	}

	if(NULL == data)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		ret = -1;
		goto end;
	}

	qinq_list_t *list_config = NULL;
	list_config = pVlan_qinq_list_config;
	//清空配置表
	vlan_qinq_sw_table_clear(list_config);
	
	//将数据恢复到配置表
	vlan_qinq_infor_t elem;
	int elem_num = 0;
	vlan_qinq_infor_t *pElem = NULL;
	vlan_qinq_infor_t *pElem_offset = NULL;

	pElem = (vlan_qinq_infor_t *)data;
	pElem_offset = pElem;
	elem_num = len / sizeof(elem);
	int i = 0;
	for(i=0; i<elem_num; i++)
	{
		memcpy(&elem, pElem_offset, sizeof(elem));
		vlan_qinq_sw_table_add(list_config, elem);
		pElem_offset++;
	}
	
	//根据操作码完成操作
	if(DATA_RECOVER == opcode)
	{
		//恢复配置
		if(CS_E_OK != vlan_qinq_table_recover(list_config))
		{
			cs_printf("in %s, line :%d\n", __func__, __LINE__);
			ret = -1;
		}
		else
		{
			//do nothing
		}
	}
	else if(DATA_SHOW == opcode)
	{
		//显示配置
		if(CS_E_OK != vlan_qinq_sw_table_dump(list_config))
		{
			cs_printf("in %s, line :%d\n", __func__, __LINE__);
			ret = -1;
		}
		else
		{
			//do nothing
		}
	}
	else
	{
		ret = -1;
	}
	
end:
	return ret;
}

extern int vlan_qinq_running_config_show(void)
{
	vlan_qinq_sw_table_dump(pVlan_qinq_list_running);
	return 0;
}
#endif	/*(#ifdef HAVE_QINQ_CFG_RESTORE)*/

#endif	/*(#ifdef HAVE_QINQ)*/


