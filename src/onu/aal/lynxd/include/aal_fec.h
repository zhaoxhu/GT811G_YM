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

    This file is used for FEC driver code
    
*/


#ifndef __AAL_FEC_H__
#define __AAL_FEC_H__

#include "cs_types.h"


typedef enum {
    AAL_FEC_STD_2003             = 0, /* FEC 2003 standard*/
    AAL_FEC_STD_2005             = 1  /* FEC 2005 standard*/    
} cs_aal_fec_std_t;

typedef enum {
    AAL_FEC_REP_DISABLE          = 0, /*EC RX replace mode enable*/
    AAL_FEC_REP_ENABLE           = 1  /*EC RX replace mode disable*/
} cs_aal_fec_rep_t;

typedef enum {
    AAL_FEC_PREAMBLE_INCLUDE     = 0, /*Include /S/ parity to caculate parity*/
    AAL_FEC_PREAMBLE_EXCLUDE     = 1  /*Exclude /S/ parity to caculate parity*/
} cs_aal_fec_preamble_mode_t;

/*
=========================================================================
                      start of debug data structures
=========================================================================
*/
typedef enum {
    AAL_FEC_COMP_MODE_0          = 0, /* no RTT compensation*/                                   
    AAL_FEC_COMP_MODE_1          = 1, /* RTT compensation without AES function, without path switching */                                   
    AAL_FEC_COMP_MODE_2          = 2, /* RTT compensation scheme with AES function and auto path switching */                                   
    AAL_FEC_COMP_MODE_3          = 3  /* no RTT compensation mode */                                   
} cs_aal_fec_comp_mode_t;

typedef enum {
    AAL_FEC_COMP_SUB             = 0, /* Compensation counter is adjust by adding the delta */
    AAL_FEC_COMP_ADD             = 1  /* Compensation counter is adjust by subtracting the delta */
} cs_aal_fec_comp_dir_t;

typedef enum {
    AAL_FEC_ENCODE_STANDARD      = 0, /* Standard FEC encoding */
    AAL_FEC_ENCODE_CORTINA       = 1  /* Always keep 8-byte preamble */
} cs_aal_fec_encode_mode_t;

typedef enum {
     AAL_FEC_ERR_SEND_DATA       = 0, /* send FEC data to RX MAC if uncorrectalbe FEC errors are detected */
     AAL_FEC_ERR_SEND_CODE       = 1  /* send error propagation codeto RX MAC if uncorrectalbe FEC errors are detected */
} cs_aal_fec_err_send_mode_t;

typedef enum {
    AAL_FEC_CNT_READ_KEEP        = 0, /* do_nothing */                                   
    AAL_FEC_CNT_READ_CLEAR_ALL   = 1, /* clear_all */
    AAL_FEC_CNT_READ_CLEAR_MSB   = 2  /* clear_msb */
} cs_aal_fec_cnt_rc_mode_t;

typedef struct {
    cs_boolean                 rx_path_clr;         /* (default=0)enable(1)/disable(0) Clear FEC RX Path*/
    cs_boolean                 tx_path_clr;         /* (default=0)enable(1)/disable(0) Clear FEC TX Path*/
    cs_boolean                 correct_cnt_clr;     /* (default=0)enable(1)/disable(0) reset FEC corrected counter*/
    cs_boolean                 uncorrect_cnt_clr;   /* (default=0)enable(1)/disable(0) reset FEC uncorrected counter*/
    cs_boolean                 stop_correct_cnt;    /* (default=0)enable(1)/disable(0) stop counting FEC corrected block counter*/
    cs_boolean                 stop_uncorrect_cnt;  /* (default=0)enable(1)/disable(0) stop counting FEC uncorrected block counter*/
    cs_aal_fec_cnt_rc_mode_t   cnt_rd_clr_mode;     /* (default=0)FEC counter read_clear mode*/ 
    cs_boolean                 rx_status_clr;       /* (default=0)enable(1)/disable(0) reset FEC RX status register*/
    cs_boolean                 rx_protection_window;/* (default=1)enable(1)/disable(0) FEC boundary protection window*/
    cs_uint8                   err_threshold;       /* (default=3)Threshold of FEC boundary errors in detecting S_FEC and T_FEC*/
    cs_aal_fec_encode_mode_t   encode_mode;         /* (default=0)*/
    cs_uint8                   tx_ipg_adjust;       /* (default=4)debug only;to adjust when to generate fec_ready signal back to TxMAC*/  
    cs_aal_fec_err_send_mode_t err_send_mode;       /* (default=0)debug only*/                                                                                       
}cs_aal_fec_ext_cfg_t;

