/* Copyright (c) 2002 by Cortina Systems Incorporated.                       */
/*****************************************************************************/
/*
 * iros_flash_oper.c
 */

#ifdef HAVE_MIF
#include "iros_config.h"
#include "plat_common.h"
#include <pkgconf/hal.h>

#include "flash_dev_driver.h"
#include "startup_cfg.h"
#include "mif.h"

#define DEV_DEBUG


static cs_uint8 mif_init_flag = 0;
cs_uint32 flash_update_mutex = 0;

//extern onu_node_t onu_node;   //redefine?
static char updater_module;
static bool mif_parser_start, need_confirm;
static mif_upgrade_state_t  parser_state;

static int image_len;
static char *buf;
static int bufsz;
static int cur_index;

static epon_l2ftp_multi_image_header_t  multi_image_header;
static epon_l2ftp_multi_image_header_t *p_multi_image_header;
static multi_tlv_header_t  tlv_header, tlv_header_temp;

static bool waiting_tlv_header, multi_image_download;
static cs_uint32  tlv_header_temp_copied_len;
static cs_uint32 wanted_tlv_data_len;
static cs_uint32  remained_all_tlvs_len, blob_part_len;
static bool write_tlv_enable;
static cs_uint32 copy_len, needed_copy_len;
static unsigned char left_bytes[4];
static unsigned char left_bytes_len;

extern int fs_mount(void);
extern int fs_umount(void);

int g_jffs2_erase_block_id =0;

static void dumpBuff(char * tmp, cs_uint32 len)
{
   int i = 0, count=0;

   for(i=0; i<len; i++) {
                cs_printf("%x ", *tmp ++ );
                count++;
                if (count == 16) {
                    cs_printf("\n");
                    count = 0;
                }
    }
   cs_printf("\n");

}

/*Flash Jffs2 offset,Erase all the block if block_num is 0.*/
void do_erase_jffs2(unsigned int flash_offset,unsigned int block_num)
{
    cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_JFFS2_ID);
    cs_uint8 *flash_des;
    cs_uint8 loop = 0;
    cs_uint32 part_size = 0;
    cs_uint32 block_size = 0;
    
    if(IROS_FLASH_PARTITION_INDEX_ANY == part_index)
    {
        return;
    }   
    
    flash_des = flash_dev.info.super_block.part_tab[part_index].part_loc; 
    part_size = flash_dev.info.super_block.part_tab[part_index].part_size;
    block_size = flash_dev.info.super_block.block_size;
    
    if(flash_offset)
    {
        if(flash_offset%block_size)
            loop=flash_offset/block_size + 1;
        else
            loop=flash_offset/block_size;
    }

    /*Erase all the block*/
    if(!block_num)
    {
        block_num = (part_size/block_size);
    }

    cs_printf("Erase Jffs2 From 0x%x,Wait...\n",(flash_des+(loop*block_size)));
    while((loop*block_size < part_size)&&block_num)
    {
        flash_erase_block_main((cs_uint32)(flash_des+(loop*block_size)));
        cs_thread_delay(100);
        loop++;
        block_num--;
    }
    return;
}
/*
return 0 if ok. All other return value meaning failure.
input 1: char module,   1 for sc, 2 for web, 3 for OLT
input 2: whether I need to set the confirm field for the updated blob. From my understanding, SC updater need
set true to this parameter, web and olt updateR need to set false to this parameter*/
int prepare_mif_upgrade (char module,bool need_confirm_from_server) {
    #ifdef DEV_DEBUG
        cs_printf("Module %d try to update.\n",module);
    #endif
    
    if(0 == mif_init_flag)
    {
        if(CS_E_OSAL_OK != cs_mutex_init(&flash_update_mutex, "mif_update_mutex", 0))
        {
            return 1;
        }
        mif_init_flag = 1;
    }
    
    cs_mutex_lock(flash_update_mutex);
    //maybe some module will re-start it even when it is already occupied this parser.
    if ((updater_module ==0) ||(updater_module == module))
        updater_module = module;
    else {
        cs_mutex_unlock(flash_update_mutex);
        return 1;
    }
    cs_mutex_unlock(flash_update_mutex);

    mif_parser_start = true;
    need_confirm = need_confirm_from_server;
    #ifdef DEV_DEBUG
        cs_printf("Module %d got the parser.\n",module);
    #endif
    return 0;
}


static int multi_image_tlv_transfer_init(char *flash_start_addr, int buf_len)
{
        buf = flash_start_addr;
        bufsz = buf_len;
        image_len = 0;
        left_bytes_len = 0;

        return 1;
}

