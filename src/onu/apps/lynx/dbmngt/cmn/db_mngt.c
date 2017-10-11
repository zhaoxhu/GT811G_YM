/****************************************************************************
            Software License for Customer Use of Cortina Software
                          Grant Terms and Conditions

IMPORTANT NOTICE - READ CAREFULLY: This Software License for Customer Use
of Cortina Software ("LICENSE") is the agreement which governs use of
software of Cortina Systems, Inc. and its subsidiaries ("CORTINA"),
including computer software (source code and object code) and associated
printed materials ("SOFTWARE").  The SOFTWARE is protected by copyright laws
and international copyright treaties, as well as other intellectual property
laws and treaties.  The SOFTWARE is not sold, and instead is only licensed
for use, strictly in accordance with this document.  Any hardware sold by
CORTINA is protected by various patents, and is sold but this LICENSE does
not cover that sale, since it may not necessarily be sold as a package with
the SOFTWARE.  This LICENSE sets forth the terms and conditions of the
SOFTWARE LICENSE only.  By downloading, installing, copying, or otherwise
using the SOFTWARE, you agree to be bound by the terms of this LICENSE.
If you do not agree to the terms of this LICENSE, then do not download the
SOFTWARE.

DEFINITIONS:  "DEVICE" means the Cortina Systems? Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems? SDK software.

GRANT OF LICENSE:  Subject to the restrictions below, CORTINA hereby grants
CUSTOMER a non-exclusive, non-assignable, non-transferable, royalty-free,
perpetual copyright license to (1) install and use the SOFTWARE for
reference only with the DEVICE; and (2) copy the SOFTWARE for your internal
use only for use with the DEVICE.

RESTRICTIONS:  The SOFTWARE must be used solely in conjunction with the
DEVICE and solely with Your own products that incorporate the DEVICE.  You
may not distribute the SOFTWARE to any third party.  You may not modify
the SOFTWARE or make derivatives of the SOFTWARE without assigning any and
all rights in such modifications and derivatives to CORTINA.  You shall not
through incorporation, modification or distribution of the SOFTWARE cause
it to become subject to any open source licenses.  You may not
reverse-assemble, reverse-compile, or otherwise reverse-engineer any
SOFTWARE provided in binary or machine readable form.  You may not
distribute the SOFTWARE to your customers without written permission
from CORTINA.

OWNERSHIP OF SOFTWARE AND COPYRIGHTS. All title and copyrights in and the
SOFTWARE and any accompanying printed materials, and copies of the SOFTWARE,
are owned by CORTINA. The SOFTWARE protected by the copyright laws of the
United States and other countries, and international treaty provisions.
You may not remove any copyright notices from the SOFTWARE.  Except as
otherwise expressly provided, CORTINA grants no express or implied right
under CORTINA patents, copyrights, trademarks, or other intellectual
property rights.

DISCLAIMER OF WARRANTIES. THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY
EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING ANY IMPLIED WARRANTIES
OF MERCHANTABILITY, NONINFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE,
TITLE, AND NON-INFRINGEMENT.  CORTINA does not warrant or assume
responsibility for the accuracy or completeness of any information, text,
graphics, links or other items contained within the SOFTWARE.  Without
limiting the foregoing, you are solely responsible for determining and
verifying that the SOFTWARE that you obtain and install is the appropriate
version for your purpose.

LIMITATION OF LIABILITY. IN NO EVENT SHALL CORTINA OR ITS SUPPLIERS BE
LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, LOST
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING OUT
OF THE USE OF OR INABILITY TO USE OF OR INABILITY TO USE THE SOFTWARE, EVEN
IF CORTINA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
TERMINATION OF THIS LICENSE. This LICENSE will automatically terminate if
You fail to comply with any of the terms and conditions hereof. Upon
termination, You will immediately cease use of the SOFTWARE and destroy all
copies of the SOFTWARE or return all copies of the SOFTWARE in your control
to CORTINA.  IF you commence or participate in any legal proceeding against
CORTINA, then CORTINA may, in its sole discretion, suspend or terminate all
license grants and any other rights provided under this LICENSE during the
pendency of such legal proceedings.
APPLICABLE LAWS. Claims arising under this LICENSE shall be governed by the
laws of the State of California, excluding its principles of conflict of
laws.  The United Nations Convention on Contracts for the International Sale
of Goods is specifically disclaimed.  You shall not export the SOFTWARE
without first obtaining any required export license or other approval from
the applicable governmental entity, if required.  This is the entire
agreement and understanding between You and CORTINA relating to this subject
matter.
GOVERNMENT RESTRICTED RIGHTS. The SOFTWARE is provided with "RESTRICTED
RIGHTS." Use, duplication, or disclosure by the Government is subject to
restrictions as set forth in FAR52.227-14 and DFAR252.227-7013 et seq. or
its successor. Use of the SOFTWARE by the Government constitutes
acknowledgment of CORTINA's proprietary rights therein. Contractor or
Manufacturer is CORTINA.

Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cs_types.h"
#include "plat_common.h"
#include "app_daemon.h"
#include "db_mngt.h"
#include "db_api.h"
#ifdef HAVE_DB_MANAGMENT

static db_data_confg_t  *g_DbCfgTable = NULL;
static cs_uint32         g_DbCfgTableSize = 0;
static db_table_def_init_handle g_DbSetDefault = NULL;
static db_table_upgrade_handle  g_DbUpgrade = NULL; 
static db_core_info_t    g_db_info;
static cs_uint32 gUsrDataInst = 0;
static cs_uint16 g_DbTimerInterval = DB_CENTER_SYN_DATA_PERIOD_TIME;

void db_core_init(
        cs_uint32 flash_offset, 
        cs_boolean encrypt,
        cs_boolean auto_sync
        )
{
    memset(&g_db_info,0x00,sizeof(g_db_info));
    g_db_info.flash_offset = flash_offset;
    g_db_info.create_flag = FALSE;
    g_db_info.encrypt = encrypt;
    g_db_info.auto_sync = auto_sync;
    db_auto_sync_flash_set(g_db_info.auto_sync);
    g_db_info.dirty_count = 0;
    g_db_info.data_pool = NULL;
    g_db_info.flash_version = 0;
    g_db_info.cur_version = 0;
    
    if (CS_E_OSAL_OK != cs_mutex_init(&g_db_info.data_mutex, "db_mutex", 0)){
        DB_LOG("init db_mutex fail\n");
        return;
    }

    gUsrDataInst = 0;
}

cs_status  db_table_init(
        db_data_confg_t * dbDesc,
        cs_uint32 size,
        cs_uint32 version,
        db_table_def_init_handle defHandler,
        db_table_upgrade_handle upgradeHandler)
{
    cs_uint32 i = 0; 
    cs_uint32 len = 0; 

    DB_ASSERT_RET(dbDesc != NULL,CS_E_PARAM);
    
    if(g_db_info.create_flag){
        DB_LOG("the table already create, we only create one table \n");
        return CS_E_ERROR;
    }

    g_DbCfgTable = dbDesc;
    g_DbCfgTableSize = size;
    g_DbSetDefault = defHandler;
    g_DbUpgrade = upgradeHandler;
    g_db_info.cur_version = version;
    g_db_info.pool_size = sizeof(tlv_header_t);

    for(i = 0 ; i < size; i++){
        g_db_info.pool_size += (dbDesc[i].max_len + TLV_VALUE_OFFSET);
    }

    len = db_len_from_flash();

    
    if(len > g_db_info.pool_size){
        DB_LOG("Use Flash DB len 0x%x, SW DB len 0x%x. \n",len,g_db_info.pool_size);    
        g_db_info.pool_size = len;
    }


    g_db_info.data_pool = iros_malloc(IROS_MID_APP, g_db_info.pool_size);
    if(!g_db_info.data_pool){
        DB_LOG("fatal error for db init\n");
        return CS_E_ERROR;
    }

    memset(g_db_info.data_pool, 0, g_db_info.pool_size);

    db_table_create();

    db_daemon_task_handler_init();
    return CS_E_OK;
}


cs_status db_table_create()
{
    int begin, end;
    cs_boolean reset_default = FALSE;
    begin = cs_current_time();

    if(CS_E_OK != db_load_from_flash()){
        cs_printf("Load DB from flash failed,Use the factory configuration.\n");
        memset(g_db_info.data_pool, 0, g_db_info.pool_size);
        reset_default = TRUE;
    }

    gUsrDataInst = tlv_inst_init((void *)g_db_info.data_pool, g_db_info.pool_size, NULL);
    if(TLV_PARSER_OK != tlv_load(gUsrDataInst)){
        DB_LOG("tlv load failed \n");
        memset(g_db_info.data_pool, 0, g_db_info.pool_size);
        tlv_load(gUsrDataInst);
        reset_default = TRUE;
    }

    g_db_info.create_flag = TRUE;

    if(reset_default && g_DbSetDefault != NULL){
        g_DbSetDefault();
    }

    if(!reset_default&& g_DbUpgrade != NULL){
        /*Remove flash_version!=cur_version*/
        g_DbUpgrade(g_db_info.flash_version);
    }

    end = cs_current_time();
    cs_printf("db create cost %d tick,Size is 0x%x\n",(end-begin),g_db_info.pool_size);

    return CS_E_OK;
}

