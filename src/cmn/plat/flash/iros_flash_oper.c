/*****************************************************************************/
/* This file contains unpublished documentation and software                 */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,       */
/* in whole or in part, of the information in this file without a            */
/* written consent of an officer of Cortina Systems Incorporated is          */
/* strictly prohibited.                                                      */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                       */
/*****************************************************************************/
/*
 * iros_flash_oper.c
 */


#include "iros_config.h"
#include "cs_types.h"
#include "plat_common.h"
#include <pkgconf/hal.h>
#include <cyg/hal/hal_intr.h>           // interrupt disable/restore
#include "iros_flash_oper.h"
#include "image.h"
#include "iros_flash_partition.h"
#include "flash_dev_driver.h"

//#define FLASH_DEBUG

FLASH_FAMILY_TYPE_T flash_family_type = FLASH_FAMILY_TYPE_NULL;
flash_dev_t flash_dev;
iros_flash_partition_status_t partition_status;
cs_uint32  iros_flash_mutex=0;

//unsigned int max_blob_size = (1024 * 32 * 15);   /* 480 KB compressed */
//unsigned int flash_block_size = 0x8000; /* 32 KB */


/* flash_driver.c functions */
extern void oper_flash_read_id(void*);
extern int oper_flash_write(void* , cyg_uint16);
extern int oper_flash_erase_block(void*);
extern int oper_flash_force_erase_block(void* block);
extern int oper_flash_lock_block(void*);
extern int oper_flash_unlock_block(void*);
extern int oper_flash_bulk_write(void*, void*, int);
extern int oper_flash_force_write(void* _addr, void* _data, int len);



#if defined(IROSBOOT)
#define IROS_MALLOC_BOOT_POOL_SIZE   1280  /* bytes */
static char iros_malloc_boot_pool[IROS_MALLOC_BOOT_POOL_SIZE];
static int iros_malloc_boot_pool_next_free = 0;
char *iros_malloc_boot(int size) {
    char * return_ptr;
    if ((iros_malloc_boot_pool_next_free + size) >= IROS_MALLOC_BOOT_POOL_SIZE) {
        diag_printf("boot malloc pool too small\n");
        diag_printf("reboot\n");
        hal_delay_us(10000);
        HAL_PLATFORM_RESET();
    }
    return_ptr = &iros_malloc_boot_pool[iros_malloc_boot_pool_next_free];
    iros_malloc_boot_pool_next_free += size;
    return return_ptr;
}
#endif


#ifdef FLASH_DEBUG

void partition_status_dump(iros_flash_partition_status_t *p) {
    int i;
    if (p) {
        diag_printf("partition_status at %p\n", p);
        diag_printf("    work_part_id: %d\n", p->work_part_id);
        diag_printf(" work_part_index: %d\n", p->work_part_index);
        diag_printf("super_block_part_index: %d\n", p->super_block_part_index);
        for (i = 0; i < IROS_FLASH_PARTITION_TAB_MAX; i++) {
            if (flash_dev.info.super_block.part_tab[i].part_id) {
                diag_printf(" part index %d part_id %d part_status 0x%x part_loc_erase %p\n", i,
                            flash_dev.info.super_block.part_tab[i].part_id,
                            p->part_status[i],
                            p->part_loc_erase[i]);
            }
        }
        diag_printf(" part_status_any: 0x%x part_loc_erase_any %p\n",
                    p->part_status_any, p->part_loc_erase_any);
    }
}

void flash_info_dump(flash_info_t *p) {
    int i;
    if (p) {
        diag_printf("flash_info at %p\n", p);
        diag_printf("    flash_id:        0x%08x\n", p->flash_id);
        diag_printf("    super_block_loc: %p\n", p->super_block_loc);
        diag_printf("    super_block\n");
        diag_printf("    super_block.head_sig: 0x%08x\n", p->super_block.head_sig);
        diag_printf("    super_block.flash_id: 0x%08x\n", p->super_block.flash_id);
        diag_printf("    super_block.flash_size: 0x%08x\n", p->super_block.flash_size);
        diag_printf("    super_block.block_size: 0x%08x\n", p->super_block.block_size);
        for (i = 0; i < IROS_FLASH_PARTITION_TAB_MAX; i++) {
            if (p->super_block.part_tab[i].part_id) {
                diag_printf("    index %d  part_id %d part_type %d part_loc %p part_size 0x%08x\n",
                            i,
                            p->super_block.part_tab[i].part_id,
                            p->super_block.part_tab[i].part_type,
                            p->super_block.part_tab[i].part_loc,
                            p->super_block.part_tab[i].part_size);
            }
        }
        diag_printf("    super_block.tail_sig: 0x%08x\n\n", p->super_block.tail_sig);

    }
}

