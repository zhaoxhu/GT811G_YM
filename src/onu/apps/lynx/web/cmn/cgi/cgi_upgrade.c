#include "cgi_cmn.h" 
#include "cgi_upgrade.h"
#include "sys_cfg.h"
#include "cgi_system.h"
#include "img_upgrade.h"
#include "mif.h"
#include "app_gpio.h"

cgi_upload_cb_t g_upload_cb;
mif_upgrade_state_t mif_state;

static cs_int8 get_image[] = "Upgrade firmware successfully.";
static cs_uint8 reset_str[]=
        "Upgrade firmware successfully.ONU will be reset in 3 seconds...";
static cs_uint8 g_verify_img_flag = 0;
static cs_boolean g_web_upgrade = FALSE;

#ifdef HAVE_FLASH_FS
extern int fs_mount(void);
extern int fs_umount(void);
#endif
int oper_flash_erase_block(void* block);

cs_boolean cgi_get_web_upgrade_status()
{
    return g_web_upgrade;
}

cs_uint8 cgi_get_img_verify_flag()
{
    return g_verify_img_flag;
}

void cgi_build_upload_response(
        cgi_handler_param_t * p,
        cs_uint8 type, 
        cgi_upload_result_e result)
{
    if(type == CGI_UPLOAD_JFFS2_TYPE)
    {
        if(result == CGI_UPLOAD_SUCCESS)
        {
            cgi_send_int(p, CGI_OK);
            cgi_send_str(p, get_image, strlen(get_image));
        }
        else
        {
            cgi_send_int(p, CGI_ERROR_PARAM);
        }
    }
    else if(type == CGI_UPLOAD_WLB_TYPE)
    {
        
        if(result == CGI_UPLOAD_SUCCESS)
        {
            cgi_send_str(p, reset_str, strlen(reset_str));
            /*delay 3 seconds*/
            cs_thread_delay(3000);
            cgi_sys_reset(p);
        }
        else
        {
            cgi_send_int(p, CGI_ERROR_PARAM);
        }
    }
    else
    {
        
        if(result == CGI_UPLOAD_SUCCESS)
        {
            if(mif_state.tlv_update_successed & 0x1 ||
                mif_state.tlv_update_successed & 0x4 ||
                mif_state.tlv_update_successed & 0x200)
            {
                cgi_send_str(p, reset_str, strlen(reset_str));
                /*delay 3 seconds*/
                cs_thread_delay(3000);
                cgi_sys_reset(p);
            }
            else
            {
                cgi_send_str(p, get_image, strlen(get_image));
            }
        }
        else
        {
            cgi_send_int(p, CGI_ERROR_PARAM);
        }
    }
}

cgi_status cgi_write_upload_data_end(cs_uint8 type, cs_boolean succ)
{
    cs_uint8 tmp[4];
    if(type == CGI_UPLOAD_JFFS2_TYPE)
    {
        flash_part_write_done(g_upload_cb.part_index);
        if(succ)
        {
            fs_mount();
            g_verify_img_flag = 1;
        }
        return CGI_OK;
    }
    else if(type == CGI_UPLOAD_WLB_TYPE)
    {
        img_upgrade_download_stop(0);
        if(succ)
        {
            g_verify_img_flag = 2;
        }
        return CGI_OK;
    }
    else
    {
        if(0 != mif_upgrade(IROS_MID_WEB, tmp, 0, &mif_state, TRUE))
        {
            return CGI_INNER_ERROR;
        }
        else 
        {
            if(succ)
            {
                if(mif_state.tlv_update_successed & 0x1 ||
                    mif_state.tlv_update_successed & 0x4 ||
                    mif_state.tlv_update_successed & 0x200)
                {
                    CGI_DEBUG("need reset\n");
                    g_verify_img_flag = 2;
                }
                else
                {
                    g_verify_img_flag = 1;
                }
            }
            return CGI_OK;
        }
    }
}

