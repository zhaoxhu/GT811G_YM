/*****************************************************************************/
/* This file contains unpublished documentation and software                 */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,       */
/* in whole or in part, of the information in this file without a            */
/* written consent of an officer of Cortina Systems Incorporated is          */
/* strictly prohibited.                                                      */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                       */
/*****************************************************************************/
/*
 * flash_driver.c
 *
 */
#include "iros_config.h"
#include "flash_dev_driver.h"
#ifdef HAVE_SERIAL_FLASH
#include "ssp.h"
#endif

/*===========================================================================*/
/*          Flash drivers from flash_dev_drivers.c reside in RAM             */
/*===========================================================================*/
#ifndef HAVE_SERIAL_FLASH
typedef void c_fun(void*);
externC c_fun flash_query;
externC c_fun flash_erase_block;
externC c_fun flash_program_buf;
externC c_fun flash_program_buf_word;
externC c_fun flash_read_buf;
externC c_fun flash_lock_block;
externC c_fun flash_unlock_block;
extern const flash_dev_info_t* flash_dev_info;
static void* __anonymizer(void* p)
{
  return p;
}
#endif /* HAVE_SERIAL_FLASH */
/*===========================================================================*/


/*****************************************************************************/
/* $rtn_hdr_start  FLASH READ ID                                             */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void oper_flash_read_id(void* data)
/*                                                                           */
/* INPUTS  : o data    Point to a variable to read the  ID                   */
/* OUTPUTS : o data    The ID is stored in data                              */
/* RETURNS : o ---                                                           */
/* DESCRIPTION:                                                              */
/*     Only reads the manufacturer and part number codes for the first       */
/* device(s) in series. It is assumed that any devices in series             */
/* will be of the same type.                                                 */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
#ifndef HAVE_SERIAL_FLASH
    typedef void c_fun(void*);
    c_fun *_flash_query;
#ifndef IROSBOOT
    unsigned long cur_interrupt_state;
    HAL_FLASH_CACHES_STATE(d_cache, i_cache);
#endif

    _flash_query = (c_fun*) __anonymizer(&flash_query);
#ifndef IROSBOOT
    HAL_DISABLE_INTERRUPTS(cur_interrupt_state);
    HAL_FLASH_CACHES_OFF(d_cache, i_cache);
#endif
    (*_flash_query)(data);
#ifndef IROSBOOT
    HAL_FLASH_CACHES_ON(d_cache, i_cache);
    HAL_RESTORE_INTERRUPTS(cur_interrupt_state);
#endif
#else
    flash_query(data);
#endif /* HAVE_SERIAL_FLASH */
    return;
}


/*****************************************************************************/
/* $rtn_hdr_start  FLASH ERASE BLOCK                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
int oper_flash_erase_block(void* block)
/*                                                                           */
/* INPUTS  : o block       Block address                                     */
/* OUTPUTS : ----                                                            */
/* RETURNS : o ---                                                           */
/* DESCRIPTION:                                                              */
/*     This function is used to erase a block                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    int stat = 0;
#ifndef IROSBOOT
    unsigned long cur_interrupt_state;
    HAL_FLASH_CACHES_STATE(d_cache, i_cache);
    HAL_DISABLE_INTERRUPTS(cur_interrupt_state);
    HAL_FLASH_CACHES_OFF(d_cache, i_cache);
#endif
#ifdef HAVE_SERIAL_FLASH
    flash_erase_block((cs_uint32)block,0);
#else
    typedef int c_fun(unsigned short *, unsigned int);
    c_fun *_flash_erase_block;

    _flash_erase_block = (c_fun*) __anonymizer(&flash_erase_block);
    stat = (*_flash_erase_block)(block, flash_dev_info->block_size);
#endif /* HAVE_SERIAL_FLASH */
#ifndef IROSBOOT
    HAL_FLASH_CACHES_ON(d_cache, i_cache);
    HAL_RESTORE_INTERRUPTS(cur_interrupt_state);
