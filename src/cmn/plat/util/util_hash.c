/***********************************************************************/
/* This file contains unpublished documentation and software                                            */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,                              */
/* in whole or in part, of the information in this file without a                                              */
/* written consent of an officer of Cortina Systems Incorporated is                                     */
/* strictly prohibited.                                                                                                             */
/* Copyright (c) 2002-2010 by Cortina Systems Incorporated.                                            */
/***********************************************************************/

/************************************************************************
Example:

-Data structure define:
  typedef struct 
  {
        cs_hash_node  node;    //the declaration must be put on the header of your structure
        cs_int32  data[20];
        cs_int32  key;
  }tTest;

#define YOUR_HASH_SIZE 128

cs_uint32 hashkey_generate(void *pNode)
{
    tTest *pTest = (tTest *)pNode;

    return (pTest->key%YOUR_HASH_SIZE);
}

cs_int32 your_compare_func(void *pNode , cs_uint32 key)
{
    tTest *pTest = (tTest *)pNode;

    if(pTest->key > key) 
        return 1;
    else if(pTest->key < key)
        return -1;
    else
        return 0;
}

-- Create hash table
    cs_hash_tbl *your_hash_tbl = NULL;
    your_hash_tbl = cs_hash_create(YOUR_HASH_SIZE , hashkey_generate , your_compare_func);
    if(yor_hash_tbl == NULL)
        return ERROR;

-- Add a record to hash table
    tTest *add_node = NULL;
    add_node = (tTest *)malloc(sizeof(tTest));
    if(add_node == NULL)
        return ERROR;
    add_node->key = 123;
    cs_hash_add(your_hash_tbl , (void *)add_node);

-- Find a record from hash table
-- Del a record from hash table  
      tTest *pDelNode=NULL;
      pDelNode = cs_hash_find(your_hash_tbl , 123);
      if(pDelNode != NULL) {
        cs_hash_del(your_hash_tbl , (void *)pDelNode);
      }

-- scan hash table
    Method-1:(use 'cs_hash_first' and 'cs_hash_next')
        tTest *prev=NULL,*next=NULL;
        cs_uint32 prev_buck=0,next_buck=0;
        int ret = CS_HASH_ERROR;
        ret = cs_hash_first(your_hash_tbl , (void *)&prev , &prev_buck);
        while(ret == CS_HASH_OK)
        {
            next = NULL;
            next_buck = 0;
            ... //do what you want to do
            ret = cs_hash_next(your_hash_tbl , prev , prev_buck , (void *)&next , &next_buck);
            prev = next;
            prev_buck = next_buck;
        }

    Method-2:(use 'cs_scan_hash_bucket' and 'cs_scan_hash_record')(recommend)
        cs_int32 hash_index;
        tTest *pNode=NULL;
        cs_scan_hash_bucket(your_hash_tbl , hash_index)
            cs_scan_hash_record(&your_hash_tbl->hash_list[hash_index] , pNode , tTest *)
        {
            ...  //do what you want to do
        }    

-- flush all records in hash table
        tTest *pNode=NULL;
        while((pNode=(tTest *)cs_hash_get(your_hash_tbl)) != NULL)
        {
            ... //call your own free function to free the node
        }

************************************************************************/
#include "stdio.h"
#include "osal_common.h"
#include "util_hash.h"

cs_hash_tbl *cs_hash_create
    (
    cs_uint32 hash_size ,
    cs_uint32(*hashkey_gen)(void *pNode),
    cs_int32 (*compare)(void *pNode , cs_uint32 key)
    )
{
    cs_hash_tbl *pHashTbl = (cs_hash_tbl *)0;
    cs_int8 hash_name[20];
    cs_int32 i;

    if (hashkey_gen == NULL || compare == NULL || hash_size == 0) {
        return (cs_hash_tbl *)NULL;
    }

    pHashTbl = (cs_hash_tbl *)iros_malloc(IROS_MID_SYS ,
                                          sizeof(cs_hash_tbl) + (hash_size - 1) * sizeof(cs_list));
    if (pHashTbl == NULL) {
        return NULL;
    }
    memset(pHashTbl , 0 , sizeof(cs_hash_tbl));

    for (i = 0 ; i < hash_size ; i++) {
        cs_lst_init(&pHashTbl->hash_list[i] , NULL);
    }

    memset(hash_name , 0 , sizeof(hash_name));
    sprintf(hash_name , "hash%d" , (cs_uint32)pHashTbl);
    if (CS_E_OSAL_OK != cs_mutex_init(&pHashTbl->hash_mutex_id, hash_name , 0)) {
        iros_free(pHashTbl);
        return NULL;
    }

    pHashTbl->hashkey_gen = hashkey_gen;
    pHashTbl->compare = compare;
    pHashTbl->hash_size = hash_size;

    return (cs_hash_tbl *)pHashTbl;
}

cs_int32 cs_hash_destroy
    (
    cs_hash_tbl *pHashTbl
    )
{
    if(pHashTbl == NULL)
        return CS_E_OSAL_ERR;

    cs_mutex_destroy(pHashTbl->hash_mutex_id);

    iros_free(pHashTbl);

    return CS_E_OSAL_OK;
}