cgi_status cgi_write_upload_data(cs_uint8 *data, cs_uint32 len, cs_uint8 type)
{
    if(type == CGI_UPLOAD_JFFS2_TYPE)
    {
        cs_uint8 *dest = NULL;
        dest = flash_dev.info.super_block.part_tab[g_upload_cb.part_index].part_loc +
                  g_upload_cb.curr_len;
        flash_write(dest, data, len);
        g_upload_cb.curr_len += len;

        return CGI_OK;
    }
    else if(type == CGI_UPLOAD_WLB_TYPE)
    {
        img_upgrade_write_buf(data, len);
        return CGI_OK;
    }
    else
    {
        if(0 != mif_upgrade(IROS_MID_WEB, data, len, &mif_state, FALSE))
        {
            mif_upgrade(IROS_MID_WEB, data, 0, &mif_state, TRUE);
            return CGI_INNER_ERROR;
        }
        else
        {
            return CGI_OK;
        }
    }
}

void cgi_img_upgrade_download_callback(img_upgrade_download_result_e result)
{
    if(result == IMG_UPGRADE_DOWNLOAD_OK)
    {
        img_upgrade_confirm_policy_set(1);
    }
}


cgi_status cgi_write_upload_data_prepare(cs_uint8 type, cs_uint32 img_len)
{
    memset(&g_upload_cb, 0, sizeof(g_upload_cb));
    g_upload_cb.img_len = img_len;
    
    if(type == CGI_UPLOAD_JFFS2_TYPE)
    {
        cyg_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_JFFS2_ID);
        cs_uint8 *flash_des;
        
        if(IROS_FLASH_PARTITION_INDEX_ANY == part_index)
        {
            return CGI_SDL_FAILED;
        }

        fs_umount();
        flash_des = flash_dev.info.super_block.part_tab[part_index].part_loc;
        g_upload_cb.part_index = part_index;

        if(flash_part_write_init(part_index, flash_des))
        {
            return CGI_OK;
        }
        else
        {
            return CGI_SDL_FAILED;
        }
        
    }
    else if(type == CGI_UPLOAD_WLB_TYPE)
    {
        if(CS_E_OK != img_upgrade_download_start(CGI_HTTP_BUF_SIZE, 
                              cgi_img_upgrade_download_callback, IMG_UPGRADE_WRITE_PART))
        {
            return CGI_SDL_FAILED;
        }
        else
        {
            img_upgrade_set_mif_support(FALSE);
            return CGI_OK;
        }
    }
    else
    {
        /*for mif*/
        if(0 != prepare_mif_upgrade(IROS_MID_WEB, FALSE))
        {
            return CGI_SDL_FAILED;
        }
        else
        {
            return CGI_OK;
        }
    }
    
}

cs_uint32 cgi_get_image_len(cs_uint8 *pData, cs_uint8 type, cs_uint32 *parse_len)
{
    cs_uint32 img_len = 0;
    cs_uint32 out_len = 0;
    
    img_len = (*pData <<24)   | (*(pData+1) <<16)  | (*(pData+2) <<8) |  *(pData+3);
    out_len = 4;
    if(type == CGI_UPLOAD_MIF_TYPE)
    {
        epon_l2ftp_multi_image_header_t *header = (epon_l2ftp_multi_image_header_t*)pData;
        img_len = ntohl(header->length) + sizeof(epon_l2ftp_multi_image_header_t);
        out_len = 0;
    }
    *parse_len = out_len;
    return img_len;
}