cs_uint32 db_cfg_len_get(cs_uint32 id)
{
    cs_status len = 0;
    cs_uint32 i = 0;
    
    cs_mutex_lock(g_db_info.data_mutex);
    
    if(!g_db_info.create_flag){
        DB_LOG("the db table has not created \n");
        goto END;
    }

    for(i = 0; i < g_DbCfgTableSize; i++){
        if(g_DbCfgTable[i].id == id){
            break;
        }
    }

    if(i == g_DbCfgTableSize){
        DB_LOG("can not find the id = %d \n",id);
        goto END;
    }
    len = tlv_type_len(gUsrDataInst,(cs_uint16)id);
END:
    cs_mutex_unlock(g_db_info.data_mutex);
    return len;
}

cs_status db_read(
        cs_uint32 id, 
        cs_uint8 *buf,
        cs_uint32 len
        )
{
    cs_status result = CS_E_OK;
    cs_int32 ret = CS_E_OK;
    cs_uint32 i = 0;
    
    DB_ASSERT_RET(buf != NULL,CS_E_PARAM);
    cs_mutex_lock(g_db_info.data_mutex);
    
    if(!g_db_info.create_flag){
        DB_LOG("the db table has not created \n");
        result = CS_E_ERROR;
        goto END;
    }

    for(i = 0; i < g_DbCfgTableSize; i++){
        if(g_DbCfgTable[i].id == id){
            break;
        }
    }

    if(i == g_DbCfgTableSize){
        DB_LOG("can not find the id = %d \n",id);
        result = CS_E_ERROR;
        goto END;
    }

    switch(g_DbCfgTable[i].type)
    {
        case DB_TYPE_UINT8:
            ret = tlv_read_char(gUsrDataInst, (cs_uint16)id, buf);
            break;
        case DB_TYPE_UINT16:
            ret = tlv_read_short(gUsrDataInst, (cs_uint16)id, (cs_uint16 *)buf);
            break;
        case DB_TYPE_UINT32:
            ret = tlv_read_int(gUsrDataInst, (cs_uint16)id, (cs_uint32 *)buf);
            break;
        case DB_TYPE_BYTES:
            ret = tlv_read_buffer(gUsrDataInst, (cs_uint16)id, len , buf);
            if(ret == 0)
                ret = TLV_PARSER_ERROR;
            break;
        default:
            ret =  TLV_PARSER_ERROR;
            break;
    }

    if(ret == TLV_PARSER_ERROR){
        result =  CS_ERROR;
    }
END:
    cs_mutex_unlock(g_db_info.data_mutex);
    return result;
}

