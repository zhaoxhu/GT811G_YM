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

Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/
#ifdef HAVE_CS_OAM
#include "plat_common.h"
#include "oam_supp.h"
#include "cs_msg.h"
#include "oam_std_pdu.h"
#include "sdl_api.h"
#ifdef HAVE_ONU_EVENT
#include "event.h"
#endif
#ifdef HAVE_ONU_HELLO
#include "onu_hello.h"
#endif



extern cs_port_id_t g_oam_port;

cs_int32 cs_oam_hdr_build(
        cs_uint8* pkt,
        cs_mac_t* dest_mac,
        cs_uint8 opcode,
        cs_uint8  *oui)
{
    cs_int32 len = 0;
    cs_oam_pdu_hdr_t *pHdr;
    cs_mac_t null_mac = {{0x0}};
    cs_callback_context_t             context;


    if(!pkt || !dest_mac || !oui)
    {
        return len;
    }

    pHdr = (cs_oam_pdu_hdr_t*)pkt;
    memcpy(&pHdr->dst_mac, dest_mac, CS_MACADDR_LEN);
    if(!memcmp(dest_mac, &null_mac, CS_MACADDR_LEN))
    {
        memcpy(&pHdr->dst_mac, &CS_MAC_ADDR_SLOW, CS_MACADDR_LEN);
    }
    epon_request_onu_pon_mac_addr_get(context, 0, 0, &pHdr->src_mac);
    pHdr->eth_type = htons(OAM_SLOW_PROTOCOLS_TYPE);
    pHdr->sub_type = OAM_SUBTYPE;
    pHdr->flags = htons(0x50);
    pHdr->code = OAM_PDU_CODE_ORG_SPEC;

    memcpy(pHdr->oui, oui, OAM_OUI_LEN);
    pHdr->opcode = opcode;

    len = sizeof(cs_oam_pdu_hdr_t);

    return len;
    
}

void handle_cs_oams(
        cs_uint32 port, 
        cs_uint8 *frame, 
        cs_uint32 len)
{
    cs_oam_pdu_hdr_t  * oam_pdu = (cs_oam_pdu_hdr_t*)frame;

    switch(oam_pdu->opcode)
    {
        case CS_OAM_OPCODE_HELLO:
            #ifdef HAVE_ONU_HELLO
            onu_hello_proc(oam_pdu->data, (cs_int32)(len - sizeof(cs_oam_pdu_hdr_t)), g_oam_port);
            #endif
            break;

        case CS_OAM_OPCODE_EVENT:
            #ifdef HAVE_ONU_EVENT
            onu_event_proc(oam_pdu->data, (cs_int32)(len - sizeof(cs_oam_pdu_hdr_t)), g_oam_port);
            #endif
            break;

        default:
            cs_printf("Not support opcode %d\n", oam_pdu->opcode);
            return;
    }

}

void oam_cs_init()
{
    oam_vendor_handlers_t cs_handle;

    oam_vendor_handlers_init(&cs_handle);

    cs_handle.org_handler = handle_cs_oams;

#ifdef HAVE_ONU_HELLO
    oam_vendor_handler_register(CS_OUI, cs_handle);
#endif
}
#endif