cgi_status cgi_check_image_magic(cs_uint8 *pMagic, cs_uint8 type, cs_uint32 *len)
{
    cs_uint8 *tmp = pMagic;

    if(type == CGI_UPLOAD_JFFS2_TYPE)
    {
        if( (*tmp++!= '.') || (*tmp++!= 'w') ||(*tmp++!= 'f') || (*tmp++!= 's'))
        {
            return CGI_ERROR_PARAM;
        }
        *len = 4;
    }
    else
    {
        
        if(type == CGI_UPLOAD_MIF_TYPE)
        {
            epon_l2ftp_multi_image_header_t * header = (epon_l2ftp_multi_image_header_t*)pMagic;
            if(ntohl(header->mif_magic) != EPON_L2FTP_MULTI_IMAGE_MAGIC)
            {
                return CGI_ERROR_PARAM;
            }
            else
            {
                *len = 0;
            }
        }
        else
        {
            if( (*tmp++!= '.') || (*tmp++!= 'w') ||(*tmp++!= 'l') || (*tmp++!= 'b'))
            {
                return CGI_ERROR_PARAM;
            }
            *len = 4;
        }
        return CGI_OK;
        
        if(type == CGI_UPLOAD_WLB_TYPE){
            if( (*tmp++!= '.') || (*tmp++!= 'w') ||(*tmp++!= 'l') || (*tmp++!= 'b'))
            {
                return CGI_ERROR_PARAM;
            }
            *len = 4;
        }
    }

    return CGI_OK;
}

#define WEB_IMG_BLOCK_SIZE   2048

typedef struct web_file_block{
    cs_uint8   buf[WEB_IMG_BLOCK_SIZE]; 
    cs_uint32  len;
    cs_uint32  offset;
}web_file_block_t;

