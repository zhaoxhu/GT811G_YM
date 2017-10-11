/*****************************************************************************/
/* This file contains unpublished documentation and software                 */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,       */
/* in whole or in part, of the information in this file without a            */
/* written consent of an officer of Cortina Systems Incorporated is          */
/* strictly prohibited.                                                      */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                       */
/*****************************************************************************/
/*
 * iros_flash_partition.h
 */
#ifndef _IROS_FLASH_PARTITION_H_
#define _IROS_FLASH_PARTITION_H_


#include "iros_config.h"
#include "iros_flash_oper.h"

/* if flash_info_tb[].flash_id does not match the flash_id in the super
   block, force the flash driver to behave like the flash defined in the
   super block.  The default settings are for typical setup. You may change
   these settings by running the iros partition tool at iROSBoot prompt
   "set partition" or "s p" The current default is the same for all flash
   sizes to make them backward compitable.  But it is the best to make
   different flash memory layouts for each flash sizes.
 */
#define IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START    ((char *)FLASH_BASE + 0x020000)


/* default map: 32MB
   BOOT            0x4000_0000     0x10000       64KB
   STARTUP_CONFIG  0x4001_0000     0x10000       64KB
   SUPER_BLOCK     0x4002_0000     0x10000       64KB
   CORE_DUMP       0x4003_0000     0x10000       64KB
   XIP             0x4004_0000    0x260000     2432KB
   BLOB0           0x402a_0000    0x180000     1536KB
   BLOB1           0x4042_0000    0x180000     1536KB
   STARTUP_CONFIG1 0x405a_0000     0x10000       64KB
   LOG             0x405b_0000     0x10000       64KB
   USER_DATA       0x405c_0000     0x10000       64KB
   some free space here
   JFFS2           0x4060_0000   0x1a00000    26624KB
*/

#define DEF_FLASH_MEMORY_LAYOUT_32MB                                \
            {                                                       \
                {                                                   \
                    IROS_FLASH_PARTITION_BOOT_ID,                   \
                    IROS_FLASH_PARTITION_TYPE_BOOT,                 \
                    (char *)FLASH_BASE,                             \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_STARTUP_CONFIG_ID,         \
                    IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG,       \
                    ((char *)FLASH_BASE + 0x010000),                \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_SUPER_BLOCK_ID,            \
                    IROS_FLASH_PARTITION_TYPE_SUPER_BLOCK,          \
                    ((char *)FLASH_BASE + 0x020000),                \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_COREDUMP_ID,               \
                    IROS_FLASH_PARTITION_TYPE_COREDUMP,             \
                    ((char *)FLASH_BASE + 0x030000),                \
                    0x10000                                         \
                }, /* core dump */                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_XIP_ID,                    \
                    IROS_FLASH_PARTITION_TYPE_XIP,                  \
                    ((char *)FLASH_BASE + 0x040000),                \
                    0x260000                                        \
                }, /* XIP */                                        \
                {                                                   \
                    IROS_FLASH_PARTITION_BLOB0_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_BLOB_ECOS,            \
                    ((char *)FLASH_BASE + 0x2a0000),                \
                    0x180000                                        \
                }, /* Blob 0 */                                     \
                {                                                   \
                    IROS_FLASH_PARTITION_BLOB1_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_BLOB_ECOS,            \
                    ((char *)FLASH_BASE + 0x420000),                \
                    0x180000                                        \
                }, /* Blob 1 */                                     \
                {                                                   \
                    IROS_FLASH_PARTITION_STARTUP_CONFIG1_ID,        \
                    IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG,       \
                    ((char *)FLASH_BASE + 0x5a0000),                \
                    0x10000                                         \
                }, /* startup config1*/                             \
                {                                                   \
                    IROS_FLASH_PARTITION_LOG_ID,                    \
                    IROS_FLASH_PARTITION_TYPE_LOG,                  \
                    ((char *)FLASH_BASE + 0x5b0000),                \
                    0x10000                                         \
                }, /* log */                                        \
                {                                                   \
                    IROS_FLASH_PARTITION_DATA_ID,                   \
                    IROS_FLASH_PARTITION_TYPE_DATA,                 \
                    ((char *)FLASH_BASE + 0x5c0000),                \
                    0x10000                                         \
                }, /* user data */                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_JFFS2_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_JFFS2,                \
                    ((char *)FLASH_BASE + 0x600000),                \
                    0x1a00000                                       \
                }, /* JFFS2 */                                      \
                {0,0,0,0}, /* the end of partitions */              \
            }


