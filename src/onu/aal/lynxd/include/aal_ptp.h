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
 
DEFINITIONS:  "DEVICE" means the Cortina Systems LynxD product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems software.
 
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
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING CS_OUT
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

    This file is used for PTP driver code
    
*/


#ifndef __AAL_PTP_H__
#define __AAL_PTP_H__


#include "cs_types.h"



typedef enum {
    AAL_PTP_CORRECT_DIR_SUB    = 0,
    AAL_PTP_CORRECT_DIR_ADD    = 1
} cs_aal_ptp_correct_dir_t;


/* ptp pon configuration table */
typedef struct {
    cs_boolean                 tx_en;            /* enable(1)/disable(0) PTP TX function in PON port*/
    cs_boolean                 rx_en;            /* enable(1)/disable(0) PTP RX function in PON port*/
    cs_boolean                 egress_bswap_en;  /* enable(1)/disable(0)Egress PTP process packet do byte swap */
    cs_boolean                 ingress_bswap_en; /* enable(1)/disable(0)Ingress PTP process packet do byte swap  */
    cs_uint8                   packet_cos;       /* enable(1)/disable(0)Ingress PTP process packet COS */
    cs_boolean                 tx_crc_recal_en;  /* enable(1)/disable(0)re-calculate crc for tx ptp pkt*/
    cs_uint16                  process_en;       /* process_en[0] == 1: PTP packet with messageType == 0 need go through PTP process flow
                                                    .. .. .. .
                                                    process_en[15] ==1: PTP packet with messageType == 15 need go through PTP process flow. */
    cs_aal_ptp_correct_dir_t   correct_dir ;     /* correction direction:add(1) or subtract(0)*/
    cs_uint16                  correct_delta;    /* The asymmetric path delay correction absolute value*/
    cs_uint8                   packet_dpid;

}cs_aal_ptp_pon_cfg_t;

typedef union {
    struct {
        cs_uint32 tx_en            : 1;
        cs_uint32 rx_en            : 1;
        cs_uint32 egress_bswap_en  : 1;
        cs_uint32 ingress_bswap_en : 1;
        cs_uint32 packet_cos       : 1;
        cs_uint32 tx_crc_recal_en  : 1;
        cs_uint32 process_en       : 1;
        cs_uint32 correct_dir      : 1;
        cs_uint32 correct_delta    : 1;
        cs_uint32 packet_dpid      : 1;
        cs_uint32 rsvd             : 22;
    } s;
    cs_uint32 u32;
}cs_aal_ptp_pon_msk_t;

/* ptp ge configuration table */
typedef struct {
    cs_boolean                 tx_en;            /* enable(1)/disable(0) PTP TX function in PON port*/
    cs_boolean                 rx_en;            /* enable(1)/disable(0) PTP RX function in PON port*/
    cs_boolean                 egress_bswap_en;  /* enable(1)/disable(0)Egress PTP process packet do byte swap */
    cs_boolean                 ingress_bswap_en; /* enable(1)/disable(0)Ingress PTP process packet do byte swap  */
    cs_uint8                   packet_cos;       /* enable(1)/disable(0)Ingress PTP process packet COS */
    cs_uint16                  process_en;       /* process_en[0] == 1: PTP packet with messageType == 0 need go through PTP process flow
                                                    .. .. .. .
                                                    process_en[15] ==1: PTP packet with messageType == 15 need go through PTP process flow. */
    cs_aal_ptp_correct_dir_t   correct_dir ;     /* correction direction:add(1) or subtract(0)*/
    cs_uint16                  correct_delta;    /* The asymmetric path delay correction absolute value*/
    cs_boolean                 tx_update_crc;
    cs_boolean                 pass_thru_hdr;
    cs_uint16                  txfifo_thrshld;
    cs_uint8                   ipg_sel;

}cs_aal_ptp_ge_cfg_t;

