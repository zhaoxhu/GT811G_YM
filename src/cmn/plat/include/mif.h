/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef __mif_h
#define __mif_h

#ifdef HAVE_MIF
#define NO_UPDATER 0
#define SC_UPDATER 1
#define WEB_UPDATER 2
#define ETH_UPDATER 3


/* TLV type */
#define TLV_ZBLOB_TYPE                0x00
#define TLV_JFFS2_TYPE                 0x01
#define TLV_START_CONFIG_TYPE    0x02
#define TLV_BINARY_TYPE               0x03

/* TLV decompress */
#define TLV_DECOMPRESS                0x00

/* mif_upgrade() function return error type */
#define  FIRST_FRAME_TOO_SHORT 1
#define  MIF_LEN_NO_MATCH 2
#define  CHIP_NOT_SUPPORT 3
#define  NAND_INIT_FAILED 4
#define  TLV_SEG_NOT_SUP 5
#define  TLV_TOO_BIG 6
#define  PARSER_IS_BUSY 7
#define  TLV_LEN_NOT_MATCH 8
#define  BOARD_NOT_SUPPORT 9  
#define OTHER_SYSTEM_ERR 10     
        

typedef struct {
        cyg_uint8          type;
        cyg_uint8          decompress;
        cyg_uint8          rsvd[2];
        cyg_uint32        destination;
        cyg_uint32        time;
        cyg_uint32        version;
        cyg_uint32        width;
        cyg_uint32        width_xor;
} __attribute__((packed)) multi_tlv_header_t;


#define EPON_L2FTP_MULTI_IMAGE_MAGIC 0x80ff7593

/* chip bit map  */
#define CHIP_BMP_LYNXP        1 << 0
#define CHIP_BMP_LYNXB        1 << 1
#define CHIP_BMP_LYNXC        1 << 2
#define CHIP_BMP_TIGER_II     1 << 16
#define CHIP_BMP_TIGER_III    1 << 17

/* board bit map */
#define ONU_1_PORT                   1 << 0
#define ONU_4_PORT_6045          1 << 1
#define ONU_4_PORT_6046          1 << 2
#define ONU_4_PORT_6097          1 << 3
#define OLT_4_ONU_PER_CHIP     1 << 16
#define OLT_2_ONU_PER_CHIP     1 << 17

typedef struct {
        cyg_uint32          mif_magic;
        cyg_uint8            version;
        cyg_uint8            rsvd[3];
        cyg_uint32          chip_bmp;
        cyg_uint32          board_bmp;
        cyg_uint32          mif_crc32;
        cyg_uint32          length;
        cyg_uint32          length_xor;
} __attribute__((packed)) epon_l2ftp_multi_image_header_t;

typedef struct mif_upgrade_state {
    bool is_mif_file;
    bool end_of_mif;   // false: not arrive the end of mif file. End of mif has reached.
    unsigned int tlv_type_included;  //bit map for all the tlv type that have encounted by now. bit 0 for ZBLOB type, bit 1 for JFFS2,
                                                //bit 2 for start configure, bit 3 for binary data or normal file to destination address.
    unsigned int tlv_update_successed;  //bit map for all the tlv segment ecounted that have updated successfully.
} mif_upgrade_state_t;

int prepare_mif_upgrade (char module,bool need_confirm_from_server);
int mif_upgrade (char module, char * input_buff, int length, 
       mif_upgrade_state_t * cur_state, bool end_of_file);
void do_erase_jffs2(unsigned int flash_offset,unsigned int block_num);
#endif
#endif /* __mif_h */