typedef union {
    struct {
        cs_uint32  rx_path_clr          : 1;
        cs_uint32  tx_path_clr          : 1;
        cs_uint32  correct_cnt_clr      : 1;
        cs_uint32  uncorrect_cnt_clr    : 1;
        cs_uint32  stop_correct_cnt     : 1;
        cs_uint32  stop_uncorrect_cnt   : 1;
        cs_uint32  cnt_rd_clr_mode      : 1;
        cs_uint32  rx_status_clr        : 1;
        cs_uint32  rx_protection_window : 1;
        cs_uint32  err_threshold        : 1;
        cs_uint32  encode_mode          : 1;
        cs_uint32  tx_ipg_adjust        : 1;
        cs_uint32  err_send_mode        : 1;
        cs_uint32  rsvd                 : 19;
    } s;
    cs_uint32 u32;
}cs_aal_fec_ext_msk_t;


typedef struct{
    cs_boolean en;          /*enable(1)/disable(0) inject error*/
    cs_boolean burst_en;    /*1:burst mode;0:HW inserts error for all output packets*/  
    cs_uint8   packet_cnt;  /*Number of error packets in one burst mode operation.*/   
    cs_uint8   byte_cnt;    /*Number of error bytes to be inserted in each specified fields(tfec2/parity/tfec1/data/sfec).*/  
    cs_uint8   bit_cnt;     /*Number of error bits to be inserted in each error byte. */ 
    cs_boolean tfec1_en;    /*enable(1)/disable(0) insert error in TFEC1/T field of packet*/  
    cs_boolean tfec2_en;    /*enable(1)/disable(0) insert error in TFEC2 field of packet*/  
    cs_boolean data_en;     /*enable(1)/disable(0) insert error in data field of packet */  
    cs_boolean sfec_en;     /*enable(1)/disable(0) insert error in S_FEC/S field of packet*/
    cs_boolean parity_en;   /*enable(1)/disable(0) insert error in parity field of packet*/
}cs_aal_fec_err_inj_cfg_t;

typedef union {
    struct{
        cs_uint32 en         : 1;  
        cs_uint32 burst_en   : 1;   
        cs_uint32 packet_cnt : 1;
        cs_uint32 byte_cnt   : 1;
        cs_uint32 bit_cnt    : 1;
        cs_uint32 tfec1_en   : 1;
        cs_uint32 tfec2_en   : 1;
        cs_uint32 data_en    : 1;
        cs_uint32 sfec_en    : 1;
        cs_uint32 parity_en  : 1;
        cs_uint32 rsvd       : 22;
    } s;
    cs_uint32 u32;
}cs_aal_fec_err_inj_msk_t;

typedef struct {
    cs_boolean              en;                /*enable(1)/disable(0)compensation;*/
    cs_aal_fec_comp_mode_t  mode;              /*compensation mode;should be set before ONU starts registration process.*/
    cs_aal_fec_comp_dir_t   direction;         /*compensation direction*/
    cs_uint16               counter;           /*Fec compensation with this counter*/               
    cs_uint8                egress;            /*compensation value for egress path,unit is 8ns.valid only When mode is 2*/
    cs_uint16               ingress;           /*compensation value for RxMAC, unit is 8ns;valid only When mode is 2*/
    cs_uint8                timestamp;         /*compensation value for timestamp insertion in TxMAC,unit is 8ns;valid only When mode is 2*/
    cs_uint8                aes_rx_path;       /*rxfec path, aes on; unit is 8ns*/
    cs_uint8                churn_rx_path;     /*rxfec path, churning on; unit is 8ns*/
    cs_uint8                aes_non_rx_path;   /*non-rxfec path, aes on; unit is 8ns*/
    cs_uint8                churn_non_rx_path; /*non-rxfec path, churning on; unit is 8ns*/
}cs_aal_fec_comp_cfg_t;

typedef union{
    struct {
        cs_uint32  en                   :1;
        cs_uint32  mode                 :1;
        cs_uint32  direction            :1;
        cs_uint32  counter              :1;
        cs_uint32  egress               :1;
        cs_uint32  ingress              :1;
        cs_uint32  timestamp            :1;
        cs_uint32  aes_rx_path          :1;
        cs_uint32  churn_rx_path        :1;
        cs_uint32  aes_non_rx_path      :1;
        cs_uint32  churn_non_rx_path    :1;
        cs_uint32  rsvd                 :21;
    }s;
    cs_uint32 u32;
}cs_aal_fec_comp_msk_t;


