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

DEFINITIONS:  "DEVICE" means the Cortina Systems?Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems?SDK software.

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

#include "plat_common.h"
#include "img_upgrade.h"
#include "app_daemon.h"
#ifdef HAVE_MIF
#include "mif.h"
#endif

/*
 * Global Variables
 */
static img_upgrade_cb_t g_img_upgrade_cb;
static cs_boolean g_mif_support = TRUE;

void img_upgrade_set_mif_support(cs_boolean enable)
{
    g_mif_support = enable;
}

static void img_upgrade_set_status(img_upgrade_status_e status)
{
    if(status != g_img_upgrade_cb.status)
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP,
                                "Pre-status(%d), curr-status(%d)\n",
                                g_img_upgrade_cb.status, status);

        switch(status)
        {
            case IMG_UPGRADE_IDLE:
                if(g_img_upgrade_cb.buf)
                {
                    iros_free(g_img_upgrade_cb.buf);
                }
                memset(&g_img_upgrade_cb, 0, sizeof(g_img_upgrade_cb));
                break;
                
            default:
                break;
        }

        g_img_upgrade_cb.status = status;
        
    }
}


void img_upgrade_write_flash_handler(void* data)
{
    img_upgrade_write_flash_result_e write_result = IMG_UPGRADE_WRITE_FLASH_OK;

    if(CS_OK == iros_image_write_flash(g_img_upgrade_cb.buf, g_img_upgrade_cb.curr_len))
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_APP, 
                                "Image upgrade complete,size:%d...\n", g_img_upgrade_cb.curr_len);
        /*write flash ok*/
        write_result = IMG_UPGRADE_WRITE_FLASH_OK;
    }
    else
        write_result = IMG_UPGRADE_WRITE_FLASH_FAIL;    

    if(g_img_upgrade_cb.callback.write_call)
            g_img_upgrade_cb.callback.write_call(write_result);

    if(g_img_upgrade_cb.buf != NULL)
        iros_free(g_img_upgrade_cb.buf);
    g_img_upgrade_cb.buf = NULL;

    img_upgrade_set_status(IMG_UPGRADE_FINISH);
}

void img_upgrade_commit_handler(void* data)
{
    watchdog_disable();
    iros_image_commit();

    if(g_img_upgrade_cb.callback.commit_call)
            g_img_upgrade_cb.callback.commit_call(IMG_UPGRADE_COMMIT_OK);
}

void img_upgrade_reset_handler(void* data)
{
    iros_system_reset(RESET_TYPE_FORCE);
}

 void img_upgrade_confirm_policy_set(cs_boolean direct_confirm)
 {
    g_img_upgrade_cb.direct_confirm = direct_confirm;
 }
 
void img_upgrade_init()
{
    memset(&g_img_upgrade_cb, 0, sizeof(g_img_upgrade_cb));
    
    /*reg handler for flash write*/
    app_daemon_reg_task_handler(DAEMON_UPGRADE_WIRTE_FLASH_ID, img_upgrade_write_flash_handler);

    /*reg handler for upgrade commit*/
    app_daemon_reg_task_handler(DAEMON_UPGRADE_COMMIT_ID, img_upgrade_commit_handler);

    /*reg handler for upgrade reset*/
    app_daemon_reg_task_handler(DAEMON_ONU_RESET_ID, img_upgrade_reset_handler);
}

void img_upgrade_flash_write_prepare()
{
    cs_uint32 curr_blob_idx = current_blob_desc.blob_idx;
    cs_uint8 *blob_info = NULL;
    cs_uint32 part_index = 0;

    g_img_upgrade_cb.blob_idx = 0x1 &(curr_blob_idx + 1);
    blob_info = (cs_uint8*)blob_info_tab[g_img_upgrade_cb.blob_idx];
    g_img_upgrade_cb.flash_des = (cs_uint8*)((cs_uint32)blob_info + sizeof(blob_info_desc_t));

    if(g_mif_support)
    {
        #ifdef HAVE_MIF
        prepare_mif_upgrade(32, TRUE);
        #endif
    }
    else
    {
        /*handle zblob*/
        
        part_index = blob_part_tab[g_img_upgrade_cb.blob_idx];
        
        flash_part_write_init(part_index, blob_info);       
    }
}