/* default map: 16MB
   BOOT            0x4000_0000     0x10000       64KB
   STARTUP_CONFIG  0x4001_0000     0x10000       64KB
   SUPER_BLOCK     0x4002_0000     0x10000       64KB
   CORE_DUMP       0x4003_0000     0x10000       64KB
   XIP             0x4004_0000    0x260000     2432KB
   BLOB0           0x402a_0000    0x180000     1536KB
   BLOB1           0x4042_0000    0x180000     1536KB
   STARTUP_CONFIG1 0x405a_0000     0x10000       64KB
   LOG             0x405b_0000     0x10000       64KB
   USER_DATA       0x405c_0000     0x10000       64KB
   some free space here
   JFFS2           0x4060_0000    0xa00000    10240KB
*/

#define DEF_FLASH_MEMORY_LAYOUT_16MB                                \
            {                                                       \
                {                                                   \
                    IROS_FLASH_PARTITION_BOOT_ID,                   \
                    IROS_FLASH_PARTITION_TYPE_BOOT,                 \
                    (char *)FLASH_BASE,                             \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_STARTUP_CONFIG_ID,         \
                    IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG,       \
                    ((char *)FLASH_BASE + 0x010000),                \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_SUPER_BLOCK_ID,            \
                    IROS_FLASH_PARTITION_TYPE_SUPER_BLOCK,          \
                    ((char *)FLASH_BASE + 0x020000),                \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_COREDUMP_ID,               \
                    IROS_FLASH_PARTITION_TYPE_COREDUMP,             \
                    ((char *)FLASH_BASE + 0x030000),                \
                    0x10000                                         \
                }, /* core dump */                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_XIP_ID,                    \
                    IROS_FLASH_PARTITION_TYPE_XIP,                  \
                    ((char *)FLASH_BASE + 0x040000),                \
                    0x260000                                        \
                }, /* XIP */                                        \
                {                                                   \
                    IROS_FLASH_PARTITION_BLOB0_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_BLOB_ECOS,            \
                    ((char *)FLASH_BASE + 0x2a0000),                \
                    0x180000                                        \
                }, /* Blob 0 */                                     \
                {                                                   \
                    IROS_FLASH_PARTITION_BLOB1_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_BLOB_ECOS,            \
                    ((char *)FLASH_BASE + 0x420000),                \
                    0x180000                                        \
                }, /* Blob 1 */                                     \
                {                                                   \
                    IROS_FLASH_PARTITION_STARTUP_CONFIG1_ID,        \
                    IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG,       \
                    ((char *)FLASH_BASE + 0x5a0000),                \
                    0x10000                                         \
                }, /* startup config1*/                             \
                {                                                   \
                    IROS_FLASH_PARTITION_LOG_ID,                    \
                    IROS_FLASH_PARTITION_TYPE_LOG,                  \
                    ((char *)FLASH_BASE + 0x5b0000),                \
                    0x10000                                         \
                }, /* log */                                        \
                {                                                   \
                    IROS_FLASH_PARTITION_DATA_ID,                   \
                    IROS_FLASH_PARTITION_TYPE_DATA,                 \
                    ((char *)FLASH_BASE + 0x5c0000),                \
                    0x10000                                         \
                }, /* user data */                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_JFFS2_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_JFFS2,                \
                    ((char *)FLASH_BASE + 0x600000),                \
                    0xa00000                                        \
                }, /* JFFS2 */                                      \
                {0,0,0,0}, /* the end of partitions */              \
            }



/* default map: 8MB
   BOOT            0x4000_0000     0x10000       64KB
   STARTUP_CONFIG  0x4001_0000     0x10000       64KB
   SUPER_BLOCK     0x4002_0000     0x10000       64KB
   CORE_DUMP       0x4003_0000     0x10000       64KB
   XIP             0x4004_0000    0x260000     2432KB
   BLOB0           0x402a_0000    0x180000     1536KB
   BLOB1           0x4042_0000    0x180000     1536KB
   STARTUP_CONFIG1 0x405a_0000     0x10000       64KB
   LOG             0x405b_0000     0x10000       64KB
   USER_DATA       0x405c_0000     0x10000       64KB
   some free space here
   JFFS2           0x4060_0000    0x200000     2048KB
*/

