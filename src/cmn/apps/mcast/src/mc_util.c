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
its successor. Use of the SOFTWARE by the Government  itutes
acknowledgment of CORTINA's proprietary rights therein. Contractor or
Manufacturer is CORTINA.

Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/
#include "mc_type.h"
#include "mc_pkt.h"
#include "mc_core.h"
#include "mc_util.h"
#include "mc_src.h"
#include "mc_fdb.h"
#include "mc_adapter.h"
#include <string.h>

cs_status mc_ip_ntohl(  ip_type_t *ip, ip_type_t *out_ip)
{
    cs_int32       i = 0;
    mc_ip_type_t       ip_type = MC_IPV4;

    ip_type = ip->type;

    if (ip_type == MC_IPV4){
        out_ip->addr.ipv4 = ntohl((ip->addr.ipv4));
    }
    else{
        for (i = 0; i < MC_IPV6_IP_LEN / 4; i++ ){
            out_ip->addr.ipv6.mc_addr32[i] = ntohl(ip->addr.ipv6.mc_addr32[i]);
        }
    }
    out_ip->type = ip_type;

    return CS_E_OK;
}

cs_status mc_ip_htonl(  ip_type_t *ip, ip_type_t *out_ip)
{
    cs_int32       i = 0;
    mc_ip_type_t       ip_type = MC_IPV4;

    ip_type = ip->type;
    if(ip_type == MC_IPV4){
        out_ip->addr.ipv4 = htonl((ip->addr.ipv4));
    } 
    else {
        for (i = 0; i < MC_IPV6_IP_LEN / 4; i++ ) {
            out_ip->addr.ipv6.mc_addr32[i] = htonl(ip->addr.ipv6.mc_addr32[i]);
        }
    }

    out_ip->type = ip_type;
    return CS_E_OK;
}


cs_int32 mc_ip_cmp(ip_type_t *dst_ip, ip_type_t *src_ip)
{
    cs_int32    status = 1;
    cs_uint16  ip_addr_len = 0;

    if (dst_ip->type == src_ip->type){
        if (dst_ip->type == MC_IPV4) {
            ip_addr_len = MC_IPV4_IP_LEN;
        }
        else{
            ip_addr_len = MC_IPV6_IP_LEN;
        }
        status = memcmp(&dst_ip->addr, &src_ip->addr, ip_addr_len);
    }

    return status;
}

cs_boolean mc_ip_is_zero(ip_type_t *in_ip)
{
    ip_type_t ip;
    cs_int32  status = 0;

    memset(&ip, 0 , sizeof(ip_type_t));
    ip.type = in_ip->type;
    status = mc_ip_cmp(&ip, in_ip);
    if (status == 0){
        return 1;/* true */
    } 
    else {
        return 0;/* false */
    }
}

/* convert memory to ipv4 format. */
void mc_data_to_ipv4(cs_uint8 *data,ip_type_t *ip)
{
    cs_uint32 ip_addr;
    memcpy(&ip_addr,data,MC_IPV4_IP_LEN);
    ip->addr.ipv4 = ntohl(ip_addr);
    ip->type = MC_IPV4;
}

/* convert memory to ipv6 format. */
void mc_data_to_ipv6(cs_uint8 *data,ip_type_t *ip)
{
    cs_int32    i=0;

    for (i=0; i < MC_IPV6_IP_LEN; i=i+4) {
        ip->addr.ipv6.mc_addr32[i/4] = (data[i]<<24)+ (data [i+1]<<16)+(data [i+2]<<8)+ (data [i+3]) ;
    }
    ip->type = MC_IPV6 ;
}

void mc_data_to_ip(mc_ip_type_t type,cs_uint8 *data,ip_type_t *ip)
{
    memset(ip, 0, sizeof(ip_type_t));
    
    if (type == MC_IPV4) {
        mc_data_to_ipv4(data, ip);
    }
    else {
        mc_data_to_ipv6(data,ip);
    }
}