void img_upgrade_flash_write_stop()
{
    cs_uint32 part_index = blob_part_tab[g_img_upgrade_cb.blob_idx];
    blob_info_desc_t info;
    blob_info_desc_t *blob_info = blob_info_tab[g_img_upgrade_cb.blob_idx];
    cs_uint32 wdt_time = 0;
    #ifdef HAVE_MIF
    mif_upgrade_state_t cur_state; 
    #endif

    if(g_mif_support)
    {
        #ifdef HAVE_MIF
        mif_upgrade(32, g_img_upgrade_cb.buf, 0, &cur_state, 1);
        #endif
        current_blob_desc.update_timeout = 0xfffe;
        memcpy((char *)CURRENT_BLOB_DESC_BASEADDR, (char *)&current_blob_desc, sizeof(current_blob_desc_t));
    }
    else
    {
        info.head_signature = BLOB_INFO_HEAD_SIGNATURE;
        info.upgrade_stamp = blob_info_tab[current_blob_desc.blob_idx]->upgrade_stamp + 1;
        info.len = g_img_upgrade_cb.total_len;
        info.blob_start_ptr = (struct image_blob_header_t *) (g_img_upgrade_cb.flash_des);
        if(g_img_upgrade_cb.direct_confirm) /* confirm directly */
        {
            info.is_not_confirmed = 0;
            info.is_not_executed  = 0;
        }
        else
        {
            info.is_not_confirmed = 0xffff;
            info.is_not_executed  = 0xffff;
            cs_printf("should not come here\n");
        }

        info.xip_len    = 0xffff;
        info.xip_crc32  = 0xffff;
        info.tail_signature = BLOB_INFO_TAIL_SIGNATURE;

        /* write control block of this blob, each by field */
        flash_write((char *)&blob_info->head_signature, (char *)&info.head_signature, sizeof(info.head_signature));
        flash_write((char *)&blob_info->upgrade_stamp, (char *)&info.upgrade_stamp, sizeof(info.upgrade_stamp));
        flash_write((char *)&blob_info->blob_start_ptr, (char *)&info.blob_start_ptr, sizeof(info.blob_start_ptr));
        flash_write((char *)&blob_info->tail_signature, (char *)&info.tail_signature, sizeof(info.tail_signature));
        flash_write((char *)&blob_info->len, (char *)&info.len, sizeof(info.len));
        flash_write((char *)&blob_info->is_not_confirmed, (char *)&info.is_not_confirmed, sizeof(info.is_not_confirmed));
        flash_write((char *)&blob_info->is_not_executed, (char *)&info.is_not_executed, sizeof(info.is_not_executed));

        part_index = blob_part_tab[g_img_upgrade_cb.blob_idx];
        
        flash_part_write_done(part_index);      

        /* get current WDT, transfer to loader */
        watchdog_get_time(&wdt_time);
        current_blob_desc.update_timeout = wdt_time;
        memcpy((char *)CURRENT_BLOB_DESC_BASEADDR, (char *)&current_blob_desc, sizeof(current_blob_desc_t));
    }

}

cs_boolean img_upgrade_in_progress()
{
    if(g_img_upgrade_cb.status == IMG_UPGRADE_DL_START ||
        g_img_upgrade_cb.status == IMG_UPGRADE_TRANSFER ||
        g_img_upgrade_cb.status == IMG_UPGRADE_WRITTING)
    {
        return TRUE;
    }
    else 
    {
        return FALSE;
    }
}