//return 0 for failure, 1 for ok.
static int multi_image_nand_update_init(unsigned char part_type){
    int part_index;
    int flash_len;
    char *des;
    int cur_blob_idx;

   switch (part_type) {
   case TLV_ZBLOB_TYPE:
       // first, make sure the current blob is committed already, otherwise abort the update
       cur_blob_idx = current_blob_desc.blob_idx;
       //char *blob_start = (char *)(blob_info_tab[cur_blob_idx]->blob_start_ptr);
       if ((blob_info_tab[cur_blob_idx]->is_not_confirmed) && (need_confirm)){
                /*
                is_file_transfer_in_progress = false;
                updateInProgress = 0;
                active_update_port = 0; */
                cs_printf("update failed - unable to update before committed\n");

                // reboot
                cs_printf("reboot\n");

                //hal_delay_us(500000);
                //cyg_thread_delay(100);
                HAL_PLATFORM_RESET();

                return 0;
        }


        // simply pick the other blob to replace
        int blob_idx = 0x1 &(cur_blob_idx + 1);
        part_index = blob_part_tab[blob_idx];
        cs_printf("\nto update blob %d to %d\n", cur_blob_idx, blob_idx);
        blob_info_desc_t *blob_info = blob_info_tab[blob_idx];

        des = (char *)((unsigned int )blob_info + sizeof(blob_info_desc_t));

        if (flash_part_write_init(part_index, (char*)blob_info)) { // prepare to write flash
            // ready to write the blob partition
            cs_printf("Blob %d start loading at 0x%08x ...\n", blob_idx, (unsigned int) des);
        } else {
            // error
            cs_printf("Flash for blob %d at 0x%08x not ready\n", blob_idx, (unsigned int) blob_info);
            return 0;
        }
        int max_blob_size = flash_dev.info.super_block.part_tab[part_index].part_size;
        flash_len = max_blob_size - sizeof(blob_info_desc_t);


        multi_image_tlv_transfer_init(des, flash_len);
        break;

   case TLV_JFFS2_TYPE:
       part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_JFFS2_ID);
       if(IROS_FLASH_PARTITION_INDEX_ANY == part_index)
       {
           cs_printf("no partition for jffs2\n");
           return 0;
       }

       des = (char *)flash_dev.info.super_block.part_tab[part_index].part_loc;
       if (flash_part_write_init(part_index, des)) { /* prepare to write flash */
            flash_len = flash_dev.info.super_block.part_tab[part_index].part_size;
            multi_image_tlv_transfer_init(des, flash_len);
            #ifdef HAVE_FLASH_FS
            fs_umount();
            #endif
            /*All Jffs2 should be erase,but MPCP will timeout if jffs2 partiton is too large.*/
            /*do_erase_jffs2(0,0);*/
            {
                int block_size =flash_dev.info.super_block.block_size;               
                cs_printf("jffs2 len 0x%x\n",wanted_tlv_data_len);              
                cs_printf("Erase jffs2 partition %p,len 0x%x,block size 0x%x.\n",des,flash_len,block_size);
                if(wanted_tlv_data_len%block_size)
                    g_jffs2_erase_block_id =wanted_tlv_data_len/block_size+1;    
                else
                    g_jffs2_erase_block_id =wanted_tlv_data_len/block_size; 
                
            }

       }
       else {
            return 0;
       }

       break;

   case TLV_START_CONFIG_TYPE:
    
       part_index = startup_config_backup_index(1);
       
       if (part_index == IROS_FLASH_PARTITION_INDEX_ANY) {
            cs_printf("\nFlash startup config partition not found\n");
            return 0;
       }else {
            des = (char *) flash_dev.info.super_block.part_tab[part_index].part_loc;
       }
       /*
       if (startup_config_base_in_flash != flash_dev.info.super_block.part_tab[part_index].part_loc) {
           cs_printf("startup_config_base_in_flash does not match the startup config partition\n");
           return 0;
       } */

       if (flash_part_write_init(part_index, des)) { /* prepare to write flash */
            flash_len = STARTUP_CFG_LEN;
            multi_image_tlv_transfer_init(des, flash_len);
            #ifdef DEV_DEBUG
                cs_printf("\nFlash startup des: 0x%x, len: %d,\n", (int)des, flash_len);
            #endif
       }
       else {
            return 0;
       }

       break;
   case TLV_BINARY_TYPE:
       if (flash_part_write_init(IROS_FLASH_PARTITION_INDEX_ANY, (char *)tlv_header.destination)) { /* prepare to write flash */
            multi_image_tlv_transfer_init((char *)tlv_header.destination, tlv_header.width);
            #ifdef DEV_DEBUG
                cs_printf("\nTLV_BINARY_TYPE type update: addr: 8x%x, len: %d\n", \
                              tlv_header.destination, tlv_header.width);
            #endif
       }
       else {
            return 0;
       }break;
   default:
        return 0;
        break;

   }
   cur_index = part_index;
   return 1;
}