typedef struct {
    cs_uint16 min; /*minimum packet size allowed in rxFEC, including 8B preamble.Unit is byte.*/
    cs_uint16 max; /*maximum packet size allowed in rxFEC, including 8B preamble.Unit is byte.*/
}cs_aal_fec_pktsize_cfg_t;

typedef union{
    struct {
        cs_uint32 min        :1;
        cs_uint32 max        :1;
        cs_uint32 rsvd       :30;
    }s;
    cs_uint32 u32;        
}cs_aal_fec_pktsize_msk_t;


typedef struct {
    cs_uint16 decode; /* fec decoding latency; Unit is byte; not change its value in normal condition.*/
    cs_uint16 max_pkt;/* maximum packet latency; unit is byte, set to 1547/0x60B for RTT compensation */
}cs_aal_fec_latency_cfg_t;

typedef union{
    struct {
        cs_uint32 decode         :1;
        cs_uint32 max_pkt        :1;
        cs_uint32 rsvd           :30;
    }s;
    cs_uint32 u32;
}cs_aal_fec_latency_msk_t;

typedef struct {
    cs_uint8 rx_data_corr_ack;    /*FEC Rx data correction is done or not;    */
    cs_uint8 parity_buffer_full;  /*Parity buffer is full.                    */
    cs_uint8 cmd_buffer_full;     /*Command queue buffer is full.             */
    cs_uint8 code_buffer_full;    /*Code buffer is full.                      */
    cs_uint8 pkt_buffer_full;     /*Packet buffer is full.                    */
    cs_uint8 pipe_stuck;          /*Pipe is stalled.                          */
    cs_uint8 los_tfec1_boundary;  /*Lost FEC boundary T_FEC_E or T_FEC_O or 
                                    End_of_packet (this is for the T_FEC between
                                    packet field and data field).             */
    cs_uint8 los_tfec2_boundary;  /*Lost FEC boundary T_FEC_E (this is for the 
                                    last T_FEC of FEC packet).                */
    cs_uint8 uncorrected;         /*Uncorrectable error is detected*/
    cs_uint8 errors;              /*When fec_uncorr == 0:
                                      0000: No error is detected
                                      0001: 1 error is corrected
                                      0010: 2 errors are corrected
                                      0011: 3 errors are corrected
                                      0100: 4 errors are corrected
                                      0101: 5 errors are corrected
                                      0110: 6 errors are corrected
                                      0111: 7 errors are corrected
                                      1000: 8 errors are corrected
                                      When fec_uncor == 1:
                                      Xxxx: Errors are uncorrectable.         */
    cs_uint8 pkt_state;           /*Non-FEC packet State:
                                      0000: Idle
                                      0001: /S/
                                      0010: DATA
                                      0011: /T/
                                      0100: /R/
                                      0101: /R/
                                      FEC packet State:
                                      1000: Idle
                                      1001: S_FEC
                                      1010: DATA
                                      1011: T_FEC_E / T_FEC_O
                                      1100: PARITY
                                      1101: T_FEC_E*/
} cs_aal_fec_status_t;
/*
=========================================================================
                      End  of debug data structures
=========================================================================
*/




/* FEC configuration */
typedef struct {
    cs_aal_fec_std_t           std_mode;        /*(default=1)*/
    cs_aal_fec_rep_t           rx_rep_mode;     /*(default=1)*/ 
    cs_aal_fec_preamble_mode_t rx_preamble_mode;/*(default=1)*/
    cs_boolean                 rx_bypass;       /*(default=0):enable(1)/disable(0) bypass RX FEC datapath */
    cs_boolean                 rx_en;           /*(default=0):enable(1)/disable(0) FEC RX decoding*/
    cs_aal_fec_rep_t           tx_rep_mode;     /*(default=1)*/
    cs_aal_fec_preamble_mode_t tx_preamble_mode;/*(default=1)*/
    cs_boolean                 tx_en;           /*(default=0):enable(1)/disable(0) FEC TX encoding*/
}cs_aal_fec_cfg_t;


typedef union {
    struct {
        cs_uint32  std_mode                :1;        
        cs_uint32  rx_rep_mode             :1;
        cs_uint32  rx_preamble_mode        :1;
        cs_uint32  rx_bypass               :1;
        cs_uint32  rx_en                   :1;
        cs_uint32  tx_rep_mode             :1;
        cs_uint32  tx_preamble_mode        :1;
        cs_uint32  tx_en                   :1;
        cs_uint32  rsvd                    :24;
    } s;
    cs_uint32 u32;
}cs_aal_fec_msk_t;