cs_status db_write(
        cs_uint32 id, 
        cs_uint8 *buf, 
        cs_uint32 len)
{
    cs_status result = CS_E_OK;
    cs_int32 ret = CS_E_OK;
    cs_uint16 tmpShort = 0;
    cs_uint32 tmpInt = 0;
    cs_uint32 i = 0;
    
    DB_ASSERT_RET(buf != NULL,CS_E_PARAM);

    cs_mutex_lock(g_db_info.data_mutex);
    
    if(!g_db_info.create_flag){
        DB_LOG("the db table has not created \n");
        result = CS_E_ERROR;
        goto END;
    }

    if(len <= 0){
        DB_LOG("invalid len = %d \n",len);
        result = CS_E_PARAM;
        goto END;
    }


    for(i = 0; i < g_DbCfgTableSize; i++){
        if(g_DbCfgTable[i].id == id){
            break;
        }
    }

    if(i == g_DbCfgTableSize){
        DB_LOG("can not find the id = %d \n",id);
        result = CS_E_ERROR;
        goto END;
    }

    {
        /*Don't Update DB if the content is the same.*/   
        cs_uint8* tmp = iros_malloc(IROS_MID_APP, len);
        if(tmp)
        {
            memset(tmp, 0, len);
            if(tlv_read_buffer(gUsrDataInst,(cs_uint16)id, len ,tmp)&&
                (tlv_type_len(gUsrDataInst,(cs_uint16)id)==len))
            {
                if(!memcmp(buf, tmp, len))
                {
                    iros_free(tmp);
                    goto END;
                }
            }

            iros_free(tmp);
        }
    }

    switch(g_DbCfgTable[i].type)
    {
        case DB_TYPE_UINT8:
            ret = tlv_write_char(gUsrDataInst, (cs_uint16)id, buf[0]);
            break;
        case DB_TYPE_UINT16:
            memcpy(&tmpShort, buf, sizeof(tmpShort));
            ret = tlv_write_short(gUsrDataInst, (cs_uint16)id, tmpShort);
            break;
        case DB_TYPE_UINT32:
            memcpy(&tmpInt, buf, sizeof(tmpInt));
            ret = tlv_write_int(gUsrDataInst, (cs_uint16)id, tmpInt);
            break;
        case DB_TYPE_BYTES:
            ret = tlv_write_buffer(gUsrDataInst, (cs_uint16)id, g_DbCfgTable[i].max_len, len , buf);
            break;
        default:
            ret = TLV_PARSER_ERROR;
            break;
    }
    
    if(ret != TLV_PARSER_OK){
        DB_LOG("write the db failed, id = %d ,ret = %d \n",i,ret);
        goto END;
    }

    g_db_info.dirty_count++;
    if(g_db_info.auto_sync){
        if(DB_DIRTY_COUNT_MAX == g_db_info.dirty_count)
        {
            db_send_msg(DB_CENTER_SYN_DATA_FORCE_MSG);
        }
    }

END:
    cs_mutex_unlock(g_db_info.data_mutex);
    return result;

}

