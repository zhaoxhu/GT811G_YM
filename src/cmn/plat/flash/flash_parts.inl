/*****************************************************************************/
/* This file contains unpublished documentation and software                 */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,       */
/* in whole or in part, of the information in this file without a            */
/* written consent of an officer of Cortina Systems Incorporated is          */
/* strictly prohibited.                                                      */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                       */
/*****************************************************************************/
/*
 * flash_parts.inl
 *
 */

/*****************************************************************************/
/*                         SAMPLE TEMPLATE                                   */
/*****************************************************************************/
/*
    {   // LH28F640BFHE-B Sharp
        device_id  : FLASHWORD(0x00B2),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 128,
        device_size: 0x800000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x800000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : true,
        buffered_w : false,
        bootblock  : true,
        // Starting offset address of boot block IROS Flash address starts at 0x2f000000
        bootblocks : { 0x2f3f0000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
Top Boot:
---------
IROS ADDRESS 4mb
0x2f3f0000
0x2f3f2000
0x2f3f4000
0x2f3f6000
0x2f3f8000
0x2f3fa000
0x2f3fc000
0x2f3fe000

IROS ADDRESS 8mb
0x2f7f0000
0x2f7f2000
0x2f7f4000
0x2f7f6000
0x2f7f8000
0x2f7fa000
0x2f7fc000
0x2f7fe000

Bottom Boot:
------------
IROS ADDRESS 4mb
0x2f000000
0x2f002000
0x2f004000
0x2f006000
0x2f008000
0x2f00a000
0x2f00c000
0x2f00e000

*/
/*****************************************************************************/
#ifdef HAVE_SERIAL_FLASH
    {   //MX25L25635E
        device_id  : FLASHWORD(0x2019),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 512,
        device_size: 0x2000000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x2000000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   //MX25L12845E and S25FL128P01
        device_id  : FLASHWORD(0x2018),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 256,
        device_size: 0x1000000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x1000000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   //MX25L6445E
        device_id  : FLASHWORD(0x2017),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 128,
        device_size: 0x800000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x800000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   //MX25L3206E
        device_id  : FLASHWORD(0x2016),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   //MX25L3235D
        device_id  : FLASHWORD(0x5e16),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   //MX25L1606E
        device_id  : FLASHWORD(0x2015),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 32,
        device_size: 0x200000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x200000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // N25Q128A130B
        device_id  : FLASHWORD(0xba18),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 256,
        device_size: 0x1000000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x1000000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // GD25Q32
        device_id  : FLASHWORD(0x4016),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // SST25VF032B
        device_id  : FLASHWORD(0x254a),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // M25PX64
        device_id  : FLASHWORD(0x7117),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 128,
        device_size: 0x800000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x800000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
#endif

#ifdef HAVE_PARALLEL_FLASH
     {   // JS28F256C3-B Bottom Boot
        device_id  : FLASHWORD(0x8922),
        block_size : 0x20000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 256,
        device_size: 0x2000000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x2000000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : true,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x30000000 (next 4 block of size 0x8000)
        bootblocks : { 0x30000000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00008000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00008000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00008000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00008000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },

    {   // M58LT128KTB FLASH_TYPE_M58LT128KTB_DEV_ID
        device_id  : FLASHWORD(FLASH_TYPE_M58LT128KTB_DEV_ID),
        block_size : 0x20000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x20000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 128,
        device_size: 0x1000000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x1000000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : true,
        buffered_w : false,
        bootblock  : false,
        // IROS MAP: 0x2f3f0000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00020000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00020000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00020000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00020000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00020000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00020000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00020000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00020000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },

    {   // 28F320C3-T Top Boot
        device_id  : FLASHWORD(0x88c4),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : true,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f3f0000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f3f0000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // 28F320C3-B Bottom Boot
        device_id  : FLASHWORD(0x88c5),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : true,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // 28F160C3-B Bottom Boot
        device_id  : FLASHWORD(0x88C3),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 32,
        device_size: 0x200000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x200000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : true,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // LH28F640BFHE-B Sharp Top boot
        device_id  : FLASHWORD(0x00B2),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 128,
        device_size: 0x800000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x800000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : true,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f7f0000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },

    {   // SST39VF1601      2MB bottom boot
        device_id  : FLASHWORD(0x234b),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 32,
        device_size: 0x200000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x200000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // SST39VF3201  bottom boot
        device_id  : FLASHWORD(0x235b),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : true,
        buffered_w : false,
        bootblock  : false,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // SST39VF3201B  bottom boot
        device_id  : FLASHWORD(0x235d),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : true,
        buffered_w : false,
        bootblock  : false,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // SST39VF6401B  bottom boot
        device_id  : FLASHWORD(0x236d),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 128,
        device_size: 0x800000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x800000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // S29AL032  4Mb top boot
        device_id  : FLASHWORD(0x22f6),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f3f0000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f3f0000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // S29AL032  4Mb bottom boot
        device_id  : FLASHWORD(0x22f9),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // S29AL016, M29W160E 2MB top Boot
        device_id  : FLASHWORD(0x22c4),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 32,
        device_size: 0x200000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x200000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f1f0000 (next 4 block)
        bootblocks : { 0x2f1f0000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00008000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00004000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
    {   // S29AL016, M29W160E  2MB bottom boot
        device_id  : FLASHWORD(0x2249),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 32,
        device_size: 0x200000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x200000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f000000 (next 4 block)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00004000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00008000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // S29GL064, M29W640G  8Mb top boot
        device_id  : FLASHWORD(0x0317),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 128,
        device_size: 0x800000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x800000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f7f0000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f7f0000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // S29GL064, M29W640G  8Mb bottom boot
        device_id  : FLASHWORD(0x0217),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 128,
        device_size: 0x800000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x800000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // S29GL032  4Mb top boot
        device_id  : FLASHWORD(0x0316),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f3f0000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f3f0000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // S29GL032  4Mb bottom boot
        device_id  : FLASHWORD(0x0216),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // MX29LV640D  8Mb bottom boot
        device_id  : FLASHWORD(0x22cb),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 128,
        device_size: 0x800000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x800000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // MX29LV320D  4Mb top boot
        device_id  : FLASHWORD(0x22a7),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f3f0000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f3f0000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // MX29LV320D  4Mb bottom boot
        device_id  : FLASHWORD(0x22a8),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
    {   // W19B320B  4Mb bottom boot
        device_id  : FLASHWORD(0x222a),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 64,
        device_size: 0x400000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x400000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : true,
        // IROS MAP: 0x2f000000 (next 8 block of sixe 0x2000)
        bootblocks : { 0x2f000000 * CYGNUM_FLASH_INTERLEAVE, // Starting offset address of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE, // Size of boot block
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0x00002000 * CYGNUM_FLASH_INTERLEAVE,
                       0                                   // End of boot block
                     },
        banked     : false
    },
#endif
#ifdef HAVE_SERIAL_FLASH
    {   // Defualt 2M Flash Info
        device_id  : FLASHWORD(0x1234),
        block_size : 0x10000 * CYGNUM_FLASH_INTERLEAVE,
        block_mask  : ~(0x10000 * CYGNUM_FLASH_INTERLEAVE - 1),
        block_count: 32,
        device_size: 0x200000 * CYGNUM_FLASH_INTERLEAVE,
        base_mask  : ~(0x200000 * CYGNUM_FLASH_INTERLEAVE - 1),
        locking    : false,
        buffered_w : false,
        bootblock  : false,
        bootblocks : { 0x40000000 * CYGNUM_FLASH_INTERLEAVE,
                       0
                     },
        banked     : false
    },
#endif
/*****************************************************************************/
/*                                END                                        */
/*****************************************************************************/