void  mc_mac_to_ip(cs_uint8 *mac , ip_type_t  *ip)
{
    memset(ip, 0, sizeof(ip_type_t));
    
    if( (mac[0] == 0x01) && (mac[1] == 0x00) && ( mac[2]==0x5e )) {
        ip->type = MC_IPV4;
        ip->addr.ipv4 = mac[5] + ((mac[4]<<8)&(0xff00)) + ((mac[3] << 16)&(0x7f0000))
            + ((0xe6<<24)&(0xff000000)) ;
    } 
    else {
        ip->type = MC_IPV6;
        ip->addr.ipv6.mc_addr8[3] = 0xff;
        ip->addr.ipv6.mc_addr8[2] = 0x0e;
        ip->addr.ipv6.mc_addr8[12] = mac[5];
        ip->addr.ipv6.mc_addr8[13] = mac[4];
        ip->addr.ipv6.mc_addr8[14] = mac[3];
        ip->addr.ipv6.mc_addr8[15] = mac[2];         
    }
    
}

void  mc_ip_to_mac(ip_type_t  * ip, cs_uint8 * mac)
{
    if( ip->type ==MC_IPV4) {
        mac[0] = 0x01;
        mac[1] = 0x00;
        mac[2] = 0x5e;
        mac[3] = (ip->addr.ipv4 >>16) &0x7f;
        mac[4] = (ip->addr.ipv4 >>8) &0xff;
        mac[5] = (ip->addr.ipv4) &0xff;
    } 
    else {
        mac[0] = 0x33;
        mac[1] = 0x33;
        mac[2] = ip->addr.ipv6.mc_addr8[15];
        mac[3] = ip->addr.ipv6.mc_addr8[14];
        mac[4] = ip->addr.ipv6.mc_addr8[13];
        mac[5] = ip->addr.ipv6.mc_addr8[12];
    }
    
}

/* set to default group ip address 230.*.*.* used only when MC_ENGINE_L2 enabled */
void mc_set_dft_ip(mc_ip_type_t type,cs_uint8 *data,ip_type_t *ip)
{
    if (type == MC_IPV4) {
        cs_uint32 ip_addr;
        memcpy(&ip_addr,data,MC_IPV4_IP_LEN);
        ip->type = MC_IPV4;
        ip->addr.ipv4 = ntohl(ip_addr) & 0x00FFFFFF;
        ip->addr.ipv4 |=  0xE6000000;
    } 
    else {
        memset(ip,0,sizeof(ip_type_t));
        
        ip->addr.ipv6.mc_addr8[3] = 0xff;
        ip->addr.ipv6.mc_addr8[2] = 0x0e;
        ip->addr.ipv6.mc_addr32[3] = (data [12]<<24)+ (data [13]<<16)+(data [14]<<8)+ (data [15]) ;
        ip->type = MC_IPV6;
    }

}

/* host byte order */
cs_boolean mc_is_grp_ip(ip_type_t *ip)
{
    if(ip->type == MC_IPV4) {
        if((ip->addr.ipv4&0xFF000000) >= 0xE0000000 && (ip->addr.ipv4&0xFF000000) <= 0xEF000000)
            return TRUE;
    }
    else {
        /* TODO */
        if(ip->addr.ipv6.mc_addr8[3] == 0xFF) {
            return TRUE;
        }

    }
    
    return FALSE;
}
mc_group_record_list_t  *mc_record_create()
{
    mc_group_record_list_t  *record = NULL;
    
    record = (mc_group_record_list_t  *)mc_malloc(sizeof(mc_group_record_list_t));
    if(record) {
        memset(record, 0, sizeof(mc_group_record_list_t));
        cs_lst_init(&record->entry.src_list, NULL);
    }
    
    return record;
}

void mc_record_destroy(mc_group_record_list_t *record)
{
    mc_src_list_clear( &record->entry.src_list);
    mc_free((void *)record);

    return;
}


cs_status  mc_record_add(cs_list *record_list, mc_group_record_list_t *record)
{    
    if (record_list == NULL) {
        return CS_E_ERROR;
    }

    cs_lst_add(record_list, &record->node);
    return CS_E_OK;
}

cs_status  mc_record_del(cs_list *record_list, mc_group_record_list_t *record)
{
    if(record_list == NULL || record == NULL) {
        return CS_E_ERROR;
    }
    
    cs_lst_delete(record_list, &record->node);
    
    return CS_E_OK;

}


void  mc_record_list_clear(cs_list *record_list)
{
    mc_group_record_list_t *record=NULL;

    if(NULL == record_list || 0 == record_list->count) {
        return ;
    }

    while((record = (mc_group_record_list_t * )cs_lst_first(record_list)) != NULL) {
        mc_record_del(record_list,  record);
        mc_record_destroy(record);
    } 
    
}