void db_encrypt_set(cs_boolean enable)
{
    g_db_info.encrypt = enable;
}

void db_auto_sync_flash_set(cs_boolean enable)
{
    g_db_info.auto_sync = enable;
    if(enable){
        g_db_info.poll_timer = cs_timer_add(g_DbTimerInterval, db_sync_check_tmfunc, NULL); 
    }else{
        cs_timer_del(g_db_info.poll_timer);
    }
}



/*****************************************************************************/
/* $rtn_hdr_start  db_sync_check_tmfunc                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : 8030                                                         */
/*                                                                           */
void db_sync_check_tmfunc(void *data)
    /*                                                                           */
    /* INPUTS  : ----                                           */
    /* OUTPUTS : ----                                                            */
    /* RETURNS : ----                                                            */
    /* DESCRIPTION:                                                              */
    /* Period check dirty data                                       */
    /* $rtn_hdr_end                                                              */
    /*****************************************************************************/
{
    db_send_msg(DB_CENTER_SYN_DATA_PERIOD_MSG);
    g_db_info.poll_timer = cs_timer_add(g_DbTimerInterval,  db_sync_check_tmfunc, NULL);
}



void db_send_msg(cs_uint32 msg)
{
    app_daemon_msg_t* pMsg = NULL;

    /*send msg to daemon task to handle*/
    if((pMsg = (app_daemon_msg_t*)iros_malloc(IROS_MID_APP, sizeof(app_daemon_msg_t))) == NULL)
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_CRI, IROS_MID_APP, "db_center_send_msg:malooc fail\n");
        return;
    }
    memset(pMsg, 0, sizeof(app_daemon_msg_t));

    if(msg == DB_CENTER_SYN_DATA_FORCE_MSG)
    {
        pMsg->task_id = DAEMON_DB_FORCE_SYN_IMMEDIATE_ID;
    }
    else if(msg == DB_CENTER_SYN_DATA_PERIOD_MSG)
    {
        pMsg->task_id = DAEMON_DB_PERIOD_SYN_ID;
    }
    else
    {
        DB_LOG("Not support now(0x%x)\n", msg);
        iros_free(pMsg);
        return;
    }

    app_daemon_msg_send((void*)&pMsg, sizeof(pMsg));

    return;
}

