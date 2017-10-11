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
 
Copyright (c) 2010 by Cortina Systems Incorporated
****************************************************************************/
#include "sdl_mac_filterbind_util.h"
#include "sdl_init.h"
#include "plat_common.h"
#include "sdl.h"


#define  SDL_UNI_PORT_RANGE_CHECK(port)  ((port)>UNI_PORT_MAX)||((port)<CS_UNI_PORT_ID1)   

/********************************************************************
 *************************** Global variables ***********************
 ********************************************************************/

static cs_sdl_mac_filterbind_table_t filterbind_table[UNI_PORT_MAX];


static cs_boolean  macaddr_equal(cs_uint8 *mac_a, cs_uint8 *mac_b, cs_uint8 len)
{
    cs_uint8 i;

    if (!mac_a || !mac_b) {
            return 0;
    }

    if (len > 6)
            len = 6;

    for (i=0; i<len; i++)
            if (mac_a[i] != mac_b[i])
                    return 0;

    return 1;
}

void mac_filterbind_status_set(   
    CS_IN  cs_port_id_t                  port_id,
    CS_IN  cs_sdl_mac_filterbind_flag_t  flag   
)
{
    if((SDL_UNI_PORT_RANGE_CHECK(port_id))||(flag >SDL_PORT_MAC_BIND))
    {  
        SDL_MIN_LOG("para error. [%s %d]\n", __FUNCTION__, __LINE__);
        return;
    }
    
    filterbind_table[port_id-1].flag = flag;
          
    return;        
}

void mac_filterbind_status_get(   
    CS_IN   cs_port_id_t                  port_id,
    CS_OUT  cs_sdl_mac_filterbind_flag_t  *flag,
    CS_OUT  cs_uint8                      *entry_count
)
{
    if((SDL_UNI_PORT_RANGE_CHECK(port_id))||(flag==NULL)||(entry_count==NULL))
    {  
        SDL_MIN_LOG("para error. port_id: %d. [%s %d]\n", port_id,  __FUNCTION__, __LINE__);
        return;
    }

    *flag        = filterbind_table[port_id-1].flag;
    *entry_count = filterbind_table[port_id-1].count;
         
    return;
}

cs_boolean mac_find_tail_filterbindrecord(
    CS_IN   cs_port_id_t                   port_id, 
    CS_OUT  cs_sdl_mac_filterbind_entry_t  *entry
)
{   
    if(SDL_UNI_PORT_RANGE_CHECK(port_id))
    {  
        SDL_MIN_LOG("para error. port_id: %d. [%s %d]\n", port_id,  __FUNCTION__, __LINE__);
        return FALSE;
    }

    if(entry==NULL)
        return FALSE;
     
    if(filterbind_table[port_id-1].count ==0)
        return FALSE;   
    
    entry->hw_index = filterbind_table[port_id-1].entry[filterbind_table[port_id-1].count-1].hw_index;
    entry->mac_type = filterbind_table[port_id-1].entry[filterbind_table[port_id-1].count-1].mac_type;
    memcpy(&(entry->mac), &(filterbind_table[port_id-1].entry[filterbind_table[port_id-1].count-1].mac), sizeof(cs_mac_t));

    return TRUE;
}