void dump_flash_dev(flash_dev_t *p) {
    diag_printf("flash_dev_t dump at %p\n", p);
    flash_info_dump(&p->info);
#if 0
    diag_printf("   erase 0x%08x\n", p->erase);
    diag_printf("   write 0x%08x\n", p->write);
    diag_printf("   lock 0x%08x\n", p->lock);
    diag_printf("   unlock 0x%08x\n", p->unlock);
#endif
    return;
}

#endif


cs_uint32 flash_part_id_to_part_index(cs_uint32 part_id) {
    cs_uint32 i;
    for (i = 0; i < IROS_FLASH_PARTITION_TAB_MAX; i++) {
        if (flash_dev.info.super_block.part_tab[i].part_id == part_id) {
            return i;
        }
    }
    return IROS_FLASH_PARTITION_INDEX_ANY;
}


void show_flash_partition_help(void) {
    int i;
    diag_printf("  f  p                      - show current partitions\n");

    diag_printf("  f  d id                   - delete partiton id\n");
    diag_printf("  f  c type start size [id] - create a new partion id\n");
    // show type table here : TBD
    for (i = 0; i < IROS_FLASH_PARTITION_TYPE_MAX; i++) {
        diag_printf("       type code: 0x%0x %s\n", i, part_type_to_str[i]);
    }
    diag_printf("  f  a                    - apply partition changes\n");

    return;
}

void flash_partition_table_show(void) {
    int i;

    iros_flash_super_block_t *p;

    p = &flash_dev.info.super_block;

    for (i = 0; i < IROS_FLASH_PARTITION_TAB_MAX; i++) {
        if (p->part_tab[i].part_id) {
            diag_printf("Partition Index 0x%x ID 0x%08x Type 0x%x Start 0x%08x Size 0x%08x %s\n",
                        i,
                        p->part_tab[i].part_id,
                        p->part_tab[i].part_type,
                        (unsigned int) p->part_tab[i].part_loc,
                        p->part_tab[i].part_size,
                        part_type_to_str_short[p->part_tab[i].part_type]);
        }
    }

    return;
}


void flash_partition_delete(cs_uint32 id) {
    cs_uint32 part_index;
    iros_flash_super_block_t *p;
    p = &flash_dev.info.super_block;
    part_index = flash_part_id_to_part_index(id);
    if ((p->part_tab[part_index].part_type == IROS_FLASH_PARTITION_TYPE_SUPER_BLOCK) ||
        (p->part_tab[part_index].part_type == IROS_FLASH_PARTITION_TYPE_BOOT)) {
        diag_printf("Err: not allowed\n");
        return;
    }
    if (part_index != IROS_FLASH_PARTITION_INDEX_ANY) {
        diag_printf("deleted partition index 0x%x id 0x%x\n",
                    part_index,
                    p->part_tab[part_index].part_id);
        p->part_tab[part_index].part_id = 0;
    }
}


void flash_partition_create(cs_uint32 type, char *start, cs_uint32 size, cs_uint32 id) {
    int i;
    iros_flash_super_block_t *p;
    p = &flash_dev.info.super_block;
    switch(type) {
        case IROS_FLASH_PARTITION_TYPE_SUPER_BLOCK:
        case IROS_FLASH_PARTITION_TYPE_BOOT:
        {
            diag_printf("Err: not allowed\n");
            break;
        }
        case IROS_FLASH_PARTITION_TYPE_BLOB_ECOS:
        case IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG:
        case IROS_FLASH_PARTITION_TYPE_JFFS2:
        case IROS_FLASH_PARTITION_TYPE_DATA:
        case IROS_FLASH_PARTITION_TYPE_XIP:
        {
            for (i = 0; i < IROS_FLASH_PARTITION_TAB_MAX; i++) {
                if (p->part_tab[i].part_id == 0) {
                    // found
                    if (id == 0) {
                        id = (cyg_uint32) (&p->part_tab[i]); // assign a unique id
                    }
                    p->part_tab[i].part_id = id;
                    p->part_tab[i].part_loc = start;
                    p->part_tab[i].part_type = type;
                    p->part_tab[i].part_size = size;
                    diag_printf("created partition index 0x%x id 0x%x\n", i, id);
                    break;
                }
            }
        }
        default:
            break;
    }
}