#define DEF_FLASH_MEMORY_LAYOUT_8MB                                 \
            {                                                       \
                {                                                   \
                    IROS_FLASH_PARTITION_BOOT_ID,                   \
                    IROS_FLASH_PARTITION_TYPE_BOOT,                 \
                    (char *)FLASH_BASE,                             \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_STARTUP_CONFIG_ID,         \
                    IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG,       \
                    ((char *)FLASH_BASE + 0x010000),                \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_SUPER_BLOCK_ID,            \
                    IROS_FLASH_PARTITION_TYPE_SUPER_BLOCK,          \
                    ((char *)FLASH_BASE + 0x020000),                \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_COREDUMP_ID,               \
                    IROS_FLASH_PARTITION_TYPE_COREDUMP,             \
                    ((char *)FLASH_BASE + 0x030000),                \
                    0x10000                                         \
                }, /* core dump */                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_XIP_ID,                    \
                    IROS_FLASH_PARTITION_TYPE_XIP,                  \
                    ((char *)FLASH_BASE + 0x040000),                \
                    0x260000                                        \
                }, /* XIP */                                        \
                {                                                   \
                    IROS_FLASH_PARTITION_BLOB0_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_BLOB_ECOS,            \
                    ((char *)FLASH_BASE + 0x2a0000),                \
                    0x180000                                        \
                }, /* Blob 0 */                                     \
                {                                                   \
                    IROS_FLASH_PARTITION_BLOB1_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_BLOB_ECOS,            \
                    ((char *)FLASH_BASE + 0x420000),                \
                    0x180000                                        \
                }, /* Blob 1 */                                     \
                {                                                   \
                    IROS_FLASH_PARTITION_STARTUP_CONFIG1_ID,        \
                    IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG,       \
                    ((char *)FLASH_BASE + 0x5a0000),                \
                    0x10000                                         \
                }, /* startup config1*/                             \
                {                                                   \
                    IROS_FLASH_PARTITION_LOG_ID,                    \
                    IROS_FLASH_PARTITION_TYPE_LOG,                  \
                    ((char *)FLASH_BASE + 0x5b0000),                \
                    0x10000                                         \
                }, /* log */                                        \
                {                                                   \
                    IROS_FLASH_PARTITION_DATA_ID,                   \
                    IROS_FLASH_PARTITION_TYPE_DATA,                 \
                    ((char *)FLASH_BASE + 0x5c0000),                \
                    0x10000                                         \
                }, /* user data */                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_JFFS2_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_JFFS2,                \
                    ((char *)FLASH_BASE + 0x600000),                \
                    0x200000                                        \
                }, /* JFFS2 */                                      \
                {0,0,0,0}, /* the end of partitions */              \
            }



/* default map: 4MB
   BOOT            0x4000_0000     0x10000     64KB
   STARTUP_CONFIG  0x4001_0000     0x10000     64KB
   SUPER_BLOCK     0x4002_0000     0x10000     64KB
   CORE_DUMP       0x4003_0000     0x10000     64KB
   XIP             0x4004_0000    0x150000   1344KB
   BLOB0           0x4019_0000     0xa0000    640KB
   BLOB1           0x4023_0000     0xa0000    640KB
   STARTUP_CONFIG1 0x402D_0000     0x10000     64KB
   LOG             0x402E_0000     0x10000     64KB
   USER_DATA       0x402F_0000     0x10000     64KB
   JFFS2           0x4030_0000    0x100000   1024KB
*/

