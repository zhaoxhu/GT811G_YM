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

DEFINITIONS:  "DEVICE" means the Cortina Systems?LynxD SDK product.
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
/*
 *
 * image.c
 *
 * $Id: image.c,v 1.1.4.7.6.1.2.3.2.1.2.2.8.8.4.1.2.6.2.1.2.3 2012/12/28 11:25:49 yding Exp $
 */

#include "iros_config.h"
#include "plat_common.h"
#include "image.h"
#include "watchdog.h"
#include <cyg/crc/crc.h>

#define CRC_CHECK_FAIL      0
#define CRC_CHECK_SUCEESS   1

#define WRT_BLOCK_LEN       (4*1024)
#define MAX_IMAGE_SIZE      (512*1024)


blob_info_desc_t *blob_info_tab[2];
unsigned int      blob_part_tab[2]; /* partition table index for blob slot0 and slot1 */

#ifdef HAVE_NTT_OAM
static image_sync_state_e sync_status = IMAGE_SYNC_DONE;
#endif

cs_uint32 iros_image_write_flash(cs_int8* _image, cs_uint32 _len)
{
    cs_uint8 cur_blob_idx;
    cs_int8* flash_des;
    cs_uint8 blob_idx;
    cs_uint8 part_index;
    blob_info_desc_t *blob_info;
    cs_uint32 wrt_len, wrt_blk;
    cs_uint32 left_len;
    cs_int8 *flash_ptr, *img_ptr;
    cs_uint32 wdt_time;

    cur_blob_idx = current_blob_desc.blob_idx;

    blob_idx = 0x1 &(cur_blob_idx + 1);
    blob_info = blob_info_tab[blob_idx];
    flash_des = (cs_int8 *)((cs_int8 *)blob_info + sizeof(blob_info_desc_t));
    part_index = blob_part_tab[blob_idx];

    if(0 == flash_part_write_init(part_index, (char*)blob_info))
    {
        diag_printf("flash part not ready\n");
        return CS_ERROR;
    }

    wrt_len = 0;
    wrt_blk = 0;
    img_ptr = _image;
    left_len = _len;
    flash_ptr = flash_des;

    while (left_len)
    {
        wrt_len = (left_len > WRT_BLOCK_LEN) ? WRT_BLOCK_LEN : left_len;

        // diag_printf("flash write: %p, left 0x%x, wrt 0x%x\n", flash_ptr, left_len, wrt_len);

        if(wrt_len != flash_write(flash_ptr, img_ptr, wrt_len))
        {
            flash_part_write_done(part_index);
            diag_printf("flash wrt not equal\n");
            return CS_ERROR;
        }

        if(!flash_write_verify(flash_ptr, img_ptr, wrt_len))
        {
            flash_part_write_done(part_index);
            diag_printf("verify failed at %p\n", flash_ptr);
            return CS_ERROR;
        }

        flash_ptr += wrt_len;
        img_ptr   += wrt_len;
        left_len  -= wrt_len;

        wrt_blk++;
        if(!(wrt_blk % 32))
        {
            diag_printf("\n");
        }
        diag_printf(".");
        cs_thread_delay(5);
    }

    diag_printf("image wrt done\n");

    blob_info_desc_t info;
    info.head_signature = BLOB_INFO_HEAD_SIGNATURE;
    info.upgrade_stamp = blob_info_tab[cur_blob_idx]->upgrade_stamp + 1;
    info.len = _len;
    info.blob_start_ptr = (struct image_blob_header_t *) flash_des;
    #ifdef HAVE_KT_OAM /* confirm directly */
    info.is_not_confirmed = 0;
    info.is_not_executed  = 0;
    #else
    info.is_not_confirmed = 0xffff;
    info.is_not_executed  = 0xffff;
    #endif
    info.xip_len    = 0xffffffff;
    info.xip_crc32  = 0xffffffff;
    info.tail_signature = BLOB_INFO_TAIL_SIGNATURE;

    /* write control block of this blob, each by field */
    flash_write((char *)&blob_info->head_signature, (char *)&info.head_signature, sizeof(info.head_signature));
    flash_write((char *)&blob_info->upgrade_stamp, (char *)&info.upgrade_stamp, sizeof(info.upgrade_stamp));
    flash_write((char *)&blob_info->blob_start_ptr, (char *)&info.blob_start_ptr, sizeof(info.blob_start_ptr));
    flash_write((char *)&blob_info->tail_signature, (char *)&info.tail_signature, sizeof(info.tail_signature));
    flash_write((char *)&blob_info->len, (char *)&info.len, sizeof(info.len));
    flash_write((char *)&blob_info->is_not_confirmed, (char *)&info.is_not_confirmed, sizeof(info.is_not_confirmed));
    flash_write((char *)&blob_info->is_not_executed, (char *)&info.is_not_executed, sizeof(info.is_not_executed));

    diag_printf("update blob %d done\n", blob_idx);
    flash_part_write_done(part_index);

    /* get current WDT, transfer to loader */
    watchdog_get_time(&wdt_time);
    current_blob_desc.update_timeout = wdt_time;
    memcpy((char *)CURRENT_BLOB_DESC_BASEADDR, (char *)&current_blob_desc, sizeof(current_blob_desc_t));

    return CS_OK;
}