typedef union {
    struct {
        cs_uint32 tx_en            : 1;
        cs_uint32 rx_en            : 1;
        cs_uint32 egress_bswap_en  : 1;
        cs_uint32 ingress_bswap_en : 1;
        cs_uint32 packet_cos       : 1;
        cs_uint32 process_en       : 1;
        cs_uint32 correct_dir      : 1;
        cs_uint32 correct_delta    : 1;
        cs_uint32 tx_update_crc    : 1;
        cs_uint32 pass_thru_hdr    : 1;
        cs_uint32 txfifo_thrshld   : 1;
        cs_uint32 ipg_sel          : 1;
        cs_uint32 rsvd             : 20;
    } s;
    cs_uint32 u32;
}cs_aal_ptp_ge_msk_t;

/* ptp ma configuration table */
typedef struct {
    cs_boolean                 en;               /* enable(1)/disable(0) function in MA port*/
    cs_boolean                 egress_bswap_en;  /* enable(1)/disable(0)Egress PTP process packet do byte swap */
    cs_boolean                 ingress_bswap_en; /* enable(1)/disable(0)Ingress PTP process packet do byte swap  */
    cs_boolean                 es_en;            /* enable(1)/disable(0)cpu receive the packet from ES module */
    cs_boolean                 lookup_bypass_en; /* 1 - Bypass look-up; 0 - Look-up enable */
    cs_uint8                   mir_cos;
    cs_uint8                   mir_dpid;
    cs_uint8                   cpu_cos;
    cs_uint8                   cpu_dpid;
    cs_uint8                   dma_cos;
    cs_uint8                   dma_dpid;
    cs_uint8                   mii0_cos;
    cs_uint8                   mii0_dpid;
}cs_aal_ptp_ma_cfg_t;

typedef union {
    struct {
        cs_uint32 en               : 1;
        cs_uint32 egress_bswap_en  : 1;
        cs_uint32 ingress_bswap_en : 1;
        cs_uint32 es_en            : 1;
        cs_uint32 lookup_bypass_en : 1;
        cs_uint32 mir_cos          : 1;
        cs_uint32 mir_dpid         : 1;
        cs_uint32 cpu_cos          : 1;
        cs_uint32 cpu_dpid         : 1;
        cs_uint32 dma_cos          : 1;
        cs_uint32 dma_dpid         : 1; 
        cs_uint32 mii0_cos         : 1;
        cs_uint32 mii0_dpid        : 1;
        cs_uint32 rsvd             : 19;
    } s;
    cs_uint32 u32;
}cs_aal_ptp_ma_msk_t;


/*ptp globle configuration table*/
typedef struct {
    cs_uint32  ref_timer ;     /* the EPON local timer reference value and should be > 100nsec + EPON local timer */
    cs_uint32  set_timestamp;  /* the value that needs to be written into PTP free-running timer*/
}cs_aal_ptp_glb_cfg_t;

typedef union {
    struct {
        cs_uint32 ref_timer     : 1;
        cs_uint32 set_timestamp : 1;
        cs_uint32 rsvd          : 30;
    } s;
    cs_uint32 u32;
}cs_aal_ptp_glb_msk_t;

/*ptp globle status */
typedef struct {
    cs_uint32   get_timestamp ;       /*the latched PTP timer value  */
    cs_uint32   get_mpcp_timestamp;   /*the latched MPCP local timer */                                  
    cs_uint32   get_timestamp_cmd ;   /*0:HW latches PTP timer value;1:HW takes no action.*/
    cs_uint32   set_timestamp_cmd ;   /*0:no action; 1:latch PTP_SET_TIMESTAMP register value into PTP timer*/                                                                    
}cs_aal_ptp_glb_status_t;

/* ptp pon status stable */
typedef struct {
    cs_uint32  ingress_timer_latch;
    cs_uint32  ingress_timer_fwd;
    cs_uint32  egress_timer_latch;
    cs_uint8   cf_offset;          /* PTP process packet CF offset to PTP packet byte 15. */
    cs_uint8   msg_type;           /* PTP process packet messageType */
    cs_boolean valid;              /* Egress PTP process packet has transmitted.*/
}cs_aal_ptp_status_t;

