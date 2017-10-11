/****************************************************************************
            Software License for Customer Use of Cortina Software
                          Grant Terms and Conditions

IMPORTANT NOTICE - READ CAREFULLY: This Software License for Customer Use
of Cortina Software ("LICENSE") is the agreement which governs use of
software of Cortina Systems, Inc. and its subsidiaries ("CORTINA"),
including computer software (source code and object code) and associated
printed materials ("SOFTWARE").  The SOFTWARE is protected by copyright laws
and international copyright treaties, as well as other intellectual property
laws and treaties.  The SOFTWARE is not sold, and instead is only licensed
for use, strictly in accordance with this document.  Any hardware sold by
CORTINA is protected by various patents, and is sold but this LICENSE does
not cover that sale, since it may not necessarily be sold as a package with
the SOFTWARE.  This LICENSE sets forth the terms and conditions of the
SOFTWARE LICENSE only.  By downloading, installing, copying, or otherwise
using the SOFTWARE, you agree to be bound by the terms of this LICENSE.
If you do not agree to the terms of this LICENSE, then do not download the
SOFTWARE.

DEFINITIONS:  "DEVICE" means the Cortina Systems?LynxD SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems?SDK software.

GRANT OF LICENSE:  Subject to the restrictions below, CORTINA hereby grants
CUSTOMER a non-exclusive, non-assignable, non-transferable, royalty-free,
perpetual copyright license to (1) install and use the SOFTWARE for
reference only with the DEVICE; and (2) copy the SOFTWARE for your internal
use only for use with the DEVICE.

RESTRICTIONS:  The SOFTWARE must be used solely in conjunction with the
DEVICE and solely with Your own products that incorporate the DEVICE.  You
may not distribute the SOFTWARE to any third party.  You may not modify
the SOFTWARE or make derivatives of the SOFTWARE without assigning any and
all rights in such modifications and derivatives to CORTINA.  You shall not
through incorporation, modification or distribution of the SOFTWARE cause
it to become subject to any open source licenses.  You may not
reverse-assemble, reverse-compile, or otherwise reverse-engineer any
SOFTWARE provided in binary or machine readable form.  You may not
distribute the SOFTWARE to your customers without written permission
from CORTINA.

OWNERSHIP OF SOFTWARE AND COPYRIGHTS. All title and copyrights in and the
SOFTWARE and any accompanying printed materials, and copies of the SOFTWARE,
are owned by CORTINA. The SOFTWARE protected by the copyright laws of the
United States and other countries, and international treaty provisions.
You may not remove any copyright notices from the SOFTWARE.  Except as
otherwise expressly provided, CORTINA grants no express or implied right
under CORTINA patents, copyrights, trademarks, or other intellectual
property rights.

DISCLAIMER OF WARRANTIES. THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY
EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING ANY IMPLIED WARRANTIES
OF MERCHANTABILITY, NONINFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE,
TITLE, AND NON-INFRINGEMENT.  CORTINA does not warrant or assume
responsibility for the accuracy or completeness of any information, text,
graphics, links or other items contained within the SOFTWARE.  Without
limiting the foregoing, you are solely responsible for determining and
verifying that the SOFTWARE that you obtain and install is the appropriate
version for your purpose.

LIMITATION OF LIABILITY. IN NO EVENT SHALL CORTINA OR ITS SUPPLIERS BE
LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, LOST
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING OUT
OF THE USE OF OR INABILITY TO USE OF OR INABILITY TO USE THE SOFTWARE, EVEN
IF CORTINA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
TERMINATION OF THIS LICENSE. This LICENSE will automatically terminate if
You fail to comply with any of the terms and conditions hereof. Upon
termination, You will immediately cease use of the SOFTWARE and destroy all
copies of the SOFTWARE or return all copies of the SOFTWARE in your control
to CORTINA.  IF you commence or participate in any legal proceeding against
CORTINA, then CORTINA may, in its sole discretion, suspend or terminate all
license grants and any other rights provided under this LICENSE during the
pendency of such legal proceedings.
APPLICABLE LAWS. Claims arising under this LICENSE shall be governed by the
laws of the State of California, excluding its principles of conflict of
laws.  The United Nations Convention on Contracts for the International Sale
of Goods is specifically disclaimed.  You shall not export the SOFTWARE
without first obtaining any required export license or other approval from
the applicable governmental entity, if required.  This is the entire
agreement and understanding between You and CORTINA relating to this subject
matter.
GOVERNMENT RESTRICTED RIGHTS. The SOFTWARE is provided with "RESTRICTED
RIGHTS." Use, duplication, or disclosure by the Government is subject to
restrictions as set forth in FAR52.227-14 and DFAR252.227-7013 et seq. or
its successor. Use of the SOFTWARE by the Government constitutes
acknowledgment of CORTINA's proprietary rights therein. Contractor or
Manufacturer is CORTINA.

Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/
/*
 *
 * image.h
 *
 * $Id: image.h,v 1.1.4.3.10.1.6.1.8.1.6.2 2011/07/07 08:08:58 ljin Exp $
 */


// definitions for image and blob inforamtion
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "iros_config.h"
#include "cs_types.h"