static unsigned int active_blob_get(void)
{
    unsigned int  blob_idx = 0;

    /* One confirmed and another not confirmed*/
    if((blob_info_tab[0]->is_not_confirmed) ^ (blob_info_tab[1]->is_not_confirmed))
    {
        if (blob_info_tab[0]->is_not_confirmed != 0)
            blob_idx = 1;
        else
            blob_idx = 0;
    }
    else /* Both not confirmed or both confirmed*/
    {
            if(blob_info_tab[0]->upgrade_stamp > blob_info_tab[1]->upgrade_stamp)
                blob_idx = 0;
            else
                blob_idx = 1;
    }

    return blob_idx;
}



unsigned int blob_active()
{
    unsigned int  status = 0;
    unsigned int curr_blob_idex,other_blob_idex;
    blob_info_desc_t *blob_info;
    void *temp_ptr=NULL;
    blob_info_desc_t *active_blob_info;

    /* used to back up first block data*/
    temp_ptr=iros_malloc(IROS_MID_SYS, flash_dev.info.super_block.block_size);
    if(temp_ptr == NULL)
    {
        diag_printf("blob_active failed no memory\n");
        status = 1;
        return status;
    }
 
    curr_blob_idex = current_blob_desc.blob_idx;
    active_blob_info = blob_info_tab[curr_blob_idex];
    other_blob_idex = 0x1 &(curr_blob_idex + 1);
    memcpy(temp_ptr, blob_info_tab[other_blob_idex], flash_dev.info.super_block.block_size);
    blob_info = (blob_info_desc_t *)temp_ptr;
    blob_info->is_not_executed = 0xffff;
    blob_info->is_not_confirmed = 0xffff;
    blob_info->xip_crc32 = 0xffffffff;
    
    if (!flash_part_write_init(blob_part_tab[other_blob_idex], (char*)blob_info_tab[other_blob_idex])){
        diag_printf("can not init nand in blob_active\n");
        iros_free(temp_ptr);
        status = 1;
        return status;
    }
    if(blob_info->upgrade_stamp <= active_blob_info->upgrade_stamp)
        blob_info->upgrade_stamp = active_blob_info->upgrade_stamp + 1;

    flash_write((char *)blob_info_tab[other_blob_idex], temp_ptr, flash_dev.info.super_block.block_size);       
    flash_part_write_done(blob_part_tab[other_blob_idex]);
    iros_free(temp_ptr);

    current_blob_desc.update_timeout = 0xfffe;
    memcpy((char *)CURRENT_BLOB_DESC_BASEADDR, (char *)&current_blob_desc, sizeof(current_blob_desc_t));
    diag_printf("Blob%d active OK!\n",other_blob_idex);

    return status;
}

