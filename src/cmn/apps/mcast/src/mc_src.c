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
#include "mc_type.h"
#include "mc_timer.h"
#include "mc_fdb.h"
#include "mc_src.h"
#include "mc_util.h"
#include "mc_adapter.h"

mc_source_record_list_t *mc_src_create()
{
    mc_source_record_list_t *src = NULL;

    src = (mc_source_record_list_t *)mc_malloc(sizeof(mc_source_record_list_t));
    if(src) {
        memset(src, 0, sizeof(mc_source_record_list_t));
    }

    return src;
}

/* out_List = X * Y */
cs_status mc_x_intersection_y(cs_list *x_list, cs_list *y_list, cs_list *out_list)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *out_src = NULL;
    mc_source_record_list_t *new_src = NULL;
    cs_status ret = CS_E_OK;
    
    cs_lst_scan(y_list, src, mc_source_record_list_t *) {
        ret = mc_src_find(x_list, src, &out_src);
        if(ret == CS_E_OK) {
            new_src = mc_src_create();
            if(new_src == NULL) {
                return CS_E_ERROR;
            }
            mc_src_copy(new_src, out_src);
            mc_src_add(out_list, new_src);
        }
    }

    return ret;

}

/* out_list = X - Y */
cs_status mc_x_minus_y(cs_list *x_list,  cs_list *y_list ,  cs_list *out_list)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *new_src = NULL;
    cs_status ret = CS_E_OK;
    
    cs_lst_scan(x_list, src, mc_source_record_list_t *) {
        ret = mc_src_find(y_list, src, &new_src);
        if(ret == CS_E_OK) {
            continue;
        } 
        else {
            new_src = mc_src_create();
            if(new_src == NULL) {
                return CS_E_ERROR;
            }
            mc_src_copy(new_src,  src);
            mc_src_add(out_list, new_src);
        }

    }

    return ret;

}

/* out_list = X + Y , remove duplicate element */
cs_status mc_x_union_y(cs_list *x,  cs_list *y ,  cs_list *out_list)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *out_src = NULL;
    cs_status ret = CS_E_OK;

    cs_lst_scan(y, src, mc_source_record_list_t *) {
        ret = mc_src_find(x, src, &out_src);
        if(ret != CS_E_OK) {
            out_src = mc_src_create();
            if(out_src == NULL) {
                return CS_E_ERROR;
            }

            mc_src_copy(out_src, src);
            mc_src_add(out_list, out_src);
        }
    }

    return ret;

}

/* dst_list = src_list */
cs_status mc_x_set_y(cs_list *dst_list,  cs_list *src_list)
{
    return mc_src_list_copy(dst_list, src_list);
}

/* src_list = src_list * sub_list */
cs_status mc_x_intersection_subset(cs_list *src_list,  cs_list *sub_list)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *out_src = NULL;
    cs_status                     ret = CS_E_OK;
    mc_source_record_list_t * prev = NULL;

    for(src = (mc_source_record_list_t *)cs_lst_first(src_list); src;) {
        ret = mc_src_find(sub_list, src, &out_src);
        if(ret != CS_E_OK) {
            prev = (mc_source_record_list_t *)cs_lst_prev((cs_node *)src);
            /* remove node and release the memory */
            mc_src_del(src_list,  src);
            if(prev != NULL) {
                src = (mc_source_record_list_t *)cs_lst_next(&prev->node);
            } 
            else {
                src = (mc_source_record_list_t *)cs_lst_first(src_list);
            }
        } 
        else {
            /*find the node*/
            src = (mc_source_record_list_t *)cs_lst_next((cs_node *)src);
        }

    }

    return ret;

}

cs_status mc_x_mc_timer_set(cs_list *src_list, cs_uint16 second, cs_uint16 oper_flag)
{
    return mc_src_mc_timer_set(src_list,  second, oper_flag);
}

