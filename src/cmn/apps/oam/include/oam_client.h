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

DEFINITIONS:  "DEVICE" means the Cortina Systems? Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems? SDK software.

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

Copyright (c) 2011 by Cortina Systems Incorporated
****************************************************************************/

#ifndef __OAM_STD_CLIENT_H__
#define __OAM_STD_CLIENT_H__
#include "oam_core.h"
/*oam mib var defines */
#define OAM_PASSAVE_BRANCH                      0x87

#define OAM_PASSAVE_PON_REG_REQ_TX              0x1f
#define OAM_PASSAVE_PON_REG_ACK_TX              0x20
#define OAM_PASSAVE_PON_REG_RX                  0x21
#define OAM_PASSAVE_OAM_TX                      0x22
#define OAM_PASSAVE_OAM_RX                      0x23
#define OAM_PASSAVE_GATE_RX                     0x24
#define OAM_PASSAVE_REPORT_TX                   0x25
#define OAM_PASSAVE_PON_BYT_TX                  0x26
#define OAM_PASSAVE_PON_BYT_RX                  0x27
#define OAM_PASSAVE_PON_ERR                     0x30
#define OAM_PASSAVE_REG_REQ_TX                  0x31
#define OAM_PASSAVE_REG_ACK_TX                  0x32
#define OAM_PASSAVE_REG_RX                      0x33
#define OAM_PASSAVE_RPT_RX                      0x37
#define OAM_PASSAVE_NOTSUP_RX                   0x38
#define OAM_PASSAVE_OAMINF_TX                   0x73
#define OAM_PASSAVE_OAMINF_RX                   0x74
#define OAM_PASSAVE_OAMEVT_RX                   0x75
#define OAM_PASSAVE_OAMUEVT_RX                  0x76
#define OAM_PASSAVE_OAMDEVT_RX                  0x77
#define OAM_PASSAVE_OAMLBC_TX                   0x78
#define OAM_PASSAVE_OAMLBC_RX                   0x79
#define OAM_PASSAVE_OAMVR_TX                    0x80
#define OAM_PASSAVE_OAMVR_RX                    0x81
#define OAM_PASSAVE_OAMVRSP_TX                  0x82
#define OAM_PASSAVE_OAMVRSP_RX                  0x83
#define OAM_PASSAVE_OAMOS_TX                    0x84
#define OAM_PASSAVE_OAMOS_RX                    0x85
#define OAM_PASSAVE_OAMLST                      0x98


#define OAM_MIB_VARIABLE_NOT_SUPORT   0x21
#define OAM_MIB_VARIABLE_INDICATION_MASK  0x80

#define MIBVARSET(a)  var = htonl(a);\
                      pD2Cont->width = sizeof(unsigned long);\
                      memcpy(pD2Cont->value, &var, pD2Cont->width);

#define STDMIBVARSET(a)  var = htonl(a);\
                      pCont->width = sizeof(unsigned long);\
                      memcpy((cs_uint8*)(pCont)+4, &var, pCont->width);

#define IFMIBVARSET(a) if (intf) \
                                var = htonl(a);\
                       else \
                                var = 0; \
                       pD2Cont->width = sizeof(unsigned long); \
                       memcpy(pD2Cont->value, &var, pD2Cont->width);



void oam_ind_oami(
        cs_llid_t llid, 
        oam_pdu_hdr_t *pdu, 
        cs_int32 len);

cs_int32 oam_build_pdu_info(
        oam_if_t *intf, 
        void *parm,
        oam_pdu_info_t *pdu);

void oam_build_pdu_hdr(
        oam_if_t *intf, 
        oam_pdu_hdr_t *pdu, 
        cs_uint8 code);

cs_int32 oam_proc_var_desc(
        oam_var_desc_t *curr, 
        cs_int32 *max, 
        oam_var_desc_t **next);

void oam_llid_registration(cs_llid_t llid);
void oam_llid_deregistration(cs_llid_t llid);

#endif