/*****************************************************************************/
/* $rtn_hdr_start  db_sync_check_tmfunc                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : 8030                                                         */
/*                                                                           */
void db_sync(void* data)
    /*                                                                           */
    /* INPUTS  : ----                                           */
    /* OUTPUTS : ----                                                            */
    /* RETURNS : ----                                                            */
    /* DESCRIPTION:                                                              */
    /* Period check dirty data                                       */
    /* $rtn_hdr_end                                                              */
    /*****************************************************************************/
{
    /*check whether there is dirty data*/
    if(g_db_info.dirty_count)
    {
        db_save_to_flash();
    }

}

/*****************************************************************************/
/* $rtn_hdr_start  db_sync_check_tmfunc                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : 8030                                                         */
/*                                                                           */
void db_force_sync(void* data)
    /*                                                                           */
    /* INPUTS  : ----                                           */
    /* OUTPUTS : ----                                                            */
    /* RETURNS : ----                                                            */
    /* DESCRIPTION:                                                              */
    /* Period check dirty data                                       */
    /* $rtn_hdr_end                                                              */
    /*****************************************************************************/
{
    /*save to flash and clear counter*/
    db_save_to_flash();
}


void db_daemon_task_handler_init()
{
    /*reg handler for period syn*/
    app_daemon_reg_task_handler(DAEMON_DB_PERIOD_SYN_ID, db_sync);

    /*reg handler for force syn*/
    app_daemon_reg_task_handler(DAEMON_DB_FORCE_SYN_IMMEDIATE_ID, db_force_sync);
}