/* src_list = src_list + sub_list, update timer(src_list * sub_list) */
cs_status mc_x_union_subset(cs_list *src_list,  cs_list *sub_list)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *new_src = NULL;
    mc_source_record_list_t *out_src = NULL;
    cs_status                     ret = CS_E_OK;


    cs_lst_scan(sub_list, src, mc_source_record_list_t *) {
        ret = mc_src_find(src_list , src, &out_src);
        if(ret != CS_E_OK) {
            new_src = mc_src_create();
            if(new_src == NULL) {
                return CS_E_ERROR;
            }
            mc_src_copy(new_src, src);
            mc_src_add(src_list, new_src);
        } 
        else {
            /*update src timer*/
            mc_timer_set(&out_src->entry.timer, src->entry.timer.age, src->entry.timer.active_flag);
        }
    }

    return CS_E_OK;

}

/* src_list = src_list - sub_list */
cs_status mc_x_minus_subset(cs_list *src_list,  cs_list *sub_list)
{
    return mc_src_sub_clear(src_list, sub_list);
}

/* src_list = 0 */
cs_status mc_x_clear(cs_list *src_list)
{
    return mc_src_list_clear(src_list);
}

/* 
* if list1 and list2 intersection set is empty, return false, otherwise return true.
*/
cs_boolean mc_src_find_intersection (cs_list *list1, cs_list *list2)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *out_src = NULL;
    cs_status ret = CS_E_OK;
    
    cs_lst_scan(list1, src, mc_source_record_list_t *) {
        ret = mc_src_find(list2, src, &out_src);
        if(ret == CS_E_OK) {
            return 1/* true */;
        }
    }

    return 0/* false */;

}

cs_status mc_src_copy(mc_source_record_list_t *dst_list, mc_source_record_list_t *src_list)
{
    if(dst_list == NULL) {
        return CS_E_ERROR;
    }

    memcpy(&dst_list->entry, &src_list->entry, sizeof(mc_source_record_t));
    return CS_E_OK;
}

cs_status mc_src_list_copy(cs_list *dst_list , cs_list *src_list)
{
    mc_source_record_list_t * src = NULL;
    mc_source_record_list_t * dst_src = NULL;

    if(dst_list == NULL){
        return CS_E_ERROR;
    }

    mc_src_list_clear(dst_list);
    if(src_list->count == 0){
        return CS_E_OK;
    }

    cs_lst_scan(src_list , src, mc_source_record_list_t *){
        dst_src = mc_src_create();
        if(dst_src == NULL){
            return CS_E_ERROR;
        }
        mc_src_copy(dst_src,  src);
        cs_lst_add(dst_list, &dst_src->node);
    }

    return CS_E_OK;
}

cs_status mc_src_find(
    cs_list *dst_list, 
    mc_source_record_list_t *in_src, 
    mc_source_record_list_t **out_src)
{
    mc_source_record_list_t *src = NULL;
    cs_status                  ret = CS_E_ERROR;

    if(dst_list == NULL) {
        return CS_E_ERROR;
    }

    cs_lst_scan(dst_list , src, mc_source_record_list_t *){
        ret = mc_ip_cmp(&src->entry.src_ip, &in_src->entry.src_ip);
        if(ret == CS_E_OK) {
            *out_src = src ;
            return CS_E_OK;
        }
    }

    return ret;
}

/*return 0, if dst_list==src_list
   return 1, if src_list is in dst_list
   return -1 , if  src_list node is out of dst_list
*/
cs_int32 mc_src_cmp(cs_list *dst_list, cs_list *src_list)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *dst_src = NULL;
    cs_int32                      status = 1;
    cs_status                    ret = CS_E_OK;

    if(dst_list == NULL){
        return -1;
    }

    cs_lst_scan(src_list , src, mc_source_record_list_t *){
        ret = mc_src_find(dst_list, src, &dst_src);
        if(ret  != CS_E_OK){
            break;
        }
    }

    if(ret == CS_E_OK){
        if(dst_list->count == src_list->count){
            status = 0;
        } 
        else {
            status = 1;
        }
    } 
    else {
        status = -1;
    }

    return status;
}

cs_status mc_src_add(cs_list *dst_list , mc_source_record_list_t *src)
{
    if(dst_list == NULL){
        return CS_E_ERROR;
    }

    cs_lst_add(dst_list,  &src->node);
    return CS_E_OK;
}

