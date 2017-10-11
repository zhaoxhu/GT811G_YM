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
 * cs_types.h
 *
 * Include file containing the basic data types
 *
 * $Id: cs_types.h,v 1.1.4.2.6.1.2.3.16.6.2.3.2.1.2.13.12.1 2012/05/25 05:56:49 jsheng Exp $
 */

#ifndef _CS_TYPES_H_
#define _CS_TYPES_H_

typedef unsigned long long      cs_uint64;
typedef long long               cs_int64;
typedef unsigned int            cs_uint32;
typedef int                     cs_int32;
typedef unsigned short          cs_uint16;
typedef short                   cs_int16;
typedef unsigned char           cs_uint8;
typedef char                    cs_int8;
typedef unsigned char           cs_boolean;

#if 1
//add by zhuxh
typedef unsigned long           cs_ulong32;
typedef long					cs_long32;
#endif

typedef enum {
    CS_PON_PORT_ID     = 0x00,
    CS_UNI_PORT_ID1    = 0x01,
    CS_UNI_PORT_ID2    = 0x02,
    CS_UNI_PORT_ID3    = 0x03,
    CS_UNI_PORT_ID4    = 0x04,
    CS_DOWNLINK_PORT   = 0x20,
    CS_UPLINK_PORT     = 0x30,
    CS_MGMT_PORT_ID    = 0x4F,
    CS_VOIP_PORT_ID    = 0x50,
    CS_ALL_UNI_PORT_ID = 0xFF,
} cs_port_id_t;


typedef enum {
    CS_E_OK               = 0,
    CS_E_RESOURCE         = 1,
    CS_E_PARAM            = 2,
    CS_E_NOT_FOUND        = 3,
    CS_E_CONFLICT         = 4,
    CS_E_TIMEOUT          = 5,
    CS_E_NOT_SUPPORT      = 6,
    CS_E_FOUND            = 7,
    CS_E_ERROR            = 0xffffffff
} cs_status;


typedef int (*cs_funcptr)(void *arg);

typedef cs_uint16                cs_ipv6_t[8];

enum {
    CS_IPV4,
    CS_IPV6
};

typedef struct {
    cs_uint32   ipver;
    union {
        cs_uint32 ipv4;
        cs_ipv6_t ipv6;
    } addr;
} cs_ip_t;

typedef cs_uint16                cs_dev_id_t;
typedef cs_uint16                cs_llid_t;
typedef struct {
    cs_uint32    bmp[4];
}                                cs_llid_bmp_t;

typedef cs_uint16                cs_sub_id_t;


#define CS_MACADDR_LEN          6

typedef union {
    cs_uint8       addr[CS_MACADDR_LEN];
}__attribute__((packed)) cs_mac_t;


#define CS_OK               CS_E_OK
#define CS_ERROR            CS_E_ERROR

#ifndef TRUE
#define TRUE                       1
#endif

#ifndef FALSE
#define FALSE                  0
#endif

#ifndef NULL
#define NULL                   ((void *) 0)
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef CS_IN
#define CS_IN
#endif

#ifndef CS_OUT
#define CS_OUT
#endif

#ifndef CS_IN_OUT
#define CS_IN_OUT
#endif

#define _CS_INLINE_ __inline__ static

#ifdef __BIG_ENDIAN__

#ifndef ntohl
#define ntohl(x)        (x)
#endif

#ifndef ntohs
#define ntohs(x)        (x)
#endif

#ifndef htonl
#define htonl(x)        (x)
#endif

#ifndef htons
#define htons(x)        (x)
#endif

#ifndef ntohll
#define ntohll(x)       (x)
#endif


#ifndef htonll
#define htonll(x)       (x)
#endif

#elif defined(__LITTLE_ENDIAN__)

#ifndef ntohl
#define ntohl(x)        ((((x) & 0x000000ff) << 24) | \
                            (((x) & 0x0000ff00) <<  8) | \
                            (((x) & 0x00ff0000) >>  8) | \
                            (((x) & 0xff000000) >> 24))