/*****************************************************************************/
/* $rtn_hdr_start  db_encrypt                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : 8030                                                         */
/*                                                                           */
void db_encrypt(cs_uint8 *buf, cs_uint32 len)
    /*                                                                           */
    /* INPUTS  : ----                                           */
    /* OUTPUTS : ----                                                            */
    /* RETURNS : ----                                                            */
    /* DESCRIPTION:                                                              */
    /* Db encrypt                                        */
    /* $rtn_hdr_end                                                              */
    /*****************************************************************************/
{
    cs_uint8 c;
    cs_uint32 i;

    for(i = 0; i < len; i++)
    {
        c = 0;
        c |= ((buf[i]>>7)&0x01)|
            ((buf[i]>>3)&0x08)|
            ((buf[i]>>4)&0x02)|
            ((buf[i]>>2)&0x04)|
            ((buf[i]<<1)&0x10)|
            ((buf[i]<<5)&0x80)|
            ((buf[i]<<4)&0x20)|
            ((buf[i]<<6)&0x40);
        buf[i] = c;
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  db_decrypt                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : 8030                                                         */
/*                                                                           */
void db_decrypt(cs_uint8 *buf, cs_uint32 len)
    /*                                                                           */
    /* INPUTS  : ----                                           */
    /* OUTPUTS : ----                                                            */
    /* RETURNS : ----                                                            */
    /* DESCRIPTION:                                                              */
    /* Db encrypt                                        */
    /* $rtn_hdr_end                                                              */
    /*****************************************************************************/
{
    cs_uint8 c;
    cs_uint32 i;

    for(i = 0; i < len; i++)
    {
        c = 0;
        c |= ((buf[i]<<7)&0x80)|
            ((buf[i]<<3)&0x40)|
            ((buf[i]<<4)&0x20)|
            ((buf[i]<<2)&0x10)|
            ((buf[i]>>1)&0x08)|
            ((buf[i]>>5)&0x04)|
            ((buf[i]>>4)&0x02)|
            ((buf[i]>>6)&0x01);
        buf[i] = c;
    }
}


unsigned int db_len_from_flash()
{
    tlv_header_t header;
    int ret;
    unsigned int len = 0;

    cs_mutex_lock(g_db_info.data_mutex);
    
    ret = get_userdata_from_flash(
            (char *)&header, 
            g_db_info.flash_offset, 
            sizeof(tlv_header_t));

    if(ret != 0){
        DB_LOG("read flash fail\n");
        goto ERROR;
    }

    if(g_db_info.encrypt){
        db_decrypt((cs_uint8 *)&header, sizeof(tlv_header_t));
    }
    
    if((htonl(TLV_HEADER_MAGIC) != header.magic)&&(htonl(DB_TLV_HEADER_MAGIC) != header.magic)){
        goto ERROR;
    }

    len = ntohl(header.cfg_len)+sizeof(tlv_header_t);
    
ERROR:
    cs_mutex_unlock(g_db_info.data_mutex);
    return len;
}


cs_status db_load_from_flash()
{
    tlv_header_t *header;
    int ret;
    cs_status status = CS_E_OK;

    cs_mutex_lock(g_db_info.data_mutex);
    ret = get_userdata_from_flash(
            g_db_info.data_pool, 
            g_db_info.flash_offset, 
            g_db_info.pool_size);

    if(ret != 0){
        DB_LOG("read flash fail\n");
        status =  CS_ERROR;
        goto ERROR;
    }

    if(g_db_info.encrypt){
        db_decrypt(g_db_info.data_pool, g_db_info.pool_size);
    }

    header = (tlv_header_t*)g_db_info.data_pool;
    header->cfg_crc = 0;

    if((htonl(TLV_HEADER_MAGIC) != header->magic)&&(htonl(DB_TLV_HEADER_MAGIC) != header->magic)){
        status = CS_ERROR;
        goto ERROR;
    }
    
    header->magic = htonl(TLV_HEADER_MAGIC);
    
    g_db_info.flash_version  = header->version;

ERROR:
    cs_mutex_unlock(g_db_info.data_mutex);
    return status;
}

cs_uint32 db_need_save()
{
    cs_uint32 need_save=0;
    cs_mutex_lock(g_db_info.data_mutex);
    need_save = g_db_info.dirty_count;
    cs_mutex_unlock(g_db_info.data_mutex);
    return need_save;
}

cs_status db_save_to_flash()
{
    cs_int8  *pbuf = NULL;
    int iRet;
    cs_status ret = CS_E_OK;
    tlv_header_t *header = NULL;

    header = (tlv_header_t*)g_db_info.data_pool;
    header->version = g_db_info.cur_version;
    header->magic = htonl(DB_TLV_HEADER_MAGIC);

    pbuf = iros_malloc(IROS_MID_APP, g_db_info.pool_size);
    if(pbuf == NULL){
        DB_LOG("malloc failed \n");
        return CS_E_ERROR;
    }
    
    cs_mutex_lock(g_db_info.data_mutex);
    memcpy(pbuf, g_db_info.data_pool, g_db_info.pool_size);

    if(g_db_info.encrypt){
        db_encrypt(pbuf, g_db_info.pool_size);
    }
    iRet = save_userdata_to_flash(
            pbuf, 
            g_db_info.flash_offset, 
            g_db_info.pool_size);
    if(iRet != 0)
    {
        DB_LOG("update flash fail\n");
        ret = CS_E_ERROR;
    }
    else{
        cs_printf("DB update flash OK\n");
#ifdef HAVE_WEB_SERVER
        web_save=0;
#endif
        g_db_info.dirty_count = 0;
    }

    cs_mutex_unlock(g_db_info.data_mutex);
    
    iros_free(pbuf);
    return ret;
}

cs_status db_save_to_file(cs_int8 * filename)
{
    cs_int8  *pbuf = NULL;
    FILE * fp = NULL; 
    tlv_header_t *header = NULL;

    DB_ASSERT_RET(filename != NULL,CS_E_PARAM);

    header = (tlv_header_t*)g_db_info.data_pool;
    header->version = g_db_info.cur_version;
    header->magic = htonl(DB_TLV_HEADER_MAGIC);

    pbuf = iros_malloc(IROS_MID_APP, g_db_info.pool_size);
    if(pbuf == NULL){
        DB_LOG("malloc failed \n");
        return CS_E_ERROR;
    }
    
    cs_mutex_lock(g_db_info.data_mutex);
    memcpy(pbuf, g_db_info.data_pool, g_db_info.pool_size);

    if(g_db_info.encrypt){
        db_encrypt(pbuf, g_db_info.pool_size);
    }
    cs_mutex_unlock(g_db_info.data_mutex);

    fp = fopen(filename,"w+b");    
    if(fp == NULL){
        iros_free(pbuf);
        DB_LOG("open file %s failed \n",filename);
        return CS_E_ERROR;
    }
    
    fwrite(pbuf,g_db_info.pool_size,1,fp);
    fclose(fp);

    DB_LOG("save data to file = %s \n",filename);
    
    iros_free(pbuf);
    return CS_E_OK;
}

cs_status db_reset_to_factory()
{
    if(g_DbSetDefault != NULL)
        g_DbSetDefault();
    db_save_to_flash();
    return CS_E_OK;
}


void db_config_dump()
{
    cs_uint8 field_idx = 0;
    cs_uint32 i = 0;
    cs_uint8* buf = NULL;
    cs_int32 ret = TLV_PARSER_ERROR;
    cs_uint32 max_db_len = 0;
    
    for(i=0;i<g_DbCfgTableSize;i++)
    {
        if(max_db_len < g_DbCfgTable[i].max_len)
            max_db_len = g_DbCfgTable[i].max_len;
    }
    buf = iros_malloc(IROS_MID_APP,max_db_len);
    if(buf == NULL)
    {
        cs_printf("DB Memory fail.\n");
        return ;
    }
    for (field_idx = 0; field_idx<g_DbCfgTableSize; field_idx++)
    {
        cs_printf("%17s %2d  \n",
            g_DbCfgTable[field_idx].name,
            g_DbCfgTable[field_idx].id);

        memset(buf,0,max_db_len);            
        ret = db_read(g_DbCfgTable[field_idx].id , buf,
                                       g_DbCfgTable[field_idx].max_len) ;
        if(ret != CS_E_OK){
            DB_LOG("read %d failed ret = %d \n",g_DbCfgTable[field_idx].id ,ret);
            continue;
        }
        for(i = 0; i < g_DbCfgTable[field_idx].max_len; i++)
        {
            if((i)%16 == 0 && i != 0)
                cs_printf("\n");
            cs_printf("%02x ", buf[i]);
        }
        cs_printf("\n");
        cs_thread_delay(100);
    }
    iros_free(buf);
}
#endif
