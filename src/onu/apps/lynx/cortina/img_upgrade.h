#ifndef _IMG_UPGRADE_H
#define _IMG_UPGRADE_H

#define MAX_ONU_IMAGE_FRAME_SIZE      1400

#define MAX_ONU_IMAGE_SIZE      1024*1024



#define IMG_UPGRADE_FILE_NAME_MAX_LEN  32

typedef struct{
    cs_uint16 opcode;
    cs_uint8 data[1];
} __attribute__((packed)) img_upgrade_tftp_hdr_t;

typedef struct{
    cs_uint16 opcode;
    cs_uint16 block;
    cs_uint8 data[1];
} __attribute__((packed)) img_upgrade_tftp_data_t;

typedef struct{
    cs_uint16 opcode;
    cs_uint16 block;
    cs_uint8 data[0];
} __attribute__((packed)) img_upgrade_tftp_rsp_t;

typedef enum{
    IMG_UPGRADE_WRITE_FLASH_OK = 0,
    IMG_UPGRADE_WRITE_FLASH_FAIL ,
    IMG_UPGRADE_NO_SPACE ,
    IMG_UPGRADE_ALREADY_EXIST,
    IMG_UPGRADE_CRC_FAIL,
}img_upgrade_write_flash_result_e;

typedef enum{
    IMG_UPGRADE_PROC_OK = 0,
    IMG_UPGRADE_UNKOWN_OP ,
    IMG_UPGRADE_WRONG_STATUS,
    IMG_UPGRADE_DUM_PACKET,
    IMG_UPGRADE_WRONG_PACKET,
    IMG_UPGRADE_OVER_BUF
}img_upgrade_frame_proc_result_e;

typedef enum{
    IMG_UPGRADE_COMMIT_OK = 0,
    IMG_UPGRADE_COMMIT_FAIL
}img_upgrade_commit_result_e;

typedef enum{
    IMG_UPGRADE_DOWNLOAD_OK = 0,
}img_upgrade_download_result_e;

typedef enum{
    IMG_UPGRADE_WRITE_FLASH_MSG = 0,
    IMG_UPGRADE_COMMIT_MSG,
    IMG_UPGRADE_RESET
}img_upgrade_msg_t;

typedef enum{
    IMG_UPGRADE_TFTP_RRQ = 1,
    IMG_UPGRADE_TFTP_WRQ = 2,
    IMG_UPGRADE_TFTP_DATA = 3,
    IMG_UPGRADE_TFTP_ACK = 4,
    IMG_UPGARDE_TFTP_ERR  =5,
}img_upgrade_tftp_op_e;

typedef void  (*img_upgrade_write_flash_callback)(img_upgrade_write_flash_result_e result);
typedef void  (*img_upgrade_download_end_callback)(img_upgrade_download_result_e result);
typedef void  (*img_upgrade_commit_callback)(img_upgrade_commit_result_e result);

typedef struct{
    img_upgrade_write_flash_callback write_call;
    img_upgrade_download_end_callback  download_call;
    img_upgrade_commit_callback  commit_call;
}img_upgrade_callback_funct_t;

typedef enum{
    IMG_UPGRADE_IDLE = 0,
    IMG_UPGRADE_DL_START ,
    IMG_UPGRADE_TRANSFER,
    IMG_UPGRADE_DL_STOP,
    IMG_UPGRADE_WRITTING,
    IMG_UPGRADE_FINISH
}img_upgrade_status_e;

typedef enum{
    IMG_UPGRADE_WRITE_WHOLE = 0,
    IMG_UPGRADE_WRITE_PART
}img_upgrade_write_mode_e;

typedef struct{
    img_upgrade_status_e status;  /*work status*/
    img_upgrade_write_mode_e write_mode;  /*write mode*/
    img_upgrade_callback_funct_t callback;  /*callback handler*/
    cs_uint32 size;     /*image buf size*/
    cs_uint32 curr_len;  /*current  size in buf*/
    cs_uint32 total_len; /*total download size*/
    cs_int32 blob_idx;  /*blob index*/
    cs_uint8 *flash_des;
    cs_uint16 next_seq;
    cs_boolean direct_confirm;
    cs_boolean watchdog_enable;
    cs_uint8* buf;      /*image buf*/
    cs_uint8 name[IMG_UPGRADE_FILE_NAME_MAX_LEN+1];
}img_upgrade_cb_t;

img_upgrade_frame_proc_result_e img_upgrade_tftp_frame_proc(
            img_upgrade_tftp_hdr_t *tftp_hdr,
            cs_uint32 in_len,
            cs_uint8 *pOut,
            cs_uint32 *out_len);

void img_upgrade_init();
cs_status img_upgrade_download_start(cs_uint32 img_size, img_upgrade_download_end_callback callback,img_upgrade_write_mode_e mode);
cs_status img_upgrade_download_stop(cs_boolean hold_buf);
cs_uint32 img_upgrade_downloaded_size_get();
cs_uint8* img_upgrade_download_buf_get();
img_upgrade_frame_proc_result_e img_upgrade_write_buf(cs_uint8* data, cs_uint32 len);
cs_status img_upgrade_wirte_flash(img_upgrade_write_flash_callback callback);
cs_status img_upgrade_commit(img_upgrade_commit_callback callback);
cs_status img_upgrade_reset();
 void img_upgrade_confirm_policy_set(cs_boolean direct_confirm);
 cs_boolean img_upgrade_in_progress();
 cs_status img_upgrade_clean();
 void img_upgrade_set_mif_support(cs_boolean enable);

#endif /* #ifndef _IMG_UPGRADE_H */

