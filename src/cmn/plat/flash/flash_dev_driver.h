/*****************************************************************************/
/* This file contains unpublished documentation and software                 */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,       */
/* in whole or in part, of the information in this file without a            */
/* written consent of an officer of Cortina Systems Incorporated is          */
/* strictly prohibited.                                                      */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                       */
/*****************************************************************************/
/*
 * flash_dev_driver.h
 *
 */

#ifndef _FLASH_DEV_DRIVER_H_
#define _FLASH_DEV_DRIVER_H_


#include "iros_config.h"

typedef enum
{
    FLASH_FAMILY_TYPE_NULL = 0,
    FLASH_FAMILY_TYPE_INTEL,
    FLASH_FAMILY_TYPE_SST,
    FLASH_FAMILY_TYPE_SPANSION,
    FLASH_FAMILY_TYPE_UNKOWN
} FLASH_FAMILY_TYPE_T;

/*==========================================================================*/
/*                Platform code must define the below                       */
/*==========================================================================*/
/* Description...
#define CYGNUM_FLASH_INTERLEAVE : Number of interleaved devices in parallel
#define CYGNUM_FLASH_SERIES     : Number of devices in series
#define CYGNUM_FLASH_WIDTH      : Width of devices on platform
#define CYGNUM_FLASH_BASE       : Address of first device */

#define CYGNUM_FLASH_INTERLEAVE     (1)
#define CYGNUM_FLASH_SERIES         (1)
#define CYGNUM_FLASH_WIDTH          (16)
#define CYGNUM_FLASH_BASE           (0x2f300000)
/*==========================================================================*/



/*==========================================================================*/
/*                ECOS specific header files                                */
/*==========================================================================*/
#include <pkgconf/hal.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/hal_cache.h>
#include CYGHWR_MEMORY_LAYOUT_H
#include <cyg/hal/hal_io.h>
#define  _FLASH_PRIVATE_

#ifdef HAVE_FLASH_FS
#include <pkgconf/io_flash.h>
#include <pkgconf/devs_flash_intel_28fxxx.h>
#include <cyg/io/flash.h>
#else
#include "flash.h"
#endif
/*==========================================================================*/



/*==========================================================================*/
/*                Flash operation code - Flash type specific                */
/*==========================================================================*/
//#define HAVE_FLASH_FAMILY_TYPE_INTEL
//#define HAVE_FLASH_FAMILY_TYPE_SST
//#define HAVE_FLASH_FAMILY_TYPE_SPANSION

// --------------  HAVE_FLASH_FAMILY_TYPE_INTEL
#define FLASH_INTEL_Read_ID                   FLASHWORD( 0x90 )
#define FLASH_INTEL_Reset                     FLASHWORD( 0xFF )
#define FLASH_INTEL_Program                   FLASHWORD( 0x40 )
#define FLASH_INTEL_Write_Buffer              FLASHWORD( 0xe8 )
#define FLASH_INTEL_Block_Erase               FLASHWORD( 0x20 )
#define FLASH_INTEL_Confirm                   FLASHWORD( 0xD0 )
#define FLASH_INTEL_Resume                    FLASHWORD( 0xD0 )
#define FLASH_INTEL_Set_Lock                  FLASHWORD( 0x60 )
#define FLASH_INTEL_Set_Lock_Confirm          FLASHWORD( 0x01 )
#define FLASH_INTEL_Clear_Lock                FLASHWORD( 0x60 )
#define FLASH_INTEL_Clear_Lock_Confirm        FLASHWORD( 0xd0 )
#define FLASH_INTEL_Read_Status               FLASHWORD( 0x70 )
#define FLASH_INTEL_Clear_Status              FLASHWORD( 0x50 )
#define FLASH_INTEL_Status_Ready              FLASHWORD( 0x80 )
// Status that we read back:
#define FLASH_INTEL_ErrorMask                 FLASHWORD( 0x7E )
#define FLASH_INTEL_ErrorProgram              FLASHWORD( 0x10 )
#define FLASH_INTEL_ErrorErase                FLASHWORD( 0x20 )
#define FLASH_INTEL_ErrorLock                 FLASHWORD( 0x30 )
#define FLASH_INTEL_ErrorLowVoltage           FLASHWORD( 0x08 )
#define FLASH_INTEL_ErrorLocked               FLASHWORD( 0x02 )
#define FLASH_INTEL_TIMEOUT                   1000000000

// ----------- HAVE_FLASH_FAMILY_TYPE_SST
//cmd
#define FLASH_SST_Clear_Status              FLASHWORD( 0xf0 )
#define FLASH_SST_Reset                     FLASHWORD( 0xf0 )
#define FLASH_SST_Busy                      FLASHWORD( 0x40 ) // "Toggle" bit