int
flash_memcmp(unsigned char *m1, unsigned char *m2, int len)
{
    int i;

    for (i=0;i<len;i++) {
        if (m1[i] != m2[i]) {
            return (i + 1);
        }
    }
    return 0;
}

void flash_partition_table_update(void) {
    // if modified, update, otherwise ignore
    if (flash_memcmp((char *)&flash_dev.info.super_block,
                     flash_dev.info.super_block_loc,
                     sizeof(iros_flash_super_block_t)) != 0)
    {
        cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_SUPER_BLOCK_ID);
        if (part_index != IROS_FLASH_PARTITION_INDEX_ANY) {
            int len;
            char *des = flash_dev.info.super_block_loc;
            if (flash_part_write_init(part_index, des)) { /* prepare to write flash */
                len = flash_write(des, (char *)&flash_dev.info.super_block, sizeof(iros_flash_super_block_t));
                flash_part_write_done(part_index);
                flash_write_verify(des, (char *)&flash_dev.info.super_block, sizeof(iros_flash_super_block_t));
                diag_printf("Applied the modified partition table\n");
            }
        }
    } else {
        diag_printf("Partition table not modified.\n");
    }
}


// flash write protection and partition control
// return 1 ok; 0 failed
// start_loc should be aligned to the block size
int flash_part_write_init(cs_uint32 part_index, char * start) {
    char *start_loc;
    int return_status = 0;
    cs_uint32 block_size = flash_dev.info.super_block.block_size;

    cs_mutex_lock(iros_flash_mutex);
    start_loc = (char *)(((unsigned int)start) & (~(block_size - 1))); // aligned to the block size;
    if (part_index == IROS_FLASH_PARTITION_INDEX_ANY) {
        partition_status.work_part_id = IROS_FLASH_PARTITION_ID_ANY;
        partition_status.work_part_index = IROS_FLASH_PARTITION_INDEX_ANY;
        partition_status.part_status_any = IROS_FLASH_PARTITION_WRITE;
        partition_status.part_loc_erase_any = start_loc;
        return_status = 1;
    } else {
        if (part_index < IROS_FLASH_PARTITION_TAB_MAX) {
            if (flash_dev.info.super_block.part_tab[part_index].part_id) {
                partition_status.work_part_id = flash_dev.info.super_block.part_tab[part_index].part_id;
                partition_status.work_part_index = part_index;
                partition_status.part_status[part_index] = IROS_FLASH_PARTITION_WRITE;
                partition_status.part_loc_erase[part_index] = start_loc;
                // check the loc is within the partition
                if ((start - start_loc) < flash_dev.info.super_block.part_tab[part_index].part_size) {
                    return_status = 1; // ok
                }
            }
        }
    }
    if (return_status == 0) {
        cs_mutex_unlock(iros_flash_mutex);
        diag_printf("Not ready to write flash at 0x%08x\n",(unsigned int) start_loc);
    }
    return return_status; // error
}

// flash partition write sequence done
int flash_part_write_done(cs_uint32 part_index) {
    int return_status = 0;
    if (part_index == IROS_FLASH_PARTITION_INDEX_ANY) {
        partition_status.work_part_id = 0;
        partition_status.work_part_index = 0;
        partition_status.part_status_any = 0;
        partition_status.part_loc_erase_any = 0;
        return_status = 1;
    } else {
        if (part_index < IROS_FLASH_PARTITION_TAB_MAX) {
            if (flash_dev.info.super_block.part_tab[part_index].part_id) {
                partition_status.work_part_id = 0;
                partition_status.work_part_index = 0;
                partition_status.part_status[part_index] = 0;
                partition_status.part_loc_erase[part_index] = 0;
                return_status = 1;
            }
        }
    }
    cs_mutex_unlock(iros_flash_mutex);
    
    return return_status;
}