cs_status img_upgrade_clean()
{
    if(g_img_upgrade_cb.status == IMG_UPGRADE_WRITTING)
    {
        return CS_E_ERROR;
    }

    if(g_img_upgrade_cb.status == IMG_UPGRADE_DL_START ||
       g_img_upgrade_cb.status ==  IMG_UPGRADE_TRANSFER)
    {
        img_upgrade_download_stop(1);
    }

    img_upgrade_set_status(IMG_UPGRADE_IDLE);
    return CS_E_OK;
}
cs_status img_upgrade_download_start(
        cs_uint32 img_size, 
        img_upgrade_download_end_callback callback,
        img_upgrade_write_mode_e mode)
{
    if(IMG_UPGRADE_IDLE != g_img_upgrade_cb.status &&
        IMG_UPGRADE_DL_STOP != g_img_upgrade_cb.status)
        return CS_E_ERROR;

    img_upgrade_set_status(IMG_UPGRADE_IDLE);
    /*malloc buf to prepare download*/
    if(img_size > 0 )
    {
        g_img_upgrade_cb.buf = iros_malloc(IROS_MID_APP, img_size);
        if(!(g_img_upgrade_cb.buf))
        {
            IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_APP, "img_upgrade_download_start:malloc fail!\n");
            return CS_E_ERROR;
        }
        memset(g_img_upgrade_cb.buf, 0, img_size);
        g_img_upgrade_cb.size = img_size;
        g_img_upgrade_cb.callback.download_call = callback;
        g_img_upgrade_cb.write_mode = mode;
        g_img_upgrade_cb.next_seq = 1;
        
        g_img_upgrade_cb.total_len = 0 ;
        img_upgrade_set_status(IMG_UPGRADE_DL_START);
    }
     
    return CS_E_OK;
    
}

cs_status img_upgrade_download_stop(cs_boolean hold_buf)
{
    if(g_img_upgrade_cb.status != IMG_UPGRADE_DL_START && 
        g_img_upgrade_cb.status != IMG_UPGRADE_TRANSFER)
        return CS_E_ERROR;
    
    img_upgrade_set_status(IMG_UPGRADE_DL_STOP);

    if(!hold_buf)
    {
        if(g_img_upgrade_cb.buf)
        {
            iros_free(g_img_upgrade_cb.buf);
            g_img_upgrade_cb.buf = NULL;
            g_img_upgrade_cb.size = 0;
        }
    }

    if(g_img_upgrade_cb.write_mode == IMG_UPGRADE_WRITE_PART)
    {
        if(g_img_upgrade_cb.callback.download_call)
        {
            g_img_upgrade_cb.callback.download_call(IMG_UPGRADE_DOWNLOAD_OK);
        }
        img_upgrade_flash_write_stop();
    }
    return CS_E_OK;
}

cs_uint32 img_upgrade_downloaded_size_get()
{
    return g_img_upgrade_cb.total_len;
}

cs_uint8* img_upgrade_download_buf_get()
{
    return g_img_upgrade_cb.buf;
}

cs_status img_upgrade_syn_buf_to_flash()
{
    cs_uint8 *start_addr = NULL;
    #ifdef HAVE_MIF
    mif_upgrade_state_t cur_state;
    #endif
    
    if(g_img_upgrade_cb.next_seq == 1)
    {
        /*prepare flash write*/
        img_upgrade_flash_write_prepare();
    }
    if(g_mif_support)
    {
        #ifdef HAVE_MIF
        if(mif_upgrade(32, g_img_upgrade_cb.buf, g_img_upgrade_cb.curr_len, &cur_state, 0))
        {
            return CS_E_ERROR;
        }
        #endif
    }
    else
    {
        start_addr = g_img_upgrade_cb.flash_des + g_img_upgrade_cb.total_len;
        flash_write(start_addr, g_img_upgrade_cb.buf, g_img_upgrade_cb.curr_len);
    }
    return CS_E_OK;
}

img_upgrade_frame_proc_result_e img_upgrade_write_buf(cs_uint8* data, cs_uint32 len)
{
    if(g_img_upgrade_cb.write_mode == IMG_UPGRADE_WRITE_WHOLE)
    {
        if(g_img_upgrade_cb.curr_len + len > g_img_upgrade_cb.size)
        {
            return IMG_UPGRADE_OVER_BUF;
        }

        memcpy(&(g_img_upgrade_cb.buf[g_img_upgrade_cb.curr_len]), data, len);
        g_img_upgrade_cb.curr_len += len;
        g_img_upgrade_cb.total_len = g_img_upgrade_cb.curr_len;
    }
    else
    {
        if(len > g_img_upgrade_cb.size)
        {
            return IMG_UPGRADE_OVER_BUF;
        }
        memcpy(g_img_upgrade_cb.buf, data, len);
        
        g_img_upgrade_cb.curr_len = len;
        /*write current buf to flash*/
        img_upgrade_syn_buf_to_flash();
        
        g_img_upgrade_cb.total_len += len;
        
    }
    g_img_upgrade_cb.next_seq++;
    return IMG_UPGRADE_PROC_OK;
}

