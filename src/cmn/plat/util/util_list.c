/***********************************************************************/
/* This file contains unpublished documentation and software                                            */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,                              */
/* in whole or in part, of the information in this file without a                                              */
/* written consent of an officer of Cortina Systems Incorporated is                                     */
/* strictly prohibited.                                                                                                             */
/* Copyright (c) 2002-2010 by Cortina Systems Incorporated.                                            */
/***********************************************************************/
/******************************************************
*
* Example:
*
*
*
*    typedef struct{
*        cs_node node;
*        int a;
*        int b;
*        int c[100];
*    }structA;
*
*   cs_list myList;
*
*   lstInit(&myList);
*   for(i = 0 ; i < 50 ; i++)
*   {
*        structA *p;
*        p = (structA *)iros_malloc(pool, sizeof(structA));
*        p->a = i;
*        p->b = i;
*        lstAdd(&myList , (cs_node *)p);
*   }
*
*   for(p = (structA *)lstFirst(&myList) ; p ; p = (structA *)lstNext((cs_node *)p))
*   {
*        <scan the list>
*   }

Description:
    init list with the compare function, the compare function is necessary for
    remove the specific node and find the specific node
    compare function has two arguments,Eg.
    compare(pNodeA , pNodeB)
    if(pNodeA < pNodeB) return -1;
    if(pNodeA = pNodeB) return 0;
    if(pNodeA > pNodeB) return 1;
    typedef struct {
        cs_node node;
        int key1;
        int key2;
        int key3;
        char data[1000];
    }tStruct;

    yourself_compre((tStruct *)TA , (tStruct *)TB)
    {
        if(TA->key1 > TB->key1) return 1;
        else if(TA->key1 < TB->key1) return -1;
        else ;

        if(TA->key2 > TB->key2) return 1;
        else if(TA->key2 < TB->key2) return -1;
        else ;

        if(TA->key3 > TB->key3) return 1;
        else if(TA->key3 < TB->key3) return -1;
        else return 0;
    }
**********************************************************/

#include "cs_types.h"
#include "osal_common.h"
#include "util_list.h"


void cs_lst_init(cs_list * pLst , cs_int32 (*compare)(void * , cs_uint32))
{
    pLst->cs_head = NULL;
    pLst->cs_tail = NULL;
    pLst->count = 0;
    pLst->compare = compare;
}

/**********************************************************
Description:
   insert a new node 'pNode' after  the specific node  'pPrevious'
**********************************************************/
void cs_lst_insert(cs_list * pLst, cs_node * pPrevious, cs_node * pNode)
{
    cs_node * pNext = NULL;

    if (NULL == pPrevious) {
        pNext = pLst->cs_head;
        pLst->cs_head = pNode;
    } else {
        pNext = pPrevious->next;
        pPrevious->next = pNode;
    }

    if (NULL == pNext) {
        pLst->cs_tail = pNode;
    } else {
        pNext->previous = pNode;
    }

    pNode->previous = pPrevious;
    pNode->next = pNext;

    pLst->count++;
}

/**********************************************************
Description:
   add a new node to the tail of 'pLst'
**********************************************************/
void cs_lst_add(cs_list * pLst, cs_node * pNode)
{
    cs_lst_insert(pLst, pLst->cs_tail, pNode);
}

/**********************************************************
Description:
   Find the actual node according to the 'key' and remove it from 'pLst',
   The operatoin doesn't mean to free the node , it is only cut off from 'pLst'
**********************************************************/
cs_node *cs_lst_remove(cs_list * pLst , cs_uint32 key)
{
    cs_node *remove_node = (cs_node *)NULL;

    if (pLst->compare == NULL) {
        return (cs_node *)remove_node;
    }

    cs_lst_scan(pLst , remove_node , cs_node *) {
        if (pLst->compare((void *)remove_node , key) == 0) {
            cs_lst_delete(pLst , remove_node);
            return (cs_node *)remove_node;
        }
    }

    return (cs_node *)NULL;
}

/**********************************************************
Description:
   The operatoin doesn't mean to free the node , it is only cut off from 'pLst'
**********************************************************/
void cs_lst_delete(cs_list * pLst, cs_node * pNode)
{
    if (pNode->previous == NULL) {
        pLst->cs_head = pNode->next;
    } else {
        pNode->previous->next = pNode->next;
    }

    if (pNode->next == NULL) {
        pLst->cs_tail = pNode->previous;
    } else {
        pNode->next->previous = pNode->previous;
    }

    pLst->count--;
}

cs_node * cs_lst_first(cs_list * pLst)
{
    return pLst->cs_head;
}

cs_node * cs_lst_last(cs_list * pLst)
{
    return pLst->cs_tail;
}

/**********************************************************
Description:
   get the pointer of the first node in the list , and the node will be cutted off
   from the header of the list
**********************************************************/
cs_node * cs_lst_get(cs_list * pLst)
{
    cs_node * pNode = NULL;

    pNode = pLst->cs_head;

    if (pNode != NULL) {
        pLst->cs_head = pNode->next;

        if (pNode->next == NULL) {
            pLst->cs_tail = NULL;
        } else {
            pNode->next->previous = NULL;
        }

        pLst->count--;
    }

    return pNode;
}


cs_node * cs_lst_nth(cs_list * pLst, int nodeNum)
{
    cs_node * pNode = NULL;

    if (nodeNum < 1 || nodeNum > pLst->count) {
        return NULL;
    }

    if (nodeNum < ((pLst->count + 1) >> 1)) {
        pNode = pLst->cs_head;

        while (--nodeNum > 0) {
            pNode = pNode->next;
        }
    } else {
        nodeNum = pLst->count - nodeNum;
        pNode = pLst->cs_tail;

        while (++nodeNum > 0) {
            pNode = pNode->previous;
        }
    }

    return pNode;
}

/**********************************************************
Description:
   Get the pointer of the actual node according to the 'key'
**********************************************************/
cs_node *cs_lst_find(cs_list *pLst , cs_uint32 key)
{
    cs_node *find_node = NULL;

    if (pLst->compare == NULL) {
        return (cs_node *)find_node;
    }

    cs_lst_scan(pLst , find_node , cs_node *) {
        if (pLst->compare((void *)find_node , key) == 0)
            return (cs_node *)find_node;
    }

    return (cs_node *)NULL;
}

cs_node * cs_lst_prev(cs_node * pNode)
{
    return pNode->previous;
}

cs_node * cs_lst_next(cs_node * pNode)
{
    return pNode->next;
}

cs_uint32 cs_lst_count(cs_list * pLst)
{
    return pLst->count;
}

cs_list *cs_lst_concat(cs_list *pDst , cs_list *pSrc)
{
    if(pDst == NULL || pSrc == NULL){
        return NULL;
    }

    if(pDst->compare != pSrc->compare) {
        return NULL;
    }

    if(pDst->count == 0) {
        memcpy(pDst , pSrc , sizeof(cs_list));
        return (cs_list *)pDst;
    }

    if(pSrc->count == 0) {
        return (cs_list *)pDst;
    }

    pDst->cs_tail->next = pSrc->cs_head;
    pSrc->cs_head->previous = pDst->cs_tail;
    pDst->cs_tail = pSrc->cs_tail;

    pDst->count += pSrc->count;

    cs_lst_init(pSrc , pSrc->compare);

    return (cs_list *)pDst;
}