bool flash_write_verify(char *faddr, char *p, int len) {
    int i;
    // verify char by char
    for (i = 0; i < len; i++) {
        if (faddr[i] != p[i]) {
            diag_printf("Verifying failed at 0x%x\n", i);
            return false;
        }
    }
    //diag_printf("Verifying passed\n");
    return true;
}


// no return;
void platform_reset(void) {
    diag_printf("reboot\n");
    hal_delay_us(10000);
    HAL_PLATFORM_RESET();
}

void upgrade_confirm(current_blob_desc_t *p_blob_desc) {
        blob_info_desc_t *blob_info;
        unsigned int short is_not_confirmed;
        unsigned int short is_not_executed;
        if (p_blob_desc == 0) {
                p_blob_desc = &current_blob_desc;
        }
        blob_info = blob_info_tab[p_blob_desc->blob_idx];
        is_not_confirmed = 0;
        is_not_executed = 0;
        flash_write((char *)&blob_info->is_not_confirmed, (char *)&is_not_confirmed, sizeof(unsigned int short));
        flash_write((char *)&blob_info->is_not_executed, (char *)&is_not_executed, sizeof(unsigned int short));
};


void flash_unlock_block_main(cs_uint32 faddr) {
    int return_status = 0;

    /* Unlock flash block */
    return_status = oper_flash_unlock_block((void*)faddr);

    if (return_status != 0) {
        diag_printf("Unlock failed! Bad flash\n");
    }

    return;
}

void flash_erase_block_main(cs_uint32 faddr) {
    int return_status = 0;

    /* Unlock flash block */
    return_status = oper_flash_unlock_block((void*)faddr);

    if ( return_status != 0 ) {
        diag_printf("Unlock failed. Bad flash!\n");
    }

    /* Erase the block */
    return_status = oper_flash_erase_block((void*)faddr);

    if ( return_status != 0 ) {
        diag_printf("Erase failed. Bad flash!\n");
    }

}

int flash_write_half_word_without_erase(cs_uint32 faddr, unsigned short int val) {

    int return_status = 0;
    return_status = oper_flash_write((void *)faddr, (cs_uint16)val);

    if (return_status != 0) {
        diag_printf("Write failed. Bad flash! addr 0x%08x 0x%04x\n", faddr, val);
        return 1;
    }
    return 0;
}

void *memcpy(void *dest, const void *src, size_t n);