#endif
    return stat;
}

/*****************************************************************************/
/* $rtn_hdr_start  FLASH ERASE BLOCK                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
int oper_flash_force_erase_block(void* block)
/*                                                                           */
/* INPUTS  : o block       Block address                                     */
/* OUTPUTS : ----                                                            */
/* RETURNS : o ---                                                           */
/* DESCRIPTION:                                                              */
/*     This function is used to erase a block                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    int stat = 0;
#ifndef IROSBOOT
    unsigned long cur_interrupt_state;
    HAL_FLASH_CACHES_STATE(d_cache, i_cache);
    HAL_DISABLE_INTERRUPTS(cur_interrupt_state);
    HAL_FLASH_CACHES_OFF(d_cache, i_cache);
#endif
#ifdef HAVE_SERIAL_FLASH
    flash_force_erase_block((cs_uint32)block,0);
#else
    typedef int c_fun(unsigned short *, unsigned int);
    c_fun *_flash_erase_block;

    _flash_erase_block = (c_fun*) __anonymizer(&flash_erase_block);

    stat = (*_flash_erase_block)(block, flash_dev_info->block_size);
#endif /* HAVE_SERIAL_FLASH */
#ifndef IROSBOOT
    HAL_FLASH_CACHES_ON(d_cache, i_cache);
    HAL_RESTORE_INTERRUPTS(cur_interrupt_state);
#endif
    return stat;
}
/*****************************************************************************/
/* $rtn_hdr_start  FLASH WRITE                                               */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
int oper_flash_write(void* addr, int data)
/*                                                                           */
/* INPUTS  : o addr         Address to which data to be written              */
/*           o data         Data that need to be written                     */
/* OUTPUTS : ----                                                            */
/* RETURNS : o Status of the write operation                                 */
/* DESCRIPTION:                                                              */
/*     This function writes the data to the flash                            */
/*     THIS FUNCTION CONSIDERES THE BLOCK IN UNLOCKED                        */
/*     WRITE ONLY 2 BYTE                                                     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    int stat = 0;
#ifndef IROSBOOT
    unsigned long cur_interrupt_state;
    HAL_FLASH_CACHES_STATE(d_cache, i_cache);
    HAL_DISABLE_INTERRUPTS(cur_interrupt_state);
    HAL_FLASH_CACHES_OFF(d_cache, i_cache);
#endif
#ifdef HAVE_SERIAL_FLASH
    flash_program_buf_word(addr,(cs_uint32)data);
#else
    typedef int c_fun(void *, int);
    c_fun *_flash_program_buf_word;
    _flash_program_buf_word = (c_fun*) __anonymizer(&flash_program_buf_word);
    stat = (*_flash_program_buf_word)(addr, data);
#endif /* HAVE_SERIAL_FLASH */
#ifndef IROSBOOT
    HAL_FLASH_CACHES_ON(d_cache, i_cache);
    HAL_RESTORE_INTERRUPTS(cur_interrupt_state);
#endif
    return stat;
}