#define CURRENT_BLOB_DESC_BASEADDR        0x0000f000
#define CURRENT_BLOB_DESC_SIGNATURE       0xddeeddee
#define BLOB_INFO_TAB_PTRS_SIGNATURE      0x12345678
#define BLOB_INFO_TAB_HEAD_SIGNATURE1     0x13579bdf
#define BLOB_INFO_TAB_HEAD_SIGNATURE2     0x13579bdf
#define BLOB_INFO_TAB_TAIL_SIGNATURE      0x87654321
#define BLOB_INFO_TABL_SIZE_DEF           2

#define BLOB_INFO_TAB_PTRS_OFFSET             0x20000
#define BLOB_INFO_TAB_PTRS_BASE               (IMST_PARALLEL_FLASH_BASE + BLOB_INFO_TAB_PTRS_OFFSET)
#define BLOB_INFO_HEAD_SIGNATURE              0x13572468
#define BLOB_INFO_TAIL_SIGNATURE              0x87654321


// blob flags in current_blob_desc_t
// no defs for now

//this struct is in RAM. access/modify by both loader and app
typedef struct current_blob_desc_t {
        cyg_uint32 signature;
        cyg_uint16 update_timeout;
        cyg_uint8  blob_idx;         // 0 or 1
        cyg_uint8  num_retries;
} current_blob_desc_t;

// this struct is found at fixed location in FLASH Boot ROM area
// write during manufacturing or by app
// read-only by loader
typedef struct blob_info_tab_ptrs_t {
        unsigned int signature;
        struct blob_info_tab_head_desc_t * blob_tab_ptr;
        struct blob_info_tab_head_desc_t * blob_tab_ptr_dup;
} blob_info_tab_ptrs_t;


// head + a tab of blob_info_desc + tail in flash
typedef struct blob_info_tab_head_desc_t {
        cyg_uint32 head_signature1;
        cyg_uint32 max_num_blobs;        // 2 by default
        cyg_uint32 next_avail_stamp;
        cyg_uint32 head_signature2;      // need here to make sure max_num_blobs are correct to prevent boot failure
} blob_info_tab_head_desc_t;

typedef struct blob_info_tab_tail_desc_t {
        cyg_uint32 tail_signature;
} blob_info_tab_tail_desc_t;


typedef struct blob_info_desc_t {
        cyg_uint32 head_signature;
        cyg_uint32 upgrade_stamp;                           // a counter increased for each new blob
        cyg_uint32 len;                                     // blob length in bytes
        struct image_blob_header_t * blob_start_ptr;        // blob start addr
        unsigned int short is_not_confirmed;
        unsigned int short is_not_executed;
        cyg_uint32 xip_len;                                  // len of unzipped xip blob
        cyg_uint32 xip_crc32;                                // image CRC for unzipped xip  blob
        cyg_uint32 tail_signature;
} blob_info_desc_t;


typedef struct image_blob_header_t {
        cyg_uint32 crc32;                 // image crc for image + loading_start
        char *loading_start;              // ram address to put the uploaded image
        cyg_uint32 reserved;              // reserved for future
        char blob_info[64];               // any info such as a string to identify a blob for debugging
} image_blob_header_t;

typedef enum {
    IMAGE_ACTIVE  = 0,
    IMAGE_STANDBY = 1,
    IMAGE_NEXTBOOT= 2
} image_type_e;

typedef enum {
    IMAGE_SYNC_DONE     = 0,
    IMAGE_SYNC_NOT_COMP = 1
} image_sync_state_e;

#ifdef HAVE_NTT_IMAGE_HEADER
#define CUSTOMER_HEADER_SIGNATURE       0xffffff01
#define CUSTOMER_HEADER_LENGTH          256

#define NTT_VERSION_LEN     8
#define NTT_NAME_LEN        16
#define NTT_TYPE_LEN        8

typedef struct ntt_image_header {
    cyg_uint8   flag;
    cyg_uint8   reserved_1[3];
    cyg_uint32  fw_size;
    cyg_uint8   reserved_2[8];
    cyg_uint8   file_name[NTT_NAME_LEN];
    cyg_uint8   product_name[NTT_NAME_LEN];
    cyg_uint32  file_create_time;
    cyg_uint32  file_size;
    cyg_uint32  file_checksum;
    cyg_uint8   reserved_3[4];
    cyg_uint8   onu_type[NTT_TYPE_LEN];
    cyg_uint8   reserved_4[8];
    cyg_uint8   onu_vendor[NTT_NAME_LEN];
    cyg_uint8   onu_hw_version[NTT_VERSION_LEN];
    cyg_uint8   onu_sw_version[NTT_VERSION_LEN];
    cyg_uint8   reserved[144];
} ntt_image_header_t;
#endif


extern blob_info_desc_t *blob_info_tab[2];
extern current_blob_desc_t current_blob_desc;

extern unsigned int blob_part_tab[2];

extern cs_uint32 iros_image_write_flash(cs_int8* _image, cs_uint32 _len);
extern cs_uint32 iros_image_commit(void);
extern cs_uint32 iros_image_sync_flash();
extern void iros_image_sync_status(cs_uint8* status);
extern cs_uint32 iros_image_crc_check(cs_int8* _image, cs_uint32 _len, cs_uint8* _result);
extern cs_uint32 iros_image_get_version(image_type_e _type, cs_int8* _buf, cs_uint8 _len);
extern cs_uint32 standby_blob_crc_invalidate(void);


#endif /* _IMAGE_H_ */