void flash_writer_setup(void)
{
    int i;
    bool found;
    static bool flash_writer_setup_init = false;

    /*
     * flash_writer_setup is called at two places one in main.c
     * and other in flashiodev.c (for jffs2 supported branch).
     * Since the init flag will help keep track calling this once.
     */
    if (flash_writer_setup_init) return;
    flash_writer_setup_init = true;

    //cs_mutex_init(&iros_flash_mutex, "Flashmutex", 0);
//#ifndef HAVE_FLASH_FS
    int ret;
    ret = flash_hwr_init();
    if(ret != FLASH_ERR_OK)
    {
        diag_printf("flash_hwr_init failed: 0x%02x\n", ret);
    }
//#endif

    memset(&flash_dev, 0, sizeof(flash_dev_t));
    unsigned int id;

    /* Read flash id */
    oper_flash_read_id(&id);
    diag_printf("flash id: 0x%08x\n", id);

    // search the supported flash info table by flash id
    found = false;
    i = 0;
    while(flash_info_tb[i].flash_id) {
        if (flash_info_tb[i].flash_id == id) {
            // found;
            found = true;
            break;
        }
        i++;
    }
    if (!found) {
#ifdef HAVE_SERIAL_FLASH
        {  
            unsigned char Size_MB=0;
            unsigned char size=(id&0xff0000)>>16;
            i--;
            flash_info_tb[i].flash_id=id;
            flash_info_tb[i].super_block.flash_id=id;
            if((size<0x15)||(size>0x19))/*Suport flash from 2M to 32M */
                return ;
            flash_info_tb[i].super_block.flash_size=1<<size;
            Size_MB=flash_info_tb[i].super_block.flash_size/(0x100000);
            if(Size_MB==4){
                iros_flash_partition_t temp_part_tab[IROS_FLASH_PARTITION_TAB_MAX]=DEF_FLASH_MEMORY_LAYOUT_4MB ;
                memcpy(flash_info_tb[i].super_block.part_tab,temp_part_tab,IROS_FLASH_PARTITION_TAB_MAX*sizeof(iros_flash_partition_t));
            }else if(Size_MB==8){
                iros_flash_partition_t temp_part_tab[IROS_FLASH_PARTITION_TAB_MAX]=DEF_FLASH_MEMORY_LAYOUT_8MB;
                memcpy(flash_info_tb[i].super_block.part_tab,temp_part_tab,IROS_FLASH_PARTITION_TAB_MAX*sizeof(iros_flash_partition_t));
            }else if(Size_MB==16){
                iros_flash_partition_t temp_part_tab[IROS_FLASH_PARTITION_TAB_MAX]=DEF_FLASH_MEMORY_LAYOUT_16MB;
                memcpy(flash_info_tb[i].super_block.part_tab,temp_part_tab,IROS_FLASH_PARTITION_TAB_MAX*sizeof(iros_flash_partition_t));
            }else if(Size_MB==32){
                iros_flash_partition_t temp_part_tab[IROS_FLASH_PARTITION_TAB_MAX]=DEF_FLASH_MEMORY_LAYOUT_32MB;
                memcpy(flash_info_tb[i].super_block.part_tab,temp_part_tab,IROS_FLASH_PARTITION_TAB_MAX*sizeof(iros_flash_partition_t));
            }
            //flash_info_tb[i].super_block.part_tab[0].part_loc=(char *)FLASH_BASE;
            diag_printf("Use default flash Partition ID 0x%08x size %dMB \n", 
                flash_info_tb[i].flash_id,flash_info_tb[i].super_block.flash_size/(0x100000));          
        }
#else
        // other flash type detection - those not compatible with intel
        // such Spansion flash - TBD
        diag_printf("flash type not supported 0x%08x\n", id);
        return;
#endif
    }

#ifdef FLASH_DEBUG
    flash_info_dump(&flash_info_tb[i]);
#endif

    // prepare current flash_dev
    flash_dev.info.flash_id = id;
    flash_dev.info.super_block_loc = flash_info_tb[i].super_block_loc;

    iros_flash_super_block_t * super_block;
    super_block = (iros_flash_super_block_t *)flash_dev.info.super_block_loc;

    if ((super_block->head_sig == IROS_FLASH_SUPER_BLOCK_HEAD_SIG) &&
        (super_block->tail_sig == IROS_FLASH_SUPER_BLOCK_TAIL_SIG)) {
        // use the user defined super block
        flash_dev.info.super_block = *super_block;
    } else {
        // use the default super block
        flash_dev.info.super_block = flash_info_tb[i].super_block;
    }

    if (flash_dev.info.flash_id != flash_dev.info.super_block.flash_id) {
        diag_printf("real flash_id 0x%08x work flash_id 0x%08x\n",
                    flash_dev.info.flash_id,
                    flash_dev.info.super_block.flash_id);
        flash_dev.info.flash_id = flash_dev.info.super_block.flash_id;
    }


#ifdef FLASH_DEBUG
    dump_flash_dev(&flash_dev);
#endif

    // prepare blob_info_tab
    // the first blob partition is blob0; the second is blob1
    int blob_idx = 0;
    for (i = 0 ; i < IROS_FLASH_PARTITION_TAB_MAX; i++) {
        if (flash_dev.info.super_block.part_tab[i].part_id &&
            (flash_dev.info.super_block.part_tab[i].part_type == IROS_FLASH_PARTITION_TYPE_BLOB_ECOS)) {
            blob_part_tab[blob_idx] = i;
            blob_info_tab[blob_idx] = (blob_info_desc_t *)flash_dev.info.super_block.part_tab[i].part_loc;
            blob_idx++;
            if (blob_idx == 2) {
                break;
            }
        }
    }
    if (blob_idx != 2) {
        diag_printf("partition table config err\n");
        return;
    }

#ifdef FLASH_DEBUG
    diag_printf("blob 0 at 0x%08x\n",(unsigned int) blob_info_tab[0]);
    diag_printf("blob 1 at 0x%08x\n",(unsigned int) blob_info_tab[1]);
#endif

    // prepare current_blob_desc
    memcpy((char *)&current_blob_desc, (char *)CURRENT_BLOB_DESC_BASEADDR, sizeof(current_blob_desc_t));

#ifdef FLASH_DEBUG
    diag_printf("cur blob desc: %x %x %x %x\n", current_blob_desc.signature,
                                     current_blob_desc.update_timeout,
                                     current_blob_desc.blob_idx,
                                     current_blob_desc.num_retries);
#endif


    //initialize partiton status
    memset(&partition_status, 0, sizeof(iros_flash_partition_status_t));

    // set super block partition index
    for (i = 0 ; i < IROS_FLASH_PARTITION_TAB_MAX; i++) {
        if (flash_dev.info.super_block.part_tab[i].part_id &&
            (flash_dev.info.super_block.part_tab[i].part_type == IROS_FLASH_PARTITION_TYPE_SUPER_BLOCK)) {
            partition_status.super_block_part_index = i;
                break;
        }
    }

#ifdef FLASH_DEBUG
    partition_status_dump(&partition_status);
#endif

    IROS_DPRINTF("flash_writer_setup done\n");

    return;
}