#define DEF_FLASH_MEMORY_LAYOUT_4MB                                 \
            {                                                       \
                {                                                   \
                    IROS_FLASH_PARTITION_BOOT_ID,                   \
                    IROS_FLASH_PARTITION_TYPE_BOOT,                 \
                    (char *)FLASH_BASE,                             \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_STARTUP_CONFIG_ID,         \
                    IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG,       \
                    ((char *)FLASH_BASE + 0x10000),                 \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_SUPER_BLOCK_ID,            \
                    IROS_FLASH_PARTITION_TYPE_SUPER_BLOCK,          \
                    ((char *)FLASH_BASE + 0x20000),                 \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_COREDUMP_ID,               \
                    IROS_FLASH_PARTITION_TYPE_COREDUMP,             \
                    ((char *)FLASH_BASE + 0x30000),                 \
                    0x10000                                         \
                }, /* Core dump */                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_XIP_ID,                    \
                    IROS_FLASH_PARTITION_TYPE_XIP,                  \
                    ((char *)FLASH_BASE + 0x40000),                 \
                    0x150000                                        \
                }, /* XIP */                                        \
                {                                                   \
                    IROS_FLASH_PARTITION_BLOB0_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_BLOB_ECOS,            \
                    ((char *)FLASH_BASE + 0x190000),                \
                    0xA0000                                         \
                }, /* Blob 0 */                                     \
                {                                                   \
                    IROS_FLASH_PARTITION_BLOB1_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_BLOB_ECOS,            \
                    ((char *)FLASH_BASE + 0x230000),                \
                    0xA0000                                         \
                }, /* Blob 1 */                                     \
                {                                                   \
                    IROS_FLASH_PARTITION_STARTUP_CONFIG1_ID,        \
                    IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG,       \
                    ((char *)FLASH_BASE + 0x2D0000),                \
                    0x10000                                         \
                }, /* startup config1*/                             \
                {                                                   \
                    IROS_FLASH_PARTITION_LOG_ID,                    \
                    IROS_FLASH_PARTITION_TYPE_LOG,                  \
                    ((char *)FLASH_BASE + 0x2E0000),                \
                    0x10000                                         \
                }, /* log */                                        \
                {                                                   \
                    IROS_FLASH_PARTITION_DATA_ID,                   \
                    IROS_FLASH_PARTITION_TYPE_DATA,                 \
                    ((char *)FLASH_BASE + 0x2F0000),                \
                    0x10000                                         \
                }, /* User data */                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_JFFS2_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_JFFS2,                \
                    ((char *)FLASH_BASE + 0x300000),                \
                    0x100000                                        \
                }, /* JFFS2 */                                      \
                {0,0,0,0}, /* the end of partitions */              \
            }


/* default map: 2MB
   BOOT            0x4000_0000     0x10000    64KB
   STARTUP_CONFIG  0x4001_0000     0x10000    64KB
   USER_DATA       0x4002_0000     0x10000    64KB
   CORE_DUMP       0x4003_0000     0x10000    64KB
   XIP             0x4004_0000     0xd0000   832KB
   BLOB0           0x4011_0000     0x70000   448KB
   BLOB1           0x4018_0000     0x70000   448KB
   JFFS2           0x401f_0000     0x10000    64KB
*/

#define DEF_FLASH_MEMORY_LAYOUT_2MB                                 \
            {                                                       \
                {                                                   \
                    IROS_FLASH_PARTITION_BOOT_ID,                   \
                    IROS_FLASH_PARTITION_TYPE_BOOT,                 \
                    (char *)FLASH_BASE,                             \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_STARTUP_CONFIG_ID,         \
                    IROS_FLASH_PARTITION_TYPE_STARTUP_CONFIG,       \
                    ((char *)FLASH_BASE + 0x010000),                \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_DATA_ID,                   \
                    IROS_FLASH_PARTITION_TYPE_DATA,                 \
                    ((char *)FLASH_BASE + 0x020000),                \
                    0x10000                                         \
                },                                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_COREDUMP_ID,               \
                    IROS_FLASH_PARTITION_TYPE_COREDUMP,             \
                    ((char *)FLASH_BASE + 0x030000),                \
                    0x10000                                         \
                }, /* Core dump */                                  \
                {                                                   \
                    IROS_FLASH_PARTITION_XIP_ID,                    \
                    IROS_FLASH_PARTITION_TYPE_XIP,                  \
                    ((char *)FLASH_BASE + 0x040000),                \
                    0xd0000                                         \
                }, /* XIP */                                        \
                {                                                   \
                    IROS_FLASH_PARTITION_BLOB0_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_BLOB_ECOS,            \
                    ((char *)FLASH_BASE + 0x110000),                \
                    0x70000                                         \
                }, /* Blob 0 */                                     \
                {                                                   \
                    IROS_FLASH_PARTITION_BLOB1_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_BLOB_ECOS,            \
                    ((char *)FLASH_BASE + 0x180000),                \
                    0x70000                                         \
                }, /* Blob 1 */                                     \
                {                                                   \
                    IROS_FLASH_PARTITION_JFFS2_ID,                  \
                    IROS_FLASH_PARTITION_TYPE_JFFS2,                \
                    ((char *)FLASH_BASE + 0x1f0000),                \
                    0x10000                                         \
                }, /* JFFS2 */                                      \
                {0,0,0,0}, /* the end of partitions */              \
            }