/*****************************************************************************/
/* $rtn_hdr_start  FLASH BULK WRITE                                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
int oper_flash_force_write(void* _addr, void* _data, int len)
/*                                                                           */
/* INPUTS  : o addr         Address to which data to be written              */
/*           o data         Address of Data that need to be written          */
/*           o len          Length of data hat need to be written            */
/* OUTPUTS : ----                                                            */
/* RETURNS : o Status of the write operation                                 */
/* DESCRIPTION:                                                              */
/*     This function writes the bulk data to the flash                       */
/*     THIS FUNCTION UNLOCKS THE BLOCK FIRST BEFORE IT WRITES                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    int stat = 0;
#ifndef IROSBOOT
    unsigned long cur_interrupt_state;
    HAL_FLASH_CACHES_STATE(d_cache, i_cache);
    HAL_DISABLE_INTERRUPTS(cur_interrupt_state);
    HAL_FLASH_CACHES_OFF(d_cache, i_cache);
#endif
#ifdef HAVE_SERIAL_FLASH
    flash_program_buf_force((cs_uint32)_addr,(cs_uint32)_data,len,0,0);
#else
    int size;
    typedef int c_fun(void *, void *, int, unsigned long, int);
    c_fun *_flash_program_buf;
    unsigned char *addr = (unsigned char *)_addr;
    unsigned char *data = (unsigned char *)_data;
    unsigned long tmp;

    _flash_program_buf = (c_fun*) __anonymizer(&flash_program_buf);

    while (len > 0) {
        size = len;
        if (size > flash_dev_info->block_size) size = flash_dev_info->block_size;

        tmp = (unsigned long) addr & (~flash_dev_info->block_mask);
        if (tmp) {
                tmp = flash_dev_info->block_size - tmp;
                if (size > tmp) size = tmp;
        }

        stat = (*_flash_program_buf)(addr, data, size, flash_dev_info->block_mask, 0x0);
        if (stat != FLASH_ERR_OK) {
            break;
        }
        len -= size;
        addr += size/sizeof(*addr);
        data += size/sizeof(*data);
    }
#endif /* HAVE_SERIAL_FLASH */
#ifndef IROSBOOT
    HAL_FLASH_CACHES_ON(d_cache, i_cache);
    HAL_RESTORE_INTERRUPTS(cur_interrupt_state);
#endif
    return stat;
}
/*****************************************************************************/
/* $rtn_hdr_start  FLASH BULK WRITE                                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
int oper_flash_bulk_write(void* _addr, void* _data, int len)
/*                                                                           */
/* INPUTS  : o addr         Address to which data to be written              */
/*           o data         Address of Data that need to be written          */
/*           o len          Length of data hat need to be written            */
/* OUTPUTS : ----                                                            */
/* RETURNS : o Status of the write operation                                 */
/* DESCRIPTION:                                                              */
/*     This function writes the bulk data to the flash                       */
/*     THIS FUNCTION UNLOCKS THE BLOCK FIRST BEFORE IT WRITES                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    int stat = 0;
#ifndef IROSBOOT
    unsigned long cur_interrupt_state;
    HAL_FLASH_CACHES_STATE(d_cache, i_cache);
    HAL_DISABLE_INTERRUPTS(cur_interrupt_state);
    HAL_FLASH_CACHES_OFF(d_cache, i_cache);
#endif
#ifdef HAVE_SERIAL_FLASH
    flash_program_buf((cs_uint32)_addr,(cs_uint32)_data,len,0,0);
#else
    int size;
    typedef int c_fun(void *, void *, int, unsigned long, int);
    c_fun *_flash_program_buf;
    unsigned char *addr = (unsigned char *)_addr;
    unsigned char *data = (unsigned char *)_data;
    unsigned long tmp;


    _flash_program_buf = (c_fun*) __anonymizer(&flash_program_buf);

    while (len > 0) {
        size = len;
        if (size > flash_dev_info->block_size) size = flash_dev_info->block_size;

        tmp = (unsigned long) addr & (~flash_dev_info->block_mask);
        if (tmp) {
                tmp = flash_dev_info->block_size - tmp;
                if (size > tmp) size = tmp;
        }

        stat = (*_flash_program_buf)(addr, data, size, flash_dev_info->block_mask, 0x0);
        if (stat != FLASH_ERR_OK) {
            break;
        }
        len -= size;
        addr += size/sizeof(*addr);
        data += size/sizeof(*data);
    }

#endif /* HAVE_SERIAL_FLASH */
#ifndef IROSBOOT
    HAL_FLASH_CACHES_ON(d_cache, i_cache);
    HAL_RESTORE_INTERRUPTS(cur_interrupt_state);
#endif
    return (stat);
}