int flash_write_check(char * faddr, char *p, int len, int check, int no_erase);

int flash_write_no_erase(char * faddr, char *p, int len) {
    return flash_write_check(faddr, p, len, 1, 1);
}

int flash_write_core_force(char * faddr, char *p, int len) 
{
    cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_COREDUMP_ID);
    if(flash_dev.info.super_block.part_tab[part_index].part_loc==faddr)
        oper_flash_force_erase_block((void *)faddr);
   return oper_flash_force_write((void *)faddr, (void *)p, len); 
       
}

int flash_write(char * faddr, char *p, int len) {
    return flash_write_check(faddr, p, len, 1, 0);
}

int flash_write_nocheck(char * faddr, char *p, int len) {
    return flash_write_check(faddr, p, len, 0, 0);
}

// when connecting to uart xyzmodem, you should not print any debugging lines inside this fun
int flash_write_check(char * faddr, char *p, int len, int check, int no_erase) {
#if 0
        int return_len;
#endif

#ifdef FLASH_DEBUG
        diag_printf("flash_write start ... to %p len 0x%08x %d\n", faddr, len, no_erase);
#endif

        if (partition_status.work_part_id == 0) {
            diag_printf("flash_write: partition is not specified\n");
            return 0;
        }

        char *new_next_erase_loc = 0;
        char *old_part_loc_erase = 0;

        iros_flash_partition_t *part = 0;
        cs_uint32 block_size = flash_dev.info.super_block.block_size;

        if (partition_status.work_part_id != IROS_FLASH_PARTITION_ID_ANY) {
            cs_uint32 index;
            index = partition_status.work_part_index;
            part = &flash_dev.info.super_block.part_tab[index];
            if ( (partition_status.part_status[index] != IROS_FLASH_PARTITION_WRITE) ||
                !((faddr >= part->part_loc) &&
                  ((faddr + len) <= (part->part_loc + part->part_size)))) {
                diag_printf("flash_write not ready or out of partition id %d range 0x%08x len 0x%08x\n",
                            partition_status.work_part_id, (unsigned int)faddr, len);
                return 0;
            }

            old_part_loc_erase = partition_status.part_loc_erase[index];
            if (old_part_loc_erase < (faddr + len)) {

                // round to next block
                new_next_erase_loc = (char *) ((((unsigned int)faddr + len) + block_size - 1) & (~(block_size - 1)));
                partition_status.part_loc_erase[index] = new_next_erase_loc;
            }

        } else {
            // IROS_FLASH_PARTITION_ID_ANY
            if ((partition_status.part_status_any != IROS_FLASH_PARTITION_WRITE) ||
                !((faddr >= (char *)FLASH_BASE) &&
                  ((faddr + len) <= (flash_dev.info.super_block.flash_size + (char *)FLASH_BASE)))) {
                diag_printf("flash_write not ready or out of range 0x%08x len 0x%08x\n",
                            (unsigned int) faddr, len);
                return 0;
            }
            old_part_loc_erase = partition_status.part_loc_erase_any;
            if (old_part_loc_erase < (faddr + len)) {
                // round to next block
                new_next_erase_loc = (char *)((((unsigned int)faddr + len) + block_size - 1) & (~(block_size - 1)));
                partition_status.part_loc_erase_any = new_next_erase_loc;
            }

        }
        // erase blocks as needed
        if (new_next_erase_loc) {
#ifdef FLASH_DEBUG
            diag_printf("erase blocks from %p to %p\n", old_part_loc_erase, new_next_erase_loc);
#endif
            char *p = old_part_loc_erase;
            for (p = old_part_loc_erase; p < new_next_erase_loc; p += block_size) {
                if (no_erase != 0) {
                    oper_flash_unlock_block((void *)p);
                } else {
                    oper_flash_erase_block((void *)p);
                    oper_flash_unlock_block((void *)p);
                }
            }
        }

        // len+1 make sure the odd number of bytes will be written.
        // odd number len must be the last flash_write to write a file
#if 0
        cyg_uint32 len_half_words = ((len+1) >> 1);
        int i;
#endif
        /* write to flash in bulk */
        oper_flash_bulk_write((void *)faddr, (void *)p, len);
#if 0
        return_len = (len_half_words << 1);
        for (i = 0; i < len_half_words; i++) {
            if (flash_write_half_word_without_erase( (cyg_uint32) faddr + (i<<1), ((unsigned short int *)p)[i]) != 0) {
                return_len = (i << 1);
                break;
            }
        }
#endif

        return len;
}