// return 0 if error
// return 1 if ok
//extern bool need_to_reset;
static int app_update_file_transfer_done(int len) {

    int cur_blob_idx = current_blob_desc.blob_idx;

    char *des;
    blob_info_desc_t info;

    // simply pick the other blob to replace
    int blob_idx = 0x1 &(cur_blob_idx + 1);
    int part_index = blob_part_tab[blob_idx];
    blob_info_desc_t *blob_info = blob_info_tab[blob_idx];

    IROS_DPRINTF("app_update_file_transfer_done\n");

    if(!len)
      {
         cs_printf("file trasnfered fialed len = 0, rebooting\n");
         //hal_delay_us(100000);
         //HAL_PLATFORM_RESET();
      }
    else
        cs_printf("file trasnfered done, blob partition len: %d\n", len);
    
     if (!flash_part_write_init(part_index, (char*)blob_info)){
        cs_printf("can not init nand in app_update_file_transfer_done\n");
        return 1;
    }


    des = (char *)((unsigned int )blob_info + sizeof(blob_info_desc_t));

    info.head_signature = BLOB_INFO_HEAD_SIGNATURE;
    info.len = len;
    info.blob_start_ptr = (struct image_blob_header_t *) des;
    info.tail_signature = BLOB_INFO_TAIL_SIGNATURE;
    info.upgrade_stamp = blob_info_tab[cur_blob_idx]->upgrade_stamp + 1;
    info.is_not_confirmed = 0;
    info.is_not_executed = 0;
    
    // write control block of this blob, each by field
    flash_write_no_erase((char *)&blob_info->head_signature, (char *)&info.head_signature, sizeof(info.head_signature));
    flash_write_no_erase((char *)&blob_info->upgrade_stamp, (char *)&info.upgrade_stamp, sizeof(info.upgrade_stamp));
    flash_write_no_erase((char *)&blob_info->len, (char *)&info.len, sizeof(info.len));
    flash_write_no_erase((char *)&blob_info->blob_start_ptr, (char *)&info.blob_start_ptr, sizeof(info.blob_start_ptr));
    flash_write_no_erase((char *)&blob_info->tail_signature, (char *)&info.tail_signature, sizeof(info.tail_signature));
    if (!need_confirm) {
        flash_write_no_erase((char *)&blob_info->is_not_confirmed, (char *)&info.is_not_confirmed, sizeof(info.is_not_confirmed));
        flash_write_no_erase((char *)&blob_info->is_not_executed, (char *)&info.is_not_executed, sizeof(info.is_not_executed));
    }

    // flash_write is completed, no need to call flash_part_write_done because reboot
    flash_part_write_done(IROS_FLASH_PARTITION_INDEX_ANY);

    //hal_delay_us(500000);
    //cyg_thread_delay(100);

    // reboot
    /*
    cs_printf("reboot\n");
    HAL_PLATFORM_RESET();  */

    return 1;

}



 /* If the top application encounter error, it should call this function with end_of_file set to true, so that this function can release
 the parser to other modules. In this case, it is better to set lenght equals 0, so that know extra data is written to flash.

Return 0 if ok,
All other return value mean error.
1, the first frame is too short to contain a complete mif header, we assume the first frame contain the complete mif header.
2, mif length is not the nor of mif length_xor field.
3, mif file don't support this chip on this ONU board.
4,  can not init nand update successfully
5, some tlv segment type is not supported by current code.
6, the last processed tlv segment is too big for its flash segment.
7, the parser is occupied by other module.
8, some tlv length is not the nor of mif length_xor field.
9, mif file don't support ONU board type.
10, other system error.

After return failure to caller, if the caller want to re-try the parser, it must call prepare_mif_upgrade() first again,
because the parser already re-set the module number to 0, so that other module can get it.

Input1, module number who call this function,
Input2, Input data buffer.
Input3, data length. When first call this function, the data buffer should at least contain mif header bytes.
Input4, the pointer which this function used to tell the caller the internal status.
Input5, Set to true to indicate this block is the last data block, parser itself can not find the end of data stream when
           it working in the blob file download compatible mode. Even when working for mif data type, it now also depend
           on the caller to indicate the end of data stream, even the parser know when the data stream is ended.

*/
int mif_upgrade (char module, char * input_buff, int length, mif_upgrade_state_t * cur_state, bool end_of_file)
{
    cs_uint32 unprocessed_frame_len = length;
    cs_uint8 *unprocessed_data_pos = input_buff;
    int temp;
    int block_size =flash_dev.info.super_block.block_size;
    
    cs_mutex_lock(flash_update_mutex);
    if (updater_module != module) {
        cs_mutex_unlock(flash_update_mutex);
        return PARSER_IS_BUSY;
    }
    cs_mutex_unlock(flash_update_mutex);

    if (mif_parser_start){
        mif_parser_start = false;

        //need_to_reset = false;
        waiting_tlv_header = false;
        tlv_header_temp_copied_len = 0;
        wanted_tlv_data_len = 0;
        blob_part_len = 0;
        write_tlv_enable = true;
        left_bytes_len = 0;

        parser_state.end_of_mif = false;
        parser_state.tlv_type_included = 0;
        parser_state.tlv_update_successed = 0;

        // We assume the first block of data is longer than the mif header.
        if (length < sizeof( epon_l2ftp_multi_image_header_t )){
             cs_printf("l2ftp: too short multi image header\n");
             cs_mutex_lock(flash_update_mutex);
             updater_module = 0;
             cs_mutex_unlock(flash_update_mutex);
             return FIRST_FRAME_TOO_SHORT;
        }

         p_multi_image_header = (epon_l2ftp_multi_image_header_t *)input_buff;
         multi_image_header.mif_magic  = ntohl(p_multi_image_header->mif_magic);
         multi_image_header.version    = p_multi_image_header->version;
         multi_image_header.chip_bmp   = ntohl(p_multi_image_header->chip_bmp);
         multi_image_header.board_bmp  = ntohl(p_multi_image_header->board_bmp);
         multi_image_header.mif_crc32  = ntohl(p_multi_image_header->mif_crc32);
         multi_image_header.length     = ntohl(p_multi_image_header->length);
         multi_image_header.length_xor = ntohl(p_multi_image_header->length_xor);

         remained_all_tlvs_len = multi_image_header.length;

         if (multi_image_header.mif_magic == EPON_L2FTP_MULTI_IMAGE_MAGIC){

              if (multi_image_header.length != ~multi_image_header.length_xor){
                  cs_printf("l2ftp: multi-image length and length_xor are not matched\n");
                  cs_mutex_lock(flash_update_mutex);
                  updater_module = 0;
                  cs_mutex_unlock(flash_update_mutex);
                  return MIF_LEN_NO_MATCH;
              }
              /*
              //Check for chip_bmp and board_bmp mif_crc32 later.
              //only support LynxP(CS8015),LynxB (CS8016B) and LynxC (CS8016C) now.
              unsigned int temp_32;
              temp_32 = 0;
              switch (onu_node.chip_version.chip_type) { //here seems have some bug.
              case EPON_CHIP_TYPE_CS8015A:  //or xx_xx_xx_CS8015 ?
                     temp_32 |= CHIP_BMP_LYNXP;
                     break;
              case EPON_CHIP_TYPE_CS8016B0:
                     temp_32 |= CHIP_BMP_LYNXB;
                     break;
              case EPON_CHIP_TYPE_CS8016C0:
                     temp_32 |= CHIP_BMP_LYNXC;
                     break;
              default:
                     break;
              }

              #ifdef DEV_DEBUG
              cs_printf("chip_bmp: 0x%8x, local chip: 0x%8x\n",
                                   multi_image_header.chip_bmp, onu_node.chip_version.chip_type);
              #endif

              if(! (multi_image_header.chip_bmp & temp_32)){
                   cs_printf("Mif image don't support this chip. chip_bmp: 0x%8x, local chip: 0x%8x\n",
                                   multi_image_header.chip_bmp, temp_32);
                   cs_mutex_lock(flash_update_mutex);
                   updater_module = 0;
                   cs_mutex_unlock(flash_update_mutex);
                   return 3;
              }
              */

              waiting_tlv_header = true;
              multi_image_download = true;
              parser_state.is_mif_file = true;
              unprocessed_frame_len = length - sizeof( epon_l2ftp_multi_image_header_t);
              unprocessed_data_pos = input_buff + sizeof( epon_l2ftp_multi_image_header_t);


          } else {
              cs_printf("l2ftp: multi-image magic number is wrong or it is a blob file download\n");
              cs_printf("received mif_mage: 0x%x, should be: 0x%x\n", multi_image_header.mif_magic,EPON_L2FTP_MULTI_IMAGE_MAGIC);
              waiting_tlv_header = false;
              multi_image_download = false;
              parser_state.is_mif_file = false;

              remained_all_tlvs_len = 0xffffffff;

              //construct a tlv header for compatible.
              tlv_header.type          =  TLV_ZBLOB_TYPE;
              tlv_header.decompress = 0x00;
              //tlv_header.destination  = ntohs( tlv_header_temp.destination);
              //tlv_header.time           = ntohs( tlv_header_temp.time);
              //tlv_header.version       = ntohs( tlv_header_temp.version);
              tlv_header.width         =  0xffffffff;
              tlv_header.width_xor    = 0x0;

              wanted_tlv_data_len = tlv_header.width;

              unprocessed_frame_len = length ;
              unprocessed_data_pos = input_buff;

              parser_state.tlv_type_included |= 1<<0;
              write_tlv_enable = true;


              // prepare the nand write initilization for this partision.
              if (multi_image_nand_update_init (tlv_header.type) == 0) {
                   cs_printf("l2ftp: can not init nand update successfully\n");
                   //maybe need to do more here.
                   cs_mutex_lock(flash_update_mutex);
                   updater_module = 0;
                   cs_mutex_unlock(flash_update_mutex);
                   return NAND_INIT_FAILED;
              }
          }

          #ifdef DEV_DEBUG
                cs_printf("multi-image header received: mif_magic: 0x%x,  version: %d\n", \
                               multi_image_header.mif_magic, multi_image_header.version );
                cs_printf("chip_bmp: 0x%x,  board_bmp: 0x%x, mif_crc32: 0x%x, \n ", \
                                multi_image_header.chip_bmp, multi_image_header.board_bmp, \
                                 multi_image_header.mif_crc32 );
                cs_printf("length: 0x%x, length_xor: 0x%x \n",  \
                                multi_image_header.length, multi_image_header.length_xor);
           #endif
    }

    //while( (unprocessed_frame_len >0) &&())
    while ((unprocessed_frame_len >0) &&(remained_all_tlvs_len != 0))
    {
        #ifdef DEV_DEBUG
        if (unprocessed_frame_len >length ) {
            cs_printf("extra byte are sent to onu: %d, %d.\n",unprocessed_frame_len, length);
            cs_printf("%d, %d, %d.\n",waiting_tlv_header, wanted_tlv_data_len,remained_all_tlvs_len);
        }
        #endif

        if (waiting_tlv_header ){
            needed_copy_len = sizeof (multi_tlv_header_t) - tlv_header_temp_copied_len;
            if (needed_copy_len <= unprocessed_frame_len) {
                copy_len = needed_copy_len;
                memcpy ((((char *)&tlv_header_temp) + tlv_header_temp_copied_len),  unprocessed_data_pos, copy_len);
                unprocessed_frame_len -= copy_len;
                unprocessed_data_pos += copy_len;

                waiting_tlv_header = false;
                tlv_header_temp_copied_len = 0;

                //copy temp header to formal header.
                tlv_header.type          = tlv_header_temp.type;
                tlv_header.decompress = tlv_header_temp.decompress;
                tlv_header.destination  = ntohl( tlv_header_temp.destination);
                tlv_header.time           = ntohl( tlv_header_temp.time);
                tlv_header.version       = ntohl( tlv_header_temp.version);
                tlv_header.width         = ntohl( tlv_header_temp.width);
                tlv_header.width_xor   = ntohl( tlv_header_temp.width_xor);

                #ifdef DEV_DEBUG
                cs_printf("\n tlv header received: type: 0x%x,  decompress: %d\n", \
                            tlv_header.type, tlv_header.decompress);
                cs_printf("destination: 0x%x,  time: 0x%x, version: 0x%x \n ",  \
                             tlv_header.destination,tlv_header.time  , tlv_header.version);
                cs_printf("width: 0x%x, width_xor: 0x%x \n ",  \
                             tlv_header.width, tlv_header.width_xor);
                #endif

                if (tlv_header.width != ~tlv_header.width_xor){
                    cs_printf("l2ftp: tlv length and length_xor are not matched\n");
                    cs_mutex_lock(flash_update_mutex);
                    updater_module = 0;
                    cs_mutex_unlock(flash_update_mutex);
                    return TLV_LEN_NOT_MATCH;
                }

                wanted_tlv_data_len = tlv_header.width;

                remained_all_tlvs_len -= sizeof (multi_tlv_header_t);
                //remained_all_tlvs_len -= tlv_header.width;
                write_tlv_enable = true;

                parser_state.tlv_type_included |= (1 << tlv_header.type);

                if ((tlv_header.type != TLV_ZBLOB_TYPE ) &&
                    (tlv_header.type != TLV_JFFS2_TYPE ) &&
                    (tlv_header.type != TLV_START_CONFIG_TYPE ) &&
                    (tlv_header.type != TLV_BINARY_TYPE ))
                {
                    cs_printf("l2ftp: don't support this tlv type, type: %d\n", tlv_header.type);
                    /*
                    cs_mutex_lock(flash_update_mutex);
                    updater_module = 0;
                    cs_mutex_unlock(flash_update_mutex);
                    return 5;   */
                    write_tlv_enable = false;
                }

                if  (tlv_header.decompress !=0)
                {
                     cs_printf("l2ftp: type: %d tlv segment is ignored.\n", tlv_header.type);
                     write_tlv_enable = false;
                }

                // prepare the nand write initilization for this partision.
                if (multi_image_nand_update_init (tlv_header.type) == 0) {
                    cs_printf("l2ftp: can not init nand update successfully. Tlv type: %d\n", tlv_header.type);
                    cs_mutex_lock(flash_update_mutex);
                    updater_module = 0;
                    cs_mutex_unlock(flash_update_mutex);
                    return NAND_INIT_FAILED;
                }
            }
            else {  //don't have enough data for this tlv header
                copy_len = unprocessed_frame_len;
                memcpy (((char *)&tlv_header_temp + tlv_header_temp_copied_len),  unprocessed_data_pos, copy_len);
                unprocessed_frame_len -= copy_len; //should be 0 now.
                unprocessed_data_pos += copy_len;

                waiting_tlv_header = true; //don't need.
                tlv_header_temp_copied_len += copy_len;
            }
        }
        else {   //write data to this partition.
             /* to deal with data alignment problem, following copy code is added */
             void *temp_ptr= 0;
             unsigned int write_len;
             temp_ptr=iros_malloc(IROS_MID_UPGRADE, (unprocessed_frame_len + 4));
             if(!temp_ptr){
                 cs_printf("memory malloc error, 118899\n");
                 flash_part_write_done (cur_index);
                 return OTHER_SYSTEM_ERR;
             }

             if (left_bytes_len !=0)
                 memcpy (temp_ptr,  left_bytes, left_bytes_len);

             memcpy (temp_ptr + left_bytes_len,  unprocessed_data_pos, unprocessed_frame_len);

            if (wanted_tlv_data_len <= unprocessed_frame_len) {
                #ifdef DEV_DEBUG
                    cs_printf( "The end of a tlv seg is arrived, wanted len:%d, remain len: %d\n", \
                                       wanted_tlv_data_len, unprocessed_frame_len);
                #endif
                //not all frame data is for this partition, there maybe have data for next tlv header.
                copy_len = wanted_tlv_data_len;
                remained_all_tlvs_len -= copy_len;

                write_len = left_bytes_len + copy_len;
                left_bytes_len = 0;

                if ((image_len + write_len) <= bufsz) {
                    if (write_tlv_enable == true){
                        temp = flash_write(buf + image_len, temp_ptr, write_len);
                        if (temp != write_len)
                            cs_printf("write flash error: return len:%d, try length: %d\n",temp, write_len);
                        if (!flash_write_verify(buf + image_len, temp_ptr, write_len)) {
                            #ifdef DEV_DEBUG
                            cs_printf("flash address: 0x%x, buff addr: 0x%x\n",(cyg_uint32)(buf + image_len), (cyg_uint32)temp_ptr);
                              cs_printf("write buffer:\n");
                              dumpBuff(temp_ptr, write_len);
                              //cs_printf("flash address: 0x%x, length\n",buf + image_len, copy_len);
                              cs_printf("\n read back:\n");
                              dumpBuff(buf + image_len, write_len);
                            #endif
                            iros_free(temp_ptr);
                            flash_part_write_done (cur_index);
                              return OTHER_SYSTEM_ERR;
                        }
                        parser_state.tlv_update_successed |= (1 << tlv_header.type);
                    }

                    image_len += write_len;
                    unprocessed_data_pos += copy_len; //should >= 0;
                    unprocessed_frame_len -= copy_len;
                    wanted_tlv_data_len = 0;

                    //if (( unprocessed_frame_len >0) && (l2ftp_pdu_len >= EPON_L2FTP_BLOCK_SIZE)){
                    //if ((l2ftp_pdu_len >= EPON_L2FTP_BLOCK_SIZE) ||(remained_all_tlvs_len != 0)){
                    if ((multi_image_download == true) && (remained_all_tlvs_len != 0)) {
                        #ifdef DEV_DEBUG
                        cs_printf("There should have another tlv header, remained len: %d\n",unprocessed_frame_len);
                        cs_printf("remained_all_tlvs_len: %d\n",remained_all_tlvs_len);
                        #endif
                        waiting_tlv_header = true;
                        tlv_header_temp_copied_len = 0;
                    } else if ((multi_image_download == true) && (remained_all_tlvs_len == 0)){
                        #ifdef DEV_DEBUG
                        cs_printf("No remained tlv, should be finished now. remained length: %d\n", unprocessed_frame_len);
                                     unprocessed_frame_len = 0;
                        #endif
                    }

                    switch (tlv_header.type) {
                    case TLV_ZBLOB_TYPE:
                        blob_part_len = image_len;
                        flash_part_write_done (cur_index);
                        break;
                    case TLV_JFFS2_TYPE:
                        flash_part_write_done (cur_index);
                        #ifdef HAVE_FLASH_FS
                        fs_mount();
                        #endif
                        break;
                    case TLV_START_CONFIG_TYPE:
                        flash_part_write_done (cur_index);
                        #ifdef DEV_DEBUG
                        /* unsigned int temp_int;
                        int temp_i;
                        for (temp_i = 0; temp_i <128/4;temp_i++)
                        {
                             temp_int = *(unsigned int *)( 0x2f010000 +4*temp_i);
                             cs_printf( "temp_i: 0x%8x,   temp_int: 0x%8x\n",0x2f010000 +4*temp_i, temp_int);
                        } */
                        #endif
                        break;
                    case TLV_BINARY_TYPE:
                    default:
                        //flash_part_write_done ();
                        break;
                    }

                    //hal_delay_us(500000);
                    //cyg_thread_delay(100);
                }
                else {   //too big for this partition, something is wrong.  maybe all error return should do same thing.
                    cs_printf( "l2ftp: image too big\n");
                    cs_mutex_lock(flash_update_mutex);
                    updater_module = 0;
                    cs_mutex_unlock(flash_update_mutex);
                    iros_free(temp_ptr);
                    flash_part_write_done (cur_index);
                    return 6;
                }
            }
            else { //all data in current frame is for this partition
                copy_len = unprocessed_frame_len;
                remained_all_tlvs_len -= copy_len;

                write_len = (left_bytes_len + copy_len) & 0xfffffffc;
                left_bytes_len = (left_bytes_len + copy_len)%4;

                /* store the not allighned bytes for next time using */
                if (left_bytes_len !=0)
                    memcpy (left_bytes,  temp_ptr + write_len, left_bytes_len);

                if ((image_len + write_len) <= bufsz) {
                    if (write_tlv_enable == true){

/*Start:Use to reduce the upgrade time for jffs2*/
                        if(g_jffs2_erase_block_id&&((cs_uint32)(buf + image_len)%block_size)
                        &&((cs_uint32)(buf + image_len)/block_size==(cs_uint32)(buf + image_len + write_len)/block_size))
                        {
                            extern void flash_erase_block_main(cs_uint32 faddr);
                            int part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_JFFS2_ID);
                            char* des = (char *)flash_dev.info.super_block.part_tab[part_index].part_loc;
                            int flash_len = flash_dev.info.super_block.part_tab[part_index].part_size;

                            if((g_jffs2_erase_block_id*block_size)<flash_len)
                            {
                                flash_erase_block_main((cs_uint32)(des+g_jffs2_erase_block_id*block_size));
                                g_jffs2_erase_block_id++;
                            }
                            else
                            {
                                g_jffs2_erase_block_id=0;
                                cs_printf("Erase jffs2 finish!");
                            }
                            if(module == IROS_MID_WEB)
                                cs_thread_delay(200);
                        }
/*End*/

                        temp = flash_write(buf + image_len, temp_ptr, write_len);
                        if (temp != write_len)
                            cs_printf("write flash error: return len:%d, try length: %d\n",temp, write_len);
                        if (!flash_write_verify(buf + image_len, temp_ptr, write_len)) {
                            #ifdef DEV_DEBUG
                              cs_printf("flash address: 0x%x, buff addr: 0x%x\n",(cyg_uint32)(buf + image_len), (cyg_uint32)temp_ptr);
                              cs_printf("write buffer:\n");
                              dumpBuff(unprocessed_data_pos, write_len);
                              //cs_printf("flash address: 0x%x, length\n",buf + image_len, copy_len);
                              cs_printf("\n read back:\n");
                              dumpBuff(buf + image_len, write_len);
                            #endif

                            iros_free(temp_ptr);
                            flash_part_write_done (cur_index);
                              return OTHER_SYSTEM_ERR;
                        }
                    }

                    image_len += write_len;
                    unprocessed_data_pos += copy_len;
                    unprocessed_frame_len -= copy_len;//should = 0;
                    #ifdef DEV_DEBUG
                    if (unprocessed_frame_len != 0)
                        cs_printf( "error: 23133\n");
                    #endif
                    wanted_tlv_data_len -= copy_len;
                }
                else {   //too big for this partition, something is wrong.
                    cs_printf( "l2ftp: image too big\n");
                    cs_mutex_lock(flash_update_mutex);
                    updater_module = 0;
                    cs_mutex_unlock(flash_update_mutex);
                    iros_free(temp_ptr);
                    flash_part_write_done (cur_index);
                    return TLV_TOO_BIG;
                }
            }
            iros_free(temp_ptr);

        }
    }    //end while()

    //if ((end_of_file == true)||(remained_all_tlvs_len == 0))
    if (end_of_file == true)
    {
        #ifdef DEV_DEBUG
        cs_printf( "finish transfer now, last block length: %d, remained_all_tlvs_len:%d.\n",  \
                      length,remained_all_tlvs_len);
        #endif
        g_jffs2_erase_block_id = 0;
        if (multi_image_download == false ) {
            if (left_bytes_len !=0) {
                if ((image_len + left_bytes_len) <= bufsz) {
                    if (write_tlv_enable == true){
                        temp = flash_write(buf + image_len, left_bytes, left_bytes_len);
                        if (temp != left_bytes_len)
                            cs_printf("write flash error: return len:%d, try length: %d\n",temp, left_bytes_len);
                        if (!flash_write_verify(buf + image_len, left_bytes, left_bytes_len)) {
                            #ifdef DEV_DEBUG
                              cs_printf("flash address: 0x%x, buff addr: 0x%x\n",(cyg_uint32)(buf + image_len), (cyg_uint32)left_bytes);
                              cs_printf("write buffer:\n");
                              dumpBuff(unprocessed_data_pos, left_bytes_len);
                              //cs_printf("flash address: 0x%x, length\n",buf + image_len, copy_len);
                              cs_printf("\n read back:\n");
                              dumpBuff(buf + image_len, left_bytes_len);
                            #endif
                              flash_part_write_done (cur_index);
                              return OTHER_SYSTEM_ERR;
                        }
                    }
                }

            }

            image_len += left_bytes_len;
            left_bytes_len = 0;

            cs_printf(" not multi image download finished.\n");
            //need_to_reset = true;
            blob_part_len = image_len;
            parser_state.tlv_update_successed |= 1;
            //flash_part_write_done (IROS_FLASH_PARTITION_TYPE_BLOB_ECOS);
            flash_part_write_done(IROS_FLASH_PARTITION_INDEX_ANY);
            //hal_delay_us(500000);
            //cyg_thread_delay(100);
        }

        IROS_LOG_CRI(IROS_MID_L2FTP,
                     "\nl2ftp done: len %d\n", image_len);
        flash_part_write_done (cur_index);

        //int app_update_file_transfer_done(int len);
        if (parser_state.tlv_update_successed & 0x1){
              //hal_delay_us(500000);
            //cyg_thread_delay(100);
            app_update_file_transfer_done(blob_part_len);
        }


        parser_state.end_of_mif = true;
        cs_mutex_lock(flash_update_mutex);
        updater_module = 0;
        cs_mutex_unlock(flash_update_mutex);
    }

    cur_state->end_of_mif = parser_state.end_of_mif;
    cur_state->is_mif_file = parser_state.is_mif_file;
    cur_state->tlv_type_included = parser_state.tlv_type_included;
    cur_state->tlv_update_successed = parser_state.tlv_update_successed;

    return 0;
}
#endif