cs_int32 cs_hash_add(cs_hash_tbl * pHashTbl , void *pNode)
{
    cs_uint32 hash_key;

    if (pHashTbl == NULL || pNode == NULL) {
        return CS_HASH_ERROR;
    }

    hash_key = pHashTbl->hashkey_gen((void *)pNode);
    if (hash_key >= pHashTbl->hash_size) {
        return CS_HASH_ERROR;
    }

    HASH_TBL_LOCK(pHashTbl);
    cs_lst_add(&pHashTbl->hash_list[hash_key] , (cs_node *)pNode);
    pHashTbl->count++;
    HASH_TBL_UNLOCK(pHashTbl);

    return CS_HASH_OK;
}

cs_int32 cs_hash_del(cs_hash_tbl * pHashTbl , void *pNode)
{
    int hash_key;

    if (pHashTbl == NULL || pNode == NULL) {
        return CS_HASH_ERROR;
    }

    hash_key = pHashTbl->hashkey_gen((void *)pNode);
    if (hash_key >= pHashTbl->hash_size) {
        return CS_HASH_ERROR;
    }


    HASH_TBL_LOCK(pHashTbl);
    cs_lst_delete(&pHashTbl->hash_list[hash_key] , (cs_node *)pNode);
    pHashTbl->count--;
    HASH_TBL_UNLOCK(pHashTbl);

    return CS_HASH_OK;
}

cs_int32 cs_hash_first
    (
    cs_hash_tbl * pHashTbl ,
    void **pFirst ,
    cs_uint32 *bucket_idx
    )
{
    int hash_key;

    if (pHashTbl == NULL) {
        return CS_HASH_ERROR;
    }

    HASH_TBL_LOCK(pHashTbl);
    cs_scan_hash_bucket(pHashTbl, hash_key) {
        if (cs_lst_count(&pHashTbl->hash_list[hash_key])) {
            *pFirst = (void *)cs_lst_first(&pHashTbl->hash_list[hash_key]);
            *bucket_idx = hash_key;
            HASH_TBL_UNLOCK(pHashTbl);
            return CS_HASH_OK;
        }
    }
    HASH_TBL_UNLOCK(pHashTbl);

    return CS_HASH_ERROR;
}

cs_int32 cs_hash_next
    (
    cs_hash_tbl * pHashTbl ,
    void *pPrev,
    cs_uint32 prev_bucket_idx,
    void **pNext ,
    cs_uint32 *bucket_idx
    )
{
    int hash_key;

    if (pHashTbl == NULL) {
        return CS_HASH_ERROR;
    }

    HASH_TBL_LOCK(pHashTbl);
    *pNext = (void *)cs_lst_next((cs_node *)pPrev);
    if (*pNext == NULL) {
        if (prev_bucket_idx >= pHashTbl->hash_size) {
            HASH_TBL_UNLOCK(pHashTbl);
            return CS_HASH_ERROR;
        } else {
            for (hash_key = prev_bucket_idx + 1 ; hash_key < pHashTbl->hash_size ; hash_key++) {
                *pNext = (void *)cs_lst_first(&pHashTbl->hash_list[hash_key]);
                if (*pNext != NULL) {
                    *bucket_idx = hash_key;
                    HASH_TBL_UNLOCK(pHashTbl);
                    return CS_HASH_OK;
                }
            }
        }
    } else {
        *bucket_idx = prev_bucket_idx;
        HASH_TBL_UNLOCK(pHashTbl);
        return CS_HASH_OK;
    }
    HASH_TBL_UNLOCK(pHashTbl);

    return CS_HASH_ERROR;
}

cs_hash_node *cs_hash_find
    (
    cs_hash_tbl * pHashTbl ,
    cs_uint32 key
    )
{
    cs_uint32 hash_key;
    cs_hash_node *pNode = NULL;

    if(pHashTbl == NULL)
    {
        return NULL;
    }

    HASH_TBL_LOCK(pHashTbl);
    cs_scan_hash_bucket(pHashTbl, hash_key)
        cs_scan_hash_record(&pHashTbl->hash_list[hash_key], pNode, cs_hash_node *)
    {
        if(pHashTbl->compare((void *)pNode , key) == 0) {
            HASH_TBL_UNLOCK(pHashTbl);
            return pNode;
        }
    }
    HASH_TBL_UNLOCK(pHashTbl);

    return NULL;
}

cs_hash_node *cs_hash_get(cs_hash_tbl * pHashTbl)
{
    cs_uint32 hash_key;
    cs_hash_node *pNode = NULL;
    
    if(pHashTbl == NULL) {
        return NULL;
    }

    HASH_TBL_LOCK(pHashTbl);
    cs_scan_hash_bucket(pHashTbl, hash_key)
    {
        pNode = (cs_hash_node *)cs_lst_get(&pHashTbl->hash_list[hash_key]);
        if(pNode == NULL) {
            continue;
        }

        pHashTbl->count--;
        HASH_TBL_UNLOCK(pHashTbl);
        return pNode;
    }
    
    HASH_TBL_UNLOCK(pHashTbl);
    return NULL;
}

cs_uint32 cs_hash_count(cs_hash_tbl * pHashTbl)
{
    if(pHashTbl == NULL) {
        return 0;
    }

    return pHashTbl->count;
}