cgi_status cgi_upload_image_process(cgi_handler_param_t * p, cs_uint8 type)
{
    cgi_status status = CGI_OK;
    cs_int32 len = 0;
    web_file_block_t    *blocks = NULL; 
    cs_int8         boundary[100] = {0}; /*Rfc 1341, no longer than 70 */
    cs_int32        boundary_len = 0;
    cs_int8         crlf[] = "\r\n";
    cs_int8         crlf2[] = "\r\n\r\n";
    cs_uint8        *header = NULL;
    cs_uint32       total_len = 0;
    cs_uint8        read = 0;
    cs_uint8        write = 0;
    cs_uint8        i = 0;
    cs_uint8       * magic = NULL;
    cs_uint8       * data_buf = NULL;
    cs_uint32        magic_len = 0;
    cs_uint32        tmp_len = 0;
    cs_uint32        img_len = 0;
    cs_uint32        rec_blocks = 0;
    /*
     * we have 3 buffer, 2 for wait write and one for read
     * when the read success, and the 2 write block is full, write the first block to disk
     * when the read return 0 or -1, it indicate the read end, we need remove the 
     * boundary from the write block. if the last block len is smaller than the boundary, we need 
     * reduce buff from the first write block, that why we need three block
     */
    blocks = iros_malloc(IROS_MID_UPGRADE,sizeof(web_file_block_t)* 3);

    if(blocks == NULL){
        CGI_LOG("\nmalloc image buffer error\n");
        return CGI_INNER_ERROR;
    }

    cs_led_proc(LED_RUN, LED_GREEN, LED_HW_TWINKLE);

    g_web_upgrade = TRUE;
    if(img_upgrade_in_progress()){
        CGI_LOG("Already in upgrading....\n");
        status = CGI_INNER_ERROR;
        goto PROCESS_ERROR;
    }
    
    memset(blocks[read].buf, 0x00,WEB_IMG_BLOCK_SIZE);
    len = cgi_get_current_data(blocks[read].buf, WEB_IMG_BLOCK_SIZE, p);
    if(len < 0){
        CGI_LOG("not recv any data in current socket \n");
        status = CGI_INNER_ERROR;
        goto PROCESS_ERROR;
    }
    
    len += cgi_read_socket_data(blocks[read].buf + len , WEB_IMG_BLOCK_SIZE - len, p);
    
    if(len < 0){
        CGI_LOG("not recv any data \n");
        status = CGI_INNER_ERROR;
        goto PROCESS_ERROR;
    }
    
    /*
    cs_buf_print(blocks[read].buf,len); 
    */

    /*remove the request header*/
    header = (cs_uint8 *)strstr(((char*)blocks[read].buf), crlf2);
    if(header == NULL){
        CGI_LOG("can not get the end header \n");
        status = CGI_INNER_ERROR;
        goto PROCESS_ERROR;
    }

    blocks[read].len =  len - (header + strlen(crlf2) - blocks[read].buf);
    blocks[read].offset =  header - blocks[read].buf + strlen(crlf2);
    /*remove the request header end */


    cs_printf(".");
    rec_blocks++;
    /*
    cs_buf_print(blocks[read].buf + blocks[read].offset,len); 
    */
    data_buf = blocks[read].buf + blocks[read].offset;
    /* process the boundary */
    header = (cs_uint8 *)strstr(((char*)data_buf), crlf);
    if(header == NULL){
        CGI_LOG("can not get the right boundary \n");
        status = CGI_INNER_ERROR;
        goto PROCESS_ERROR;
    }
    
    memcpy(boundary,crlf,strlen(crlf));
    boundary_len += 2;
    memcpy(boundary + 2,data_buf,header - data_buf + strlen(crlf));
    boundary_len += header - data_buf + strlen(crlf);
    
    /*Rfc 1341, end boundary the same as start, but add with -- 
     * The encapsulation boundary following the last body part is a 
     * distinguished delimiter that indicates that no further body parts will follow.
     * Such a delimiter is identical to the previous delimiters, 
     * with the addition of two more hyphens at the end of the line:
     * */
    boundary_len += 2;
    /*
    cs_buf_print(boundary,strlen(boundary));
    */

    header = (cs_uint8 *)strstr(((char*)data_buf), crlf2);
    if(header == NULL){
        CGI_LOG("can not get the end header \n");
        status = CGI_INNER_ERROR;
        goto PROCESS_ERROR;
    }

    /*remove the multi data header*/
    blocks[read].len -=  (header + strlen(crlf2) - data_buf);
    blocks[read].offset +=  header - data_buf + strlen(crlf2);
    
    CGI_LOG("check magic\n");

    magic =  blocks[read].buf  + blocks[read].offset;
    /*
    cs_buf_print(blocks[read].buf + blocks[read].offset,blocks[read].len); 
    */

    /*check the magic number*/
    status = cgi_check_image_magic(magic, type, &magic_len);
    if(CGI_OK != status)
    {
        CGI_LOG("magic is not right %d\n", status);
        goto PROCESS_ERROR;
    }    


    blocks[read].offset += magic_len;
    blocks[read].len -= magic_len;
    magic += magic_len;

    img_len = cgi_get_image_len(magic, type, &tmp_len);
    
    blocks[read].offset += tmp_len;
    blocks[read].len -= tmp_len;
    
    /*
    cs_buf_print(blocks[read].buf + blocks[read].offset,blocks[read].len); 
    */
    /*prepare*/
    status = cgi_write_upload_data_prepare(type, img_len);
    if(CGI_OK != status)
    {
        CGI_LOG("upload data prepare fail %d\n", status);
        goto PROCESS_ERROR;
    }
    total_len +=  blocks[read].len;

    write = read;
    read += 1;

    while(1){
        memset(blocks[read].buf, 0x00,WEB_IMG_BLOCK_SIZE);
        len = cgi_read_socket_data(blocks[read].buf, WEB_IMG_BLOCK_SIZE, p);
        blocks[read].len = len;
        blocks[read].offset = 0;
        
        cs_printf(".");
        rec_blocks++;
        if(rec_blocks%32 == 0)
        {
            cs_printf("\n");
        }

        if(len > 0){
            total_len += len;
            /* read success */
            if((read + 1) %3 == write){ /* block full,wrote one block*/
                status = cgi_write_upload_data(
                        blocks[write].buf + blocks[write].offset,
                        blocks[write].len,
                        type);
                if(status != CGI_OK){
                    CGI_LOG("write file failed ,ret = %d \n",status);
                    status = CGI_INNER_ERROR;
                    goto FLASH_WRITE_FAILURE;
                }
                /*Sometime will cause ONU de-register, add the delay*/
                cs_thread_delay(50);
                write = (write + 1)%3; 
            }
            
            read = (read + 1)%3; 
            blocks[read].offset = 0;
            blocks[read].len = 0;
        }else{

            /*get the last block index */
            i = (read + 3 - 1 )%3;
            
            /* remove tail boundary from the blocked */
            if(blocks[i].len < boundary_len){
                CGI_LOG("tail cross blocks \n");
                /*remove more boundary from previous block*/
                if(write != i){ 
                    blocks[(i + 3 - 1 )%3].len -= (boundary_len - blocks[i].len);
                }else{
                    CGI_LOG("error, not found the tail \n");
                    status = CGI_INNER_ERROR;
                    goto FLASH_WRITE_FAILURE;
                }
                blocks[i].len = 0;
            }else{
                blocks[i].len -= boundary_len;
            }

            /*wrote the remain block*/
            for(i = write; i != read && blocks[i].len != 0 ; i = (i+1)%3){
                status = cgi_write_upload_data(
                        blocks[i].buf + blocks[i].offset,
                        blocks[i].len,
                        type);

                if(status != CGI_OK){
                    CGI_LOG("write last file part error,ret =  \n",status);
                    status = CGI_INNER_ERROR;
                    goto FLASH_WRITE_FAILURE;
                }
                /*cs_buf_print(blocks[i].buf + blocks[i].offset,blocks[i].len);*/
            }

            break;
        }
    }

    cs_printf("image len = %d ,total len = %d \n",img_len,total_len - boundary_len);
    /*Erase the other block*/
    if(type == CGI_UPLOAD_JFFS2_TYPE){
       do_erase_jffs2(img_len,0);
    }


FLASH_WRITE_FAILURE:
    if(status != CGI_OK){
        status =  cgi_write_upload_data_end(type, FALSE);
    }else{
        status = cgi_write_upload_data_end(type, TRUE);
    }

PROCESS_ERROR:
    g_web_upgrade = FALSE;
    do{
        len = cgi_read_socket_data(blocks[0].buf, WEB_IMG_BLOCK_SIZE, p);
        if(len > 0){
            cs_printf("len = %d \n",len);
            total_len += len;
        }
    }while(len > 0);

    cs_printf("finnal image len = %d ,total len = %d \n",img_len,total_len - boundary_len);

    if(blocks != NULL){
        iros_free(blocks);
    }

    cs_led_proc(LED_RUN, LED_GREEN, LED_TWINKLE);
    return status;
}