/*****************************************************************************/
/* $rtn_hdr_start  FLASH READ                                                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
unsigned char oper_flash_read(unsigned char* addr)
/*                                                                           */
/* INPUTS  : o addr         Address from which data to be read               */
/* OUTPUTS : ----                                                            */
/* RETURNS : o Value                                                         */
/* DESCRIPTION:                                                              */
/*     This function reads the data from the flash                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    return *addr;
}


/*****************************************************************************/
/* $rtn_hdr_start  FLASH BULK READ                                           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void oper_flash_bulk_read(unsigned char* addr, unsigned char* data, int len)
/*                                                                           */
/* INPUTS  : o addr         Address from which data to be read               */
/*           o data         Address to which read data are stored            */
/*           o len          Length of data to be read                        */
/* OUTPUTS : ----                                                            */
/* RETURNS : o Value                                                         */
/* DESCRIPTION:                                                              */
/*     This function reads bulk data from the flash                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
#ifdef HAVE_SERIAL_FLASH
    sflash_block_read((cs_uint32)data,(cs_uint32)addr,len);
#else
    memcpy(data, addr, len);
#endif /* HAVE_SERIAL_FLASH */

    return;
}


/*****************************************************************************/
/* $rtn_hdr_start  FLASH LOCK BLOCK                                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
int oper_flash_lock_block(void* block)
/*                                                                           */
/* INPUTS  : o block    Block address                                        */
/* OUTPUTS : ----                                                            */
/* RETURNS : o Lock operation status                                         */
/* DESCRIPTION:                                                              */
/*     This function will lock the block.                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    int stat = 0;
#ifndef HAVE_SERIAL_FLASH
    typedef int c_fun(unsigned short *);
    c_fun *_flash_lock_block;
#ifndef IROSBOOT
    unsigned long cur_interrupt_state;
    HAL_FLASH_CACHES_STATE(d_cache, i_cache);
#endif

    _flash_lock_block = (c_fun*) __anonymizer(&flash_lock_block);
#ifndef IROSBOOT
    HAL_DISABLE_INTERRUPTS(cur_interrupt_state);
    HAL_FLASH_CACHES_OFF(d_cache, i_cache);
#endif
    stat = (*_flash_lock_block)(block);
#ifndef IROSBOOT
    HAL_FLASH_CACHES_ON(d_cache, i_cache);
    HAL_RESTORE_INTERRUPTS(cur_interrupt_state);
#endif
#endif /* HAVE_SERIAL_FLASH */

    return stat;
}


/*****************************************************************************/
/* $rtn_hdr_start  FLASH UNLOCK BLOCK                                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
int oper_flash_unlock_block(void* block)
/*                                                                           */
/* INPUTS  : o block       Block start address                               */
/* OUTPUTS : ----                                                            */
/* RETURNS : o Status of unlock operation                                    */
/* DESCRIPTION:                                                              */
/*     This function will unlock the blocks.                                 */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    int stat = 0;
#ifndef HAVE_SERIAL_FLASH
    typedef int c_fun(unsigned short *, int, int);
    c_fun *_flash_unlock_block;
#ifndef IROSBOOT
    unsigned long cur_interrupt_state;
    HAL_FLASH_CACHES_STATE(d_cache, i_cache);
#endif

    _flash_unlock_block = (c_fun*) __anonymizer(&flash_unlock_block);
#ifndef IROSBOOT
    HAL_DISABLE_INTERRUPTS(cur_interrupt_state);
    HAL_FLASH_CACHES_OFF(d_cache, i_cache);
#endif
    stat = (*_flash_unlock_block)(block, flash_dev_info->block_size, 0x1);
#ifndef IROSBOOT
    HAL_FLASH_CACHES_ON(d_cache, i_cache);
    HAL_RESTORE_INTERRUPTS(cur_interrupt_state);
#endif
#endif /* HAVE_SERIAL_FLASH */

    return stat;
}