static flash_info_t flash_info_tb[] = {
#ifdef HAVE_SERIAL_FLASH
    /* FLASH_TYPE_M25P32V6G_U_ID, 32MB */
    {
        FLASH_TYPE_M25P32V6G_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_M25P32V6G_U_ID,
            0x400000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_4MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_S25FL128P01_U_ID, 16MB */
    {
        FLASH_TYPE_S25FL128P01_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_S25FL128P01_U_ID,
            0x1000000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_16MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_MX25L25635E_U_ID, 32MB */
    {
        FLASH_TYPE_MX25L25635E_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_MX25L25635E_U_ID,
            0x2000000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_32MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_MX25L12845E_U_ID, 16MB */
    {
        FLASH_TYPE_MX25L12845E_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_MX25L12845E_U_ID,
            0x1000000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_16MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_MX25L6445E_U_ID, 8MB */
    {
        FLASH_TYPE_MX25L6445E_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_MX25L6445E_U_ID,
            0x800000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_8MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_M25PX64_U_ID, 8MB */
    {
        FLASH_TYPE_M25PX64_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_M25PX64_U_ID,
            0x800000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_8MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_MX25L3206E_U_ID, 4MB */
    {
        FLASH_TYPE_MX25L3206E_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_MX25L3206E_U_ID,
            0x400000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_4MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_MX25L3235D_U_ID, 4MB */
    {
        FLASH_TYPE_MX25L3235D_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_MX25L3235D_U_ID,
            0x400000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_4MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_MX25L1606E_U_ID, 2MB */
    {
        FLASH_TYPE_MX25L1606E_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_MX25L1606E_U_ID,
            0x200000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_2MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_N25Q128A130_B_ID, 16MB */
    {
        FLASH_TYPE_N25Q128A130_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_N25Q128A130_B_ID,
            0x1000000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_16MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_GD25Q32_U_ID, 4MB */
    {
        FLASH_TYPE_GD25Q32_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_GD25Q32_U_ID,
            0x400000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_4MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_SST25VF032B_U_ID, 4MB */
    {
        FLASH_TYPE_SST25VF032B_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_SST25VF032B_U_ID,
            0x400000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_4MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_W25Q32_U_ID, 4MB */
    {
        FLASH_TYPE_W25Q32_U_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,
        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_W25Q32_U_ID,
            0x400000,
            IROS_FLASH_BLOCK_SIZE_64KB,
            DEF_FLASH_MEMORY_LAYOUT_4MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
#endif

#ifdef HAVE_PARALLEL_FLASH
    /* Intel 16MB, Top FLASH_TYPE_M58LT128KTB_ID */
    {
        FLASH_TYPE_M58LT128KTB_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_M58LT128KTB_ID,
            0x1000000,                     /* 16MB */
            0x20000,                      /* 128KB */

            // default partition table, using the first 8MB
            DEF_FLASH_MEMORY_LAYOUT_16MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* Intel 32MB, embedded flash j3 device */
    {
        FLASH_TYPE_EMB_J3_32MB_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_EMB_J3_32MB_ID,
            0x2000000,                     /* 32MB */
            0x20000,                      /* 128KB */

            // default partition table, using the first 8MB
            DEF_FLASH_MEMORY_LAYOUT_8MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    {
        FLASH_TYPE_JS28F256P3_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_JS28F256P3_B_ID,
            0x2000000,                    /* 32MB */
            0x20000,                      /* 128KB */
            // default partition table, using the first 16MB
            DEF_FLASH_MEMORY_LAYOUT_32MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* Intel 16MB, embedded flash j3 device */
    {
        FLASH_TYPE_EMB_J3_16MB_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_EMB_J3_16MB_ID,
            0x1000000,                     /* 16MB */
            0x20000,                      /* 128KB */

            // default partition table, using the first 8MB
            DEF_FLASH_MEMORY_LAYOUT_8MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* Intel 8MB, embedded flash j3 device */
    {
        FLASH_TYPE_EMB_J3_8MB_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_EMB_J3_8MB_ID,
            0x800000,                     /* 8MB */
            0x20000,                      /* 128KB */

            // default partition table, using the first 8MB
            DEF_FLASH_MEMORY_LAYOUT_8MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* Sharp 8MB, top boot device */
    {
        FLASH_TYPE_LH28F640BFHE_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_LH28F640BFHE_ID,
            0x800000,                     /* 8MB */
            IROS_FLASH_BLOCK_SIZE_64KB, /* 64KB */

            // default partition table,
            DEF_FLASH_MEMORY_LAYOUT_8MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* Intel 8MB, top boot device */
    {
        FLASH_TYPE_JS28F160CC_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_JS28F160CC_ID,
            0x800000,                     /* 8MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table,
            DEF_FLASH_MEMORY_LAYOUT_8MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* Intel 8MB, bottom device */
    {
        FLASH_TYPE_JS28F160CD_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_JS28F160CD_ID,
            0x800000,                     /* 8MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table,
            DEF_FLASH_MEMORY_LAYOUT_8MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* Intel 4MB top boot device*/
    {
        FLASH_TYPE_JS28F320C4_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_JS28F320C4_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table, using the first 4MB
            DEF_FLASH_MEMORY_LAYOUT_4MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },


    /* Intel 4MB bottom boot device*/
    {
        FLASH_TYPE_JS28F320C5_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_JS28F320C5_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table, using the first 4MB
            DEF_FLASH_MEMORY_LAYOUT_4MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* Intel 2MB top boot*/
    {
        FLASH_TYPE_TE28F160C2_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_TE28F160C2_ID,
            0x200000,                     /* 2MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_2MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },


    /* Intel 2MB bottom boot*/
    {
        FLASH_TYPE_JS28F160C3_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_JS28F160C3_ID,
            0x200000,                     /* 2MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 32KB */
            DEF_FLASH_MEMORY_LAYOUT_2MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* SST 2MB bottom boot device*/
    {
        FLASH_TYPE_SST39VF1601_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_SST39VF1601_B_ID,
            0x200000,                     /* 2MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_2MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* SST 4MB bottom boot device*/
    {
        FLASH_TYPE_SST39VF3201_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_SST39VF3201_B_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table, using the first 4MB
            DEF_FLASH_MEMORY_LAYOUT_4MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* SST 4MB bottom boot device*/
    {
        FLASH_TYPE_SST39VF3201B_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_SST39VF3201B_B_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table, using the first 4MB
            DEF_FLASH_MEMORY_LAYOUT_4MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /*SST6401B bottom boot 8MB*/
    {
        FLASH_TYPE_SST39VF6401B_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_SST39VF6401B_B_ID,
            0x800000,                     /* 8MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table, using the first 4MB
            DEF_FLASH_MEMORY_LAYOUT_8MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* FLASH_TYPE_S29AL032_B_ID 4MB bottom boot device*/
    {
        FLASH_TYPE_S29AL032_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_S29AL032_B_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table, using the first 4MB
            DEF_FLASH_MEMORY_LAYOUT_4MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_S29AL032_T_ID 4MB top boot device*/
    {
        FLASH_TYPE_S29AL032_T_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_S29AL032_T_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table, using the first 4MB
            DEF_FLASH_MEMORY_LAYOUT_4MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_M29W160E_B_ID 2MB bottom boot device*/
    {
        FLASH_TYPE_M29W160E_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_M29W160E_B_ID,
            0x200000,                     /* 2MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_2MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_M29W160E_T_ID 2MB top boot device*/
    {
        FLASH_TYPE_M29W160E_T_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_M29W160E_T_ID,
            0x200000,                     /* 2MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_2MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_M29W640G_B_ID 8MB bottom boot device*/
    {
        FLASH_TYPE_M29W640G_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_M29W640G_B_ID,
            0x800000,                     /* 8MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_8MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_M29W640G_T_ID 8MB top boot device*/
    {
        FLASH_TYPE_M29W640G_T_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_M29W640G_T_ID,
            0x800000,                     /* 8MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_8MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },

    /* FLASH_TYPE_S29AL016_B_ID 2MB bottom boot device*/
    {
        FLASH_TYPE_S29AL016_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_S29AL016_B_ID,
            0x200000,                     /* 2MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_2MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_S29AL016_T_ID 2MB top boot device*/
    {
        FLASH_TYPE_S29AL016_T_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_S29AL016_T_ID,
            0x200000,                     /* 2MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_2MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_S29GL064_B_ID  8MB bottome boot device */
    {
        FLASH_TYPE_S29GL064_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_S29GL064_B_ID,
            0x800000,                     /* 8MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_8MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    /* FLASH_TYPE_S29GL064_T_ID  8MB top boot device */
    {
        FLASH_TYPE_S29GL064_T_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_S29GL064_T_ID,
            0x800000,                     /* 8MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_8MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
   /* FLASH_TYPE_S29GL032_B_ID 4MB bottom boot device*/
   {
        FLASH_TYPE_S29GL032_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_S29GL032_B_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table, using the first 4MB
            DEF_FLASH_MEMORY_LAYOUT_4MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
   /* FLASH_TYPE_S29GL032_T_ID 4MB top boot device*/
   {
        FLASH_TYPE_S29GL032_T_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_S29GL032_T_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */

            // default partition table, using the first 4MB
            DEF_FLASH_MEMORY_LAYOUT_4MB,

            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
   /* FLASH_TYPE_MX29LV640D_B_ID 8MB bottom boot device*/
   {
        FLASH_TYPE_MX29LV640D_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_MX29LV640D_B_ID,
            0x800000,                     /* 8MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_8MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
   /* FLASH_TYPE_MX29LV320D_T_ID 4MB top boot device*/
   {
        FLASH_TYPE_MX29LV320D_T_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_MX29LV320D_T_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_4MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
   /* FLASH_TYPE_MX29LV320D_B_ID 4MB bottom boot device*/
   {
        FLASH_TYPE_MX29LV320D_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_MX29LV320D_B_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_4MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
   /* FLASH_TYPE_W19B320B_B_ID 4MB bottom boot device*/
   {
        FLASH_TYPE_W19B320B_B_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_W19B320B_B_ID,
            0x400000,                     /* 4MB */
            IROS_FLASH_BLOCK_SIZE_64KB,                        /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_4MB,
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
#endif
   {    /*Default Flash ID 0x12345678*/
        FLASH_TYPE_DEFUALT_ID,
        IROS_FLASH_PARTITION_SUPER_BLOCK_DEF_START,   /* super block loc */

        // default super block
        {
            IROS_FLASH_SUPER_BLOCK_HEAD_SIG,
            FLASH_TYPE_DEFUALT_ID,
            0x200000,                                                              /*2M by Default*/
            IROS_FLASH_BLOCK_SIZE_64KB,                           /* 64KB */
            DEF_FLASH_MEMORY_LAYOUT_2MB,                      
            IROS_FLASH_SUPER_BLOCK_TAIL_SIG
        }
    },
    { 0, } // the end of the flash_info_tab

};


char *part_type_to_str[] = {
    "UNDEF",
    "BOOT partition",
    "BLOB iros blob/ecos partition",
    "STARTUP_CONFIG startup configuration parameter partition",
    "JFFS2 file system partition",
    "DATA user data partition",
    "CORE_DUMP core dump data partition",
    "XIP  eXecution In Place partition",
    "SUPER_BLOCK flash super block",
    "LOG log partition"
};

char *part_type_to_str_short[] = {
    "UNDEF",
    "BOOT ",
    "BLOB ",
    "STARTUP_CONFIG",
    "JFFS2 ",
    "DATA  ",
    "CORE_DUMP",
    "XIP   ",
    "SUPER_BLOCK",
    "LOG"
};

#endif // _IROS_FLASH_PARTITION_H_