img_upgrade_frame_proc_result_e img_upgrade_data_frame_proc(cs_uint8* data, cs_uint32 len)
{    
    img_upgrade_frame_proc_result_e result = IMG_UPGRADE_PROC_OK;
    if((!data) || (len==0))
    {
        return IMG_UPGRADE_WRONG_PACKET;
    }
    
    if(g_img_upgrade_cb.status == IMG_UPGRADE_DL_START)
    {
        g_img_upgrade_cb.status = IMG_UPGRADE_TRANSFER;
    }

    if(g_img_upgrade_cb.status != IMG_UPGRADE_TRANSFER)
    {
        return IMG_UPGRADE_WRONG_STATUS;
    }
    else
    {
        result =  img_upgrade_write_buf(data, len);
        if(g_img_upgrade_cb.next_seq == 1)
        {
            img_upgrade_set_status(IMG_UPGRADE_TRANSFER);
        }
        g_img_upgrade_cb.next_seq++;
    }

    return result;
}


img_upgrade_frame_proc_result_e img_upgrade_tftp_frame_proc(
            img_upgrade_tftp_hdr_t *tftp_hdr,
            cs_uint32 in_len,
            cs_uint8 *pOut,
            cs_uint32 *out_len)
{
    img_upgrade_frame_proc_result_e result = IMG_UPGRADE_PROC_OK;
    img_upgrade_tftp_rsp_t *pRsp = (img_upgrade_tftp_rsp_t*)pOut;
    img_upgrade_tftp_data_t *pData = (img_upgrade_tftp_data_t *)tftp_hdr;
    cs_uint32 data_len = 0;
    
    if(!tftp_hdr || !pOut || !out_len || in_len < sizeof(img_upgrade_tftp_hdr_t))
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_APP, 
                                "img_upgrade_tftp_proc:param error");
        return IMG_UPGRADE_WRONG_PACKET;
    }

    IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP,
                            "opcode(%d), inlen(%d)\n", tftp_hdr->opcode, in_len);
    /*tftp data process*/
    switch(tftp_hdr->opcode)
    {
        case IMG_UPGRADE_TFTP_RRQ:
            
        case IMG_UPGRADE_TFTP_WRQ:
        {
            cs_uint32 len = strlen(tftp_hdr->data);

            cs_printf("#\n");

            if(IMG_UPGRADE_IDLE != g_img_upgrade_cb.status && 
                IMG_UPGRADE_DL_STOP != g_img_upgrade_cb.status)
            {
                return IMG_UPGRADE_WRONG_STATUS;
            }

            img_upgrade_set_status(IMG_UPGRADE_IDLE);
            
            if(len > IMG_UPGRADE_FILE_NAME_MAX_LEN)
                len = IMG_UPGRADE_FILE_NAME_MAX_LEN;
            strncpy(g_img_upgrade_cb.name, tftp_hdr->data, len);         
            IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "img name:%s", g_img_upgrade_cb.name);

            pRsp->opcode = htons(IMG_UPGRADE_TFTP_ACK);
            pRsp->block = 0;
            *out_len = sizeof(img_upgrade_tftp_rsp_t);

            /*next data block*/
            g_img_upgrade_cb.next_seq = 1;
 
            result = IMG_UPGRADE_PROC_OK;
            break;
        }
            
        case IMG_UPGRADE_TFTP_DATA:
            cs_printf(".");

            if(pData->block == g_img_upgrade_cb.next_seq - 1)
            {
                IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_APP, "dum packet, block is %d\n", pData->block);
                pRsp->opcode = htons(IMG_UPGRADE_TFTP_ACK);
                pRsp->block = htons(pData->block);
                *out_len = sizeof(img_upgrade_tftp_rsp_t); 
                result = IMG_UPGRADE_DUM_PACKET;
            }
            else if(pData->block == g_img_upgrade_cb.next_seq)
            {
                /*expected packet*/
                data_len = in_len - sizeof(img_upgrade_tftp_data_t) + 1;

                result = img_upgrade_write_buf(pData->data, data_len);
                if(IMG_UPGRADE_PROC_OK != result)
                {
                    IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP,
                            "write buf fail, result:%d\n", result);
                    break;
                }

                pRsp->opcode = htons(IMG_UPGRADE_TFTP_ACK);
                pRsp->block = htons(pData->block);
                *out_len = sizeof(img_upgrade_tftp_rsp_t); 

                if(pData->block == 1)
                    img_upgrade_set_status(IMG_UPGRADE_TRANSFER);

                
                if((g_img_upgrade_cb.next_seq % 32) == 0)
                    cs_printf("\n");
                result = IMG_UPGRADE_PROC_OK;               
            }
            else
            {
                IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_APP, "wrong packet, block is %d\n", pData->block);
                result = IMG_UPGRADE_WRONG_PACKET;                 
            }
            break;
            
        case IMG_UPGRADE_TFTP_ACK:
            break;
            
        case IMG_UPGARDE_TFTP_ERR:
            break;
            
        default:
            result = IMG_UPGRADE_UNKOWN_OP;
            break;
    }

    return result;
}

