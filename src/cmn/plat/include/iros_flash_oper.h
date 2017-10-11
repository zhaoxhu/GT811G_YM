/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef _iros_flash_oper_h_
#define _iros_flash_oper_h_


// flash type supported
#define FLASH_TYPE_JS28F160C3_ID       0x88c30089                /* Intel 2MB bottom boot device*/
#define FLASH_TYPE_TE28F160C2_ID       0x88c20089                /* Intel 2MB, top boot device*/
#define FLASH_TYPE_JS28F320C5_ID       0x88c50089                /* Intel 4MB bottom boot device */
#define FLASH_TYPE_JS28F320C4_ID       0x88c40089                /* Intel 4MB top boot device */
#define FLASH_TYPE_JS28F160CC_ID       0x88cc0089                /* Intel 8MB, top boot device */
#define FLASH_TYPE_JS28F160CD_ID       0x88cd0089                /* Intel 8MB, bottom boot device */
#define FLASH_TYPE_JS28F256P3_B_ID     0x89220089                /* Intel 32MB, bottom boot device */
#define FLASH_TYPE_M58LT128KTB_REAL_ID 0x88c40020                /* Intel16MB, top boot device */
#define FLASH_TYPE_M58LT128KTB_ID      0x90aa0020                /* Intel16MB, top boot device */
#define FLASH_TYPE_M58LT128KTB_DEV_ID      0x90aa                /*Device code for FLASH_TYPE_M58LT128KTB_ID*/


#define FLASH_TYPE_EMB_J3_4MB_ID       0x00160089                /* Intel embedded flash memory j3 4MBytes */
#define FLASH_TYPE_EMB_J3_8MB_ID       0x00170089                /* Intel embedded flash memory j3 8MBytes */
#define FLASH_TYPE_EMB_J3_16MB_ID      0x00180089                /* Intel embedded flash memory j3 16MBytes */
#define FLASH_TYPE_EMB_J3_32MB_ID      0x001d0089                /* Intel embedded flash memory j3 32MBytes */

#define FLASH_TYPE_LH28F640BFHE_ID     0x00b200b0                /* Sharp 8MB, bottom boot device */

#define FLASH_TYPE_SST39VF1601_B_ID    0x234b00bf                /* SST1601 Bottom Boot 2MB */
#define FLASH_TYPE_SST39VF3201_B_ID    0x235b00bf                /* SST3201 Bottom Boot 4MB */
#define FLASH_TYPE_SST39VF3201B_B_ID   0x235d00bf                /* SST3201B Bottom Boot 4MB */
#define FLASH_TYPE_SST39VF6401B_B_ID   0x236d00bf                /* SST6401B Bottom Boot 8MB */

// S29GL series, cover S29GL032(M,N), S29GL064(M,N)
#define FLASH_TYPE_S29GL032_B_ID       0x02160001                /* S29GL032 4MB, bottom boot */
#define FLASH_TYPE_S29GL032_T_ID       0x03160001                /* S29GL032 4MB, top boot */
#define FLASH_TYPE_S29GL064_B_ID       0x02170001                /* S29GL064 8MB, bottome boot */
#define FLASH_TYPE_S29GL064_T_ID       0x03170001                /* S29GL064 8MB, top boot */
// S29AL series, cover S29AL016(M,J,D), S29AL032(D)
#define FLASH_TYPE_S29AL016_B_ID       0x22490001                /* S29AL016 2MB, bottom boot */
#define FLASH_TYPE_S29AL016_T_ID       0x22c40001                /* S29AL016 2MB, top boot */
#define FLASH_TYPE_S29AL032_B_ID       0x22f90001                /* S29AL032 4MB, bottom boot */
#define FLASH_TYPE_S29AL032_T_ID       0x22f60001                /* S29AL032 4MB, top boot */

#define FLASH_TYPE_M29W160E_B_ID       0x22490020                /* NUMONYX M29W160E 2MB, bottom boot */
#define FLASH_TYPE_M29W160E_T_ID       0x22c40020                /* NUMONYX M29W160E 2MB, top boot */
#define FLASH_TYPE_M29W640G_B_ID       0x02170020                /* NUMONYX M29W640G 8MB, bottom boot */
#define FLASH_TYPE_M29W640G_T_ID       0x03170020                /* NUMONYX M29W640G 8MB, top boot */


#define FLASH_TYPE_MX29LV640D_B_ID     0x22cb00c2                /* MX29LV640D 8MB, bottom boot */
#define FLASH_TYPE_MX29LV320D_T_ID     0x22a700c2                /* MX29LV320D 4MB, top boot */
#define FLASH_TYPE_MX29LV320D_B_ID     0x22a800c2                /* MX29LV320D 4MB, bottom boot */

#define FLASH_TYPE_W19B320B_B_ID       0x222a00da                /* Winbond W19B320B 4MB, bottom boot */