cgi_status cgi_get_wlb_image(cgi_handler_param_t * p)
{
    CGI_DEBUG("cgi_get_wlb_image enter \n");
    if(CGI_OK == cgi_upload_image_process(p, CGI_UPLOAD_WLB_TYPE))
    {
        cgi_send_int(p, CGI_OK);

        /*delay 3 seconds*/
        cs_thread_delay(3000);
        cgi_sys_reset(p);

        return CGI_OK;
    }
    else
    {
        cgi_send_int(p, CGI_INNER_ERROR);
        return CGI_INNER_ERROR;
    }
}

cgi_status cgi_get_mif_image(cgi_handler_param_t * p)
{
    CGI_DEBUG("cgi_get_mif_image enter \n");
    if(CGI_OK == cgi_upload_image_process(p, CGI_UPLOAD_MIF_TYPE))
    {
        cgi_send_int(p, CGI_OK);
        if(g_verify_img_flag == 2)
        {
            /*delay 3 seconds*/
            cs_thread_delay(3000);
            cgi_sys_reset(p);
        }
        return CGI_OK;
    }
    else
    {
        cgi_send_int(p, CGI_INNER_ERROR);
        return CGI_INNER_ERROR;
    }
}

#ifdef HAVE_FLASH_FS
cgi_status cgi_get_jffs2_image(cgi_handler_param_t * p)
{
    CGI_DEBUG("cgi_get_jffs2_image enter \n");
    
    if(CGI_OK == cgi_upload_image_process(p, CGI_UPLOAD_JFFS2_TYPE))
    {
        cgi_send_int(p, CGI_OK);
        return CGI_OK;
    }
    else
    {
        cgi_send_int(p, CGI_INNER_ERROR);
        return CGI_INNER_ERROR;
    }
}
#endif



