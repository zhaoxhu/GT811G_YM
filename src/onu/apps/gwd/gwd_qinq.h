#ifndef __GWD_QINQ_H__
#define __GWD_QINQ_H__
#include "cs_types.h"
#include "cs_cmn.h"

#ifdef HAVE_QINQ
#if 0
typedef enum
{
	VLAN = 0,
}QINQ_QUALIFIER;

typedef enum
{
	DOWN = 0,
	UP = 1,
}QINQ_DIRECTION;

typedef enum
{
	ORIGINAL = 0,
}QINQ_PRIORITY;

typedef enum
{
	EXCHANGE = 0,
	ATTACH,
}QINQ_ACTION;

//此结构用于tlv 保存,不能修改
typedef struct vlan_qinq_infor_s
{
	QINQ_QUALIFIER qualifier;
	int old_vid;
	QINQ_DIRECTION direction;
	QINQ_PRIORITY prio_source;
	int new_vid;
	int vlan_type;	//tpid
	QINQ_ACTION action;
}vlan_qinq_infor_t;
#else
typedef enum
{
	DOWN = 0,
	UP = 1,
}QINQ_DIRECTION;

typedef enum
{
	EXCHANGE = 0,
	ATTACH,
}QINQ_ACTION;

typedef struct vlan_qinq_infor_s
{
	int old_vid;
	QINQ_DIRECTION direction;
	int new_vid;
	QINQ_ACTION action;
}vlan_qinq_infor_t;
#endif

typedef struct qinq_node_s
{
	vlan_qinq_infor_t elem;
	struct qinq_node_s *next;
}qinq_node_t,*qinq_link_t;

typedef struct list_s
{
	qinq_link_t head;	//链表的头结点
	qinq_link_t tail;	//链表的尾结点
}qinq_list_t;


extern cs_status vlan_qinq_sw_table_init();
extern cs_status vlan_qinq_sw_table_add(qinq_list_t *list, vlan_qinq_infor_t vlan_entry);
extern cs_status vlan_qinq_sw_table_del(qinq_list_t *list, vlan_qinq_infor_t vlan_entry);
extern cs_status vlan_qinq_sw_table_clear(qinq_list_t *list);
extern cs_status vlan_qinq_sw_table_dump(qinq_list_t *list);
extern cs_status vlan_qinq_sw_table_get_length(qinq_list_t *list, int *length);
extern cs_status vlan_qinq_sw_table_update_hw(qinq_list_t *list);
extern cs_status vlan_qinq_hw_table_add(vlan_qinq_infor_t vlan_entry);
extern cs_status vlan_qinq_hw_table_del(vlan_qinq_infor_t vlan_entry);
extern cs_status vlan_qinq_hw_table_clear(qinq_list_t *list);
extern cs_status vlan_qinq_hw_table_dump(void);
extern cs_status pkt_qinq_vlan_translate(cs_pkt_t *pkt);
#endif

#endif	/*end of the file*/