//addr
#define FLASH_SST_Setup_Addr1               (0x5555)
#define FLASH_SST_Setup_Addr2               (0x2AAA)
#define FLASH_SST_Setup_Code1               FLASHWORD( 0xAA )
#define FLASH_SST_Setup_Code2               FLASHWORD( 0x55 )
#define FLASH_SST_Setup_Erase               FLASHWORD( 0x80 )

// Common device details.
#define FLASH_SST_Read_ID                   FLASHWORD( 0x90 )
#define FLASH_SST_Read_ID_Exit              FLASHWORD( 0xF0 )
#define FLASH_SST_Program                   FLASHWORD( 0xA0 )
#define FLASH_SST_Block_Erase               FLASHWORD( 0x30 )

//status
#define FLASH_SST_CMD_DELAY                 1000
#define FLASH_SST_DELAY_SST_PROGRAM         1000
#define FLASH_SST_DELAY_SST_ERASE           1000000
#define SST_FLASH_SST_BUSY                  0x0040
#define FLASH_SST_TIMEOUT                   5000000

// -------- HAVE_FLASH_FAMILY_TYPE_SPANSION

#define FLASH_SPANSION_Clear_Status              FLASHWORD( 0xf0 )
#define FLASH_SPANSION_Reset                     FLASHWORD( 0xf0 )
#define FLASH_SPANSION_Busy                      FLASHWORD( 0x40 ) // "Toggle" bit
#define FLASH_SPANSION_Setup_Addr1               (0x555)
#define FLASH_SPANSION_Setup_Addr2               (0x2AA)
#define FLASH_SPANSION_Setup_Code1               FLASHWORD( 0xAA )
#define FLASH_SPANSION_Setup_Code2               FLASHWORD( 0x55 )
#define FLASH_SPANSION_Setup_Erase               FLASHWORD( 0x80 )

#define FLASH_SPANSION_Read_ID                   FLASHWORD( 0x90 )
#define FLASH_SPANSION_Program                   FLASHWORD( 0xA0 )
#define FLASH_SPANSION_CMD_DELAY                 1000
#define FLASH_SPANSION_TIMEOUT                   5000000

/*==========================================================================*/



/*==========================================================================*/
/*                                 Flash info                               */
/*==========================================================================*/
#define CYGNUM_FLASH_BLANK              (1)
#define CYGNUM_FLASH_DEVICES            (CYGNUM_FLASH_INTERLEAVE*CYGNUM_FLASH_SERIES)

#ifndef FLASH_P2V
# define FLASH_P2V( _a_ ) ((volatile flash_data_t *)((CYG_ADDRWORD)(_a_)))
#endif

// Now that device properties are defined, include magic for defining
// accessor type and constants.
#ifdef HAVE_FLASH_FS
#include <cyg/io/flash_dev.h>
#else
#include "flash_dev.h"
#endif

// Information about supported devices
typedef struct flash_dev_info {
    flash_data_t device_id;
    cyg_uint32   block_size;
    cyg_int32    block_count;
    cyg_uint32   base_mask;
    cyg_uint32   block_mask;
    cyg_uint32   device_size;
    cyg_bool     locking;               // supports locking
    cyg_bool     buffered_w;            // supports buffered writes
    cyg_bool     bootblock;
    cyg_uint32   bootblocks[12];         // 0 is bootblock offset, 1-11 sub-sector sizes (or 0)
    cyg_bool     banked;
    cyg_uint32   banks[2];               // bank offets, highest to lowest (lowest should be 0)
                                         // (only one entry for now, increase to support devices
                                         // with more banks).
} flash_dev_info_t;

#ifndef _IO_FLASH_H_
/* These are already defined in flash.h at cyg/io/flash.h this just a reference */
#define FLASH_ERR_OK              0x00  // No error - operation complete
#define FLASH_ERR_INVALID         0x01  // Invalid FLASH address
#define FLASH_ERR_ERASE           0x02  // Error trying to erase
#define FLASH_ERR_LOCK            0x03  // Error trying to lock/unlock
#define FLASH_ERR_PROGRAM         0x04  // Error trying to program
#define FLASH_ERR_PROTOCOL        0x05  // Generic error
#define FLASH_ERR_PROTECT         0x06  // Device/region is write-protected
#define FLASH_ERR_NOT_INIT        0x07  // FLASH info not yet initialized
#define FLASH_ERR_HWR             0x08  // Hardware (configuration?) problem
#define FLASH_ERR_ERASE_SUSPEND   0x09  // Device is in erase suspend mode
#define FLASH_ERR_PROGRAM_SUSPEND 0x0a  // Device is in in program suspend mode
#define FLASH_ERR_DRV_VERIFY      0x0b  // Driver failed to verify data
#define FLASH_ERR_DRV_TIMEOUT     0x0c  // Driver timed out waiting for device
#define FLASH_ERR_DRV_WRONG_PART  0x0d  // Driver does not support device
#define FLASH_ERR_LOW_VOLTAGE     0x0e  // Not enough juice to complete job
#endif
/*==========================================================================*/


#endif // _FLASH_DEV_DRIVER_H_
