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

#ifndef _TLV_PARSER_H_
#define _TLV_PARSER_H_

#define TLV_PARSER_OK        (0)
#define TLV_PARSER_ERROR    (-1)

#define TLV_HEADER_MAGIC    (0x84739458)

#define MAX_TLV_INSTANCE    2

#define startup_cfg_printf  cs_printf
#define TLV_PARSER_OFFSET(structure, member) ((int) &(((structure *) 0) -> member))

#define CFG_MAGIC_OFFSET    TLV_PARSER_OFFSET(tlv_header_t, magic)
#define CFG_CRC_OFFSET      TLV_PARSER_OFFSET(tlv_header_t, cfg_crc)
#define CFG_ACTIVE_OFFSET   TLV_PARSER_OFFSET(tlv_header_t, active)
#define CFG_FLAG_OFFSET     TLV_PARSER_OFFSET(tlv_header_t, reserved)
#define CFG_LEN_OFFSET      TLV_PARSER_OFFSET(tlv_header_t, cfg_len)
#define CFG_VERSION_OFFSET  TLV_PARSER_OFFSET(tlv_header_t, version)

#define TLV_TYPE_OFFSET     TLV_PARSER_OFFSET(tlv_t, type)
#define TLV_LEN_OFFSET      TLV_PARSER_OFFSET(tlv_t, len)
#define TLV_VALUE_OFFSET    TLV_PARSER_OFFSET(tlv_t, data)

#define TLV_INVALID_INST_ID        0

typedef struct
{
    unsigned int    magic;
    unsigned short  cfg_crc;
    unsigned char   active;
    unsigned char   reserved;
    unsigned int    cfg_len;
    unsigned int    version;
} tlv_header_t;

#define TLV_CFG_ENC_FLAG     0x01

typedef struct
{
    unsigned short  type;
    unsigned short  len;    /*The max length(8, align issue) of this field, not the real length*/
    unsigned char   data[1];
} tlv_t;

typedef struct
{
    void            *base_address;
    unsigned int    *tlv_ptr_list;
    unsigned int    tlv_num;
    unsigned int    max_tlv_len;
    int             (*store)(void *base_addr , int len);
} tlv_instance_t;

unsigned int tlv_inst_init
    (
    void            *base_address,
    unsigned int    max_cfg_len,
    int             (*store)(void *base_addr , int len)
    );
unsigned int tlv_inst_duplicate
    (
    tlv_instance_t *src
    );

int tlv_load
    (
    unsigned int instance_id
    );
int tlv_read_char
    (
    unsigned int instance_id,
    unsigned short type, 
    unsigned char *value
    );
int tlv_read_short
    (
    unsigned int instance_id,
    unsigned short type, 
    unsigned short *value
    );
int tlv_read_int
    (
    unsigned int instance_id,
    unsigned short type , 
    unsigned int *value
    );
int tlv_read_buffer
    (
    unsigned int instance_id,
    unsigned short type, 
    unsigned int len, 
    unsigned char *value
    );
int tlv_write_char
    (
    unsigned int instance_id,
    unsigned short type, 
    unsigned char value
    );
int tlv_write_short
    (
    unsigned int instance_id,
    unsigned short type, 
    unsigned short value
    );
int tlv_write_int
    (
    unsigned int instance_id,
    unsigned short type, 
    unsigned int value
    );
int tlv_write_buffer
    (
    unsigned int instance_id,
    unsigned short type , 
    int max_len , 
    int len , 
    unsigned char *value
    );
int tlv_type_len
    (
    unsigned int instance_id,
    unsigned short type
    );
#if 0
void tlv_dump
    (
    unsigned int instance_id
    );
#endif
int tlv_write_flag
    (
    unsigned int instance_id,
    unsigned char value
    );
int tlv_read_flag
    (
    unsigned int instance_id,
    unsigned char *value
    );
int tlv_write_active
    (
    unsigned int instance_id,
    unsigned char value
    );
int tlv_write_version
    (
    unsigned int instance_id,
    unsigned int value
    );

#endif