cs_status mac_save_tail_filterbindrecord(
    CS_IN   cs_port_id_t                   port_id, 
    CS_IN   cs_sdl_mac_filterbind_entry_t  *entry
)
{   
    if((SDL_UNI_PORT_RANGE_CHECK(port_id))||(entry==NULL))
    {  
        SDL_MIN_LOG("para error. port_id: %d. [%s %d]\n", port_id,  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    if(filterbind_table[port_id-1].count ==0)
        return CS_E_OK;
        
    filterbind_table[port_id-1].entry[filterbind_table[port_id-1].count-1].hw_index = entry->hw_index;
    filterbind_table[port_id-1].entry[filterbind_table[port_id-1].count-1].mac_type = entry->mac_type;
    memcpy(&(filterbind_table[port_id-1].entry[filterbind_table[port_id-1].count-1].mac), &(entry->mac),sizeof(cs_mac_t));
        
    return CS_E_OK;       
}

cs_boolean mac_find_filterbindrecord(
    CS_IN   cs_port_id_t                   port_id, 
    CS_IN   cs_sdl_mac_filterbind_entry_t  *entry,
    CS_OUT  cs_uint8                       *hw_index
)
{
    cs_uint8 i;

    if(SDL_UNI_PORT_RANGE_CHECK(port_id))
    {  
        SDL_MIN_LOG("para error. port_id: %d. [%s %d]\n", port_id,  __FUNCTION__, __LINE__);
        return FALSE;
    }

    if((entry==NULL)||(hw_index == NULL))
        return FALSE;
          
    for(i=0; i<filterbind_table[port_id-1].count; i++)
    {
        if((filterbind_table[port_id-1].entry[i].mac_type == entry->mac_type)&& 
            (macaddr_equal((cs_uint8 *)&(filterbind_table[port_id-1].entry[i].mac), (cs_uint8 *)&(entry->mac),CS_MACADDR_LEN)))
        {        
                *hw_index = filterbind_table[port_id-1].entry[i].hw_index;
               return TRUE;
        }
    }
     
    return FALSE;
}

cs_status mac_add_filterbindrecord(
    CS_IN   cs_port_id_t                   port_id, 
    CS_IN   cs_sdl_mac_filterbind_entry_t  *entry
)
{   
    cs_uint8 i;
 
    if((SDL_UNI_PORT_RANGE_CHECK(port_id))||(entry==NULL))
    {  
        SDL_MIN_LOG("para error. port_id: %d. [%s %d]\n", port_id,  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    // add after find    
    for(i=0; i<filterbind_table[port_id-1].count; i++)
    {
        if((filterbind_table[port_id-1].entry[i].mac_type == entry->mac_type)&& 
            (macaddr_equal((cs_uint8 *)&(filterbind_table[port_id-1].entry[i].mac), (cs_uint8 *)&(entry->mac), CS_MACADDR_LEN)))
        {
               return CS_E_OK;
        }
    }
                 
    if(filterbind_table[port_id-1].count >= MAX_MAC_ENTRY)
        return CS_E_RESOURCE;
    
    memcpy(&(filterbind_table[port_id-1].entry[filterbind_table[port_id-1].count]), entry, sizeof(cs_sdl_mac_filterbind_entry_t));
    filterbind_table[port_id-1].count++;
        
    return CS_E_OK;       
}

cs_status mac_del_filterbindrecord(
    CS_IN   cs_port_id_t                   port_id, 
    CS_IN   cs_sdl_mac_filterbind_entry_t  *entry
)
{
    cs_uint8 i;
    cs_boolean found = FALSE;
   
    if((SDL_UNI_PORT_RANGE_CHECK(port_id))||(entry==NULL))
    {  
        SDL_MIN_LOG("para error. port_id: %d. [%s %d]\n", port_id,  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
             
    if(filterbind_table[port_id-1].count== 0)
        return CS_E_RESOURCE;
    
    for(i=0; i<filterbind_table[port_id-1].count; i++)
    {
        if((filterbind_table[port_id-1].entry[i].mac_type == entry->mac_type)&& 
            (macaddr_equal((cs_uint8 *)&(filterbind_table[port_id-1].entry[i].mac), (cs_uint8 *)&(entry->mac), CS_MACADDR_LEN)))
        {
            found = TRUE;
            break;
        }
    }
    
    if(found)
    {     
        memcpy(&(filterbind_table[port_id-1].entry[i]), 
                        &(filterbind_table[port_id-1].entry[filterbind_table[port_id-1].count-1]), sizeof(cs_sdl_mac_filterbind_entry_t));
        memset(&(filterbind_table[port_id-1].entry[filterbind_table[port_id-1].count-1]), 0, sizeof(cs_sdl_mac_filterbind_entry_t));
        filterbind_table[port_id-1].count--;
        
        return CS_E_OK;  
    }
    
    return CS_E_NOT_FOUND;
}

cs_status mac_get_filterbindrecord(
    CS_IN  cs_port_id_t                     port_id,
    CS_OUT cs_uint8                         *number,
    CS_OUT cs_sdl_mac_filterbind_entry_t    *entrylist
)
{    
    cs_uint16               i =0;
   
    if(SDL_UNI_PORT_RANGE_CHECK(port_id))
    {  
        SDL_MIN_LOG("para error. port_id: %d. [%s %d]\n", port_id,  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if((number == NULL)||(entrylist == NULL))
    {    
        SDL_MIN_LOG("para error. NULL point. [%s %d]\n", __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    *number = filterbind_table[port_id-1].count;
       
    for(i=0; i<filterbind_table[port_id-1].count; i++){
            memcpy(entrylist, &(filterbind_table[port_id-1].entry[i]), sizeof(cs_sdl_mac_filterbind_entry_t));
            entrylist++;
    }
    
    return CS_E_OK; 
}

void sdl_mac_tbl_init(void)
{
    
    memset(filterbind_table, 0, UNI_PORT_MAX*sizeof(cs_sdl_mac_filterbind_table_t));
      
    return;
}

    