// ONU app update

#if defined(CYG_HAL_ARM_IMST_ONU)
#if !defined(IROSBOOT)
bool is_in_update = false;

// return 0 if error
// return 1 if ok
int app_update_from_sc_imp(char * buffer, int len, int timeout /*seconds*/) {
        IROS_DPRINTF("app_update_from_sc_imp 0x%08x 0x%x 0x%x\n", buffer, len, timeout);
        return 1;
}


bool is_file_transfer_in_progress = false;

extern unsigned char updateInProgress;
extern unsigned int active_update_port;
extern unsigned int active_mgmt_port;
// return 0 if error
// return 1 if ok
// this is for test only
int app_update_from_sc(int timeout) {
        IROS_DPRINTF("app_update_from_sc\n");
        int result;
        if (updateInProgress) {
                // reboot
                diag_printf("duplicated update ignored\n");
                result = 0;
        } else {
                extern int app_update_init(int timeout);
                extern unsigned char updateInProgress;
                updateInProgress = 1;
                result = app_update_init(timeout);
                /*is_file_transfer_in_progress = true;*/
                active_update_port = active_mgmt_port;
        }
        return result;
}


bool app_update_in_progress(void) {
    int cur_blob_idx = current_blob_desc.blob_idx;
    if (blob_info_tab[cur_blob_idx]->is_not_confirmed) {
        return true;
    }
    return false;
}


// return 0 if error
// return 1 if ok
int app_update_init(int timeout) {
        IROS_DPRINTF("app_update_init\n");

#if 0
        extern int prepare_mif_upgrade (char module,bool need_confirm_from_server);
        if (prepare_mif_upgrade (1, true) != 0)
            return 0;
#endif
        current_blob_desc.update_timeout = timeout;
        extern int app_update_file_transfer_init();
        app_update_file_transfer_init();
        return 1;
}




bool is_ethload(void) {
    int image_download_direction;
    HAL_READ_UINT32(IMST_IMAGE_DOWNLOAD, image_download_direction);
    image_download_direction &= IMAGE_DOWNLOAD_BITS;
    if (image_download_direction == IMAGE_FROM_ETH) {
        return true;
    }
    return false;

}

// return 0 if error
// return 1 if ok
int app_update_continue_after_reboot(void) {
        blob_info_desc_t *blob_info;
        //unsigned int short is_not_executed;

        IROS_DPRINTF("app_update_commit\n");



        // if it is in upgrade stage, turn off is_not_executed flag
        int cur_blob_idx = current_blob_desc.blob_idx;
        blob_info = blob_info_tab[cur_blob_idx];
        if (blob_info->is_not_confirmed) {
                // we are in update stage
                is_in_update = true;
        }

        return 1;
}