#endif

#ifndef htonl
#define htonl(x)        ((((unsigned long)(x) & 0x000000ff) << 24) | \
                            (((unsigned long)(x) & 0x0000ff00) <<  8) | \
                            (((unsigned long)(x) & 0x00ff0000) >>  8) | \
                            (((unsigned long)(x) & 0xff000000) >> 24))
#endif


#ifndef ntohs
#define ntohs(x)        ((((x) & 0x00ff) << 8) | \
                            (((x) & 0xff00) >> 8))
#endif

#ifndef htons
#define htons(x)        ((((x) & 0x00ff) << 8) | \
                            (((x) & 0xff00) >> 8))

#endif


#ifndef ntohll
#define ntohll(x)        ((((cs_uint64)ntohl(x)) << 32) | \
                                       ntohl((x) >> 32))
#endif


#ifndef htonll
#define htonll(x)        ((((cs_uint64)htonl(x)) << 32) | \
                                 htonl((x) >> 32))
#endif

#else
#error Endianness not defined
#endif



/* range: should assume only two values: 0 or 1 */
typedef unsigned int                    epon_mask_t;
typedef unsigned char                   epon_macaddr_t[CS_MACADDR_LEN];
typedef unsigned char                   *epon_macaddr_pt;

typedef epon_macaddr_t                  epon_onu_device_id_t;
typedef cs_uint32                   epon_olt_device_id_t;

typedef cs_uint32                   epon_onu_version_t;
typedef cs_uint32                   epon_olt_version_t;


#define INVALID_PORT_ID 0xffffffff
#define ONU_LLIDPORT_FOR_API   (-1)
#define ONU_DEVICEID_FOR_API   (-1)

#define EPON_MAX_ONU_PON_PORTS          1
#define EPON_MAX_ONU_LLID_PORTS         (EPON_MAX_ONU_PON_PORTS * EPON_MAX_LLIDS_PER_ONU_PON_PORT)

/* number of LLIDs assigned on an ONU PON port */
#define EPON_MAX_LLIDS_PER_ONU_PON_PORT 1

#define USER_NAME_LEN               32
#define PASSWORD_LEN                32


/* asynchronous API result code */
typedef enum {
    EPON_RESULT_SUCCESS = 0,
    EPON_RESULT_FAIL,
    EPON_RESULT_NO_REQ_RECORD,
    EPON_RESULT_NO_OLT_MAC,
    EPON_RESULT_NO_MEMORY,
    EPON_RESULT_WRONG_EVENT,
    EPON_RESULT_NULL_HANDLER,
    EPON_RESULT_REQ_TIMEOUT,
    EPON_RESULT_NO_ONU_MAC,
    EPON_RESULT_PING_TOO_LONG,
    EPON_RESULT_CONTROL_IF_DOWN,
    EPON_RESULT_INV_PARAM,
    EPON_RESULT_INV_CHIP_VERSION,
    EPON_RESULT_DEVICE_NOT_SUPPORTED
} epon_result_code_t;


typedef enum {
    EPON_RETURN_SUCCESS = 0,
    EPON_RETURN_FAIL,
    EPON_RETURN_REINIT,
    EPON_RETURN_INV_CHIP_VERSION,
    EPON_RETURN_INV_STATE,
    EPON_RETURN_INV_MAC,
    EPON_RETURN_INV_PARAM,
    EPON_RETURN_NOT_SUPPORT,
    EPON_RETURN_NOT_FOUND,
    EPON_RETURN_NO_RESOURCE
} epon_return_code_t;


/*default CPU pkt priority */
#define ONU_CPU_PKT_PRIO        7
#define ONU_CPU_PKT_PRIO_6      6

/* boolean values */
#define EPON_TRUE                       1
#define EPON_FALSE                      0

#endif /* _CS_TYPES_H_ */