#define FLASH_TYPE_DEFUALT_ID               0x12345678
#ifdef HAVE_SERIAL_FLASH
#define FLASH_TYPE_S25FL128P01_U_ID    0x20180001               /* SPANSION S25FL128P   16MB, uniform with 64/256KB block */
#define FLASH_TYPE_MX25L25635E_U_ID    0x201900c2               /* MACRONIX MX25L25635E 32MB, uniform with 4KB sector */
#define FLASH_TYPE_MX25L12845E_U_ID    0x201800c2               /* MACRONIX MX25L12845E 16MB, uniform with 4KB sector */
#define FLASH_TYPE_MX25L6445E_U_ID     0x201700c2               /* MACRONIX MX25L6445E   8MB, uniform with 4KB sector */
#define FLASH_TYPE_MX25L3206E_U_ID     0x201600c2               /* MACRONIX MX25L3206E   4MB, uniform with 4KB sector */
#define FLASH_TYPE_MX25L3235D_U_ID     0x5e1600c2               /* MACRONIX MX25L3235D   4MB, uniform with 4KB sector */
#define FLASH_TYPE_MX25L1606E_U_ID     0x201500c2               /* MACRONIX MX25L1606E   2MB, uniform with 4KB sector */
#define FLASH_TYPE_N25Q128A130_B_ID    0xba180020               /* NUMONYX  N25Q128A    16MB, bottom boot */
#define FLASH_TYPE_GD25Q32_U_ID        0x401600C8               /* GIGADEV  GD25Q32      4MB, uniform with 4KB sector*/
#define FLASH_TYPE_M25P32V6G_U_ID      0x20160020               /* ST  M25P32V6G        32MB, uniform with 64KB sector*/
#define FLASH_TYPE_M25PX64_U_ID        0x71170020               /* NUMONYX  M25PX64      8MB, uniform with 64KB sector*/
#define FLASH_TYPE_SST25VF032B_U_ID    0x254A00BF               /* SST  SST25VF032B      4MB, uniform with 64KB sector*/
#define FLASH_TYPE_W25Q32_U_ID         0x401600EF               /* Winbond  W25Q32BV     4MB, uniform with 64KB sector*/
#endif

#define IROS_FLASH_BLOCK_SIZE_64KB       0x10000
#define IROS_FLASH_BLOCK_SIZE_128KB      0x20000
#define IROS_FLASH_BLOCK_SIZE_256KB      0x40000


#define IROS_FLASH_PARTITION_TAB_MAX      12

#define IROS_FLASH_PARTITION_TYPE_UNDEF           0
#define IROS_FLASH_PARTITION_TYPE_BOOT            1
#define IROS_FLASH_PARTITION_TYPE_BLOB_ECOS       2   /* blob or zblob */
#define IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG  3
#define IROS_FLASH_PARTITION_TYPE_JFFS2           4
#define IROS_FLASH_PARTITION_TYPE_DATA            5
#define IROS_FLASH_PARTITION_TYPE_COREDUMP        6
#define IROS_FLASH_PARTITION_TYPE_XIP             7    /* eXecution In Place area */
#define IROS_FLASH_PARTITION_TYPE_SUPER_BLOCK     8
#define IROS_FLASH_PARTITION_TYPE_LOG             9
#define IROS_FLASH_PARTITION_TYPE_MAX             10

#define IROS_FLASH_PARTITION_INDEX_ANY            0xffffffff
#define IROS_FLASH_PARTITION_ID_ANY               0xffffffff

#define IROS_FLASH_PARTITION_WRITE                1

#define IROS_FLASH_SUPER_BLOCK_HEAD_SIG           0x55551111
#define IROS_FLASH_SUPER_BLOCK_TAIL_SIG           0x55552222

#define IROS_FLASH_PARTITION_BOOT_ID              0x77770001
#define IROS_FLASH_PARTITION_XIP_ID               0x77770002
#define IROS_FLASH_PARTITION_SUPER_BLOCK_ID       0x77770003
#define IROS_FLASH_PARTITION_BLOB0_ID             0x77770004
#define IROS_FLASH_PARTITION_BLOB1_ID             0x77770005
#define IROS_FLASH_PARTITION_STARTUP_CONFIG_ID    0x77770006
#define IROS_FLASH_PARTITION_JFFS2_ID             0x77770007
#define IROS_FLASH_PARTITION_DATA_ID              0x77770008
#define IROS_FLASH_PARTITION_COREDUMP_ID          0x77770009
#define IROS_FLASH_PARTITION_STARTUP_CONFIG1_ID   0x7777000A
#define IROS_FLASH_PARTITION_LOG_ID               0x7777000B

/* User data partition size is 64KB.Reserved the first 32K for common use 
   So set DB management offset at the beginning of the second 32K */
#define DB_DATA_OFFSET          0x8000

#define USER_DATA_USED_LEN      0x10000