// return 0 if error
// return 1 if ok
int app_update_commit(void) {
        diag_printf("app_update_commit\n");


        blob_info_desc_t *blob_info;
        unsigned int short is_not_confirmed;
     unsigned int short  is_not_executed;

        IROS_DPRINTF("app_update_commit\n");

        is_in_update = false; // turn off update timeout timer
        current_blob_desc.update_timeout = 0;

        // if it is in upgrade stage, turn off is_not_confirmed flag
        int cur_blob_idx = current_blob_desc.blob_idx;
        blob_info = blob_info_tab[cur_blob_idx];

        diag_printf("blob_info 0x%08x\n",(unsigned int) blob_info);
        if (blob_info_tab[cur_blob_idx]->is_not_confirmed != 0) {
                // turn off the flag
            //diag_printf("is not confirmed\n");

                is_not_confirmed = 0;

                int part_index = blob_part_tab[cur_blob_idx];
                //int max_blob_size = flash_dev.info.super_block.part_tab[part_index].part_size;

                if (flash_part_write_init(part_index, (char*)blob_info)) { /* prepare to write flash without erase */
                    flash_write_no_erase((char *)&blob_info->is_not_confirmed, (char *)&is_not_confirmed, sizeof(unsigned int short));

                    //diag_printf("idx %x is_not_confirmed %x timeout 0x%x\n", cur_blob_idx, is_not_confirmed, current_blob_desc.update_timeout);
                    diag_printf("cur blob committed\n");


                    if (blob_info->is_not_executed) {
                        // turn off the flag
                        is_not_executed = 0;
                        flash_write_no_erase((char *)&blob_info->is_not_executed, (char *)&is_not_executed, sizeof(unsigned int short));
                        diag_printf("cur blob executed\n");
                    }

                    flash_part_write_done(part_index);

                    return 1;
                } else {
                    diag_printf("cur blob commit failed\n");
                    is_in_update = false;
                    return 0;
                }

        }
        return 0;
}

#endif
#endif

int  save_userdata_to_flash (char * src, unsigned int flash_offset,unsigned int data_lenth)
{
     char *FlashStartAddr;
     void *temp_ptr=0;
     int   UserdataMaxLen=0;

     cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_DATA_ID);
     if (part_index == IROS_FLASH_PARTITION_INDEX_ANY) {
         diag_printf("\nFlash user data partition not found\n");
         return 1;
     }
     else {
         FlashStartAddr = flash_dev.info.super_block.part_tab[part_index].part_loc;
         UserdataMaxLen = flash_dev.info.super_block.part_tab[part_index].part_size;
    }
    if( (flash_offset+data_lenth) > USER_DATA_USED_LEN ){
        diag_printf("data overflow;max size is %d\n",UserdataMaxLen);
         return 1;
    }

    temp_ptr=iros_malloc(IROS_MID_SYS, USER_DATA_USED_LEN );
     if(!temp_ptr){
        diag_printf("save_userdate_to_flash malloc failed\n");
         return 1;
     }

     memcpy(temp_ptr,FlashStartAddr,USER_DATA_USED_LEN);
     memcpy((temp_ptr+flash_offset), src, data_lenth);

     if (flash_part_write_init(part_index, FlashStartAddr)) { /* prepare to write flash */

         flash_write(FlashStartAddr, temp_ptr, USER_DATA_USED_LEN);
         flash_part_write_done(part_index);
     }
     iros_free(temp_ptr);
     return 0;

}

int  get_userdata_from_flash (char * dest, unsigned int flash_offset,unsigned int data_lenth)
{
     char *FlashStartAddr;
     int   UserdataMaxLen=0;
     cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_DATA_ID);
     if (part_index == IROS_FLASH_PARTITION_INDEX_ANY) {
       diag_printf("\nFlash user data partition not found\n");
       return 1;
     }
     else {
       FlashStartAddr = flash_dev.info.super_block.part_tab[part_index].part_loc;
       UserdataMaxLen = flash_dev.info.super_block.part_tab[part_index].part_size;
     }
     if( (flash_offset+data_lenth) > USER_DATA_USED_LEN ){
      diag_printf("data overflow;USER_DATA_USED_LEN 0x%x\n",USER_DATA_USED_LEN);
       return 1;
     }

     memcpy((char *)dest, (void *)(FlashStartAddr+flash_offset),data_lenth);
     return 0;

}



int iros_flash_write(cs_int8* flash_addr, cs_int8* buf, cs_int32 len)
{
    cs_int32 real_len;
    real_len = flash_write(flash_addr, buf, len);
    if(real_len != len)
        return CS_ERROR;

    return CS_OK;
}

void iros_flash_read(cs_int8* flash_addr, cs_int8* buf, cs_int32 len)
{
    memcpy(buf, flash_addr, len);
    return;
}