cs_status img_upgrade_send_msg(img_upgrade_msg_t msg)
{
    app_daemon_msg_t *pMsg = NULL;

    if((pMsg = (app_daemon_msg_t*)iros_malloc(IROS_MID_APP, sizeof(app_daemon_msg_t))) == NULL)
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_APP, "img_upgrade_send_msg:malloc fail\n");
        return CS_E_ERROR;
    }

    memset(pMsg, 0, sizeof(app_daemon_msg_t));
    
    /*send msg to daemon task for handle*/
    if(msg == IMG_UPGRADE_WRITE_FLASH_MSG)
    {
        pMsg->task_id = DAEMON_UPGRADE_WIRTE_FLASH_ID;
    }
    else if(msg == IMG_UPGRADE_COMMIT_MSG)
    {
        pMsg->task_id = DAEMON_UPGRADE_COMMIT_ID;
    }
    else if(msg == IMG_UPGRADE_RESET)
    {
        pMsg->task_id = DAEMON_ONU_RESET_ID;
    }
    
    app_daemon_msg_send(&pMsg, sizeof(pMsg));

    return CS_E_OK;
}

cs_status img_upgrade_wirte_flash(img_upgrade_write_flash_callback callback)
{
    if(g_img_upgrade_cb.status != IMG_UPGRADE_DL_STOP ||
        g_img_upgrade_cb.write_mode != IMG_UPGRADE_WRITE_WHOLE)
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_APP, "Can not support write whole image!\n");
        return CS_E_ERROR;
    }
    
    g_img_upgrade_cb.callback.write_call = callback;

    img_upgrade_set_status(IMG_UPGRADE_WRITTING);
    
    return img_upgrade_send_msg(IMG_UPGRADE_WRITE_FLASH_MSG);

}

cs_status img_upgrade_commit(img_upgrade_commit_callback callback)
{
    g_img_upgrade_cb.callback.commit_call = callback;
    return img_upgrade_send_msg(IMG_UPGRADE_COMMIT_MSG);
}

cs_status img_upgrade_reset()
{
    return img_upgrade_send_msg(IMG_UPGRADE_RESET);
}

void img_upgrade_show()
{
    cs_printf("status:---------(%d)\n", g_img_upgrade_cb.status);
    cs_printf("cabllback:-------(%p)\n", g_img_upgrade_cb.callback);
    cs_printf("max size:--------(%d)\n", g_img_upgrade_cb.size);
    cs_printf("curr size:--------(%d)\n", g_img_upgrade_cb.curr_len);
    cs_printf("total size:--------(%d)\n", g_img_upgrade_cb.total_len);
    cs_printf("next seq:--------(%d)\n", g_img_upgrade_cb.next_seq);
    cs_printf("conf policy-------(%d)\n", g_img_upgrade_cb.direct_confirm);
    cs_printf("watch dog-------(%d)\n", g_img_upgrade_cb.watchdog_enable);
    cs_printf("name:-----------(%s)\n", g_img_upgrade_cb.name);
    cs_printf("buff addr:--------(%p)\n", g_img_upgrade_cb.buf);
}


