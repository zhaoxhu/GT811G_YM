/***********************************************************************/
/* This file contains unpublished documentation and software                                            */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,                              */
/* in whole or in part, of the information in this file without a                                              */
/* written consent of an officer of Cortina Systems Incorporated is                                     */
/* strictly prohibited.                                                                                                             */
/* Copyright (c) 2002-2010 by Cortina Systems Incorporated.                                            */
/***********************************************************************/

#ifndef _UTIL_LIST_H_
#define _UTIL_LIST_H_

#include "cs_types.h"

typedef struct node_t
{
    struct node_t * previous;
    struct node_t * next;
}cs_node;

typedef struct list
{
    cs_node node;
    cs_uint32 count;
    cs_int32 (*compare)(void * , cs_uint32);
}cs_list;

#define cs_tail node.previous
#define cs_head node.next

extern void cs_lst_init(cs_list * pLst , cs_int32 (*compare)(void * , cs_uint32));
extern void cs_lst_insert(cs_list * pLst, cs_node * pPrevious, cs_node * pNode);
extern void cs_lst_add(cs_list * pLst, cs_node * pNode);
cs_node *cs_lst_remove(cs_list * pLst , cs_uint32 key);
extern void cs_lst_delete(cs_list * pLst, cs_node * pNode);
extern cs_node * cs_lst_first(cs_list * pLst);
extern cs_node * cs_lst_last(cs_list * pLst);
extern cs_node * cs_lst_get(cs_list * pLst);
extern cs_node * cs_lst_nth(cs_list * pLst, int nodeNum);
cs_node *cs_lst_find(cs_list *pLst , cs_uint32 key);
extern cs_node * cs_lst_prev(cs_node * pNode);
extern cs_node * cs_lst_next(cs_node * pNode);
extern cs_uint32 cs_lst_count(cs_list * pLst);
extern cs_list *cs_lst_concat(cs_list *pDst , cs_list *pSrc);

#define cs_lst_scan(pList , pNode , type) for(pNode=(type)cs_lst_first(pList);pNode;pNode=(type)cs_lst_next((cs_node *)pNode))


#endif