#if 1
//added by zhuxh
#define SLOW_PATH_CFG_OFFSET            (1024*19+512)
#define GWD_PRODUCT_CFG_OFFSET          (SLOW_PATH_CFG_OFFSET+64)
#define GWD_TLV_OFFSET					(0)
#define GWD_TLV_LEN						(0x4000)
#ifdef HAVE_TERMINAL_SERVER
#define GWD_PRODUCT_CFG_OFFSET_W  		(1024*20)
#endif
#endif
/* User DATA used space*/
/* start   ~~ start+VENDOR_AUTHKEY_LEN_MAX  //Authkey
   (start+VENDOR_AUTHKEY_LEN_MAX) ~~ (start+VENDOR_AUTHKEY_LEN_MAX+OAM_CONFIG_LEN_MAX) //OAM config
    (start+VENDOR_AUTHKEY_LEN_MAX+OAM_CONFIG_LEN_MAX) ~~ (start+VENDOR_AUTHKEY_LEN_MAX+OAM_CONFIG_LEN_MAX) //GPIO config
    (start+VENDOR_AUTHKEY_LEN_MAX+OAM_CONFIG_LEN_MAX + GPIO_CONFIG_LEN_MAX) ~~ (start+VENDOR_AUTHKEY_LEN_MAX+OAM_CONFIG_LEN_MAX +GPIO_CONFIG_LEN_MAX +LIOD_PASSWD_LEN_MAX) //LOID+PASSWD config
*/


typedef struct iros_flash_partition_status_t {
    cs_uint32 work_part_id;                                 /* work partition id. 0 if not assigned. 0xffffffff for any */
    cs_uint32 work_part_index;                              /* work partition index */
    cs_uint32 super_block_part_index;                       /* super block partition index */
    cs_uint32 part_status[IROS_FLASH_PARTITION_TAB_MAX];    /* 0 - init; 1 - setup for write */
    char * part_loc_erase[IROS_FLASH_PARTITION_TAB_MAX];     /* the location to erase a block if needed */
    cs_uint32 part_status_any;
    char * part_loc_erase_any;
} iros_flash_partition_status_t;

typedef struct iros_flash_partition_t {
    cs_uint32 part_id;
    cs_uint32 part_type;
    char * part_loc;
    cs_uint32 part_size;
} iros_flash_partition_t;

typedef struct iros_flash_super_block_t {
    cs_uint32 head_sig;
    cs_uint32 flash_id;            /* flash id */
    cs_uint32 flash_size;          /* flash size */
    cs_uint32 block_size;    /* flash block size */
    iros_flash_partition_t part_tab[IROS_FLASH_PARTITION_TAB_MAX];
    cs_uint32 tail_sig;
} iros_flash_super_block_t;

typedef struct flash_info_t {
    cs_uint32                 flash_id;        /* flash id */
    char *                     super_block_loc; /* super block location for partition table */
    iros_flash_super_block_t   super_block;     /* default or user-defined super block */
} flash_info_t;

typedef struct flash_dev_t {
    flash_info_t  info;
} flash_dev_t;

#if !defined(IROSBOOT)

#define FLASH_TYPE_PARALLEL              1

extern bool flash_detect(void);
//extern void flash_init(void);
extern int flash_write_block(char *f_addr, char *p, int len);

extern void flash_image_info_show(void);
extern void flash_writer_setup(void);

#endif /* IROSBOOT */

extern int flash_write(char * faddr, char *p, int len);
extern int flash_write_core_force(char * faddr, char *p, int len);
extern int flash_write_no_erase(char * faddr, char *p, int len);
extern void flash_writer_setup(void);

#if defined(CYG_HAL_ARM_IMST_ONU)
#if !defined(IROSBOOT)
extern int app_update_from_sc(int timeout);
extern int app_update_commit(void);
extern int app_update_continue_after_reboot(void);
extern bool is_ethload(void);
int save_userdata_to_flash(char * src, unsigned int flash_offset, unsigned int data_lenth);
int get_userdata_from_flash(char * dest, unsigned int flash_offset, unsigned int data_lenth);

#endif
#endif

#define IROS_BOOT_PARAMS_HEAD_SIG  0xabcd1111
#define IROS_BOOT_PARAMS_TAIL_SIG  0xabcd2222
#define IROS_BOOT_PARAMS_BLOCK     0x2f010000


extern unsigned int blob_part_tab[2];
extern flash_dev_t flash_dev;
extern int flash_part_write_init(cs_uint32 part_index, char * start_loc);
extern int flash_part_write_done(cs_uint32 part_index);
extern cs_uint32 flash_part_id_to_part_index(cs_uint32 part_id);
extern bool flash_write_verify(char *faddr, char *p, int len);
extern void flash_erase_block_main(cs_uint32 faddr);
extern int flash_hwr_init(void);
extern int flash_memcmp(unsigned char *m1, unsigned char *m2, int len);

extern void show_flash_partition_help(void);
extern void flash_partition_table_show(void);
extern void flash_partition_delete(cs_uint32 id);
extern void flash_partition_create(cs_uint32 type, char *start, cs_uint32 size, cs_uint32 id);
extern void flash_partition_table_update(void);

extern int iros_flash_write(cs_int8* flash_addr, cs_int8* buf, cs_int32 len);
extern void iros_flash_read(cs_int8* flash_addr, cs_int8* buf, cs_int32 len);

#endif /* _iros_flash_oper_h_ */