/* FEC Counter*/
typedef struct {
    cs_uint32 rx_blks;            /* Indicate the RX FEC blocks received      */
    cs_uint32 rx_correct_bytes;   /* rx error bytes and corrected             */
    cs_uint32 rx_correct_blks;    /* rx error blocks and corrected            */
    cs_uint32 rx_uncorrect_blks;  /* rx error blocks and unable be corrected  */
    cs_uint32 rx_good_blks;       /* rx blocks with no error                  */
    cs_uint32 rx_sfec;            /* rx SFEC Counter                          */
    cs_uint32 rx_tfec;            /* rx TFEC Counter                          */
    cs_uint32 rx_spkt;            /* rx SPKT Counter                          */ 
    cs_uint32 rx_epkt;            /* rx EPKT Counte                           */
}cs_aal_fec_cnt_t;

/*
===============================================================================
                      Start of debug functions
===============================================================================
*/

/**************************************************************************/
/* $rtn_hdr_start Set FEC Overhead                                        */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   CS8030                                                  */
cs_status aal_fec_overhead_set(
    CS_IN cs_uint8 overhead
);
/* INPUTS     : o-overhead                                                */
/*                                                                        */
/* OUTPUTS    : NA                                                        */
/*                                                                        */ 
/* RETURNS    :                                                           */
/* DESCRIPTION:                                                           */
/*    Set FEC Overhead                                                    */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start Set FEC Overhead                                        */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_fec_overhead_get(
    CS_OUT cs_uint8 *overhead
);
/* INPUTS     :                                                           */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    :                                                           */
/* DESCRIPTION:                                                           */
/*    Set FEC Overhead                                                    */
/* $rtn_hdr_end                                                           */
/**************************************************************************/



/**************************************************************************/
/* $rtn_hdr_start  Set FEC Ext Configuration                              */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */

cs_status aal_fec_ext_cfg_set(
    CS_IN cs_aal_fec_ext_msk_t msk,
    CS_IN cs_aal_fec_ext_cfg_t *cfg
);
/* INPUTS     :                                                           */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    : CS_E_OK or CS_E_PARA if parameters are wrong              */
/* DESCRIPTION:                                                           */
/*    Set FEC Ext Configuration                                           */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start  Get FEC Ext Configuration                              */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */

cs_status aal_fec_ext_cfg_get(
    CS_OUT cs_aal_fec_ext_cfg_t *cfg
);
/* INPUTS     :   cnt- pointer to the structure fec_ext_cfg_t             */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    : CS_E_OK or CS_E_PARA if parameters are wrong              */
/* DESCRIPTION:                                                           */
/*    Get FEC Ext Configuration                                           */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start Set FEC ERROR INJECTION                                 */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_fec_err_inj_cfg_set(
    CS_IN cs_aal_fec_err_inj_msk_t msk, 
    CS_IN cs_aal_fec_err_inj_cfg_t *cfg
);
/* INPUTS     :                                                           */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    :                                                           */
/* DESCRIPTION:                                                           */
/*    Set FEC ERROR INJECTION                                             */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start Get FEC ERROR INJECTION CONFIG                          */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_fec_err_inj_cfg_get(
    CS_OUT cs_aal_fec_err_inj_cfg_t *cfg
);
/* INPUTS     :                                                           */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    :                                                           */
/* DESCRIPTION:                                                           */
/*    Get FEC ERROR INJECTION CONFIG                                      */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start Set FEC PKT SIZE                                        */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */

cs_status aal_fec_pktsize_cfg_set(
    CS_IN cs_aal_fec_pktsize_msk_t msk, 
    CS_IN cs_aal_fec_pktsize_cfg_t *cfg
);
/* INPUTS     :                                                           */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    :                                                           */
/* DESCRIPTION:                                                           */
/*    Set FEC PKT SIZE                                                    */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start Get FEC PKT SIZE                                        */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_fec_pktsize_cfg_get(
    CS_OUT cs_aal_fec_pktsize_cfg_t *cfg
);
/* INPUTS     :                                                           */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    :                                                           */
/* DESCRIPTION:                                                           */
/*    Get FEC PKT SIZE                                                    */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start Set FEC LATENCY                                         */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_fec_latency_cfg_set(
    CS_IN cs_aal_fec_latency_msk_t msk, 
    CS_IN cs_aal_fec_latency_cfg_t *cfg
);
/* INPUTS     :                                                           */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    :                                                           */
/* DESCRIPTION:                                                           */
/*    Set FEC LATENCY                                                     */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start Get FEC LATENCY                                         */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_fec_latency_cfg_get(
    CS_IN cs_aal_fec_latency_cfg_t *cfg
);
/* INPUTS     :                                                           */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    :                                                           */
/* DESCRIPTION:                                                           */
/*    Get FEC LATENCY                                                     */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start Set FEC Compensation                                    */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_fec_comp_cfg_set(
    CS_IN cs_aal_fec_comp_msk_t msk, 
    CS_IN cs_aal_fec_comp_cfg_t *cfg
);
/* INPUTS     :                                                           */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    :                                                           */
/* DESCRIPTION:                                                           */
/*    Set FEC Compensation                                                */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start Set FEC Compensation                                    */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_fec_comp_cfg_get(
    CS_OUT cs_aal_fec_comp_cfg_t *cfg
);
/* INPUTS     :                                                           */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    :                                                           */
/* DESCRIPTION:                                                           */
/*    Get FEC Compensation Configuration                                  */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start  Get FEC status                                         */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_fec_status_get(
    CS_OUT cs_aal_fec_status_t *status
);
/* INPUTS     : status - pointer to the structure fec_status_t            */
/*                                                                        */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    : CS_E_OK or CS_E_PARA if parameters are wrong              */
/* DESCRIPTION:                                                           */
/*    Get FEC status                                                      */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/*
============================================================================
                      End of debug functions
============================================================================
*/


/**************************************************************************/
/* $rtn_hdr_start  FEC Module Initiation                                  */
/* CATEGORY   :    Device                                                 */
/* ACCESS     :    Public                                                 */
/* BLOCK      :    AAL                                                    */
/* CHIP       :    LynxD                                                  */
cs_status fec_init(void);
/* INPUTS     :                                                           */
/*                               */
/* OUTPUTS    :                                                           */
/*    o---                                                                */ 
/* RETURNS    : CS_E_OK or CS_E_PARA if parameters are wrong              */
/* DESCRIPTION:                                                           */
/*    FEC Module Initiation                                               */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Set FEC General Configuration                              */
/* CATEGORY   :    Device                                                     */
/* ACCESS     :    Public                                                     */
/* BLOCK      :    FEC                                                        */
/* CHIP       :    CS8030                                                     */
cs_status aal_fec_cfg_set(
    CS_IN cs_aal_fec_msk_t msk,
    CS_IN cs_aal_fec_cfg_t *cfg
);
/* INPUTS     : o  msk - Parameter mask                                       */
/*              o  cfg - FEC general configuration                            */
/* OUTPUTS    :    None                                                       */
/* RETURNS    : o  CS_OK: Set successfully                                    */
/*              o  CS_ERROR: Set failed                                       */
/* DESCRIPTION:                                                               */
/*    Set FEC General Configuration, including following information          */
/*    cs_aal_fec_std_t      std_mode;      (default=1)                        */
/*                                              1: FEC 2005 standard;         */
/*                                              0: FEC 2003 standard;         */
/*    cs_aal_fec_rep_t      rx_rep_mode;    (default=1)                       */
/*                                              1: FEC RX replace mode enable;*/ 
/*                                              0:FEC RX non-replace mode     */ 
/*    cs_aal_fec_preamble_mode_t rx_preamble_mode;(default=1);                */ 
/*                                              1: Exclude /S/ parity cal(rx);*/ 
/*                                              0: Include /S/ parity cal(rx);*/
/*    cs_boolean                 rx_bypass;     (default=0):                  */ 
/*                                              1: bypass RX FEC;             */
/*                                              0: not bypass RX FEC;         */
/*    cs_boolean                 rx_en;     (default=0);                      */ 
/*                                              1:FEC RX function Enable;     */ 
/*                                              0= FEC RX function Disable;   */
/*    cs_aal_fec_rep_t      tx_rep_mode;   (default=1);                  */ 
/*                                              1:FEC TX replace mode enable; */ 
/*                                              0:FEC TX non-replace mode;    */
/*    cs_aal_fec_preamble_mode_t tx_preamble_mode;(default=1);                */
/*                                              1: Exclude /S/ parity cal(tx);*/ 
/*                                              0: Include /S/ parity cal(tx);*/
/*    cs_boolean                 tx_en;     (default=0);                      */ 
/*                                              1: Enable FEC TX Encoding;    */
/*                                              0: Disable FEC TX Encoding    */
/* }cs_aal_fec_cfg_t;                                                         */
/* $rtn_hdr_end                                                               */
/******************************************************************************/



