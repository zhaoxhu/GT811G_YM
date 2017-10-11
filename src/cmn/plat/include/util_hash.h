#ifndef _CS_HASH_H_
#define _CS_HASH_H_

typedef cs_node cs_hash_node;

typedef struct {
    cs_uint32 hash_size;
    cs_uint32 hash_mutex_id;
    cs_uint32 (*hashkey_gen)(void *pNode);
    cs_int32 (*compare)(void *pNode , cs_uint32 key);
    cs_uint32 count;
    cs_list hash_list[1];
} cs_hash_tbl;

#define CS_HASH_OK 0
#define CS_HASH_ERROR -1

#define CS_INVALID_HASHTBL_ID 0

#define HASH_TBL_LOCK(p) do{if(p) cs_mutex_lock(p->hash_mutex_id);} while(0)
#define HASH_TBL_UNLOCK(p) do{if(p) cs_mutex_unlock(p->hash_mutex_id);} while(0)

#define cs_scan_hash_bucket(pHashTbl , hash_index) \
                    for(hash_index = 0 ; hash_index < pHashTbl->hash_size ; hash_index++)

#define cs_scan_hash_record(pLst , pNode , type) \
                    for(pNode=(type)cs_lst_first(pLst);pNode;pNode=(type)cs_lst_next((cs_node *)pNode))

extern cs_hash_tbl *cs_hash_create
                                        (
                                        cs_uint32 hash_size ,
                                        cs_uint32(*hashkey_gen)(void *pNode),
                                        cs_int32 (*compare)(void *pNode , cs_uint32 key)
                                        );
extern cs_int32 cs_hash_destroy(cs_hash_tbl *pHashTbl);
extern cs_int32 cs_hash_add(cs_hash_tbl * pHashTbl , void *pNode);          
extern cs_int32 cs_hash_del(cs_hash_tbl * pHashTbl , void *pNode);
extern cs_int32 cs_hash_first
                                        (
                                        cs_hash_tbl * pHashTbl ,
                                        void **pFirst ,
                                        cs_uint32 *bucket_idx
                                        );
extern cs_int32 cs_hash_next
                                        (
                                        cs_hash_tbl * pHashTbl ,
                                        void *pPrev,
                                        cs_uint32 prev_bucket_idx,
                                        void **pNext ,
                                        cs_uint32 *bucket_idx
                                        );
extern cs_hash_node *cs_hash_find
                                        (
                                        cs_hash_tbl * pHashTbl ,
                                        cs_uint32 key
                                        );
extern cs_hash_node *cs_hash_get(cs_hash_tbl * pHashTbl);
extern cs_uint32 cs_hash_count(cs_hash_tbl * pHashTbl);
#endif