cs_status mc_src_sub_clear(cs_list *dst_list, cs_list *src_list)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *out_src = NULL;
    cs_status                      ret = CS_E_OK;

    if(dst_list == NULL){
        return CS_E_ERROR;
    }

    cs_lst_scan(src_list, src, mc_source_record_list_t *){
        ret = mc_src_find(dst_list, src, &out_src);
        if(ret == CS_E_OK){
            mc_src_del(dst_list, out_src);
        }
    }

    return ret;
}

cs_status mc_src_rm(cs_list *dst_list , mc_source_record_list_t *src)
{
    cs_status ret = CS_E_OK;

    if(dst_list == NULL){
        return CS_E_ERROR;
    }

    cs_lst_delete(dst_list,  &src->node);
    return ret;
}

cs_status mc_src_del(cs_list *dst_list , mc_source_record_list_t *src)
{
    cs_status ret = CS_E_OK;
    if(dst_list == NULL){
        return CS_E_ERROR;
    }

    cs_lst_delete(dst_list,  &src->node);
    mc_free(src);
    return ret;
}

cs_status mc_src_list_clear(cs_list *src_list)
{
    mc_source_record_list_t *src = NULL;

    if(src_list == NULL){
        return CS_E_ERROR;
    }

    for(src = (mc_source_record_list_t *)cs_lst_first(src_list); src;) {
        mc_src_del(src_list,  src);
        src = (mc_source_record_list_t *)cs_lst_first(src_list);
    }

    return CS_E_OK;
}

cs_status mc_src_append(cs_list *dst_list, cs_list *src_list)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *new_src = NULL;
    mc_source_record_list_t *out_src = NULL;
    cs_status                       ret = CS_E_OK;

    if(dst_list == NULL){
        return CS_E_ERROR;
    }

    cs_lst_scan(src_list, src, mc_source_record_list_t *) {
        ret = mc_src_find(dst_list, src, &out_src);
        if(ret != CS_E_OK){
            new_src = mc_src_create();
            if(new_src == NULL){
                return CS_E_ERROR;
            }

            mc_src_copy(new_src,  src);
            mc_src_add(dst_list,  new_src);
        }
    }

    return ret;
}

/* TODO replaced by mc_src_append*/
cs_status mc_src_contact(cs_list *dst_list, cs_list *src_list)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *new_src = NULL;
    mc_source_record_list_t *out_src = NULL;
    cs_status                       ret = CS_E_OK;

    if(dst_list == NULL){
        return CS_E_ERROR;
    }

    cs_lst_scan(src_list, src, mc_source_record_list_t *) {
        ret = mc_src_find(dst_list, src, &out_src);
        if(ret != CS_E_OK){
            new_src = mc_src_create();
            if(new_src == NULL){
                return CS_E_ERROR;
            }

            mc_src_copy(new_src,  src);
            mc_src_add(dst_list,  new_src);
        }
    }

    return ret;
}

cs_status mc_src_mc_timer_set(cs_list *src_list, cs_uint16 age,  cs_uint16 oper_flag)
{
    mc_source_record_list_t *src = NULL;

    if(src_list == NULL) {
        return CS_E_ERROR;
    }

    cs_lst_scan(src_list, src, mc_source_record_list_t *){
        mc_timer_set(&src->entry.timer, age,  oper_flag);
    }

    return CS_E_OK;
}

cs_status mc_x_subnet_mc_timer_set(cs_list *src_list,  cs_list *sub_list)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *out_src = NULL;
    cs_status                      ret = CS_E_OK;

    if(src_list == NULL){
        return CS_E_ERROR;
    }

    cs_lst_scan(sub_list, src , mc_source_record_list_t *) {
        ret = mc_src_find(src_list, src, &out_src);
        if(ret == CS_E_OK){
            mc_timer_set(&out_src->entry.timer, src->entry.timer.age, src->entry.timer.active_flag);
        }
    }
    return CS_E_OK;
}

cs_status mc_src_num_get(cs_list *src_list, cs_uint32 *src_num)
{
    if(src_list == NULL || src_num == NULL) {
        return CS_E_ERROR;
    }

    *src_num = src_list->count;
    return CS_E_OK;
}