/******************************************************************************/
/* $rtn_hdr_start  Get FEC General Configuration                              */
/* CATEGORY   :    Device                                                     */
/* ACCESS     :    Public                                                     */
/* BLOCK      :    FEC                                                        */
/* CHIP       :    CS8030                                                     */
cs_status aal_fec_cfg_get(
    CS_OUT cs_aal_fec_cfg_t *cfg
);
/* INPUTS     : o  none                                                       */
/* OUTPUTS    : o  cfg - FEC general configuration                            */
/* RETURNS    : o  CS_OK: Get successfully                                    */
/*              o  CS_ERROR: Get failed                                       */
/* DESCRIPTION:                                                               */
/*    Set FEC General Configuration, including following information          */
/*    cs_aal_fec_std_t      std_mode;      (default=1)                        */
/*                                              1: FEC 2005 standard;         */
/*                                              0: FEC 2003 standard;         */
/*    cs_aal_fec_rep_t      rx_rep_mode;    (default=1)                       */
/*                                              1: FEC RX replace mode enable;*/ 
/*                                              0:FEC RX non-replace mode     */ 
/*    cs_aal_fec_preamble_mode_t rx_preamble_mode;(default=1);                */ 
/*                                              1: Exclude /S/ parity cal(rx);*/ 
/*                                              0: Include /S/ parity cal(rx);*/
/*    cs_boolean                 rx_bypass;     (default=0):                  */ 
/*                                              1: bypass RX FEC;             */
/*                                              0: not bypass RX FEC;         */
/*    cs_boolean                 rx_en;     (default=0);                      */ 
/*                                              1:FEC RX function Enable;     */ 
/*                                              0= FEC RX function Disable;   */
/*    cs_aal_fec_rep_t      tx_rep_mode;   (default=1);                       */ 
/*                                              1:FEC TX replace mode enable; */ 
/*                                              0:FEC TX non-replace mode;    */
/*    cs_aal_fec_preamble_mode_t tx_preamble_mode;(default=1);                */
/*                                              1: Exclude /S/ parity cal(tx);*/ 
/*                                              0: Include /S/ parity cal(tx);*/
/*    cs_boolean                 tx_en;     (default=0);                      */ 
/*                                              1: Enable FEC TX Encoding;    */
/*                                              0: Disable FEC TX Encoding    */
/* }cs_aal_fec_cfg_t;                                                         */
/* $rtn_hdr_end                                                               */
/******************************************************************************/



/******************************************************************************/
/* $rtn_hdr_start  Get FEC Statistics counters                                */
/* CATEGORY   :    Device                                                     */
/* ACCESS     :    Public                                                     */
/* BLOCK      :    FEC                                                        */
/* CHIP       :    CS8030                                                     */
cs_status aal_fec_cnt_get(
    CS_OUT cs_aal_fec_cnt_t *cnt
);
/* INPUTS     : ----                                                          */
/* OUTPUTS    : o  cfg - FEC statistics counters                              */
/* RETURNS    : o  CS_OK: Get successfully                                    */
/*              o  CS_ERROR: Get failed                                       */
/* DESCRIPTION:                                                               */
/*    Get FEC statistics counters, including following counters defined in    */
/* following structure  */
/*typedef struct { */
/*    cs_uint32 rx_blks;               // Indicate the RX FEC blocks received */
/*    cs_uint32 rx_correct_bytes;      // rx error bytes and corrected by FEC */
/*    cs_uint32 rx_correct_blks;       // rx error blocks and corrected by FEC*/
/*    cs_uint32 rx_uncorrect_blks;     // rx error blocks and unable to be    */
/*                                         corrected by FEC                   */
/*    cs_uint32 rx_good_blks;          // rx blocks with no error             */
/*    cs_uint32 rx_sfec;               // rx SFEC Counter                     */
/*    cs_uint32 rx_tfec;               // rx TFEC Counter                     */
/*    cs_uint32 rx_spkt;               // rx SPKT Counter                     */
/*    cs_uint32 rx_epkt;               // rx EPKT Counter                     */
/*}cs_aal_fec_cnt_t;                                                          */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


#endif /* __AAL_FEC_H__ */