/* TOD config */
typedef struct {
    cs_boolean en;
    cs_uint32  s_pps;       /*Start of 1PPS pulse rise*/
    cs_uint16  pps_width;   /*Width of pps pulse*/
    cs_uint32  s_tod;       /* Start of TOD pulse generate to TOD generate.*/
    cs_uint8   counter;     /*Total number of UART character to slave device.*/
}cs_aal_tod_cfg_t;

typedef union {
    struct {
        cs_uint32 en        : 1;
        cs_uint32 s_pps     : 1;
        cs_uint32 pps_width : 1;
        cs_uint32 s_tod     : 1;
        cs_uint32 counter   : 1;
        cs_uint32 rsvd      : 28;
    } s;
    cs_uint32 u32;
}cs_aal_tod_msk_t;

typedef volatile union 
{
    struct
    {
#ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 set_ts_intI          :  1 ; 
    cs_uint32 get_ts_intI          :  1 ; 
#else
    cs_uint32 get_ts_intI          :  1 ; 
    cs_uint32 set_ts_intI          :  1 ; 
    cs_uint32 rsrvd1               : 30 ;
#endif
    }s;
    cs_uint32 u32;
} cs_aal_ptp_int_msk_t;


cs_status aal_ptp_glb_cfg_set(
    CS_IN cs_aal_ptp_glb_msk_t msk,
    CS_IN cs_aal_ptp_glb_cfg_t *cfg
);

cs_status aal_ptp_glb_cfg_get(
    CS_OUT cs_aal_ptp_glb_cfg_t *cfg
);

cs_status aal_ptp_glb_status_get(
    CS_OUT cs_aal_ptp_glb_status_t *cfg
);


cs_status aal_ptp_pon_cfg_set(
    CS_IN cs_aal_ptp_pon_msk_t       msk,
    CS_IN cs_aal_ptp_pon_cfg_t       *cfg
);
cs_status aal_ptp_pon_cfg_get(
    CS_OUT cs_aal_ptp_pon_cfg_t       *cfg
);

cs_status aal_ptp_ge_cfg_set(
    CS_IN cs_aal_ptp_ge_msk_t       msk,
    CS_IN cs_aal_ptp_ge_cfg_t       *cfg
);
cs_status aal_ptp_ge_cfg_get(
    CS_OUT cs_aal_ptp_ge_cfg_t       *cfg
);
cs_status aal_ptp_ma_cfg_set(
    CS_IN cs_aal_ptp_ma_msk_t       msk,
    CS_IN cs_aal_ptp_ma_cfg_t       *cfg
);
cs_status aal_ptp_ma_cfg_get(
    CS_OUT cs_aal_ptp_ma_cfg_t       *cfg
);

cs_status aal_ptp_pon_status_get(
    CS_OUT cs_aal_ptp_status_t    *cfg
);

cs_status aal_ptp_ge_status_get(
    CS_OUT cs_aal_ptp_status_t    *cfg
);

cs_status ptp_init(void);


cs_status aal_tod_cfg_set(
    CS_IN cs_aal_tod_msk_t msk,
    CS_IN cs_aal_tod_cfg_t *cfg
);

cs_status aal_tod_cfg_get(
    CS_OUT cs_aal_tod_cfg_t *cfg
);

cs_status aal_ptp_int_enable(
    CS_IN  cs_aal_ptp_int_msk_t mask
);

cs_status aal_ptp_int_disable(
    CS_IN  cs_aal_ptp_int_msk_t mask
);

cs_status aal_ptp_int_status_get(
    CS_OUT cs_aal_ptp_int_msk_t* mask
);

cs_status aal_ptp_int_status_clr(
    CS_IN  cs_aal_ptp_int_msk_t src
);

cs_status aal_ptp_int_en_get(
    CS_IN  cs_aal_ptp_int_msk_t *mask
);

#endif /* __AAL_PTP_H__ */