bool current_blob_commited()
{
    unsigned int active_idx, current_idx;

    active_idx = active_blob_get();
    current_idx = current_blob_desc.blob_idx;

    diag_printf("active_idx %d, current_idx %d\n", active_idx, current_idx);

    if(active_idx == current_idx)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* This function will exchange active blob and standby blob */
cs_uint32 iros_image_commit(void)
{
    blob_info_desc_t *active_blob_info;
    blob_info_desc_t *standby_blob_info;
    unsigned int act_blob_idex,sty_blob_idex;
    void *temp_ptr=NULL;

    /* used to back up first block data*/
    temp_ptr=iros_malloc(IROS_MID_SYS, flash_dev.info.super_block.block_size);
    if(NULL == temp_ptr){
        cs_printf("commit failed no memory\n");
        return CS_ERROR;
    }

    act_blob_idex = active_blob_get();
    active_blob_info = blob_info_tab[act_blob_idex];
    sty_blob_idex = 0x1 &(act_blob_idex + 1);
    /* back up standby data */
    memcpy(temp_ptr, blob_info_tab[sty_blob_idex], flash_dev.info.super_block.block_size);
    standby_blob_info = (blob_info_desc_t *)temp_ptr;
    standby_blob_info->is_not_confirmed = 0;
    standby_blob_info->is_not_executed = 0;
    standby_blob_info->xip_crc32 = 0xffffffff;
    if(standby_blob_info->upgrade_stamp < active_blob_info->upgrade_stamp)
        standby_blob_info->upgrade_stamp = active_blob_info->upgrade_stamp + 1;

    /* prepare to write flash without erase */
    if (flash_part_write_init(blob_part_tab[sty_blob_idex], 
                               (char*)blob_info_tab[sty_blob_idex])) { 
        flash_write((char *)blob_info_tab[sty_blob_idex], 
            (char *)temp_ptr, flash_dev.info.super_block.block_size);
        cs_printf("Cur blob committed..\n");

        flash_part_write_done(blob_part_tab[sty_blob_idex]);
    } 
    else {
        cs_printf("Cur blob commit failed..\n");
        iros_free(temp_ptr);
        return CS_ERROR;
    }

    iros_free(temp_ptr);
    return CS_OK;
    
}

/* Invalid standby blob XIP crc to 0xFFFFFFFF for Bug 23377 */
cs_uint32 standby_blob_crc_invalidate(void)
{
    blob_info_desc_t *active_blob_info;
    blob_info_desc_t *standby_blob_info;
    unsigned int act_blob_idex,sty_blob_idex;
    void *temp_ptr=NULL;

    act_blob_idex = current_blob_desc.blob_idx;
    active_blob_info = blob_info_tab[act_blob_idex];
    
    /* if current active blob is not commited invalid standby blob crc */
    if(active_blob_info->is_not_confirmed){

        /* Disable watchdog enabled by loader for Bug 28609 */
        watchdog_disable();
        
        /* used to back up first block data*/
        temp_ptr=iros_malloc(IROS_MID_SYS, flash_dev.info.super_block.block_size);
        if(NULL == temp_ptr){
            cs_printf("Invalidate standby blob xip crc failed for no memory\n");
            return CS_ERROR;
        }

        sty_blob_idex = 0x1 &(act_blob_idex + 1);
        /* back up standby data */      
        memcpy(temp_ptr,blob_info_tab[sty_blob_idex],flash_dev.info.super_block.block_size);
        standby_blob_info = (blob_info_desc_t *)temp_ptr;
        standby_blob_info->xip_crc32 = 0xffffffff;

        /* prepare to write flash with erase */
        if (flash_part_write_init(blob_part_tab[sty_blob_idex], 
                                    (char*)blob_info_tab[sty_blob_idex])) { 
            flash_write((char *)blob_info_tab[sty_blob_idex], 
                         (char *)temp_ptr, flash_dev.info.super_block.block_size);
            cs_printf("Standby blob xip crc invalidated \n");

            flash_part_write_done(blob_part_tab[sty_blob_idex]);
        } 
        else {
           // cs_printf("Standby blob invalidate failed \n");
            iros_free(temp_ptr);
            return CS_ERROR;
        }
        iros_free(temp_ptr);
    }
    
    return CS_OK;  
}


#ifdef HAVE_NTT_OAM
cs_uint32 iros_image_sync_flash()
{
    cs_uint8 active_blob_idx, standby_blob_idx;
    cs_int8 *standby_flash_des, *active_flash_des;
    blob_info_desc_t *standby_blob_info, *active_blob_info;
    cs_uint8 part_index;
    cs_uint32 wrt_len, wrt_blk;
    cs_uint32 image_len, left_len;
    cs_int8 *flash_ptr, *img_buf, *img_ptr;
    cs_uint32 ver_offset;
    cs_int8 ver_buf[NTT_VERSION_LEN + 1];

    sync_status = IMAGE_SYNC_NOT_COMP;

    active_blob_idx = current_blob_desc.blob_idx;
    active_blob_info = blob_info_tab[active_blob_idx];
    active_flash_des = (cs_int8 *)active_blob_info + sizeof(blob_info_desc_t);

    standby_blob_idx = 0x1 &(active_blob_idx + 1);
    standby_blob_info = blob_info_tab[standby_blob_idx];
    standby_flash_des = (cs_int8 *)standby_blob_info + sizeof(blob_info_desc_t);

    // check if images are the same
    if(active_blob_info->len == standby_blob_info->len
        && standby_blob_info->is_not_confirmed == 0
        && standby_blob_info->is_not_executed == 0)
    {
        ver_offset = CS_OFFSET(ntt_image_header_t, onu_sw_version);

        if(!flash_memcmp(standby_flash_des + ver_offset, active_flash_des + ver_offset, NTT_VERSION_LEN))
        {
            memcpy(ver_buf, standby_flash_des + ver_offset, NTT_VERSION_LEN);
            ver_buf[NTT_VERSION_LEN] = 0;
            diag_printf("NTT sw version: %s\n", ver_buf);
            sync_status = IMAGE_SYNC_DONE;
            return CS_OK;
        }
    }

    // get standby blob partition ready, and sync
    part_index = blob_part_tab[standby_blob_idx];
    if(0 == flash_part_write_init(part_index, (cs_int8 *)standby_blob_info))
    {
        diag_printf("flash part not ready\n");
        sync_status = IMAGE_SYNC_DONE;
        return CS_ERROR;
    }

    image_len = active_blob_info->len;
    if(image_len > MAX_IMAGE_SIZE)
    {
        flash_part_write_done(part_index);
        diag_printf("image size exceeding max\n");
        sync_status = IMAGE_SYNC_DONE;
        return CS_ERROR;
    }

    img_buf = iros_malloc(IROS_MID_SYS, WRT_BLOCK_LEN);
    if(img_buf == NULL)
    {
        flash_part_write_done(part_index);
        diag_printf("no memory for image sync\n");
        sync_status = IMAGE_SYNC_DONE;
        return CS_ERROR;
    }

    //iros_flash_read(active_flash_des, img_buf, image_len);
    diag_printf("\n Begin to sync image on flash...\n");

    wrt_len = 0;
    wrt_blk = 0;
    img_ptr = img_buf;
    left_len = image_len;
    flash_ptr = standby_flash_des;

    while (left_len)
    {
        wrt_len = (left_len > WRT_BLOCK_LEN) ? WRT_BLOCK_LEN : left_len;
        iros_flash_read(active_flash_des + image_len - left_len, img_buf, wrt_len);

        // diag_printf("flash write: %p, left 0x%x, wrt 0x%x\n", flash_ptr, left_len, wrt_len);

        if(wrt_len != flash_write(flash_ptr, img_ptr, wrt_len))
        {
            diag_printf("flash wrt not equal\n");
            break;
        }

        if(!flash_write_verify(flash_ptr, img_ptr, wrt_len))
        {
            diag_printf("verify failed at %p\n", flash_ptr);
            break;
        }

        flash_ptr += wrt_len;
        //img_ptr   += wrt_len;
        left_len  -= wrt_len;

        wrt_blk++;
        if(!(wrt_blk % 32))
        {
            diag_printf("\n");
        }
        diag_printf(".");
        cs_thread_delay(5);
    }

    iros_free(img_buf);

    blob_info_desc_t info;
    info.head_signature = BLOB_INFO_HEAD_SIGNATURE;
    // standby stamp less than active, active always from 1, standby always -1
    info.upgrade_stamp = blob_info_tab[active_blob_idx]->upgrade_stamp - 1;
    info.len = image_len;
    info.blob_start_ptr = (struct image_blob_header_t *) standby_flash_des;
    // cofirm the image copy
    info.is_not_confirmed   = 0;
    info.is_not_executed    = 0;
    info.tail_signature     = BLOB_INFO_TAIL_SIGNATURE;

    /* write control block of this blob, each by field */
    flash_write((char *)&standby_blob_info->head_signature, (char *)&info.head_signature, sizeof(info.head_signature));
    flash_write((char *)&standby_blob_info->upgrade_stamp, (char *)&info.upgrade_stamp, sizeof(info.upgrade_stamp));
    flash_write((char *)&standby_blob_info->blob_start_ptr, (char *)&info.blob_start_ptr, sizeof(info.blob_start_ptr));
    flash_write((char *)&standby_blob_info->tail_signature, (char *)&info.tail_signature, sizeof(info.tail_signature));
    flash_write((char *)&standby_blob_info->len, (char *)&info.len, sizeof(info.len));
    flash_write((char *)&standby_blob_info->is_not_confirmed, (char *)&info.is_not_confirmed, sizeof(info.is_not_confirmed));
    flash_write((char *)&standby_blob_info->is_not_executed, (char *)&info.is_not_executed, sizeof(info.is_not_executed));

    flash_part_write_done(part_index);
    sync_status = IMAGE_SYNC_DONE;
    diag_printf("sync blob %d done\n", standby_blob_idx);

    return CS_OK;
}

void iros_image_sync_status(cs_uint8* status)
{
    *status = (cs_uint8)sync_status;
}

cs_uint32 iros_image_crc_check(cs_int8* _image, cs_uint32 _len, cs_uint8* _result)
{
    cs_uint32 cal_crc;
    cs_uint32 recv_crc, cal_len;
    cs_int8* calc_buf;

    calc_buf = _image;
    cal_crc = 0;
    cal_len = CS_OFFSET(ntt_image_header_t, file_checksum);
    cal_crc = cyg_crc32_accumulate(cal_crc, (cs_uint8 *)calc_buf, (cs_int32)cal_len);

    memcpy(&recv_crc, calc_buf + cal_len, sizeof(cs_uint32));
    recv_crc = ntohl(recv_crc);

    calc_buf = _image + cal_len + sizeof(cs_uint32);
    cal_len = _len - cal_len - sizeof(cs_uint32);
    cal_crc = cyg_crc32_accumulate(cal_crc, (cs_uint8 *)calc_buf, (cs_int32)cal_len);

    if(cal_crc != recv_crc)
    {
        *_result = CRC_CHECK_FAIL;
        diag_printf("calc 0x%08x recv 0x%08x\n", cal_crc, recv_crc);
    }
    else
        *_result = CRC_CHECK_SUCEESS;

    return CS_OK;
}

cs_uint32 iros_image_get_version(image_type_e _type, cs_int8* _buf, cs_uint8 _len)
{
    cs_uint8 blob_idx;
    cs_int8* flash_des;
    cs_uint8 other_blob_idx = 0x1 & (current_blob_desc.blob_idx + 1);
    cs_uint8 cur_blob_idx   = current_blob_desc.blob_idx;

    IROS_LOG_DBG3(IROS_MID_UPGRADE, "current %d %d, other %d\n",
        cur_blob_idx, current_blob_desc.update_timeout, other_blob_idx);

    switch(_type)
    {
        case IMAGE_ACTIVE:
            blob_idx = cur_blob_idx;
            break;

        case IMAGE_STANDBY:
            blob_idx = other_blob_idx;
            break;

        case IMAGE_NEXTBOOT:
            if(current_blob_desc.update_timeout > 0)
            {
                // in process of upgrade
                // after write image, update_timeout will be set
                if(blob_info_tab[other_blob_idx]->is_not_executed)
                {
                    // new image in flash, not reboot
                    blob_idx = other_blob_idx;
                }
                else if(blob_info_tab[cur_blob_idx]->is_not_confirmed)
                {
                    // new image in trial
                    blob_idx = other_blob_idx;
                }
                else
                {
                    // new image commited
                    blob_idx = cur_blob_idx;
                    IROS_LOG_DBG3(IROS_MID_UPGRADE, "flag: exec %d, confirm %d\n",
                        blob_info_tab[other_blob_idx]->is_not_executed,
                        blob_info_tab[cur_blob_idx]->is_not_confirmed);
                }
            }
            else
            {
                // after commit image, update_timeout will be clear
                blob_idx = cur_blob_idx;
            }
            break;

        default:
            diag_printf("invalid type\n");
            return CS_ERROR;
    }

    if(_len < NTT_VERSION_LEN)
    {
        diag_printf("invalid buf len\n");
        return CS_ERROR;
    }

    IROS_LOG_DBG3(IROS_MID_UPGRADE, "%d image: idx %d, addr %p\n", _type, blob_idx, flash_des);
    flash_des = (cs_int8 *)blob_info_tab[blob_idx] + sizeof(blob_info_desc_t);
    memcpy(_buf, flash_des + CS_OFFSET(ntt_image_header_t, onu_sw_version), NTT_VERSION_LEN);

    return CS_OK;
}
#else
cs_uint32 iros_image_crc_check(cs_int8* _image, cs_uint32 _len, cs_uint8* _result)
{
    return CS_OK;
}

#endif

#ifdef _IMAGE_TEST_

static const cyg_uint8 image_tab[] = {
    0x01, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x6d, 0x2d, 0x6f, 0x6e, 0x75, 0x2e, 0x62, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x47, 0x45, 0x2d, 0x50, 0x4f, 0x4e, 0x20, 0x4f, 0x4e, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x4b, 0xfb, 0x1c, 0x16, 0x00, 0x00, 0x01, 0x04, 0xeb, 0xf1, 0x65, 0x7b, 0xff, 0xff, 0xff, 0xff,
    0x48, 0x30, 0x30, 0x31, 0x48, 0x31, 0x30, 0x38, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x53, 0x75, 0x6d, 0x69, 0x74, 0x6f, 0x6d, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x63, 0x73, 0x38, 0x30, 0x33, 0x30, 0x00, 0x00, 0x78, 0x2e, 0x79, 0x2e, 0x7a, 0x2e, 0x61, 0x62,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x31, 0x32, 0x33, 0x00,
};

// this is a NTT image with standard header plus string "123"

void test_crc()
{
    cs_uint8 result;
    iros_image_crc_check((cs_int8 *)image_tab, sizeof(image_tab), &result);

}

#endif
