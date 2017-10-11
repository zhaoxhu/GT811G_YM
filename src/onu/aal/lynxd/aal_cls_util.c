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
#include "plat_common.h" 
#include "aal_cls_util.h"
#include "aal_util.h"
#include "aal.h"


cs_uint32  aal_keylist_mutex;
cs_uint32  aal_clstbl_mutex;
cs_uint32  aal_fibtbl_mutex;
cs_uint32  aal_spectbl_mutex;
cs_uint32  aal_l4port_mutex;
cs_uint32  aal_filter_mutex;



/*****************************************************************************/
/*                    Local Functions                                        */
/*****************************************************************************/

#define  CL_LE_REG_BASE        ((char*)&(g_lynxd_reg_ptr->fe.le))
#define  CL_LE_PORT_WIDTH      (0x400)
#define  CL_PORT_BASE(port_id) (FE_LE_t *)(CL_LE_REG_BASE + port_id* CL_LE_PORT_WIDTH)

#define LE_CL_TABLE_INITIATE   1
#define LE_CL_TABLE_READ       0
#define LE_CL_TABLE_WRITE      1
#define LE_CL_OPER_TIMEOUT     10000

cs_uint32 g_mfilter_aging_time[CL_PORT_TYPE_NUM][64];

typedef struct{

    FE_LE_CE_KEY_MEM_DATA9_t  data9;
    FE_LE_CE_KEY_MEM_DATA8_t  data8;
    FE_LE_CE_KEY_MEM_DATA7_t  data7;
    FE_LE_CE_KEY_MEM_DATA6_t  data6;
    FE_LE_CE_KEY_MEM_DATA5_t  data5;
    FE_LE_CE_KEY_MEM_DATA4_t  data4;
    FE_LE_CE_KEY_MEM_DATA3_t  data3;
    FE_LE_CE_KEY_MEM_DATA2_t  data2;
    FE_LE_CE_KEY_MEM_DATA1_t  data1;
    FE_LE_CE_KEY_MEM_DATA0_t  data0;

}FE_LE_CE_KEY_DATA_T;

typedef struct{

    FE_LE_CE_FIB_MEM_DATA2_t  data2;
    FE_LE_CE_FIB_MEM_DATA1_t  data1;
    FE_LE_CE_FIB_MEM_DATA0_t  data0;

}FE_LE_CE_FIB_DATA_T;

static cs_status __cl_key_link_list_table_read(cs_uint8 port_id, cs_uint8 index, FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA_t *data)
{
    cs_uint16 i = 0;
    FE_LE_t *base = CL_PORT_BASE(port_id);
    cs_status   ret = CS_E_OK;
    FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS_t temp;

    if(data ==NULL)
    {   
        AAL_MIN_LOG("NULL pointer. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    temp = base->CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS;
    temp.bf.access = 1;
    temp.bf.rbw = 0;
    temp.bf.address = index;

    cs_mutex_lock(aal_keylist_mutex);
       

    base->CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS = temp;

    while (1) {
        if (0 == base->CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS.bf.access) {
            *data = base->CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA;            
            ret = CS_E_OK;
            break;
        }

        if (++i > 2000) 
        {   
            AAL_CRI_LOG("link_list_table asic rw timeout. [%s %d]\n",  __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
            break;
        }
    }
    cs_mutex_unlock(aal_keylist_mutex);
    return ret;
}

static cs_status __cl_key_link_list_table_write(cs_uint8 port_id, cs_uint8 index, FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA_t data)
{
    FE_LE_t *base = CL_PORT_BASE(port_id);
    FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS_t temp = base->CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS;
    cs_uint16 i = 0;
    cs_status   ret = CS_E_OK;

    temp.bf.access = 1;
    temp.bf.rbw = 1;
    temp.bf.address = index;

    cs_mutex_lock(aal_keylist_mutex);
    base->CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA.wrd = data.wrd&0x3F;
    base->CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS = temp;
    while (1) {
        if (0 == base->CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS.bf.access) {
            ret = CS_E_OK;
            break;
        }

        if (++i > 2000) {
            AAL_CRI_LOG("link_list_table asic rw timeout. [%s %d]\n",  __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
            break;
        }
    }
    
    cs_mutex_unlock(aal_keylist_mutex);
        
    return ret;
}

/***************************************************************************
* function    : __cl_key_table_access                                      *
* description : this function is used to perform CE KEY table              *
*               read and write operation.  .                               *
* input       : operation flag(read or write) and data                     *
* output      : none                                                       *
* return      : CS_E_OK    - succeed in setting or getting entry           *
*               CS_E_ERROR - failed to set or get entry                    *
* globals     : none                                                       *
* registers   : FE_LE_CE_KEY_MEM_ACCESS                                    *
*               bit31 : 1 - asic busy                                      *
*                       0 - asic ready                                     *
*               bit30 : 1 - write                                          *
*                       0 - read                                           *
* others      :                                                            *
*                                                                          *
***************************************************************************/
static cs_status __cl_key_table_access(cs_uint8 port_id, cs_uint8 op_flag, cs_uint16 addr, FE_LE_CE_KEY_DATA_T *pdata)
{
    FE_LE_t *base = CL_PORT_BASE(port_id);
    FE_LE_CE_KEY_MEM_ACCESS_t  key_access = base->CE_KEY_MEM_ACCESS;
    cs_uint8 access = 0;
    cs_uint32 i = 0;
    cs_status ret = CS_E_OK;
    
    if ((op_flag > 1)||(pdata ==NULL))
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    cs_mutex_lock(aal_clstbl_mutex);
    if (op_flag == LE_CL_TABLE_WRITE)
    {   
        base->CE_KEY_MEM_DATA0.wrd = pdata->data0.wrd;
        base->CE_KEY_MEM_DATA1.wrd = pdata->data1.wrd;
        base->CE_KEY_MEM_DATA2.wrd = pdata->data2.wrd;
        base->CE_KEY_MEM_DATA3.wrd = pdata->data3.wrd;
        base->CE_KEY_MEM_DATA4.wrd = pdata->data4.wrd;
        base->CE_KEY_MEM_DATA5.wrd = pdata->data5.wrd;
        base->CE_KEY_MEM_DATA6.wrd = pdata->data6.wrd;
        base->CE_KEY_MEM_DATA7.wrd = pdata->data7.wrd;
        base->CE_KEY_MEM_DATA8.wrd = pdata->data8.wrd;
        base->CE_KEY_MEM_DATA9.wrd = pdata->data9.wrd; // &0xFFFFFFF; /* only use low 28 bits */

        key_access.bf.access  = LE_CL_TABLE_INITIATE;
        key_access.bf.rbw     = LE_CL_TABLE_WRITE;
        key_access.bf.address = addr & 0x3F;           // only use low 6 bits

        base->CE_KEY_MEM_ACCESS.wrd = key_access.wrd;

        access = base->CE_KEY_MEM_ACCESS.bf.access;
        while(access && (i < LE_CL_OPER_TIMEOUT))
        {
            access = base->CE_KEY_MEM_ACCESS.bf.access;
            i++;
            if ((i % (LE_CL_OPER_TIMEOUT / 100)) == 0)
            {
                AAL_INF_LOG("__cl_key_table_access : asic busy %d. [%s %d]\n", i / (LE_CL_OPER_TIMEOUT / 10), __FUNCTION__, __LINE__);
            }
        }
        if (i >= LE_CL_OPER_TIMEOUT)
        {
            AAL_CRI_LOG("__cl_key_table_access : asic rw timeout.[%s %d]\n", __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
        }
    }

    /* waiting untill read-operation is completed */
    else if (op_flag == LE_CL_TABLE_READ)
    {  
        key_access.bf.access  = LE_CL_TABLE_INITIATE;
        key_access.bf.rbw     = LE_CL_TABLE_READ;
        key_access.bf.address = addr & 0x3F;         // only use low 6 bits
             
        base->CE_KEY_MEM_ACCESS.wrd = key_access.wrd;

        access = base->CE_KEY_MEM_ACCESS.bf.access;
        while(access && (i < LE_CL_OPER_TIMEOUT))
        {
            access = base->CE_KEY_MEM_ACCESS.bf.access;
            i++;
            if ((i % (LE_CL_OPER_TIMEOUT / 100)) == 0)
            {
                AAL_INF_LOG("__cl_key_table_access : asic busy %d. [%s %d]\n", i / (LE_CL_OPER_TIMEOUT / 10), __FUNCTION__, __LINE__);
            }
        }
        if (i >= LE_CL_OPER_TIMEOUT)
        {
            AAL_CRI_LOG("__cl_key_table_access : asic rw timeout.[%s %d]\n", __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
        }
        pdata->data0.wrd = base->CE_KEY_MEM_DATA0.wrd;
        pdata->data1.wrd = base->CE_KEY_MEM_DATA1.wrd;
        pdata->data2.wrd = base->CE_KEY_MEM_DATA2.wrd;
        pdata->data3.wrd = base->CE_KEY_MEM_DATA3.wrd;
        pdata->data4.wrd = base->CE_KEY_MEM_DATA4.wrd;
        pdata->data5.wrd = base->CE_KEY_MEM_DATA5.wrd;
        pdata->data6.wrd = base->CE_KEY_MEM_DATA6.wrd;
        pdata->data7.wrd = base->CE_KEY_MEM_DATA7.wrd;
        pdata->data8.wrd = base->CE_KEY_MEM_DATA8.wrd;
        pdata->data9.wrd = base->CE_KEY_MEM_DATA9.wrd; //&0xFFFFFFF; /* only use low 28 bits */
    }
    
    cs_mutex_unlock(aal_clstbl_mutex);
    return ret;
}

/***************************************************************************
* function    : __cl_fib_table_access                                      *
* description : this function is used to perform CE fib table              *
*               read and write operation.  .                               *
* input       : operation flag(read or write) and data                     *
* output      : none                                                       *
* return      : CS_E_OK    - succeed in setting or getting entry           *
*               CS_E_ERROR - failed to set or get entry                    *
* globals     : none                                                       *
* registers   : FE_LE_CE_FIB_MEM_ACCESS                                    *
*               bit31 : 1 - asic busy                                      *
*                       0 - asic ready                                     *
*               bit30 : 1 - write                                          *
*                       0 - read                                           *
* others      :                                                            *
*                                                                          *
***************************************************************************/
static cs_status __cl_fib_table_access(cs_uint8 port_id, cs_uint8 op_flag, cs_uint16 addr, FE_LE_CE_FIB_DATA_T *pdata)
{
    FE_LE_t *base = CL_PORT_BASE(port_id);
    FE_LE_CE_FIB_MEM_ACCESS_t  fib_access = base->CE_FIB_MEM_ACCESS;
    cs_uint8 access = 0;
    cs_uint32 i     = 0;
    cs_status ret = CS_E_OK;

    if ((op_flag > 1)||(pdata ==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    cs_mutex_lock(aal_fibtbl_mutex);
    if (op_flag == LE_CL_TABLE_WRITE)
    {   
        base->CE_FIB_MEM_DATA0.wrd = pdata->data0.wrd;
        base->CE_FIB_MEM_DATA1.wrd = pdata->data1.wrd;
        base->CE_FIB_MEM_DATA2.wrd = pdata->data2.wrd; //&0x1FFFFF; /* only use low 21 bits */

        fib_access.bf.access  = LE_CL_TABLE_INITIATE;
        fib_access.bf.rbw     = LE_CL_TABLE_WRITE;
        fib_access.bf.address = addr & 0x1FF;          // only use low 9 bits

        base->CE_FIB_MEM_ACCESS.wrd = fib_access.wrd;

        access = base->CE_FIB_MEM_ACCESS.bf.access;
        while(access && (i < LE_CL_OPER_TIMEOUT))
        {
            access = base->CE_FIB_MEM_ACCESS.bf.access;
            i++;
            if ((i % (LE_CL_OPER_TIMEOUT / 100)) == 0)
            {
                AAL_MIN_LOG("__cl_fib_table_access : asic busy %d. [%s %d]\n", i / (LE_CL_OPER_TIMEOUT / 10), __FUNCTION__, __LINE__);
            }
        }
        if (i >= LE_CL_OPER_TIMEOUT)
        {
            AAL_CRI_LOG("__cl_fib_table_access : asic rw timeout.[%s %d]\n", __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
        }
    }

    /* waiting untill read-operation is completed */
    else if (op_flag == LE_CL_TABLE_READ)
    {   
        fib_access.bf.access = LE_CL_TABLE_INITIATE;
        fib_access.bf.rbw = LE_CL_TABLE_READ;
        fib_access.bf.address = addr & 0x1FF;           // only use low 9 bits
             
        base->CE_FIB_MEM_ACCESS.wrd = fib_access.wrd;

        access = base->CE_FIB_MEM_ACCESS.bf.access;
        while(access && (i < LE_CL_OPER_TIMEOUT))
        {
            access = base->CE_FIB_MEM_ACCESS.bf.access;
            i++;
            if ((i % (LE_CL_OPER_TIMEOUT / 100)) == 0)
            {
                AAL_MIN_LOG("__cl_fib_table_access : asic busy %d. [%s %d]\n", i / (LE_CL_OPER_TIMEOUT / 10), __FUNCTION__, __LINE__);
            }
        }
        if (i >= LE_CL_OPER_TIMEOUT)
        {
            AAL_CRI_LOG("__cl_fib_table_access : asic rw timeout.[%s %d]\n", __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
        }
        pdata->data0.wrd = base->CE_FIB_MEM_DATA0.wrd;
        pdata->data1.wrd = base->CE_FIB_MEM_DATA1.wrd;
        pdata->data2.wrd = base->CE_FIB_MEM_DATA2.wrd;  //&0x1FFFFF; /* only use low 21 bits */
    }

    cs_mutex_unlock(aal_fibtbl_mutex);
    return ret;
}

/***************************************************************************
* function    : __cl_specl_key_table_access                                *
* description : this function is used to perform CE special key table      *
*               read and write operation.  .                               *
* input       : operation flag(read or write) and data                     *
* output      : none                                                       *
* return      : CS_E_OK    - succeed in setting or getting entry           *
*               CS_E_ERROR - failed to set or get entry                    *
* globals     : none                                                       *
* registers   : FE_LE_CE_FIB_MEM_ACCESS                                    *
*               bit31 : 1 - asic busy                                      *
*                       0 - asic ready                                     *
*               bit30 : 1 - write                                          *
*                       0 - read                                           *
* others      :                                                            *
*                                                                          *
***************************************************************************/
static cs_status __cl_specl_key_table_access(cs_uint8 port_id, cs_uint8 op_flag, cs_uint8 addr, cs_uint32 *data1, cs_uint32 *data0)
{
    FE_LE_t *base = CL_PORT_BASE(port_id);
    FE_LE_CE_SPCL_KEY_MEM_ACCESS_t  spcl_access = base->CE_SPCL_KEY_MEM_ACCESS;
    cs_uint8 access = 0;
    cs_uint32 i     = 0;
    cs_status ret = CS_E_OK;

    if ((op_flag > 1)||(data1 ==NULL)||(data0 ==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    cs_mutex_lock(aal_spectbl_mutex);
    if (op_flag == LE_CL_TABLE_WRITE)
    {   
        
        base->CE_SPCL_KEY_MEM_DATA1.wrd = *data1; 
        base->CE_SPCL_KEY_MEM_DATA0.wrd = *data0; 
          
        spcl_access.bf.access  = LE_CL_TABLE_INITIATE;
        spcl_access.bf.rbw     = LE_CL_TABLE_WRITE;
        spcl_access.bf.address = addr & 0x7;                 // only use low 3 bits

        base->CE_SPCL_KEY_MEM_ACCESS.wrd = spcl_access.wrd;

        access = base->CE_SPCL_KEY_MEM_ACCESS.bf.access;
        while(access && (i < LE_CL_OPER_TIMEOUT))
        {
            access = base->CE_SPCL_KEY_MEM_ACCESS.bf.access;
            i++;
            if ((i % (LE_CL_OPER_TIMEOUT / 100)) == 0)
            {
                AAL_MIN_LOG("__cl_specl_key_table_access : asic busy %d. [%s %d]\n", i / (LE_CL_OPER_TIMEOUT / 10), __FUNCTION__, __LINE__);
            }
        }
        if (i >= LE_CL_OPER_TIMEOUT)
        {
            AAL_CRI_LOG("__cl_specl_key_table_access : asic rw timeout.[%s %d]\n", __FUNCTION__, __LINE__);
            ret =  CS_E_ERROR;
        }
    }

    /* waiting untill read-operation is completed */
    else if (op_flag == LE_CL_TABLE_READ)
    {   
        spcl_access.bf.access  = LE_CL_TABLE_INITIATE;
        spcl_access.bf.rbw     = LE_CL_TABLE_READ;
        spcl_access.bf.address = addr & 0x7;                  // only use low 3 bits
             
        base->CE_SPCL_KEY_MEM_ACCESS.wrd = spcl_access.wrd;

        access = base->CE_SPCL_KEY_MEM_ACCESS.bf.access;
        while(access && (i < LE_CL_OPER_TIMEOUT))
        {
            access = base->CE_SPCL_KEY_MEM_ACCESS.bf.access;
            i++;
            if ((i % (LE_CL_OPER_TIMEOUT / 100)) == 0)
            {
                AAL_MIN_LOG("__cl_specl_key_table_access : asic busy %d. [%s %d]\n", i / (LE_CL_OPER_TIMEOUT / 10), __FUNCTION__, __LINE__);
            }
        }
        if (i >= LE_CL_OPER_TIMEOUT)
        {
            AAL_CRI_LOG("__cl_specl_key_table_access : asic rw timeout.[%s %d]\n", __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
        }
        *data1 = base->CE_SPCL_KEY_MEM_DATA1.wrd;
        *data0 = base->CE_SPCL_KEY_MEM_DATA0.wrd;
    }

    cs_mutex_unlock(aal_spectbl_mutex);
    return ret;
}

/***************************************************************************
* function    : __cl_l4port_key_table_access                               *
* description : this function is used to perform CE l4 port key table      *
*               read and write operation.  .                               *
* input       : operation flag(read or write) and data                     *
* output      : none                                                       *
* return      : CS_E_OK    - succeed in setting or getting entry             *
*               CS_E_ERROR - failed to set or get entry                      *
* globals     : none                                                       *
* registers   : CE_L4_PORT_KEY_MEM_ACCESS                                  *
*               bit31 : 1 - asic busy                                      *
*                       0 - asic ready                                     *
*               bit30 : 1 - write                                          *
*                       0 - read                                           *
* others      :                                                            *
*                                                                          *
***************************************************************************/
static cs_status __cl_l4port_key_table_access(cs_uint8 port_id, cs_uint8 op_flag, cs_uint8 addr, cs_uint32 *data)
{
    FE_LE_t *base = CL_PORT_BASE(port_id);
    FE_LE_CE_L4_PORT_KEY_MEM_ACCESS_t  l4_access = base->CE_L4_PORT_KEY_MEM_ACCESS;
    cs_uint8 access = 0;
    cs_uint32     i = 0;
    cs_status ret = CS_E_OK;

    if ((op_flag > 1)||(data ==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    cs_mutex_lock(aal_l4port_mutex);
    if (op_flag == LE_CL_TABLE_WRITE)
    {   
        base->CE_L4_PORT_KEY_MEM_DATA.wrd = *data; 

        l4_access.bf.access  = LE_CL_TABLE_INITIATE;
        l4_access.bf.rbw     = LE_CL_TABLE_WRITE;
        l4_access.bf.address = addr & 0x1F; // only use low 5 bits

        base->CE_L4_PORT_KEY_MEM_ACCESS.wrd = l4_access.wrd;

        access = base->CE_L4_PORT_KEY_MEM_ACCESS.bf.access;
        while(access && (i < LE_CL_OPER_TIMEOUT))
        {
            access = base->CE_L4_PORT_KEY_MEM_ACCESS.bf.access;
            i++;
            if ((i % (LE_CL_OPER_TIMEOUT / 100)) == 0)
            {
                AAL_MIN_LOG("__cl_l4port_key_table_access : asic busy %d. [%s %d]\n", i / (LE_CL_OPER_TIMEOUT / 10), __FUNCTION__, __LINE__);
            }
        }
        if (i >= LE_CL_OPER_TIMEOUT)
        {
            AAL_CRI_LOG("__cl_l4port_key_table_access : asic rw timeout.[%s %d]\n", __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
        }
    }

    /* waiting untill read-operation is completed */
    else if (op_flag == LE_CL_TABLE_READ)
    {   
        l4_access.bf.access  = LE_CL_TABLE_INITIATE;
        l4_access.bf.rbw     = LE_CL_TABLE_READ;
        l4_access.bf.address = addr & 0x1F; // only use low 5 bits
             
        base->CE_L4_PORT_KEY_MEM_ACCESS.wrd = l4_access.wrd;

        access = base->CE_L4_PORT_KEY_MEM_ACCESS.bf.access;
        while(access && (i < LE_CL_OPER_TIMEOUT))
        {
            access = base->CE_L4_PORT_KEY_MEM_ACCESS.bf.access;
            i++;
            if ((i % (LE_CL_OPER_TIMEOUT / 100)) == 0)
            {
                AAL_MIN_LOG("__cl_l4port_key_table_access : asic busy %d. [%s %d]\n", i / (LE_CL_OPER_TIMEOUT / 10), __FUNCTION__, __LINE__);
            }
        }
        if (i >= LE_CL_OPER_TIMEOUT)
        {
            AAL_CRI_LOG("__cl_l4port_key_table_access : asic rw timeout.[%s %d]\n", __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
        }
        *data = base->CE_L4_PORT_KEY_MEM_DATA.wrd;

    }

    cs_mutex_unlock(aal_l4port_mutex);
    return CS_E_OK;
}

/***************************************************************************
* function    : __cl_mac_filter_aging_table_access                         *
* description : this function is used to perform CE mac filter aging table *
*               read and write operation.  .                               *
* input       : operation flag(read or write) and data                     *
* output      : none                                                       *
* return      : CS_E_OK    - succeed in setting or getting entry             *
*               CS_E_ERROR - failed to set or get entry                      *
* globals     : none                                                       *
* registers   : CE_MAC_FLTR_AGNG_MEM_ACCESS                                *
*               bit31 : 1 - asic busy                                      *
*                       0 - asic ready                                     *
*               bit30 : 1 - write                                          *
*                       0 - read                                           *
* others      :                                                            *
*                                                                          *
***************************************************************************/
static cs_status __cl_mac_filter_aging_table_access(cs_uint8 port_id, cs_uint8 op_flag, cs_uint8 addr, cs_uint32 *data)
{
    FE_LE_t *base = CL_PORT_BASE(port_id);
    FE_LE_CE_MAC_FLTR_AGNG_MEM_ACCESS_t  age_access = base->CE_MAC_FLTR_AGNG_MEM_ACCESS;
    cs_uint8 access = 0;
    cs_uint32     i = 0;
    cs_status ret = CS_E_OK;

    if ((op_flag > 1)||(data ==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    cs_mutex_lock(aal_filter_mutex);
    if (op_flag == LE_CL_TABLE_WRITE)
    {   
        base->CE_MAC_FLTR_AGNG_MEM_DATA.wrd = *data&0x1FFFFFF;    /* only use low 25 bits */ 

        age_access.bf.access  = LE_CL_TABLE_INITIATE;
        age_access.bf.rbw     = LE_CL_TABLE_WRITE;
        age_access.bf.address = addr & 0x3F;                      // only use low 6 bits

        base->CE_MAC_FLTR_AGNG_MEM_ACCESS.wrd = age_access.wrd;

        access = base->CE_MAC_FLTR_AGNG_MEM_ACCESS.bf.access;
        while(access && (i < LE_CL_OPER_TIMEOUT))
        {
            access = base->CE_MAC_FLTR_AGNG_MEM_ACCESS.bf.access;
            i++;
            if ((i % (LE_CL_OPER_TIMEOUT / 100)) == 0)
            {
                AAL_MIN_LOG("__cl_mac_filter_aging_table_access : asic busy %d. [%s %d]\n", i / (LE_CL_OPER_TIMEOUT / 10), __FUNCTION__, __LINE__);
            }
        }
        if (i >= LE_CL_OPER_TIMEOUT)
        {
            AAL_CRI_LOG("__cl_mac_filter_aging_table_access : asic rw timeout.[%s %d]\n", __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
        }
    }

    /* waiting untill read-operation is completed */
    else if (op_flag == LE_CL_TABLE_READ)
    {   
        
        age_access.bf.access  = LE_CL_TABLE_INITIATE;
        age_access.bf.rbw     = LE_CL_TABLE_READ;
        age_access.bf.address = addr & 0x3F;                     // only use low 6 bits
             
        base->CE_MAC_FLTR_AGNG_MEM_ACCESS.wrd = age_access.wrd;

        access = base->CE_MAC_FLTR_AGNG_MEM_ACCESS.bf.access;
        while(access && (i < LE_CL_OPER_TIMEOUT))
        {
            access = base->CE_MAC_FLTR_AGNG_MEM_ACCESS.bf.access;
            i++;
            if ((i % (LE_CL_OPER_TIMEOUT / 100)) == 0)
            {
                AAL_MIN_LOG("__cl_mac_filter_aging_table_access : asic busy %d. [%s %d]\n", i / (LE_CL_OPER_TIMEOUT / 10), __FUNCTION__, __LINE__);
            }
        }
        if (i >= LE_CL_OPER_TIMEOUT)
        {
            AAL_CRI_LOG("__cl_mac_filter_aging_table_access : asic rw timeout.[%s %d]\n", __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
        }
        *data = base->CE_MAC_FLTR_AGNG_MEM_DATA.wrd;
    }

    cs_mutex_unlock(aal_filter_mutex);
    return ret;
}

/**********************************************************************************
* function    : __cl_ipv4_long_key_entry_set
* description :  indirect  write ce ipv4_long_key memory table
*
* input       :      port_id (0-2)
*                    addr(phy table index): 0-47 (MA 0-15 )

*                    pdata:cl_ipv4_long_key_t *
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 

* others      :
*
*************************************************************************************/
cs_status __cl_ipv4_long_key_entry_set(cs_uint8 port_id, cs_uint16 addr, cl_ipv4_long_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cs_status ret =0;

    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));
    keydata.data9.bf12.kt_cl_key_type = CL_IPV4_LONG_KEY;
    
    if(pdata->mask)
    {
        //  1,Ignore the field
        keydata.data9.bf0.kt0_msk_spid_fld          = 1;
        keydata.data9.bf0.kt0_msk_p_tag_fld         = 1;
        keydata.data9.bf0.kt0_msk_tpid_enc_fld      = 1;
        keydata.data9.bf0.kt0_msk_igmp_type_fld     = 1;
        keydata.data9.bf0.kt0_msk_icmp_type_fld     = 1;
        keydata.data9.bf0.kt0_msk_tcp_flg_fld       = 1;
        keydata.data9.bf0.kt0_msk_l4_port_fld       = 1;
        keydata.data9.bf0.kt0_msk_ip_opt_flg_fld    = 1;
        keydata.data9.bf0.kt0_msk_dscp_fld          = 1;
        keydata.data9.bf0.kt0_msk_ippt_fld          = 1;
        keydata.data9.bf0.kt0_msk_vlan_cnt_fld      = 1;
        keydata.data9.bf0.kt0_msk_prsr_len_excd_fld = 1;
        keydata.data8.bf0.kt0_msk_802_1p_fld        = 1;
        keydata.data8.bf0.kt0_msk_vlanid_fld        = 1;
        keydata.data8.bf0.kt0_msk_ev2pt_fld         = 1;
        keydata.data8.bf0.kt0_msk_mac_sa_fld        = 1;
        keydata.data8.bf0.kt0_msk_mac_da_fld        = 1;
        //0,Ignore the field
        keydata.data9.bf0.kt0_msk_ip_sa_fld         = 0;
        keydata.data9.bf0.kt0_msk_ip_da_fld         = 0;
        keydata.data7.bf0.kt0_spcl_hit_map          = 0;
        keydata.data7.bf0.kt0_l4_dest_hit_map       = 0;
        keydata.data7.bf0.kt0_l4_src_hit_map        = 0;
        keydata.data6.bf0.kt0_ip_fragment_state     = 0x3;

        if(pdata->mask&CL_KEY_MASK_ALWAYS_MATCH)
            goto reg_write;
               
        if(pdata->mask&CL_KEY_MASK_SPID)
        {
            keydata.data8.bf0.kt0_spid = pdata->spid;
            keydata.data9.bf0.kt0_msk_spid_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_VLAN_COUNT)
        {
            keydata.data8.bf0.kt0_vlan_cnt = pdata->vlan_count;
            keydata.data9.bf0.kt0_msk_vlan_cnt_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_P_TAG)
        {
            keydata.data8.bf0.kt0_p_tag_flag = pdata->p_tag_flag;
            keydata.data9.bf0.kt0_msk_p_tag_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_TPID)
        {
            keydata.data8.bf0.kt0_tpid_enc = pdata->tpid_enc;
            keydata.data9.bf0.kt0_msk_tpid_enc_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_IGMP_TYPE)
        {
            keydata.data8.bf0.kt0_igmp_type = pdata->igmp_type;
            keydata.data9.bf0.kt0_msk_igmp_type_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_ICMP_TYPE)
        {
            keydata.data8.bf0.kt0_icmp_type = pdata->icmp_type;
            keydata.data9.bf0.kt0_msk_icmp_type_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_TCP_FLAGS)
        {
            if(pdata->tcp_flags > 63 )
                return CS_E_ERROR;
            keydata.data8.bf0.kt0_tcp_flg = pdata->tcp_flags>>4;
            keydata.data7.bf0.kt0_tcp_flg = pdata->tcp_flags&0xF;
            keydata.data9.bf0.kt0_msk_tcp_flg_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_L4PORT)
        {
            keydata.data7.bf0.kt0_l4_port_type = pdata->l4_port_type;
            keydata.data7.bf0.kt0_l4_port = pdata->l4_port>>13;
            keydata.data6.bf0.kt0_l4_port = pdata->l4_port&0x1FFF;
            keydata.data9.bf0.kt0_msk_l4_port_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_IPFRAGMENT)
        {
            if(pdata->ip_fragment_state > 2 )
                return CS_E_ERROR;
            keydata.data6.bf0.kt0_ip_fragment_state = pdata->ip_fragment_state;
        }
        if(pdata->mask&CL_KEY_MASK_IPOPTION)
        {
            if(pdata->ip_option_flag > 1 )
                return CS_E_ERROR;
            keydata.data6.bf0.kt0_ip_opt_flg = pdata->ip_option_flag;
            keydata.data9.bf0.kt0_msk_ip_opt_flg_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_DSCP)
        {
            keydata.data6.bf0.kt0_dscp_tos = pdata->ipdscp;
            keydata.data9.bf0.kt0_msk_dscp_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_IPPROTO)
        {
            keydata.data6.bf0.kt0_ippt = pdata->ipprotocol;
            keydata.data9.bf0.kt0_msk_ippt_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_IPSA)
        {
            keydata.data5.bf0.kt0_ip_sa = pdata->srcip;
            keydata.data9.bf0.kt0_msk_ip_sa_fld = pdata->srcipmask;
        }
        if(pdata->mask&CL_KEY_MASK_IPDA)
        {
            keydata.data4.bf0.kt0_ip_da = pdata->dstip;
            keydata.data9.bf0.kt0_msk_ip_da_fld = pdata->dstipmask;
        }
        if(pdata->mask&CL_KEY_MASK_EV2PT)
        {
            keydata.data3.bf0.kt0_ev2pt = pdata->ev2pt;
            keydata.data8.bf0.kt0_msk_ev2pt_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_MACSA)
        {
            keydata.data3.bf0.kt0_mac_sa = ((pdata->srcmac[0]<<8) | (pdata->srcmac[1]));
            keydata.data2.bf0.kt0_mac_sa = ((pdata->srcmac[2]<<24)
                                                             | (pdata->srcmac[3]<<16)
                                                             | (pdata->srcmac[4]<<8)
                                                             | (pdata->srcmac[5]));
            keydata.data8.bf0.kt0_msk_mac_sa_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_MACDA)
        {
            keydata.data1.bf0.kt0_mac_da = ((pdata->dstmac[0]<<8) | (pdata->dstmac[1]));
            keydata.data0.bf0.kt0_mac_da = ((pdata->dstmac[2]<<24)
                                                             | (pdata->dstmac[3]<<16)
                                                             | (pdata->dstmac[4]<<8)
                                                             | (pdata->dstmac[5]));
            keydata.data8.bf0.kt0_msk_mac_da_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
        {
            keydata.data1.bf0.kt0_prsr_len_excd = pdata->prsr_len_excds;
            keydata.data9.bf0.kt0_msk_prsr_len_excd_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_DOT1P)
        {
            keydata.data1.bf0.kt0_vl_802_1p = pdata->dot1p;
            keydata.data8.bf0.kt0_msk_802_1p_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_VLANID)
        {
            keydata.data1.bf0.kt0_vlanid = pdata->vlanid;
            keydata.data8.bf0.kt0_msk_vlanid_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_SPECPKT)
        {   
            keydata.data7.bf0.kt0_spcl_hit_map = pdata->spcl_hit_map;
        }
        if(pdata->mask&CL_KEY_MASK_L4SRC_RANGE)
        {  
            keydata.data7.bf0.kt0_l4_src_hit_map= pdata->l4_src_hit_map;
        }
        if(pdata->mask&CL_KEY_MASK_L4DST_RANGE)
        {   
            keydata.data7.bf0.kt0_l4_dest_hit_map= pdata->l4_dest_hit_map;
        }
    }

 reg_write:
    
     /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);

    return ret;
}

/**********************************************************************************
* function    : __cl_ipv4_short_key_entry_set
* description :  indirect write ce cl_ipv4_short_key_t  memory table
*
* input       :      port_id (0-2)
*                    addr(phy table entry index): 0-47 (MA 0-15 )
*
*                    pdata:cl_ipv4_short_key_t data[4]
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 
*
* others      :
*
*************************************************************************************/
cs_status __cl_ipv4_short_key_entry_set(cs_uint8 port_id, cs_uint16 addr,  cl_ipv4_short_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cl_ipv4_short_key_t *pkey= NULL;
    cs_status ret =0;
    cs_uint32 i;
    
    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));

    keydata.data9.bf12.kt_cl_key_type = CL_IPV4_SHORT_KEY;
    
    for (i = 0; i<4; i++)
    {  
        pkey = (cl_ipv4_short_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_ipv4_short_key_t));
    
        if(pkey->mask==0)
            continue;   
        switch (i)
        {
            case 3:
            {
                keydata.data2.bf3.kt1_3_msk_dscp_tos_fld =1;
                keydata.data2.bf3.kt1_3_msk_ippt_fld =1;
                keydata.data2.bf3.kt1_3_msk_l4_port_fld =1;
                keydata.data9.bf3.kt1_3_msk_prsr_len_excd_fld =1;
                keydata.data2.bf3.kt1_3_msk_ip_addr_fld =0;

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                
                if(pkey->mask&CL_KEY_MASK_IPADDR)
                {
                    keydata.data0.bf3.kt1_3_ip_addr = pkey->ip;
                    keydata.data2.bf3.kt1_3_ip_addr_type = pkey->ip_type;
                    keydata.data2.bf3.kt1_3_msk_ip_addr_fld = pkey->ipmask;          
                }
                if(pkey->mask&CL_KEY_MASK_DSCP)
                {
                    keydata.data1.bf3.kt1_3_dscp_tos = pkey->dscp_tos;
                    keydata.data2.bf3.kt1_3_msk_dscp_tos_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_IPPROTO)
                {
                    keydata.data1.bf3.kt1_3_ippt = pkey->ipprotocol;
                    keydata.data2.bf3.kt1_3_msk_ippt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_L4PORT)
                {
                    keydata.data1.bf3.kt1_3_l4_port = pkey->l4_port;
                    keydata.data2.bf3.kt1_3_l4_port_type = pkey->l4_port_type;
                    keydata.data2.bf3.kt1_3_msk_l4_port_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
                {
                    keydata.data9.bf3.kt1_3_prsr_len_excd = pkey->prsr_len_excds;
                    keydata.data9.bf3.kt1_3_msk_prsr_len_excd_fld =0;
                }
                break;
            }
            case 2:
            {
                keydata.data4.bf3.kt1_2_msk_dscp_tos_fld =1;
                keydata.data4.bf3.kt1_2_msk_ippt_fld =1;
                keydata.data4.bf3.kt1_2_msk_l4_port_fld =1;
                keydata.data9.bf3.kt1_2_msk_prsr_len_excd_fld =1;
                keydata.data4.bf3.kt1_2_msk_ip_addr_fld = 0;

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                                
                if(pkey->mask&CL_KEY_MASK_IPADDR)
                {
                    keydata.data3.bf3.kt1_2_ip_addr = (pkey->ip>>21)&0x7FF;
                    keydata.data2.bf3.kt1_2_ip_addr = pkey->ip&0x1FFFFF;
                    keydata.data4.bf3.kt1_2_ip_addr_type = pkey->ip_type;
                    keydata.data4.bf3.kt1_2_msk_ip_addr_fld = pkey->ipmask;          
                }
                if(pkey->mask&CL_KEY_MASK_DSCP)
                {
                    keydata.data4.bf3.kt1_2_dscp_tos = pkey->dscp_tos;
                    keydata.data4.bf3.kt1_2_msk_dscp_tos_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_IPPROTO)
                {
                    keydata.data4.bf3.kt1_2_ippt = (pkey->ipprotocol>>5)&0x7;
                    keydata.data3.bf3.kt1_2_ippt = pkey->ipprotocol&0x1F;
                    keydata.data4.bf3.kt1_2_msk_ippt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_L4PORT)
                {
                    keydata.data3.bf3.kt1_2_l4_port = pkey->l4_port;
                    keydata.data4.bf3.kt1_2_l4_port_type = pkey->l4_port_type;
                    keydata.data4.bf3.kt1_2_msk_l4_port_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
                {
                    keydata.data9.bf3.kt1_2_prsr_len_excd = pkey->prsr_len_excds;
                    keydata.data9.bf3.kt1_2_msk_prsr_len_excd_fld =0;
                }
                break;
            }
            case 1:
            {
                keydata.data7.bf3.kt1_1_msk_dscp_tos_fld =1;
                keydata.data6.bf3.kt1_1_msk_ippt_fld =1;
                keydata.data6.bf3.kt1_1_msk_l4_port_fld =1;
                keydata.data9.bf3.kt1_1_msk_prsr_len_excd_fld =1;
                keydata.data6.bf3.kt1_1_msk_ip_addr_fld =0;

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                                
                if(pkey->mask&CL_KEY_MASK_IPADDR)
                {
                    keydata.data5.bf3.kt1_1_ip_addr = (pkey->ip>>10)&0x3FFFFF;
                    keydata.data4.bf3.kt1_1_ip_addr = pkey->ip&0x3FF;
                    keydata.data6.bf3.kt1_1_ip_addr_type = pkey->ip_type;
                    keydata.data6.bf3.kt1_1_msk_ip_addr_fld = pkey->ipmask;          
                }
                if(pkey->mask&CL_KEY_MASK_DSCP)
                {
                    keydata.data6.bf3.kt1_1_dscp_tos = pkey->dscp_tos;
                    keydata.data7.bf3.kt1_1_msk_dscp_tos_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_IPPROTO)
                {
                    keydata.data6.bf3.kt1_1_ippt = pkey->ipprotocol;
                    keydata.data6.bf3.kt1_1_msk_ippt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_L4PORT)
                {
                    keydata.data6.bf3.kt1_1_l4_port = (pkey->l4_port>>10)&0x3F;
                    keydata.data5.bf3.kt1_1_l4_port = pkey->l4_port&0x3FF;
                    keydata.data6.bf3.kt1_1_l4_port_type = pkey->l4_port_type;
                    keydata.data6.bf3.kt1_1_msk_l4_port_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
                {
                    keydata.data9.bf3.kt1_1_prsr_len_excd = pkey->prsr_len_excds;
                    keydata.data9.bf3.kt1_1_msk_prsr_len_excd_fld =0;
                }
                break;
            }
            case 0:
            {
                keydata.data9.bf3.kt1_0_msk_dscp_tos_fld =1;
                keydata.data9.bf3.kt1_0_msk_ippt_fld =1;
                keydata.data9.bf3.kt1_0_msk_l4_port_fld =1;
                keydata.data9.bf3.kt1_0_msk_prsr_len_excd_fld =1;
                keydata.data9.bf3.kt1_0_msk_ip_addr_fld =0;

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                               
                if(pkey->mask&CL_KEY_MASK_IPADDR)
                {
                    keydata.data8.bf3.kt1_0_ip_addr = (pkey->ip>>31)&0x1;
                    keydata.data7.bf3.kt1_0_ip_addr = pkey->ip&0X7FFFFFFF;
                    keydata.data9.bf3.kt1_0_ip_addr_type = pkey->ip_type;
                    keydata.data9.bf3.kt1_0_msk_ip_addr_fld = pkey->ipmask;          
                }
                if(pkey->mask&CL_KEY_MASK_DSCP)
                {
                    keydata.data9.bf3.kt1_0_dscp_tos = (pkey->dscp_tos>>7)&0x1;
                    keydata.data8.bf3.kt1_0_dscp_tos = pkey->dscp_tos&0x7F;
                    keydata.data9.bf3.kt1_0_msk_dscp_tos_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_IPPROTO)
                {
                    keydata.data8.bf3.kt1_0_ippt = pkey->ipprotocol;
                    keydata.data9.bf3.kt1_0_msk_ippt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_L4PORT)
                {
                    keydata.data8.bf3.kt1_0_l4_port = pkey->l4_port;
                    keydata.data9.bf3.kt1_0_l4_port_type = pkey->l4_port_type;
                    keydata.data9.bf3.kt1_0_msk_l4_port_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
                {
                    keydata.data9.bf3.kt1_0_prsr_len_excd = pkey->prsr_len_excds;
                    keydata.data9.bf3.kt1_0_msk_prsr_len_excd_fld =0;
                }
                break;
            }
             default:
                break;
        }
    }

    /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);

    return ret;
}

/**********************************************************************************
* function    : __cl_ipv6_long_key_entry_set
* description :  indirect  write ce ipv6_long_key memory table
*
* input       :      port_id (0-2)
*                    addr(phy table index): 0-46 (MA 0-14 ) one rule two entry

*                    pdata:cl_ipv6_long_key_t *
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 

* others      :
*
*************************************************************************************/
cs_status __cl_ipv6_long_key_entry_set(cs_uint8 port_id, cs_uint16 addr, cl_ipv6_long_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cs_status ret =0;

    if((pdata==NULL)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    if(port_id == AAL_FE_MA_ID)
    {
        if(addr>14)
        {
            AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_PARAM;
        }
    }
    else
    {
        if(addr>46)
        {
            AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_PARAM;
        }
    }
    
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));
    keydata.data9.bf12.kt_cl_key_type = CL_IPV6_LONG_KEY;
    if(pdata->mask)
    {
         //Ignore the field
        keydata.data9.bf1.kt2_0_msk_ipv6_flow_label_fld = 1;
        keydata.data8.bf1.kt2_0_msk_ip_sa_fld = 0;
        keydata.data8.bf1.kt2_0_msk_ip_da_fld = 0;
        if(!(pdata->mask&CL_KEY_MASK_ALWAYS_MATCH))
        {
            if(pdata->mask&CL_KEY_MASK_FLOW_LABEL)
            {
                keydata.data9.bf1.kt2_0_ipv6_flow_label = (pdata->flow_label>>16)&0xF;
                keydata.data8.bf1.kt2_0_ipv6_flow_label = pdata->flow_label&0xFFFF;
                keydata.data9.bf1.kt2_0_msk_ipv6_flow_label_fld = 0;
            }
            if(pdata->mask&CL_KEY_MASK_IPDA)
            {            
                keydata.data8.bf1.kt2_0_msk_ip_da_fld = pdata->dstipmask;  
                keydata.data3.bf1.kt2_0_ip_da = ((pdata->ipv6_da[0]<<16) |(pdata->ipv6_da[1]));
                keydata.data2.bf1.kt2_0_ip_da = ((pdata->ipv6_da[2]<<16) |(pdata->ipv6_da[3]));
                keydata.data1.bf1.kt2_0_ip_da = ((pdata->ipv6_da[4]<<16) |(pdata->ipv6_da[5]));
                keydata.data0.bf1.kt2_0_ip_da = ((pdata->ipv6_da[6]<<16) |(pdata->ipv6_da[7]));
            }
            if(pdata->mask&CL_KEY_MASK_IPSA)
            {             
                keydata.data8.bf1.kt2_0_msk_ip_sa_fld = pdata->srcipmask;  
                keydata.data7.bf1.kt2_0_ip_sa = ((pdata->ipv6_sa[0]<<16) |(pdata->ipv6_sa[1]));
                keydata.data6.bf1.kt2_0_ip_sa = ((pdata->ipv6_sa[2]<<16) |(pdata->ipv6_sa[3]));
                keydata.data5.bf1.kt2_0_ip_sa = ((pdata->ipv6_sa[4]<<16) |(pdata->ipv6_sa[5]));
                keydata.data4.bf1.kt2_0_ip_sa = ((pdata->ipv6_sa[6]<<16) |(pdata->ipv6_sa[7]));
            }
        }
    }
        /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);
    if (ret)
    {   
        AAL_MIN_LOG("ipv6_long_key entry 0 write error.\n");
        return CS_E_ERROR;
    }
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));
    keydata.data9.bf12.kt_cl_key_type = CL_IPV6_LONG_KEY;
    if(pdata->mask)
    {
         //  1,Ignore the field
        keydata.data9.bf2.kt2_1_msk_spid_fld          = 1;
        keydata.data9.bf2.kt2_1_msk_p_tag_fld         = 1;
        keydata.data9.bf2.kt2_1_msk_tpid_enc_fld      = 1;
        keydata.data9.bf2.kt2_1_msk_igmp_type_fld     = 1;
        keydata.data9.bf2.kt2_1_msk_icmp_type_fld     = 1;
        keydata.data9.bf2.kt2_1_msk_tcp_flg_fld       = 1;
        keydata.data9.bf2.kt2_1_msk_l4_port_fld       = 1;
        keydata.data9.bf2.kt2_1_msk_ip_opt_flg_fld    = 1;
        keydata.data9.bf2.kt2_1_msk_dscp_fld          = 1;
        keydata.data9.bf2.kt2_1_msk_ippt_fld          = 1;
        keydata.data9.bf2.kt2_1_msk_vlan_cnt_fld      = 1;
        keydata.data9.bf2.kt2_1_msk_prsr_len_excd_fld = 1;
        keydata.data8.bf2.kt2_1_msk_802_1p_fld        = 1;
        keydata.data8.bf2.kt2_1_msk_vlanid_fld        = 1;
        keydata.data8.bf2.kt2_1_msk_ev2pt_fld         = 1;
        keydata.data8.bf2.kt2_1_msk_mac_sa_fld        = 1;
        keydata.data8.bf2.kt2_1_msk_mac_da_fld        = 1;
        //0,Ignore the field
        keydata.data7.bf2.kt2_1_spcl_hit_map          = 0;
        keydata.data7.bf2.kt2_1_l4_dest_hit_map       = 0;
        keydata.data7.bf2.kt2_1_l4_src_hit_map        = 0;
        keydata.data6.bf2.kt2_1_ip_fragment_state     = 0x3;

        if(pdata->mask&CL_KEY_MASK_ALWAYS_MATCH)
            goto reg_write;
           
        if(pdata->mask&CL_KEY_MASK_SPID)
        {
            keydata.data8.bf2.kt2_1_spid = pdata->spid;
            keydata.data9.bf2.kt2_1_msk_spid_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_VLAN_COUNT)
        {
            keydata.data8.bf2.kt2_1_vlan_cnt = pdata->vlan_count;
            keydata.data9.bf2.kt2_1_msk_vlan_cnt_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_P_TAG)
        {
            keydata.data8.bf2.kt2_1_p_tag_flag = pdata->p_tag_flag;
            keydata.data9.bf2.kt2_1_msk_p_tag_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_TPID)
        {
            keydata.data8.bf2.kt2_1_tpid_enc = pdata->tpid_enc;
            keydata.data9.bf2.kt2_1_msk_tpid_enc_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_IGMP_TYPE)
        {
            keydata.data8.bf2.kt2_1_igmp_type = pdata->igmp_type;
            keydata.data9.bf2.kt2_1_msk_igmp_type_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_ICMP_TYPE)
        {
            keydata.data8.bf2.kt2_1_icmp_type = pdata->icmp_type;
            keydata.data9.bf2.kt2_1_msk_icmp_type_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_TCP_FLAGS)
        {
            if(pdata->tcp_flags > 63 )
                return CS_E_ERROR;
            keydata.data8.bf2.kt2_1_tcp_flg = pdata->tcp_flags>>4;
            keydata.data7.bf2.kt2_1_tcp_flg = pdata->tcp_flags&0xF;
            keydata.data9.bf2.kt2_1_msk_tcp_flg_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_L4PORT)
        {
            keydata.data7.bf2.kt2_1_l4_port_type = pdata->l4_port_type;
            keydata.data7.bf2.kt2_1_l4_port = pdata->l4_port>>13;
            keydata.data6.bf2.kt2_1_l4_port = pdata->l4_port&0x1FFF;
            keydata.data9.bf2.kt2_1_msk_l4_port_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_IPFRAGMENT)
        {
            if(pdata->ip_fragment_state > 2 )
                return CS_E_ERROR;
            keydata.data6.bf2.kt2_1_ip_fragment_state = pdata->ip_fragment_state;
        }
        if(pdata->mask&CL_KEY_MASK_IPOPTION)
        {
            keydata.data6.bf2.kt2_1_ip_opt_flg = pdata->ip_option_flag;
            keydata.data9.bf2.kt2_1_msk_ip_opt_flg_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_DSCP)
        {
            keydata.data6.bf2.kt2_1_dscp_tos = pdata->ipdscp;
            keydata.data9.bf2.kt2_1_msk_dscp_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_IPPROTO)
        {
            keydata.data6.bf2.kt2_1_ippt = pdata->ipprotocol;
            keydata.data9.bf2.kt2_1_msk_ippt_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_EV2PT)
        {
            keydata.data3.bf2.kt2_1_ev2pt = pdata->ev2pt;
            keydata.data8.bf2.kt2_1_msk_ev2pt_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_MACSA)
        {
            keydata.data3.bf2.kt2_1_mac_sa = ((pdata->srcmac[0]<<8) | (pdata->srcmac[1]));
            keydata.data2.bf2.kt2_1_mac_sa = ((pdata->srcmac[2]<<24)
                                                                 | (pdata->srcmac[3]<<16)
                                                                 | (pdata->srcmac[4]<<8)
                                                                 | (pdata->srcmac[5]));
            keydata.data8.bf2.kt2_1_msk_mac_sa_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_MACDA)
        {
            keydata.data1.bf2.kt2_1_mac_da = ((pdata->dstmac[0]<<8) | (pdata->dstmac[1]));
            keydata.data0.bf2.kt2_1_mac_da = ((pdata->dstmac[2]<<24)
                                                                 | (pdata->dstmac[3]<<16)
                                                                 | (pdata->dstmac[4]<<8)
                                                                 | (pdata->dstmac[5]));
            keydata.data8.bf2.kt2_1_msk_mac_da_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
        {
            keydata.data1.bf2.kt2_1_prsr_len_excd = pdata->prsr_len_excds;
            keydata.data9.bf2.kt2_1_msk_prsr_len_excd_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_DOT1P)
        {
            keydata.data1.bf2.kt2_1_vl_802_1p = pdata->dot1p;
            keydata.data8.bf2.kt2_1_msk_802_1p_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_VLANID)
        {
            keydata.data1.bf2.kt2_1_vlanid = pdata->vlanid;
            keydata.data8.bf2.kt2_1_msk_vlanid_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_SPECPKT)
        {   
            keydata.data7.bf2.kt2_1_spcl_hit_map = pdata->spcl_hit_map;
        }
        if(pdata->mask&CL_KEY_MASK_L4SRC_RANGE)
        {  
            keydata.data7.bf2.kt2_1_l4_src_hit_map= pdata->l4_src_hit_map;
        }
        if(pdata->mask&CL_KEY_MASK_L4DST_RANGE)
        {   
            keydata.data7.bf2.kt2_1_l4_dest_hit_map= pdata->l4_dest_hit_map;
        }
    }
    
reg_write:        
     /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, (addr+1), &keydata);
 
    return ret;
}

/**********************************************************************************
* function    : __cl_ipv6_short_key_entry_set
* description :  indirect write ce cl_ipv6_short_key_t  memory table
*
* input       :      port_id (0-2)
*                    addr(phy table entry index): 0-47 (MA 0-15 )
*
*                    pdata:cl_ipv6_short_key_t  data
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 
*
* others      :
*
*************************************************************************************/
cs_status __cl_ipv6_short_key_entry_set(cs_uint8 port_id, cs_uint16 addr,  cl_ipv6_short_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cs_status ret =0;

    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));
    keydata.data9.bf12.kt_cl_key_type = CL_IPV6_SHORT_KEY;
    if(pdata->mask)
    {
        //  1,Ignore the field
        keydata.data6.bf4.kt3_0_msk_prsr_len_excd_fld   = 1; 
        keydata.data6.bf4.kt3_0_msk_ipv6_flow_label_fld = 1; 
        keydata.data5.bf4.kt3_0_msk_dscp_tos_fld        = 1;    
        keydata.data5.bf4.kt3_0_msk_ippt_fld            = 1;    
        keydata.data5.bf4.kt3_0_msk_l4_port_fld         = 1;    
        keydata.data5.bf4.kt3_0_msk_ip_addr_fld         = 0;    

        if(pdata->mask&CL_KEY_MASK_ALWAYS_MATCH)
            goto regwrite;
            
        if(pdata->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
        {
            keydata.data6.bf4.kt3_0_prsr_len_excd = pdata->prsr_len_excds;
            keydata.data6.bf4.kt3_0_msk_prsr_len_excd_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_FLOW_LABEL)
        {
            keydata.data6.bf4.kt3_0_ipv6_flow_label = (pdata->flow_label>>19)&0x1;
            keydata.data5.bf4.kt3_0_ipv6_flow_label = pdata->flow_label&0x7FFFF;
            keydata.data6.bf4.kt3_0_msk_ipv6_flow_label_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_DSCP)
        {
            keydata.data4.bf4.kt3_0_dscp_tos = pdata->dscp_tos;
            keydata.data5.bf4.kt3_0_msk_dscp_tos_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_IPPROTO)
        {
            keydata.data4.bf4.kt3_0_ippt = pdata->ipprotocol;
            keydata.data5.bf4.kt3_0_msk_ippt_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_L4PORT)
        {
            keydata.data5.bf4.kt3_0_l4_port_type = pdata->l4_port_type;
            keydata.data4.bf4.kt3_0_l4_port = pdata->l4_port;     
            keydata.data5.bf4.kt3_0_msk_l4_port_fld = 0;
        }
        if(pdata->mask&CL_KEY_MASK_IPADDR)
        {       
            keydata.data5.bf4.kt3_0_ip_addr_type = pdata->ip_type;
            keydata.data5.bf4.kt3_0_msk_ip_addr_fld = pdata->ipmask;  
            keydata.data3.bf4.kt3_0_ip_addr = ((pdata->ipv6_addr[0]<<16) |(pdata->ipv6_addr[1]));
            keydata.data2.bf4.kt3_0_ip_addr = ((pdata->ipv6_addr[2]<<16) |(pdata->ipv6_addr[3]));
            keydata.data1.bf4.kt3_0_ip_addr = ((pdata->ipv6_addr[4]<<16) |(pdata->ipv6_addr[5]));
            keydata.data0.bf4.kt3_0_ip_addr = ((pdata->ipv6_addr[6]<<16) |(pdata->ipv6_addr[7]));
        }
    }
    
regwrite:    
   /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);
 
    return ret;
}

/**********************************************************************************
* function    : __cl_mirror_key_entry_set
* description :  indirect  write ce mirror_key  memory table
*
* input       :      port_id (0-2)
*                    addr(phy table entry index): 0-47 (MA 0-15 )
*                    pdata:cl_mirror_key_t  data[8]
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 
*
* others      :
*
*************************************************************************************/
cs_status __cl_mirror_key_entry_set(cs_uint8 port_id, cs_uint16 addr, cl_mirror_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cl_mirror_key_t *pkey= NULL;
    cs_status ret =0;
    cs_uint32 i;

    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));
    keydata.data9.bf12.kt_cl_key_type = CL_MIRROR_KEY;

    for (i = 0; i<8; i++)
    {  
        pkey = (cl_mirror_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_mirror_key_t));
        
        if(pkey->mask==0)
            continue;   
        switch (i)
        {
            case 0:
            {        
                keydata.data0.bf5.kt4_0_msk_vlanid_fld =1;
                keydata.data0.bf5.kt4_0_msk_spid_fld =1;
                keydata.data0.bf5.kt4_0_msk_mirror_group_id_fld =1;
                keydata.data1.bf5.kt4_0_mirror_pkt_flag = pkey->mirror_pkt_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPID)
                {
                    keydata.data0.bf5.kt4_0_spid =  pkey->spid;                 
                    keydata.data0.bf5.kt4_0_msk_spid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data0.bf5.kt4_0_vlanid = pkey->vlanid ;
                    keydata.data0.bf5.kt4_0_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_MIRROR_GROUP)
                {
                    keydata.data0.bf5.kt4_0_mirror_group_id = pkey->mirror_group_id;
                    keydata.data0.bf5.kt4_0_msk_mirror_group_id_fld =0;
                }          
                break;
            }
            case 1:
            {        
                keydata.data1.bf5.kt4_1_msk_vlanid_fld =1;
                keydata.data1.bf5.kt4_1_msk_spid_fld =1;
                keydata.data1.bf5.kt4_1_msk_mirror_group_id_fld =1;
                keydata.data2.bf5.kt4_1_mirror_pkt_flag = pkey->mirror_pkt_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPID)
                {
                    keydata.data1.bf5.kt4_1_spid =  pkey->spid;                 
                    keydata.data1.bf5.kt4_1_msk_spid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data1.bf5.kt4_1_vlanid = pkey->vlanid ;
                    keydata.data1.bf5.kt4_1_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_MIRROR_GROUP)
                {
                    keydata.data1.bf5.kt4_1_mirror_group_id = pkey->mirror_group_id;
                    keydata.data1.bf5.kt4_1_msk_mirror_group_id_fld =0;
                }         
                break;
            }
            case 2:
            {        
                keydata.data2.bf5.kt4_2_msk_vlanid_fld =1;
                keydata.data2.bf5.kt4_2_msk_spid_fld =1;
                keydata.data3.bf5.kt4_2_msk_mirror_group_id_fld =1;
                keydata.data3.bf5.kt4_2_mirror_pkt_flag = pkey->mirror_pkt_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPID)
                {
                    keydata.data2.bf5.kt4_2_spid =  pkey->spid;                 
                    keydata.data2.bf5.kt4_2_msk_spid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data2.bf5.kt4_2_vlanid = pkey->vlanid ;
                    keydata.data2.bf5.kt4_2_msk_vlanid_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_MIRROR_GROUP)
                {
                    keydata.data3.bf5.kt4_2_mirror_group_id = (pkey->mirror_group_id>>1)&0xF;
                    keydata.data2.bf5.kt4_2_mirror_group_id = pkey->mirror_group_id&0x1;
                    keydata.data3.bf5.kt4_2_msk_mirror_group_id_fld =0;
                }       
                break;
            }
            case 3:
            {        
                keydata.data4.bf5.kt4_3_msk_vlanid_fld =1;
                keydata.data3.bf5.kt4_3_msk_spid_fld =1;
                keydata.data4.bf5.kt4_3_msk_mirror_group_id_fld =1;
                keydata.data4.bf5.kt4_3_mirror_pkt_flag = pkey->mirror_pkt_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPID)
                {
                    keydata.data3.bf5.kt4_3_spid =  pkey->spid;                 
                    keydata.data3.bf5.kt4_3_msk_spid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data4.bf5.kt4_3_vlanid = (pkey->vlanid>>7)&0x1F;
                    keydata.data3.bf5.kt4_3_vlanid = pkey->vlanid&0x7F;
                    keydata.data4.bf5.kt4_3_msk_vlanid_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_MIRROR_GROUP)
                {
                    keydata.data4.bf5.kt4_3_mirror_group_id = pkey->mirror_group_id;
                    keydata.data4.bf5.kt4_3_msk_mirror_group_id_fld =0;
                }       
                break;
            }
            case 4:
            {        
                keydata.data5.bf5.kt4_4_msk_vlanid_fld =1;
                keydata.data4.bf5.kt4_4_msk_spid_fld =1;
                keydata.data5.bf5.kt4_4_msk_mirror_group_id_fld =1;
                keydata.data6.bf5.kt4_4_mirror_pkt_flag = pkey->mirror_pkt_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPID)
                {
                    keydata.data4.bf5.kt4_4_spid =  pkey->spid;                 
                    keydata.data4.bf5.kt4_4_msk_spid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data5.bf5.kt4_4_vlanid = pkey->vlanid;                
                    keydata.data5.bf5.kt4_4_msk_vlanid_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_MIRROR_GROUP)
                {
                    keydata.data5.bf5.kt4_4_mirror_group_id = pkey->mirror_group_id;
                    keydata.data5.bf5.kt4_4_msk_mirror_group_id_fld =0;
                }       
                break;
            }
            case 5:
            {        
                keydata.data6.bf5.kt4_5_msk_vlanid_fld =1;
                keydata.data6.bf5.kt4_5_msk_spid_fld =1;
                keydata.data6.bf5.kt4_5_msk_mirror_group_id_fld =1;
                keydata.data7.bf5.kt4_5_mirror_pkt_flag = pkey->mirror_pkt_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPID)
                {
                    keydata.data6.bf5.kt4_5_spid =  pkey->spid;                 
                    keydata.data6.bf5.kt4_5_msk_spid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data6.bf5.kt4_5_vlanid = pkey->vlanid;                
                    keydata.data6.bf5.kt4_5_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_MIRROR_GROUP)
                {
                    keydata.data6.bf5.kt4_5_mirror_group_id = pkey->mirror_group_id;
                    keydata.data6.bf5.kt4_5_msk_mirror_group_id_fld =0;
                }       
                break;
            }
            case 6:
            {        
                keydata.data7.bf5.kt4_6_msk_vlanid_fld =1;
                keydata.data7.bf5.kt4_6_msk_spid_fld =1;
                keydata.data8.bf5.kt4_6_msk_mirror_group_id_fld =1;
                keydata.data8.bf5.kt4_6_mirror_pkt_flag = pkey->mirror_pkt_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPID)
                {
                    keydata.data7.bf5.kt4_6_spid =  pkey->spid;                 
                    keydata.data7.bf5.kt4_6_msk_spid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data7.bf5.kt4_6_vlanid = pkey->vlanid;                
                    keydata.data7.bf5.kt4_6_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_MIRROR_GROUP)
                {
                    keydata.data7.bf5.kt4_6_mirror_group_id = pkey->mirror_group_id;
                    keydata.data8.bf5.kt4_6_msk_mirror_group_id_fld =0;
                }    
                break;
            }
            case 7:
            {        
                keydata.data9.bf5.kt4_7_msk_vlanid_fld =1;
                keydata.data8.bf5.kt4_7_msk_spid_fld =1;
                keydata.data9.bf5.kt4_7_msk_mirror_group_id_fld =1;
                keydata.data9.bf5.kt4_7_mirror_pkt_flag = pkey->mirror_pkt_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPID)
                {
                    keydata.data8.bf5.kt4_7_spid =  pkey->spid;                 
                    keydata.data8.bf5.kt4_7_msk_spid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data9.bf5.kt4_7_vlanid = (pkey->vlanid>>11)&0x1;      
                    keydata.data8.bf5.kt4_7_vlanid = pkey->vlanid&0x7FF;                
                    keydata.data9.bf5.kt4_7_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_MIRROR_GROUP)
                {
                    keydata.data9.bf5.kt4_7_mirror_group_id = pkey->mirror_group_id;
                    keydata.data9.bf5.kt4_7_msk_mirror_group_id_fld =0;
                }    
                break;
            }
             default:
                break;
        }
    }
    /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);
 
    return ret;
}

/**********************************************************************************
* function    : __cl_mac_vlan_key_entry_set
* description :  indirect  write ce cl_mac_vlan_key memory table
*
* input       :      port_id (0-2)
*                    cl_key_type(CL_MAC_VLAN_KEY,CL_MAC_VLAN_KEY_C)
*                    pdata:cl_mac_vlan_key_t  data[4]
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 

* others      :
*
*************************************************************************************/
cs_status __cl_mac_vlan_key_entry_set(cs_uint8 port_id, cs_uint8 cl_key_type, cs_uint16 addr, cl_mac_vlan_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cl_mac_vlan_key_t *pkey= NULL;
    cs_status ret =0;
    cs_uint32 i;

    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));

    if((cl_key_type != CL_MAC_VLAN_KEY)
        &&(cl_key_type != CL_MAC_VLAN_KEY_C)){
        AAL_MIN_LOG("key type:%d. [%s %d]\n", cl_key_type, __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    keydata.data9.bf12.kt_cl_key_type = cl_key_type;

    for (i = 0; i<4; i++)
    {  
        pkey = (cl_mac_vlan_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_mac_vlan_key_t));
        
        if(pkey->mask==0)
            continue;   
        switch (i)
        {
            case 3:
            {
                keydata.data2.bf6.kt5_3_msk_802_1p_fld     = 1;
                keydata.data2.bf6.kt5_3_msk_p_tag_flag_fld = 1;
                keydata.data2.bf6.kt5_3_msk_vlan_cnt_fld   = 1;
                keydata.data2.bf6.kt5_3_msk_vlanid_fld     = 1;
                keydata.data2.bf6.kt5_3_msk_mac_addr_fld   = 1;
                keydata.data2.bf6.kt5_3_age_tmr_offset = pkey->age_timer_offset;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_MACADDR)
                {
                    keydata.data1.bf6.kt5_3_mac_type = pkey->mac_type;
                    keydata.data1.bf6.kt5_3_mac_addr = ((pkey->mac[0]<<8) | (pkey->mac[1]));
                    keydata.data0.bf6.kt5_3_mac_addr = ((pkey->mac[2]<<24)
                                                                           | (pkey->mac[3]<<16)
                                                                           | (pkey->mac[4]<<8)
                                                                           | (pkey->mac[5]));
                    keydata.data2.bf6.kt5_3_msk_mac_addr_fld = 0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data1.bf6.kt5_3_vlanid = pkey->vlanid;
                    keydata.data2.bf6.kt5_3_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_DOT1P)
                {
                    keydata.data2.bf6.kt5_3_vl_802_1p = pkey->dot1p;
                    keydata.data2.bf6.kt5_3_msk_802_1p_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_P_TAG)
                {
                    keydata.data1.bf6.kt5_3_p_tag_flag = pkey->p_tag_flag;
                    keydata.data2.bf6.kt5_3_msk_p_tag_flag_fld =0;
                }    
                if(pkey->mask&CL_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data1.bf6.kt5_3_vlan_cnt = pkey->vlan_count;
                    keydata.data2.bf6.kt5_3_msk_vlan_cnt_fld =0;
                }    
                break;
            }
            case 2:
            {
                keydata.data4.bf6.kt5_2_msk_802_1p_fld =1;
                keydata.data4.bf6.kt5_2_msk_p_tag_flag_fld =1;
                keydata.data4.bf6.kt5_2_msk_vlan_cnt_fld =1;
                keydata.data4.bf6.kt5_2_msk_vlanid_fld =1;
                keydata.data4.bf6.kt5_2_msk_mac_addr_fld =1;
                keydata.data4.bf6.kt5_2_age_tmr_offset = pkey->age_timer_offset;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_MACADDR)
                {
                    keydata.data3.bf6.kt5_2_mac_type = pkey->mac_type;
                    keydata.data3.bf6.kt5_2_mac_addr = ((pkey->mac[0]<<22)
                                                                           | (pkey->mac[1]<<14)
                                                                           | (pkey->mac[2]<<6)
                                                                           | (pkey->mac[3]>>2));
                    keydata.data2.bf6.kt5_2_mac_addr = ((pkey->mac[3]<<16)
                                                                            |(pkey->mac[4]<<8)
                                                                            |(pkey->mac[5]));
                    keydata.data4.bf6.kt5_2_msk_mac_addr_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data4.bf6.kt5_2_vlanid = (pkey->vlanid>>1)&0x7FF;
                    keydata.data3.bf6.kt5_2_vlanid = pkey->vlanid&0x1;
                    keydata.data4.bf6.kt5_2_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_DOT1P)
                {
                    keydata.data4.bf6.kt5_2_vl_802_1p = pkey->dot1p;
                    keydata.data4.bf6.kt5_2_msk_802_1p_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_P_TAG)
                {
                    keydata.data4.bf6.kt5_2_p_tag_flag = pkey->p_tag_flag;
                    keydata.data4.bf6.kt5_2_msk_p_tag_flag_fld =0;
                }    
                if(pkey->mask&CL_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data4.bf6.kt5_2_vlan_cnt = pkey->vlan_count;
                    keydata.data4.bf6.kt5_2_msk_vlan_cnt_fld =0;
                }    
                break;
            }
            case 1:
            {
                keydata.data7.bf6.kt5_1_msk_802_1p_fld =1;
                keydata.data7.bf6.kt5_1_msk_p_tag_flag_fld =1;
                keydata.data7.bf6.kt5_1_msk_vlan_cnt_fld =1;
                keydata.data7.bf6.kt5_1_msk_vlanid_fld =1;
                keydata.data6.bf6.kt5_1_msk_mac_addr_fld =1;
                keydata.data7.bf6.kt5_1_age_tmr_offset = pkey->age_timer_offset;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_MACADDR)
                {
                    keydata.data6.bf6.kt5_1_mac_type = pkey->mac_type;
                    keydata.data6.bf6.kt5_1_mac_addr = ((pkey->mac[0]<<4) | (pkey->mac[1]>>4));
                    keydata.data5.bf6.kt5_1_mac_addr = ((pkey->mac[1]<<28)
                                                                           | (pkey->mac[2]<<20)
                                                                           | (pkey->mac[3]<<12)
                                                                           | (pkey->mac[4]<<4)
                                                                           | (pkey->mac[5]>>4));
                    keydata.data4.bf6.kt5_1_mac_addr = pkey->mac[5]&0xF;
                    keydata.data6.bf6.kt5_1_msk_mac_addr_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {                  
                    keydata.data6.bf6.kt5_1_vlanid = pkey->vlanid;
                    keydata.data7.bf6.kt5_1_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_DOT1P)
                {
                    keydata.data6.bf6.kt5_1_vl_802_1p = pkey->dot1p;
                    keydata.data7.bf6.kt5_1_msk_802_1p_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_P_TAG)
                {
                    keydata.data6.bf6.kt5_1_p_tag_flag = pkey->p_tag_flag;
                    keydata.data7.bf6.kt5_1_msk_p_tag_flag_fld =0;
                }    
                if(pkey->mask&CL_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data6.bf6.kt5_1_vlan_cnt = pkey->vlan_count;
                    keydata.data7.bf6.kt5_1_msk_vlan_cnt_fld =0;
                }    
                break;
            }
            case 0:
            {
                keydata.data9.bf6.kt5_0_msk_802_1p_fld =1;
                keydata.data9.bf6.kt5_0_msk_p_tag_flag_fld =1;
                keydata.data9.bf6.kt5_0_msk_vlan_cnt_fld =1;
                keydata.data9.bf6.kt5_0_msk_vlanid_fld =1;
                keydata.data9.bf6.kt5_0_msk_mac_addr_fld =1;
                keydata.data9.bf6.kt5_0_age_tmr_offset = pkey->age_timer_offset;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_MACADDR)
                {
                    keydata.data8.bf6.kt5_0_mac_type = pkey->mac_type;
                    keydata.data8.bf6.kt5_0_mac_addr = ((pkey->mac[0]<<18)
                                                                           | (pkey->mac[1]<<10)
                                                                           | (pkey->mac[2]<<2)
                                                                           | (pkey->mac[3]>>6));
                    keydata.data7.bf6.kt5_0_mac_addr = ((pkey->mac[3]<<16)
                                                                            |(pkey->mac[4]<<8)
                                                                            |(pkey->mac[5]));
                    keydata.data9.bf6.kt5_0_msk_mac_addr_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {  
                    keydata.data9.bf6.kt5_0_vlanid = (pkey->vlanid>>5)&0x7F;
                    keydata.data8.bf6.kt5_0_vlanid = pkey->vlanid&0x1F;
                    keydata.data9.bf6.kt5_0_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_DOT1P)
                {
                    keydata.data9.bf6.kt5_0_vl_802_1p = pkey->dot1p;
                    keydata.data9.bf6.kt5_0_msk_802_1p_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_P_TAG)
                {
                    keydata.data9.bf6.kt5_0_p_tag_flag = pkey->p_tag_flag;
                    keydata.data9.bf6.kt5_0_msk_p_tag_flag_fld =0;
                }    
                if(pkey->mask&CL_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data9.bf6.kt5_0_vlan_cnt = pkey->vlan_count;
                    keydata.data9.bf6.kt5_0_msk_vlan_cnt_fld =0;
                }    
                break;
            }
             default:
                break;
        }
    }
   
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);

    return ret;
}

/**********************************************************************************
* function    : __cl_mac_etype_key_entry_set
* description :  indirect  write ce cl_mac_etype_key memory table
*
* input       :      port_id (0-2)
*                    cl_key_type(CL_MAC_ETYPE_KEY,CL_MAC_ETYPE_KEY_D)
*                    pdata:cl_mac_etype_key_t  data[4]
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 

* others      :
*
*************************************************************************************/
cs_status __cl_mac_etype_key_entry_set(cs_uint8 port_id, cs_uint8 cl_key_type, cs_uint16 addr, cl_mac_etype_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cl_mac_etype_key_t *pkey= NULL;
    cs_status ret =0;
    cs_uint32 i;

    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));

    if((cl_key_type != CL_MAC_ETYPE_KEY)
        &&(cl_key_type != CL_MAC_ETYPE_KEY_D)){
        AAL_MIN_LOG("key type:%d. [%s %d]\n", cl_key_type, __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    keydata.data9.bf12.kt_cl_key_type = cl_key_type;

    for (i = 0; i<4; i++)
    {  
        pkey = (cl_mac_etype_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_mac_etype_key_t));
        
        if(pkey->mask==0)
            continue;   
        switch (i)
        {
            case 3:
            {
                keydata.data2.bf7.kt6_3_msk_mac_addr_fld =1;
                keydata.data2.bf7.kt6_3_msk_ev2pt_fld =1;
                keydata.data2.bf7.kt6_3_msk_tpid_enc_fld =1;
                keydata.data2.bf7.kt6_3_msk_mac_da_range_flag_fld =1;
                keydata.data2.bf7.kt6_3_age_tmr_offset = pkey->age_timer_offset;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_MACADDR)
                {
                    keydata.data2.bf7.kt6_3_mac_type = pkey->mac_type;
                    keydata.data1.bf7.kt6_3_mac_addr = ((pkey->mac[0]<<8) | (pkey->mac[1]));
                    keydata.data0.bf7.kt6_3_mac_addr = ((pkey->mac[2]<<24)
                                                                           | (pkey->mac[3]<<16)
                                                                           | (pkey->mac[4]<<8)
                                                                           | (pkey->mac[5]));
                    keydata.data2.bf7.kt6_3_msk_mac_addr_fld = 0;
                }
                if(pkey->mask&CL_KEY_MASK_EV2PT)
                {
                    keydata.data1.bf7.kt6_3_ev2pt = pkey->ev2pt;
                    keydata.data2.bf7.kt6_3_msk_ev2pt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_TPID)
                {
                    keydata.data2.bf7.kt6_3_tpid_enc = pkey->tpid_enc;
                    keydata.data2.bf7.kt6_3_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_MACDA_RANGE_FLAG)
                {
                    keydata.data2.bf7.kt6_3_mac_da_range_flag = pkey->mac_da_range_flag;
                    keydata.data2.bf7.kt6_3_msk_mac_da_range_flag_fld =0;
                }           
                break;
            }
            case 2:
            {
                keydata.data4.bf7.kt6_2_msk_mac_addr_fld =1;
                keydata.data4.bf7.kt6_2_msk_ev2pt_fld =1;
                keydata.data4.bf7.kt6_2_msk_tpid_enc_fld =1;
                keydata.data4.bf7.kt6_2_msk_mac_da_range_flag_fld =1;
                keydata.data4.bf7.kt6_2_age_tmr_offset = pkey->age_timer_offset;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_MACADDR)
                {
                    keydata.data4.bf7.kt6_2_mac_type = pkey->mac_type;
                    keydata.data3.bf7.kt6_2_mac_addr = ((pkey->mac[0]<<23)
                                                                           | (pkey->mac[1]<<15)
                                                                           | (pkey->mac[2]<<7)
                                                                           | (pkey->mac[3]>>1));
                    keydata.data2.bf7.kt6_2_mac_addr = ((pkey->mac[3]<<16)
                                                                            |(pkey->mac[4]<<8)
                                                                            |(pkey->mac[5]));
                    keydata.data4.bf7.kt6_2_msk_mac_addr_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_EV2PT)
                {
                    keydata.data4.bf7.kt6_2_ev2pt = (pkey->ev2pt>>1);
                    keydata.data3.bf7.kt6_2_ev2pt = pkey->ev2pt&0x1;
                    keydata.data4.bf7.kt6_2_msk_ev2pt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_TPID)
                {
                    keydata.data4.bf7.kt6_2_tpid_enc = pkey->tpid_enc;
                    keydata.data4.bf7.kt6_2_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_MACDA_RANGE_FLAG)
                {
                    keydata.data4.bf7.kt6_2_mac_da_range_flag = pkey->mac_da_range_flag;
                    keydata.data4.bf7.kt6_2_msk_mac_da_range_flag_fld =0;
                }        
                break;
            }
            case 1:
            {
                keydata.data7.bf7.kt6_1_msk_mac_addr_fld =1;
                keydata.data7.bf7.kt6_1_msk_ev2pt_fld =1;
                keydata.data7.bf7.kt6_1_msk_tpid_enc_fld =1;
                keydata.data7.bf7.kt6_1_msk_mac_da_range_flag_fld =1;
                keydata.data7.bf7.kt6_1_age_tmr_offset = (pkey->age_timer_offset&0x3F)>>1;
                keydata.data6.bf7.kt6_1_age_tmr_offset = pkey->age_timer_offset&0x1;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_MACADDR)
                {
                    keydata.data6.bf7.kt6_1_mac_type = pkey->mac_type;
                    keydata.data6.bf7.kt6_1_mac_addr = ((pkey->mac[0]<<6) | (pkey->mac[1]>>2));
                    keydata.data5.bf7.kt6_1_mac_addr = ((pkey->mac[1]<<30)
                                                                           | (pkey->mac[2]<<22)
                                                                           | (pkey->mac[3]<<14)
                                                                           | (pkey->mac[4]<<6)
                                                                           | (pkey->mac[5]>>2));
                    keydata.data4.bf7.kt6_1_mac_addr = pkey->mac[5]&0x3;
                    keydata.data7.bf7.kt6_1_msk_mac_addr_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_EV2PT)
                {
                    keydata.data6.bf7.kt6_1_ev2pt = pkey->ev2pt;
                    keydata.data7.bf7.kt6_1_msk_ev2pt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_TPID)
                {
                    keydata.data7.bf7.kt6_1_tpid_enc = pkey->tpid_enc;
                    keydata.data7.bf7.kt6_1_msk_tpid_enc_fld = 0;
                }
                if(pkey->mask&CL_KEY_MASK_MACDA_RANGE_FLAG)
                {
                    keydata.data7.bf7.kt6_1_mac_da_range_flag = pkey->mac_da_range_flag;
                    keydata.data7.bf7.kt6_1_msk_mac_da_range_flag_fld =0;
                }            
                break;
            }
            case 0:
            {
                keydata.data9.bf7.kt6_0_msk_mac_addr_fld =1;
                keydata.data9.bf7.kt6_0_msk_ev2pt_fld =1;
                keydata.data9.bf7.kt6_0_msk_tpid_enc_fld =1;
                keydata.data9.bf7.kt6_0_msk_mac_da_range_flag_fld =1;
                keydata.data9.bf7.kt6_0_age_tmr_offset = pkey->age_timer_offset;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_MACADDR)
                {
                    keydata.data9.bf7.kt6_0_mac_type = pkey->mac_type;
                    keydata.data8.bf7.kt6_0_mac_addr = ((pkey->mac[0]<<21)
                                                                           | (pkey->mac[1]<<13)
                                                                           | (pkey->mac[2]<<5)
                                                                           | (pkey->mac[3]>>3));
                    keydata.data7.bf7.kt6_0_mac_addr = ((pkey->mac[3]<<16)
                                                                            |(pkey->mac[4]<<8)
                                                                            |(pkey->mac[5]));
                    keydata.data9.bf7.kt6_0_msk_mac_addr_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_EV2PT)
                {
                    keydata.data9.bf7.kt6_0_ev2pt = pkey->ev2pt>>3;
                    keydata.data8.bf7.kt6_0_ev2pt = pkey->ev2pt&0x7;
                    keydata.data9.bf7.kt6_0_msk_ev2pt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_TPID)
                {
                    keydata.data9.bf7.kt6_0_tpid_enc = pkey->tpid_enc;
                    keydata.data9.bf7.kt6_0_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_MACDA_RANGE_FLAG)
                {
                    keydata.data9.bf7.kt6_0_mac_da_range_flag = pkey->mac_da_range_flag;
                    keydata.data9.bf7.kt6_0_msk_mac_da_range_flag_fld =0;
                }
                break;
            }
             default:
                break;
        }
    }
   
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);

    return ret;
}

/**********************************************************************************
* function    : __cl_proto_key_entry_set
* description :  indirect  write ce proto_key memory table
*
* input       :      port_id (0-2)
*                    pdata:cl_proto_key_t  data[4]
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 

* others      :
*
*************************************************************************************/
cs_status __cl_proto_key_entry_set(cs_uint8 port_id, cs_uint16 addr, cl_proto_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cl_proto_key_t *pkey= NULL;
    cs_status ret =0;
    cs_uint32 i;

    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));

    keydata.data9.bf12.kt_cl_key_type = CL_PROTO_KEY;
    
    for (i = 0; i<4; i++)
    {  
        pkey = (cl_proto_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_proto_key_t));
        if(pkey->mask==0)
            continue;   
        switch (i)
        {
            case 3:
            {
                keydata.data9.bf11.kt10_3_msk_tpid_enc_fld =1;
                keydata.data9.bf11.kt10_3_msk_mac_da_flags_fld =1;
                keydata.data9.bf11.kt10_3_msk_ippt_fld =1;
                keydata.data9.bf11.kt10_3_msk_icmp_type_fld =1;
                keydata.data9.bf11.kt10_3_msk_mac_da_low_fld =1;
                keydata.data9.bf11.kt10_3_msk_ev2pt_fld =1;
                keydata.data9.bf11.kt10_3_msk_prsr_len_excd_fld =1;
                keydata.data8.bf11.kt10_3_l4_dest_hit_map = 0;
                keydata.data8.bf11.kt10_3_l4_src_hit_map = 0;
                keydata.data8.bf11.kt10_3_spcl_hit_map = 0;
                keydata.data7.bf11.kt10_3_spcl_hit_map = 0;

                keydata.data9.bf11.kt10_3_cfi_flg = pkey->cfi_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPECPKT)
                {
                    keydata.data8.bf11.kt10_3_spcl_hit_map = (pkey->spcl_hit_map>>4)&0xF;
                    keydata.data7.bf11.kt10_3_spcl_hit_map = pkey->spcl_hit_map&0xF;
                }
                if(pkey->mask&CL_KEY_MASK_L4SRC_RANGE)
                {
                    keydata.data8.bf11.kt10_3_l4_src_hit_map = pkey->l4_src_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_L4DST_RANGE)
                {
                    keydata.data8.bf11.kt10_3_l4_dest_hit_map = pkey->l4_dest_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_TPID)
                {
                    keydata.data9.bf11.kt10_3_tpid_enc = pkey->tpid_enc;
                    keydata.data9.bf11.kt10_3_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_EV2PT)
                {
                    keydata.data7.bf11.kt10_3_ev2pt = pkey->ev2pt;
                    keydata.data9.bf11.kt10_3_msk_ev2pt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_IPPROTO)
                {
                    keydata.data9.bf11.kt10_3_ippt = (pkey->ipprotocol>>4)&0xF;
                    keydata.data8.bf11.kt10_3_ippt = pkey->ipprotocol&0xF;
                    keydata.data9.bf11.kt10_3_msk_ippt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_ICMP_TYPE)
                {  
                    keydata.data8.bf11.kt10_3_icmp_type = pkey->icmp_type;
                    keydata.data9.bf11.kt10_3_msk_icmp_type_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_MAC_DA_LOW)
                {            
                    keydata.data7.bf11.kt10_3_mac_da_low = pkey->mac_da_low;
                    keydata.data9.bf11.kt10_3_msk_mac_da_low_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_DA_FLAGS)
                {
                    keydata.data9.bf11.kt10_3_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data9.bf11.kt10_3_mcst_ip_flag = pkey->mcst_ip_flag;
                    keydata.data9.bf11.kt10_3_mymac_flag = pkey->myMAC_flag;
                    keydata.data9.bf11.kt10_3_msk_mac_da_flags_fld =0;
                }  
                if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
                {            
                    keydata.data9.bf11.kt10_3_prsr_len_excd = pkey->prsr_len_excds;
                    keydata.data9.bf11.kt10_3_msk_prsr_len_excd_fld =0;
                } 
                break;
            }
            case 2:
            {
                keydata.data7.bf11.kt10_2_msk_tpid_enc_fld =1;
                keydata.data7.bf11.kt10_2_msk_mac_da_flags_fld =1;
                keydata.data7.bf11.kt10_2_msk_ippt_fld =1;
                keydata.data7.bf11.kt10_2_msk_icmp_type_fld =1;
                keydata.data6.bf11.kt10_2_msk_mac_da_low_fld =1;
                keydata.data6.bf11.kt10_2_msk_ev2pt_fld =1;
                keydata.data9.bf11.kt10_2_msk_prsr_len_excd_fld =1;
                keydata.data6.bf11.kt10_2_l4_dest_hit_map = 0;
                keydata.data5.bf11.kt10_2_l4_src_hit_map = 0;
                keydata.data5.bf11.kt10_2_spcl_hit_map = 0;

                keydata.data9.bf11.kt10_2_cfi_flg = pkey->cfi_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPECPKT)
                {
                    keydata.data5.bf11.kt10_2_spcl_hit_map = pkey->spcl_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_L4SRC_RANGE)
                {
                    keydata.data5.bf11.kt10_2_l4_src_hit_map = pkey->l4_src_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_L4DST_RANGE)
                {
                    keydata.data6.bf11.kt10_2_l4_dest_hit_map = pkey->l4_dest_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_TPID)
                {
                    keydata.data6.bf11.kt10_2_tpid_enc = pkey->tpid_enc;
                    keydata.data7.bf11.kt10_2_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_EV2PT)
                {  
                    keydata.data5.bf11.kt10_2_ev2pt = (pkey->ev2pt>>8)&0xFF;
                    keydata.data4.bf11.kt10_2_ev2pt = pkey->ev2pt&0xFF;
                    keydata.data6.bf11.kt10_2_msk_ev2pt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_IPPROTO)
                {
                    keydata.data6.bf11.kt10_2_ippt = pkey->ipprotocol;
                    keydata.data7.bf11.kt10_2_msk_ippt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_ICMP_TYPE)
                {
                    keydata.data6.bf11.kt10_2_icmp_type = pkey->icmp_type;
                    keydata.data7.bf11.kt10_2_msk_icmp_type_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_MAC_DA_LOW)
                {
                    keydata.data5.bf11.kt10_2_mac_da_low = pkey->mac_da_low;
                    keydata.data6.bf11.kt10_2_msk_mac_da_low_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_DA_FLAGS)
                {
                    keydata.data6.bf11.kt10_2_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data6.bf11.kt10_2_mcst_ip_flag = pkey->mcst_ip_flag;
                    keydata.data6.bf11.kt10_2_mymac_flag = pkey->myMAC_flag;
                    keydata.data7.bf11.kt10_2_msk_mac_da_flags_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
                {            
                    keydata.data9.bf11.kt10_2_prsr_len_excd = pkey->prsr_len_excds;
                    keydata.data9.bf11.kt10_2_msk_prsr_len_excd_fld =0;
                } 
                break;
            }
            case 1:
            {
                keydata.data4.bf11.kt10_1_msk_tpid_enc_fld =1;
                keydata.data4.bf11.kt10_1_msk_mac_da_flags_fld =1;
                keydata.data4.bf11.kt10_1_msk_ippt_fld =1;
                keydata.data4.bf11.kt10_1_msk_icmp_type_fld =1;
                keydata.data4.bf11.kt10_1_msk_mac_da_low_fld =1;
                keydata.data4.bf11.kt10_1_msk_ev2pt_fld =1;
                keydata.data9.bf11.kt10_1_msk_prsr_len_excd_fld =1;
                keydata.data3.bf11.kt10_1_l4_dest_hit_map = 0;
                keydata.data3.bf11.kt10_1_l4_src_hit_map = 0;
                keydata.data3.bf11.kt10_1_spcl_hit_map = 0;

                keydata.data9.bf11.kt10_1_cfi_flg = pkey->cfi_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPECPKT)
                {
                    keydata.data3.bf11.kt10_1_spcl_hit_map = pkey->spcl_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_L4SRC_RANGE)
                {
                    keydata.data3.bf11.kt10_1_l4_src_hit_map = pkey->l4_src_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_L4DST_RANGE)
                {
                    keydata.data3.bf11.kt10_1_l4_dest_hit_map = pkey->l4_dest_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_TPID)
                {
                    keydata.data4.bf11.kt10_1_tpid_enc = pkey->tpid_enc;
                    keydata.data4.bf11.kt10_1_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_EV2PT)
                {
                    keydata.data2.bf11.kt10_1_ev2pt = pkey->ev2pt;
                    keydata.data4.bf11.kt10_1_msk_ev2pt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_IPPROTO)
                {
                    keydata.data4.bf11.kt10_1_ippt = pkey->ipprotocol;
                    keydata.data4.bf11.kt10_1_msk_ippt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_ICMP_TYPE)
                {  
                    keydata.data4.bf11.kt10_1_icmp_type = (pkey->icmp_type>>4)&0xF;
                    keydata.data3.bf11.kt10_1_icmp_type = pkey->icmp_type&0xF;
                    keydata.data4.bf11.kt10_1_msk_icmp_type_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_MAC_DA_LOW)
                {
                    keydata.data3.bf11.kt10_1_mac_da_low = (pkey->mac_da_low>>4)&0xF;
                    keydata.data2.bf11.kt10_1_mac_da_low = pkey->mac_da_low&0xF;
                    keydata.data4.bf11.kt10_1_msk_mac_da_low_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_DA_FLAGS)
                {
                    keydata.data4.bf11.kt10_1_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data4.bf11.kt10_1_mcst_ip_flag = pkey->mcst_ip_flag;
                    keydata.data4.bf11.kt10_1_mymac_flag = pkey->myMAC_flag;
                    keydata.data4.bf11.kt10_1_msk_mac_da_flags_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
                {            
                    keydata.data9.bf11.kt10_1_prsr_len_excd = pkey->prsr_len_excds;
                    keydata.data9.bf11.kt10_1_msk_prsr_len_excd_fld =0;
                } 
                break;
            }
            case 0:
            {
                keydata.data2.bf11.kt10_0_msk_tpid_enc_fld =1;
                keydata.data2.bf11.kt10_0_msk_mac_da_flags_fld =1;
                keydata.data2.bf11.kt10_0_msk_ippt_fld =1;
                keydata.data2.bf11.kt10_0_msk_icmp_type_fld =1;
                keydata.data2.bf11.kt10_0_msk_mac_da_low_fld =1;
                keydata.data2.bf11.kt10_0_msk_ev2pt_fld =1;
                keydata.data9.bf11.kt10_0_msk_prsr_len_excd_fld =1;
                keydata.data1.bf11.kt10_0_l4_dest_hit_map = 0;
                keydata.data1.bf11.kt10_0_l4_src_hit_map = 0;
                keydata.data0.bf11.kt10_0_spcl_hit_map = 0;

                keydata.data9.bf11.kt10_0_cfi_flg = pkey->cfi_flag;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_SPECPKT)
                {
                    keydata.data0.bf11.kt10_0_spcl_hit_map = pkey->spcl_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_L4SRC_RANGE)
                {
                    keydata.data1.bf11.kt10_0_l4_src_hit_map = pkey->l4_src_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_L4DST_RANGE)
                {
                    keydata.data1.bf11.kt10_0_l4_dest_hit_map = pkey->l4_dest_hit_map;
                }
                if(pkey->mask&CL_KEY_MASK_TPID)
                {
                    keydata.data2.bf11.kt10_0_tpid_enc = pkey->tpid_enc;
                    keydata.data2.bf11.kt10_0_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_EV2PT)
                {
                    keydata.data0.bf11.kt10_0_ev2pt = pkey->ev2pt;
                    keydata.data2.bf11.kt10_0_msk_ev2pt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_IPPROTO)
                {
                    keydata.data1.bf11.kt10_0_ippt = pkey->ipprotocol;
                    keydata.data2.bf11.kt10_0_msk_ippt_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_ICMP_TYPE)
                {
                    keydata.data1.bf11.kt10_0_icmp_type = pkey->icmp_type;
                    keydata.data2.bf11.kt10_0_msk_icmp_type_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_MAC_DA_LOW)
                {
                    keydata.data0.bf11.kt10_0_mac_da_low = pkey->mac_da_low;
                    keydata.data2.bf11.kt10_0_msk_mac_da_low_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_DA_FLAGS)
                {
                    keydata.data2.bf11.kt10_0_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data2.bf11.kt10_0_mcst_ip_flag = pkey->mcst_ip_flag;
                    keydata.data2.bf11.kt10_0_mymac_flag = pkey->myMAC_flag;
                    keydata.data2.bf11.kt10_0_msk_mac_da_flags_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
                {            
                    keydata.data9.bf11.kt10_0_prsr_len_excd = pkey->prsr_len_excds;
                    keydata.data9.bf11.kt10_0_msk_prsr_len_excd_fld =0;
                } 
                break;
            }
            default:
                break;
        }
       
    }
   
      /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);

    return ret;
}

/**********************************************************************************
* function    : __cl_mcst_key_entry_set
* description :  indirect  write ce CL_MCST_KEY  memory table
*
* input       :      port_id (0-2)
*                    addr(phy table entry index): 0-47 (MA 0-15 )
*                    pdata:cl_mcst_key_t  data[2]
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 
*
* others      :
*
*************************************************************************************/
cs_status __cl_mcst_key_entry_set(cs_uint8 port_id, cs_uint16 addr, cl_mcst_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cl_mcst_key_t *pkey= NULL;
    cs_status ret =0;
    cs_uint32 i;
    
    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));

    keydata.data9.bf12.kt_cl_key_type = CL_MCST_KEY;
    
    for (i = 0; i<2; i++)
    {  
        pkey = (cl_mcst_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_mcst_key_t));
        
        switch (i)
        {  
            case 1:
            {
                keydata.data9.bf12.kt11_1_msk_vlanid_fld =1;
                keydata.data8.bf12.kt11_1_msk_addr_fld =1;
                keydata.data9.bf12.kt11_1_msk_prsr_len_excd_fld =1;
                keydata.data9.bf12.kt11_1_group_id = pkey->group_id;
                keydata.data9.bf12.kt11_1_group_id_vld = pkey->group_id_vld;
                keydata.data8.bf12.kt11_1_src_addr_vld = pkey->source_addr_vld;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_IPADDR)
                {
                    keydata.data8.bf12.kt11_1_addr = ((pkey->addr[0]<<16) |(pkey->addr[1]<<8)|(pkey->addr[2]));
                    keydata.data7.bf12.kt11_1_addr = ((pkey->addr[3]<<24) |(pkey->addr[4]<<16)|(pkey->addr[5]<<8)|(pkey->addr[6]));
                    keydata.data6.bf12.kt11_1_addr = ((pkey->addr[7]<<24) |(pkey->addr[8]<<16)|(pkey->addr[9]<<8)|(pkey->addr[10]));
                    keydata.data5.bf12.kt11_1_addr = ((pkey->addr[11]<<24) |(pkey->addr[12]<<16)|(pkey->addr[13]<<8)|(pkey->addr[14])); 
                    keydata.data4.bf12.kt11_1_addr = pkey->addr[15];
                    keydata.data8.bf12.kt11_1_addr_type = pkey->addr_type;
                    keydata.data8.bf12.kt11_1_msk_addr_fld = 0;          
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data9.bf12.kt11_1_vlanid = (pkey->vlanId>>4)&0xFF;
                    keydata.data8.bf12.kt11_1_vlanid = pkey->vlanId&0xF;
                    keydata.data9.bf12.kt11_1_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
                {            
                    keydata.data9.bf12.kt11_1_prsr_len_excd = pkey->prsr_len_excds;
                    keydata.data9.bf12.kt11_1_msk_prsr_len_excd_fld =0;
                } 
                break;
            }
            case 0:
            {
                keydata.data4.bf12.kt11_0_msk_vlanid_fld =1;
                keydata.data4.bf12.kt11_0_msk_addr_fld =1;
                keydata.data9.bf12.kt11_0_msk_prsr_len_excd_fld =1;
                keydata.data4.bf12.kt11_0_group_id = pkey->group_id;
                keydata.data4.bf12.kt11_0_group_id_vld = pkey->group_id_vld;
                keydata.data4.bf12.kt11_0_src_addr_vld = pkey->source_addr_vld;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_KEY_MASK_IPADDR)
                {
                    keydata.data3.bf12.kt11_0_addr = ((pkey->addr[0]<<24) |(pkey->addr[1]<<16)|(pkey->addr[2]<<8)|(pkey->addr[3]));
                    keydata.data2.bf12.kt11_0_addr = ((pkey->addr[4]<<24) |(pkey->addr[5]<<16)|(pkey->addr[6]<<8)|(pkey->addr[7]));
                    keydata.data1.bf12.kt11_0_addr = ((pkey->addr[8]<<24) |(pkey->addr[9]<<16)|(pkey->addr[10]<<8)|(pkey->addr[11]));
                    keydata.data0.bf12.kt11_0_addr = ((pkey->addr[12]<<24) |(pkey->addr[13]<<16)|(pkey->addr[14]<<8)|(pkey->addr[15]));                  
                    keydata.data4.bf12.kt11_0_addr_type = pkey->addr_type;
                    keydata.data4.bf12.kt11_0_msk_addr_fld = 0;          
                }
                if(pkey->mask&CL_KEY_MASK_VLANID)
                {
                    keydata.data4.bf12.kt11_0_vlanid = pkey->vlanId;                 
                    keydata.data4.bf12.kt11_0_msk_vlanid_fld =0;
                } 
                if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
                {            
                    keydata.data9.bf12.kt11_0_prsr_len_excd = pkey->prsr_len_excds;
                    keydata.data9.bf12.kt11_0_msk_prsr_len_excd_fld =0;
                } 
                break;
            }
             default:
                break;
        }
    }

    /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);

    return ret;
}

/**********************************************************************************
* function    : __cl_vlan_key_entry_set
* description :  indirect  write ce cl_vlan_key  memory table
*
* input       :      port_id (0-2)
*                    cl_key_type (CL_VLAN_KEY, CL_VLAN_KEY_E)
*                    addr(phy table entry index): 0-47 (MA 0-15 )
*                    pdata:cl_vlan_key_t  data[8]
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 
*
* others      :
*
*************************************************************************************/
cs_status __cl_vlan_key_entry_set(cs_uint8 port_id, cs_uint8 cl_key_type, cs_uint16 addr, cl_vlan_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cl_vlan_key_t *pkey= NULL;
    cs_status ret =0;
    cs_uint32 i;

    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));
 
    if((cl_key_type != CL_VLAN_KEY)&&(cl_key_type != CL_VLAN_KEY_E)){
        AAL_MIN_LOG("key type:%d. [%s %d]\n", cl_key_type, __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    keydata.data9.bf12.kt_cl_key_type = cl_key_type;
    for (i = 0; i<8; i++)
    {  
        pkey = (cl_vlan_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_vlan_key_t));
        
        if(pkey->mask==0)
            continue;   
        switch (i)
        {
            case 0:
            {
                keydata.data1.bf9.kt8_0_msk_tpid_enc_fld =1;
                keydata.data0.bf9.kt8_0_msk_ev2pt_enc_fld =1;
                keydata.data0.bf9.kt8_0_msk_bc_llid_fld =1;
                keydata.data0.bf9.kt8_0_msk_mc_flag_fld =1;
                keydata.data0.bf9.kt8_0_msk_bc_flag_fld =1;
                keydata.data0.bf9.kt8_0_msk_mac_da_flag_fld =1;
                keydata.data0.bf9.kt8_0_msk_p_tag_flag_fld =1;
                keydata.data0.bf9.kt8_0_msk_vlan_cnt_fld =1;
                keydata.data0.bf9.kt8_0_msk_vlanid_fld =1;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_VLAN_KEY_MASK_TPID_ENC)
                {
                    keydata.data1.bf9.kt8_0_tpid_enc = (pkey->tpid_enc>>1)&0x3;
                    keydata.data0.bf9.kt8_0_tpid_enc = pkey->tpid_enc&0x1;
                    keydata.data1.bf9.kt8_0_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data0.bf9.kt8_0_ev2pt_enc = pkey->ev2pt_enc ;
                    keydata.data0.bf9.kt8_0_msk_ev2pt_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG)
                {
                    keydata.data0.bf9.kt8_0_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data0.bf9.kt8_0_msk_mac_da_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data0.bf9.kt8_0_bc_flag =pkey->bc_flag;
                    keydata.data0.bf9.kt8_0_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data0.bf9.kt8_0_mc_flag =pkey->mc_flag;
                    keydata.data0.bf9.kt8_0_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_P_TAG_FLAG)
                {
                    keydata.data0.bf9.kt8_0_p_tag_flag =pkey->p_tag_flag;
                    keydata.data0.bf9.kt8_0_msk_p_tag_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data0.bf9.kt8_0_vlan_cnt =pkey->vlan_count;
                    keydata.data0.bf9.kt8_0_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data0.bf9.kt8_0_vlanid =pkey->vlanid;
                    keydata.data0.bf9.kt8_0_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_LLID)
                {
                    keydata.data0.bf9.kt8_0_bc_llid = pkey->bc_llid;
                    keydata.data0.bf9.kt8_0_msk_bc_llid_fld =0;
                }               
                break;
            }
            case 1:
            {
                keydata.data2.bf9.kt8_1_msk_tpid_enc_fld =1;
                keydata.data2.bf9.kt8_1_msk_ev2pt_enc_fld =1;
                keydata.data1.bf9.kt8_1_msk_bc_llid_fld =1;
                keydata.data1.bf9.kt8_1_msk_mc_flag_fld =1;
                keydata.data1.bf9.kt8_1_msk_bc_flag_fld =1;
                keydata.data1.bf9.kt8_1_msk_mac_da_flag_fld =1;
                keydata.data1.bf9.kt8_1_msk_p_tag_flag_fld =1;
                keydata.data1.bf9.kt8_1_msk_vlan_cnt_fld =1;
                keydata.data1.bf9.kt8_1_msk_vlanid_fld =1;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_VLAN_KEY_MASK_TPID_ENC)
                {
                    keydata.data2.bf9.kt8_1_tpid_enc = pkey->tpid_enc;
                    keydata.data2.bf9.kt8_1_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data2.bf9.kt8_1_ev2pt_enc = (pkey->ev2pt_enc>>3)&0x1;
                    keydata.data1.bf9.kt8_1_ev2pt_enc = pkey->ev2pt_enc&0x7;
                    keydata.data2.bf9.kt8_1_msk_ev2pt_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG)
                {
                    keydata.data1.bf9.kt8_1_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data1.bf9.kt8_1_msk_mac_da_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data1.bf9.kt8_1_bc_flag =pkey->bc_flag;
                    keydata.data1.bf9.kt8_1_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data1.bf9.kt8_1_mc_flag =pkey->mc_flag;
                    keydata.data1.bf9.kt8_1_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_P_TAG_FLAG)
                {
                    keydata.data1.bf9.kt8_1_p_tag_flag =pkey->p_tag_flag;
                    keydata.data1.bf9.kt8_1_msk_p_tag_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data1.bf9.kt8_1_vlan_cnt =pkey->vlan_count;
                    keydata.data1.bf9.kt8_1_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data1.bf9.kt8_1_vlanid =pkey->vlanid;
                    keydata.data1.bf9.kt8_1_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_LLID)
                {
                    keydata.data1.bf9.kt8_1_bc_llid = pkey->bc_llid;
                    keydata.data1.bf9.kt8_1_msk_bc_llid_fld =0;
                }          
                break;
            }
            case 2:
            {
                keydata.data3.bf9.kt8_2_msk_tpid_enc_fld =1;
                keydata.data3.bf9.kt8_2_msk_ev2pt_enc_fld =1;
                keydata.data2.bf9.kt8_2_msk_bc_llid_fld =1;
                keydata.data2.bf9.kt8_2_msk_mc_flag_fld =1;
                keydata.data2.bf9.kt8_2_msk_bc_flag_fld =1;
                keydata.data2.bf9.kt8_2_msk_mac_da_flag_fld =1;
                keydata.data2.bf9.kt8_2_msk_p_tag_flag_fld =1;
                keydata.data2.bf9.kt8_2_msk_vlan_cnt_fld =1;
                keydata.data2.bf9.kt8_2_msk_vlanid_fld =1;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_VLAN_KEY_MASK_TPID_ENC)
                {
                    keydata.data3.bf9.kt8_2_tpid_enc = pkey->tpid_enc;
                    keydata.data3.bf9.kt8_2_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data3.bf9.kt8_2_ev2pt_enc = pkey->ev2pt_enc;
                    keydata.data3.bf9.kt8_2_msk_ev2pt_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG)
                {
                    keydata.data2.bf9.kt8_2_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data2.bf9.kt8_2_msk_mac_da_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data2.bf9.kt8_2_bc_flag =pkey->bc_flag;
                    keydata.data2.bf9.kt8_2_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data2.bf9.kt8_2_mc_flag =pkey->mc_flag;
                    keydata.data2.bf9.kt8_2_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_P_TAG_FLAG)
                {
                    keydata.data2.bf9.kt8_2_p_tag_flag =pkey->p_tag_flag;
                    keydata.data2.bf9.kt8_2_msk_p_tag_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data2.bf9.kt8_2_vlan_cnt =pkey->vlan_count;
                    keydata.data2.bf9.kt8_2_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data2.bf9.kt8_2_vlanid =pkey->vlanid;
                    keydata.data2.bf9.kt8_2_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_LLID)
                {
                    keydata.data2.bf9.kt8_2_bc_llid = pkey->bc_llid;
                    keydata.data2.bf9.kt8_2_msk_bc_llid_fld =0;
                }       
                break;
            }
            case 3:
            {  
                keydata.data4.bf9.kt8_3_msk_tpid_enc_fld =1;
                keydata.data4.bf9.kt8_3_msk_ev2pt_enc_fld =1;
                keydata.data4.bf9.kt8_3_msk_bc_llid_fld =1;
                keydata.data4.bf9.kt8_3_msk_mc_flag_fld =1;
                keydata.data3.bf9.kt8_3_msk_bc_flag_fld =1;
                keydata.data3.bf9.kt8_3_msk_mac_da_flag_fld =1;
                keydata.data3.bf9.kt8_3_msk_p_tag_flag_fld =1;
                keydata.data3.bf9.kt8_3_msk_vlan_cnt_fld =1;
                keydata.data3.bf9.kt8_3_msk_vlanid_fld =1;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_VLAN_KEY_MASK_TPID_ENC)
                {
                    keydata.data4.bf9.kt8_3_tpid_enc = pkey->tpid_enc;
                    keydata.data4.bf9.kt8_3_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data4.bf9.kt8_3_ev2pt_enc = pkey->ev2pt_enc ;
                    keydata.data4.bf9.kt8_3_msk_ev2pt_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG)
                {
                    keydata.data3.bf9.kt8_3_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data3.bf9.kt8_3_msk_mac_da_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data3.bf9.kt8_3_bc_flag =pkey->bc_flag;
                    keydata.data3.bf9.kt8_3_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data3.bf9.kt8_3_mc_flag =pkey->mc_flag;
                    keydata.data4.bf9.kt8_3_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_P_TAG_FLAG)
                {
                    keydata.data3.bf9.kt8_3_p_tag_flag =pkey->p_tag_flag;
                    keydata.data3.bf9.kt8_3_msk_p_tag_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data3.bf9.kt8_3_vlan_cnt =pkey->vlan_count;
                    keydata.data3.bf9.kt8_3_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data3.bf9.kt8_3_vlanid =pkey->vlanid;
                    keydata.data3.bf9.kt8_3_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_LLID)
                {
                    keydata.data4.bf9.kt8_3_bc_llid = pkey->bc_llid;
                    keydata.data4.bf9.kt8_3_msk_bc_llid_fld =0;
                }           
                break;
            }
            case 4:
            {
                keydata.data5.bf9.kt8_4_msk_tpid_enc_fld =1;
                keydata.data5.bf9.kt8_4_msk_ev2pt_enc_fld =1;
                keydata.data5.bf9.kt8_4_msk_bc_llid_fld =1;
                keydata.data5.bf9.kt8_4_msk_mc_flag_fld =1;
                keydata.data5.bf9.kt8_4_msk_bc_flag_fld =1;
                keydata.data4.bf9.kt8_4_msk_mac_da_flag_fld =1;
                keydata.data4.bf9.kt8_4_msk_p_tag_flag_fld =1;
                keydata.data4.bf9.kt8_4_msk_vlan_cnt_fld =1;
                keydata.data4.bf9.kt8_4_msk_vlanid_fld =1;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_VLAN_KEY_MASK_TPID_ENC)
                {
                    keydata.data5.bf9.kt8_4_tpid_enc = pkey->tpid_enc;
                    keydata.data5.bf9.kt8_4_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data5.bf9.kt8_4_ev2pt_enc = pkey->ev2pt_enc ;
                    keydata.data5.bf9.kt8_4_msk_ev2pt_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG)
                {
                    keydata.data4.bf9.kt8_4_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data4.bf9.kt8_4_msk_mac_da_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data5.bf9.kt8_4_bc_flag =pkey->bc_flag;
                    keydata.data5.bf9.kt8_4_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data5.bf9.kt8_4_mc_flag =pkey->mc_flag;
                    keydata.data5.bf9.kt8_4_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_P_TAG_FLAG)
                {
                    keydata.data4.bf9.kt8_4_p_tag_flag =pkey->p_tag_flag;
                    keydata.data4.bf9.kt8_4_msk_p_tag_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data4.bf9.kt8_4_vlan_cnt =pkey->vlan_count;
                    keydata.data4.bf9.kt8_4_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data4.bf9.kt8_4_vlanid =pkey->vlanid;
                    keydata.data4.bf9.kt8_4_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_LLID)
                {
                    keydata.data5.bf9.kt8_4_bc_llid = pkey->bc_llid;
                    keydata.data5.bf9.kt8_4_msk_bc_llid_fld =0;
                }              
                break;
            }
            case 5:
            {
                keydata.data6.bf9.kt8_5_msk_tpid_enc_fld =1;
                keydata.data6.bf9.kt8_5_msk_ev2pt_enc_fld =1;
                keydata.data6.bf9.kt8_5_msk_bc_llid_fld =1;
                keydata.data6.bf9.kt8_5_msk_mc_flag_fld =1;
                keydata.data6.bf9.kt8_5_msk_bc_flag_fld =1;
                keydata.data6.bf9.kt8_5_msk_mac_da_flag_fld =1;
                keydata.data6.bf9.kt8_5_msk_p_tag_flag_fld =1;
                keydata.data5.bf9.kt8_5_msk_vlan_cnt_fld =1;
                keydata.data5.bf9.kt8_5_msk_vlanid_fld =1;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_VLAN_KEY_MASK_TPID_ENC)
                {
                    keydata.data6.bf9.kt8_5_tpid_enc = pkey->tpid_enc;
                    keydata.data6.bf9.kt8_5_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data6.bf9.kt8_5_ev2pt_enc = pkey->ev2pt_enc ;
                    keydata.data6.bf9.kt8_5_msk_ev2pt_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG)
                {
                    keydata.data6.bf9.kt8_5_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data6.bf9.kt8_5_msk_mac_da_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data6.bf9.kt8_5_bc_flag =pkey->bc_flag;
                    keydata.data6.bf9.kt8_5_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data6.bf9.kt8_5_mc_flag =pkey->mc_flag;
                    keydata.data6.bf9.kt8_5_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_P_TAG_FLAG)
                {
                    keydata.data5.bf9.kt8_5_p_tag_flag =pkey->p_tag_flag;
                    keydata.data6.bf9.kt8_5_msk_p_tag_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data5.bf9.kt8_5_vlan_cnt =pkey->vlan_count;
                    keydata.data5.bf9.kt8_5_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data5.bf9.kt8_5_vlanid =pkey->vlanid;
                    keydata.data5.bf9.kt8_5_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_LLID)
                {
                    keydata.data6.bf9.kt8_5_bc_llid = pkey->bc_llid;
                    keydata.data6.bf9.kt8_5_msk_bc_llid_fld =0;
                }          
                break;
            }
            case 6:
            {
                keydata.data7.bf9.kt8_6_msk_tpid_enc_fld =1;
                keydata.data7.bf9.kt8_6_msk_ev2pt_enc_fld =1;
                keydata.data7.bf9.kt8_6_msk_bc_llid_fld =1;
                keydata.data7.bf9.kt8_6_msk_mc_flag_fld =1;
                keydata.data7.bf9.kt8_6_msk_bc_flag_fld =1;
                keydata.data7.bf9.kt8_6_msk_mac_da_flag_fld =1;
                keydata.data7.bf9.kt8_6_msk_p_tag_flag_fld =1;
                keydata.data7.bf9.kt8_6_msk_vlan_cnt_fld =1;
                keydata.data6.bf9.kt8_6_msk_vlanid_fld =1;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_VLAN_KEY_MASK_TPID_ENC)
                {
                    keydata.data7.bf9.kt8_6_tpid_enc = pkey->tpid_enc;
                    keydata.data7.bf9.kt8_6_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data7.bf9.kt8_6_ev2pt_enc = pkey->ev2pt_enc ;
                    keydata.data7.bf9.kt8_6_msk_ev2pt_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG)
                {
                    keydata.data7.bf9.kt8_6_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data7.bf9.kt8_6_msk_mac_da_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data7.bf9.kt8_6_bc_flag =pkey->bc_flag;
                    keydata.data7.bf9.kt8_6_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data7.bf9.kt8_6_mc_flag =pkey->mc_flag;
                    keydata.data7.bf9.kt8_6_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_P_TAG_FLAG)
                {
                    keydata.data7.bf9.kt8_6_p_tag_flag =pkey->p_tag_flag;
                    keydata.data7.bf9.kt8_6_msk_p_tag_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data7.bf9.kt8_6_vlan_cnt = (pkey->vlan_count>>1)&0x1;
                    keydata.data6.bf9.kt8_6_vlan_cnt = pkey->vlan_count&0x1;
                    keydata.data7.bf9.kt8_6_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data6.bf9.kt8_6_vlanid =pkey->vlanid;
                    keydata.data6.bf9.kt8_6_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_LLID)
                {
                    keydata.data7.bf9.kt8_6_bc_llid = pkey->bc_llid;
                    keydata.data7.bf9.kt8_6_msk_bc_llid_fld =0;
                }             
                break;
            }
            case 7:
            {
                keydata.data8.bf9.kt8_7_msk_tpid_enc_fld =1;
                keydata.data8.bf9.kt8_7_msk_ev2pt_enc_fld =1;
                keydata.data8.bf9.kt8_7_msk_bc_llid_fld =1;
                keydata.data8.bf9.kt8_7_msk_mc_flag_fld =1;
                keydata.data8.bf9.kt8_7_msk_bc_flag_fld =1;
                keydata.data8.bf9.kt8_7_msk_mac_da_flag_fld =1;
                keydata.data8.bf9.kt8_7_msk_p_tag_flag_fld =1;
                keydata.data8.bf9.kt8_7_msk_vlan_cnt_fld =1;
                keydata.data8.bf9.kt8_7_msk_vlanid_fld =1;
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                if(pkey->mask&CL_VLAN_KEY_MASK_TPID_ENC)
                {
                    keydata.data8.bf9.kt8_7_tpid_enc = pkey->tpid_enc;
                    keydata.data8.bf9.kt8_7_msk_tpid_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data8.bf9.kt8_7_ev2pt_enc = pkey->ev2pt_enc ;
                    keydata.data8.bf9.kt8_7_msk_ev2pt_enc_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG)
                {
                    keydata.data8.bf9.kt8_7_rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
                    keydata.data8.bf9.kt8_7_msk_mac_da_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data8.bf9.kt8_7_bc_flag = pkey->bc_flag;
                    keydata.data8.bf9.kt8_7_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data8.bf9.kt8_7_mc_flag =pkey->mc_flag;
                    keydata.data8.bf9.kt8_7_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_P_TAG_FLAG)
                {
                    keydata.data8.bf9.kt8_7_p_tag_flag =pkey->p_tag_flag;
                    keydata.data8.bf9.kt8_7_msk_p_tag_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data8.bf9.kt8_7_vlan_cnt =pkey->vlan_count;
                    keydata.data8.bf9.kt8_7_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data8.bf9.kt8_7_vlanid = (pkey->vlanid>>11)&0x1;
                    keydata.data7.bf9.kt8_7_vlanid = pkey->vlanid&0x7FF;
                    keydata.data8.bf9.kt8_7_msk_vlanid_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_LLID)
                {
                    keydata.data8.bf9.kt8_7_bc_llid = pkey->bc_llid;
                    keydata.data8.bf9.kt8_7_msk_bc_llid_fld =0;
                }             
                break;
            }
             default:
                break;
        }
    }
      /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);
    if (ret)
    {
        DBG_PRINT("__cl_key_table_access error!\n");
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}
/**********************************************************************************
* function    : __cl_vlan_range_key_entry_set
* description :  indirect write ce CL_VLAN_RANGE_KEY  memory table
*
* input       :      port_id (0-2)
*                    addr(phy table entry index): 0-47 (MA 0-15 )
*                    pdata:cl_vlan_range_key_t  data[8]
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 
*
* others      :
*
*************************************************************************************/
cs_status __cl_vlan_range_key_entry_set(cs_uint8 port_id, cs_uint16 addr, cl_vlan_range_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cl_vlan_range_key_t *pkey= NULL;
    cs_status ret =0;
    cs_uint32 i;

    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM) ||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));
    keydata.data9.bf12.kt_cl_key_type = CL_VLAN_RANGE_KEY;
    
    for (i = 0; i<8; i++)
    {  
        pkey = (cl_vlan_range_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_vlan_range_key_t));
        
        if(pkey->mask==0)
            continue;   
        switch (i)
        {
            case 0:
            {
                keydata.data0.bf10.kt9_0_msk_bc_flag_fld =1;              
                keydata.data0.bf10.kt9_0_msk_vlan_cnt_fld =1;
                keydata.data0.bf10.kt9_0_msk_vlanid_fld =1;
                keydata.data1.bf10.kt9_0_msk_mc_flag_fld =1;
                keydata.data1.bf10.kt9_0_msk_ev2pt_enc_fld =1;              

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                                
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data1.bf10.kt9_0_ev2pt_enc = pkey->ev2pt_enc ;
                    keydata.data1.bf10.kt9_0_msk_ev2pt_enc_fld = 0;
                }
            
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data0.bf10.kt9_0_bc_flag =pkey->bc_flag;
                    keydata.data0.bf10.kt9_0_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data1.bf10.kt9_0_mc_flag =pkey->mc_flag;
                    keydata.data1.bf10.kt9_0_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data0.bf10.kt9_0_vlan_cnt =pkey->vlan_count;
                    keydata.data0.bf10.kt9_0_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data0.bf10.kt9_0_vlanid_hi =pkey->vlanid_hi;
                    keydata.data0.bf10.kt9_0_vlanid_lo =pkey->vlanid_lo;
                    keydata.data0.bf10.kt9_0_inner_vlan_en = pkey->inner_vlan_en;
                    keydata.data0.bf10.kt9_0_msk_vlanid_fld =0;
                }                
                break;
            }
            case 1:
            {           
                keydata.data1.bf10.kt9_1_msk_vlanid_fld =1;           
                keydata.data2.bf10.kt9_1_msk_vlan_cnt_fld =1;
                keydata.data2.bf10.kt9_1_msk_bc_flag_fld =1;
                keydata.data2.bf10.kt9_1_msk_mc_flag_fld =1;
                keydata.data2.bf10.kt9_1_msk_ev2pt_enc_fld =1;              

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data2.bf10.kt9_1_ev2pt_enc = pkey->ev2pt_enc ;
                    keydata.data2.bf10.kt9_1_msk_ev2pt_enc_fld = 0;
                }
             
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data2.bf10.kt9_1_bc_flag =pkey->bc_flag;
                    keydata.data2.bf10.kt9_1_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data2.bf10.kt9_1_mc_flag =pkey->mc_flag;
                    keydata.data2.bf10.kt9_1_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data2.bf10.kt9_1_vlan_cnt =pkey->vlan_count;
                    keydata.data2.bf10.kt9_1_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data1.bf10.kt9_1_vlanid_hi =pkey->vlanid_hi;
                    keydata.data1.bf10.kt9_1_vlanid_lo =pkey->vlanid_lo;
                    keydata.data2.bf10.kt9_1_inner_vlan_en = pkey->inner_vlan_en;
                    keydata.data1.bf10.kt9_1_msk_vlanid_fld =0;
                }              
                break;
            }
            case 2:
            {         
                keydata.data3.bf10.kt9_2_msk_vlanid_fld = 1;            
                keydata.data3.bf10.kt9_2_msk_vlan_cnt_fld =1;
                keydata.data3.bf10.kt9_2_msk_bc_flag_fld =1;
                keydata.data3.bf10.kt9_2_msk_mc_flag_fld =1;
                keydata.data3.bf10.kt9_2_msk_ev2pt_enc_fld =1;                   

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                                
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data3.bf10.kt9_2_ev2pt_enc = pkey->ev2pt_enc ;
                    keydata.data3.bf10.kt9_2_msk_ev2pt_enc_fld = 0;
                }
           
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data3.bf10.kt9_2_bc_flag = pkey->bc_flag;
                    keydata.data3.bf10.kt9_2_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data3.bf10.kt9_2_mc_flag =pkey->mc_flag;
                    keydata.data3.bf10.kt9_2_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data3.bf10.kt9_2_vlan_cnt =pkey->vlan_count;
                    keydata.data3.bf10.kt9_2_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data3.bf10.kt9_2_vlanid_hi = (pkey->vlanid_hi>>6)&0x3f;
                    keydata.data2.bf10.kt9_2_vlanid_hi = pkey->vlanid_hi&0x3f;                    
                    keydata.data2.bf10.kt9_2_vlanid_lo =pkey->vlanid_lo;
                    keydata.data3.bf10.kt9_2_inner_vlan_en = pkey->inner_vlan_en;
                    keydata.data3.bf10.kt9_2_msk_vlanid_fld =0;
                }           
                break;
            }
            case 3:
            {
                keydata.data4.bf10.kt9_3_msk_vlanid_fld = 1;               
                keydata.data4.bf10.kt9_3_msk_vlan_cnt_fld =1;
                keydata.data4.bf10.kt9_3_msk_bc_flag_fld =1;
                keydata.data4.bf10.kt9_3_msk_mc_flag_fld =1;
                keydata.data4.bf10.kt9_3_msk_ev2pt_enc_fld =1;              

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                                
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data4.bf10.kt9_3_ev2pt_enc = pkey->ev2pt_enc ;
                    keydata.data4.bf10.kt9_3_msk_ev2pt_enc_fld = 0;
                }
           
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data4.bf10.kt9_3_bc_flag =pkey->bc_flag;
                    keydata.data4.bf10.kt9_3_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data4.bf10.kt9_3_mc_flag =pkey->mc_flag;
                    keydata.data4.bf10.kt9_3_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data4.bf10.kt9_3_vlan_cnt =pkey->vlan_count;
                    keydata.data4.bf10.kt9_3_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data4.bf10.kt9_3_vlanid_hi =pkey->vlanid_hi;
                    keydata.data4.bf10.kt9_3_vlanid_lo = (pkey->vlanid_lo>>11)&0x1;
                    keydata.data3.bf10.kt9_3_vlanid_lo = pkey->vlanid_lo&0x7FF;
                    keydata.data4.bf10.kt9_3_inner_vlan_en =pkey->inner_vlan_en;
                    keydata.data4.bf10.kt9_3_msk_vlanid_fld =0;
                }                
                break;
            }
            case 4:
            {
                keydata.data5.bf10.kt9_4_msk_vlanid_fld =1;               
                keydata.data5.bf10.kt9_4_msk_vlan_cnt_fld =1;
                keydata.data5.bf10.kt9_4_msk_bc_flag_fld =1;
                keydata.data5.bf10.kt9_4_msk_mc_flag_fld =1;
                keydata.data6.bf10.kt9_4_msk_ev2pt_enc_fld =1;     

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                                
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {
                    keydata.data6.bf10.kt9_4_ev2pt_enc = (pkey->ev2pt_enc>>2)&0x3 ;
                    keydata.data5.bf10.kt9_4_ev2pt_enc = pkey->ev2pt_enc&0x3;                
                    keydata.data6.bf10.kt9_4_msk_ev2pt_enc_fld = 0;
                }
               
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data5.bf10.kt9_4_bc_flag =pkey->bc_flag;
                    keydata.data5.bf10.kt9_4_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data5.bf10.kt9_4_mc_flag =pkey->mc_flag;
                    keydata.data5.bf10.kt9_4_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data5.bf10.kt9_4_vlan_cnt =pkey->vlan_count;
                    keydata.data5.bf10.kt9_4_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data5.bf10.kt9_4_vlanid_hi =pkey->vlanid_hi;
                    keydata.data5.bf10.kt9_4_vlanid_lo = (pkey->vlanid_lo>>4)&0xFF;
                    keydata.data4.bf10.kt9_4_vlanid_lo = pkey->vlanid_lo&0xF;
                    keydata.data5.bf10.kt9_4_inner_vlan_en =pkey->inner_vlan_en;
                    keydata.data5.bf10.kt9_4_msk_vlanid_fld =0;
                }              
                break;
            }
            case 5:
            {
                keydata.data6.bf10.kt9_5_msk_vlanid_fld =1;          
                keydata.data6.bf10.kt9_5_msk_vlan_cnt_fld =1;
                keydata.data7.bf10.kt9_5_msk_bc_flag_fld =1;
                keydata.data7.bf10.kt9_5_msk_mc_flag_fld =1;
                keydata.data7.bf10.kt9_5_msk_ev2pt_enc_fld =1;              

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                                
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {                
                    keydata.data7.bf10.kt9_5_ev2pt_enc =pkey->ev2pt_enc;                                       
                    keydata.data7.bf10.kt9_5_msk_ev2pt_enc_fld = 0;
                }
            
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data7.bf10.kt9_5_bc_flag =pkey->bc_flag;
                    keydata.data7.bf10.kt9_5_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data7.bf10.kt9_5_mc_flag =pkey->mc_flag;
                    keydata.data7.bf10.kt9_5_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data6.bf10.kt9_5_vlan_cnt =pkey->vlan_count;
                    keydata.data6.bf10.kt9_5_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data6.bf10.kt9_5_vlanid_hi =pkey->vlanid_hi;
                    keydata.data6.bf10.kt9_5_vlanid_lo = pkey->vlanid_lo;
                    keydata.data7.bf10.kt9_5_inner_vlan_en = pkey->inner_vlan_en;
                    keydata.data6.bf10.kt9_5_msk_vlanid_fld =0;
                }                
                break;
            }
            case 6:
            {
                keydata.data8.bf10.kt9_6_msk_vlanid_fld =1;    
                keydata.data8.bf10.kt9_6_msk_vlan_cnt_fld =1;
                keydata.data8.bf10.kt9_6_msk_bc_flag_fld =1;
                keydata.data8.bf10.kt9_6_msk_mc_flag_fld =1;
                keydata.data8.bf10.kt9_6_msk_ev2pt_enc_fld =1;                

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                               
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {                
                    keydata.data8.bf10.kt9_6_ev2pt_enc =pkey->ev2pt_enc;                                       
                    keydata.data8.bf10.kt9_6_msk_ev2pt_enc_fld = 0;
                }
       
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data8.bf10.kt9_6_bc_flag =pkey->bc_flag;
                    keydata.data8.bf10.kt9_6_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data8.bf10.kt9_6_mc_flag =pkey->mc_flag;
                    keydata.data8.bf10.kt9_6_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data8.bf10.kt9_6_vlan_cnt =pkey->vlan_count;
                    keydata.data8.bf10.kt9_6_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data8.bf10.kt9_6_vlanid_hi = (pkey->vlanid_hi>>10)&0x3;
                    keydata.data7.bf10.kt9_6_vlanid_hi = pkey->vlanid_hi&0x3FF;
                    keydata.data7.bf10.kt9_6_vlanid_lo = pkey->vlanid_lo;
                    keydata.data8.bf10.kt9_6_inner_vlan_en = pkey->inner_vlan_en;
                    keydata.data8.bf10.kt9_6_msk_vlanid_fld =0;
                }             
                break;
            }
            case 7:
            {
                keydata.data9.bf10.kt9_7_msk_vlanid_fld =1;           
                keydata.data9.bf10.kt9_7_msk_vlan_cnt_fld =1;
                keydata.data9.bf10.kt9_7_msk_bc_flag_fld =1;
                keydata.data9.bf10.kt9_7_msk_mc_flag_fld =1;
                keydata.data9.bf10.kt9_7_msk_ev2pt_enc_fld =1;                   

                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                    break;
                               
                if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
                {                
                    keydata.data9.bf10.kt9_7_ev2pt_enc =pkey->ev2pt_enc;                                       
                    keydata.data9.bf10.kt9_7_msk_ev2pt_enc_fld = 0;
                }
           
                if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
                {
                    keydata.data9.bf10.kt9_7_bc_flag =pkey->bc_flag;
                    keydata.data9.bf10.kt9_7_msk_bc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
                {
                    keydata.data9.bf10.kt9_7_mc_flag =pkey->mc_flag;
                    keydata.data9.bf10.kt9_7_msk_mc_flag_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
                {
                    keydata.data9.bf10.kt9_7_vlan_cnt =pkey->vlan_count;
                    keydata.data9.bf10.kt9_7_msk_vlan_cnt_fld =0;
                }
                if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
                {
                    keydata.data9.bf10.kt9_7_vlanid_hi = (pkey->vlanid_hi>>3)&0x1FF;
                    keydata.data8.bf10.kt9_7_vlanid_hi = pkey->vlanid_hi&0x7;
                    keydata.data8.bf10.kt9_7_vlanid_lo = pkey->vlanid_lo;
                    keydata.data9.bf10.kt9_7_inner_vlan_en = pkey->inner_vlan_en;
                    keydata.data9.bf10.kt9_7_msk_vlanid_fld =0;
                }          
                break;
            }
             default:
                break;
        }
    }
    /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);
    if (ret)
    {
        DBG_PRINT("__cl_key_table_access error!\n");
        return CS_E_ERROR;
    }
 
    return CS_E_OK;
}

/**********************************************************************************
* function    : __cl_ctrl_pkt_key_entry_set
* description :  indirect write ce cl_ctrl_pkt_key  memory table
*
* input       :      port_id (0-2)
*                    addr(phy table entry index): 0-47 (MA 0-15 )
*
*                    pdata:cl_ctrl_pkt_key_t data[4]
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : 
*
* others      :
*
*************************************************************************************/
cs_status __cl_ctrl_pkt_key_entry_set(cs_uint8 port_id, cs_uint16 addr,  cl_ctrl_pkt_key_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cl_ctrl_pkt_key_t *pkey= NULL;
    cs_status ret =0;
    cs_uint32 i;

    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));

    keydata.data9.bf12.kt_cl_key_type = CL_CTRL_PKT_KEY;
    
    for (i = 0; i<4; i++)
    {  
        pkey = (cl_ctrl_pkt_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_ctrl_pkt_key_t));
        if((pkey->mask==0)&&(pkey->spl_pkt_en ==0))
            continue;   
        switch (i)
        {
            case 0:
            {
                keydata.data1.bf8.kt7_0_msk_mac_da_fld =63;
                keydata.data1.bf8.kt7_0_spl_pkt_en = pkey->spl_pkt_en;

                if(pkey->spl_pkt_en)
                {                 
                    keydata.data2.bf8.kt7_0_spl_pkt_vec = (pkey->special_packet_vec>>9)&0xFFF;
                    keydata.data1.bf8.kt7_0_spl_pkt_vec = pkey->special_packet_vec&0x1FF;
                }
                else
                {
                    keydata.data2.bf8.kt7_0_msk_ev2pt_fld =1;
                    if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                        break;
                    if(pkey->mask&CL_KEY_MASK_EV2PT)
                    {
                        keydata.data2.bf8.kt7_0_spl_pkt_vec = (pkey->special_packet_vec>>9)&0x7F;
                        keydata.data1.bf8.kt7_0_spl_pkt_vec = pkey->special_packet_vec&0x1FF;
                        keydata.data2.bf8.kt7_0_msk_ev2pt_fld =0;
                    }         
                } 
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                        break;
                if(pkey->mask&CL_KEY_MASK_MACDA)
                {
                    keydata.data1.bf8.kt7_0_mac_da = ((pkey->mac_da[0]<<8) |(pkey->mac_da[1]));
                    keydata.data0.bf8.kt7_0_mac_da = ((pkey->mac_da[2]<<24)
                                                                         | (pkey->mac_da[3]<<16)
                                                                         | (pkey->mac_da[4]<<8)
                                                                         | (pkey->mac_da[5]));
                    keydata.data1.bf8.kt7_0_msk_mac_da_fld = pkey->macmask;
                }
            
                break;
            }
            case 1:
            {
                keydata.data4.bf8.kt7_1_msk_mac_da_fld =7;
                keydata.data3.bf8.kt7_1_msk_mac_da_fld =7;
                keydata.data4.bf8.kt7_1_spl_pkt_en = pkey->spl_pkt_en;
                if(pkey->spl_pkt_en)
                {                 
                    keydata.data4.bf8.kt7_1_spl_pkt_vec = pkey->special_packet_vec;  
                }
                else
                {
                    keydata.data4.bf8.kt7_1_msk_ev2pt_fld =1;
                    if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                        break;
                    if(pkey->mask&CL_KEY_MASK_EV2PT)
                    {
                        keydata.data4.bf8.kt7_1_spl_pkt_vec = pkey->special_packet_vec;                     
                        keydata.data4.bf8.kt7_1_msk_ev2pt_fld =0;
                    }         
                } 
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                        break;
                if(pkey->mask&CL_KEY_MASK_MACDA)
                {
                    keydata.data3.bf8.kt7_1_mac_da = ((pkey->mac_da[0]<<21)
                                                                         | (pkey->mac_da[1]<<13)
                                                                         | (pkey->mac_da[2]<<5)
                                                                         | (pkey->mac_da[3]>>3));
                    keydata.data2.bf8.kt7_1_mac_da = ((pkey->mac_da[3]<<16)
                                                                         | (pkey->mac_da[4]<<8)
                                                                         | (pkey->mac_da[5]));
                    keydata.data4.bf8.kt7_1_msk_mac_da_fld = (pkey->macmask>>3)&0x7;
                    keydata.data3.bf8.kt7_1_msk_mac_da_fld =pkey->macmask&0x7;
                }
             
                break;
            }
            case 2:
            {
                keydata.data6.bf8.kt7_2_msk_mac_da_fld =63;
                keydata.data6.bf8.kt7_2_spl_pkt_en = pkey->spl_pkt_en;

                if(pkey->spl_pkt_en)
                {                 
                    keydata.data7.bf8.kt7_2_spl_pkt_vec = (pkey->special_packet_vec>>15)&0x3F;
                    keydata.data6.bf8.kt7_2_spl_pkt_vec = pkey->special_packet_vec&0x7FFF;
                }
                else
                {
                    keydata.data7.bf8.kt7_2_msk_ev2pt_fld =1;
                    if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                        break;
                    if(pkey->mask&CL_KEY_MASK_EV2PT)
                    {
                        keydata.data7.bf8.kt7_2_spl_pkt_vec = (pkey->special_packet_vec>>15)&0x1;
                        keydata.data6.bf8.kt7_2_spl_pkt_vec = pkey->special_packet_vec&0x7FFF;
                        keydata.data7.bf8.kt7_2_msk_ev2pt_fld =0;
                    }         
                }
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                        break;
                if(pkey->mask&CL_KEY_MASK_MACDA)
                {
                    keydata.data6.bf8.kt7_2_mac_da = ((pkey->mac_da[0]<<2) |(pkey->mac_da[1]>>6));
                    keydata.data5.bf8.kt7_2_mac_da = ((pkey->mac_da[1]<<26)
                                                                         | (pkey->mac_da[2]<<18)
                                                                         | (pkey->mac_da[3]<<10)
                                                                         | (pkey->mac_da[4]<<2)
                                                                         | (pkey->mac_da[5]>>6));
                    keydata.data4.bf8.kt7_2_mac_da = (pkey->mac_da[5]&0x3F);
                    keydata.data6.bf8.kt7_2_msk_mac_da_fld = pkey->macmask;
                }
              
                break;
            }
            case 3:
            {
                keydata.data8.bf8.kt7_3_msk_mac_da_fld =63;
                keydata.data8.bf8.kt7_3_spl_pkt_en = pkey->spl_pkt_en;
                if(pkey->spl_pkt_en)
                {                 
                    keydata.data9.bf8.kt7_3_spl_pkt_vec = (pkey->special_packet_vec>>2)&0x7FFFF;
                    keydata.data8.bf8.kt7_3_spl_pkt_vec = pkey->special_packet_vec&0x3;
                }
                else
                {
                    keydata.data9.bf8.kt7_3_msk_ev2pt_fld =1;
                    if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                        break;
                    if(pkey->mask&CL_KEY_MASK_EV2PT)
                    {
                        keydata.data9.bf8.kt7_3_spl_pkt_vec = (pkey->special_packet_vec>>2)&0x3FFF;
                        keydata.data8.bf8.kt7_3_spl_pkt_vec = pkey->special_packet_vec&0x3;
                        keydata.data9.bf8.kt7_3_msk_ev2pt_fld =0;
                    }         
                }
                if(pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
                        break;
                if(pkey->mask&CL_KEY_MASK_MACDA)
                {
                    keydata.data8.bf8.kt7_3_mac_da = ((pkey->mac_da[0]<<15) 
                                                                          |(pkey->mac_da[1]<<7)
                                                                          |(pkey->mac_da[2]>>1));
                    keydata.data7.bf8.kt7_3_mac_da = ((pkey->mac_da[2]<<24)
                                                                         | (pkey->mac_da[3]<<16)
                                                                         | (pkey->mac_da[4]<<8)
                                                                         | (pkey->mac_da[5]));
                    keydata.data8.bf8.kt7_3_msk_mac_da_fld = pkey->macmask;
                }
            
                break;
            }
             default:
                break;
        }
       
    }
   
      /*write register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, &keydata);
    if (ret)
    {
        DBG_PRINT("__cl_key_table_access error!\n");
        return CS_E_ERROR;
    }
  
    return CS_E_OK;
}

/**********************************************************************************
* function    : __cl_ctrl_pkt_key_entry_get
* description :  indirect  get ce cl_ctrl_pkt_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata: cl_ctrl_pkt_key_t data[4]
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_ctrl_pkt_key_entry_get(FE_LE_CE_KEY_DATA_T *pkeydata , cl_ctrl_pkt_key_t *pdata)
{
    cl_ctrl_pkt_key_t *pkey= NULL;
    cs_uint32 i;
    
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    for (i = 0; i<4; i++)
    {  
        pkey = (cl_ctrl_pkt_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_ctrl_pkt_key_t));
    
        switch (i)
        {
            case 0:
            {   
                pkey->macmask = pkeydata->data1.bf8.kt7_0_msk_mac_da_fld;
                if(pkey->macmask<48)
                {
                    pkey->mac_da[0] = (cs_int8)((pkeydata->data1.bf8.kt7_0_mac_da>>8)&0xFF);
                    pkey->mac_da[1] = (cs_int8)(pkeydata->data1.bf8.kt7_0_mac_da&0xFF);
                    pkey->mac_da[2] = (cs_int8)((pkeydata->data0.bf8.kt7_0_mac_da >>24)&0xFF);
                    pkey->mac_da[3] = (cs_int8)((pkeydata->data0.bf8.kt7_0_mac_da>>16)&0xFF);
                    pkey->mac_da[4] = (cs_int8)((pkeydata->data0.bf8.kt7_0_mac_da>>8)&0xFF);
                    pkey->mac_da[5] = (cs_int8)(pkeydata->data0.bf8.kt7_0_mac_da&0xFF);
                    pkey->mask |= CL_KEY_MASK_MACDA;
                } 
                pkey->spl_pkt_en = pkeydata->data1.bf8.kt7_0_spl_pkt_en;
                if(pkey->spl_pkt_en)
                {                 
                    pkey->special_packet_vec = ((pkeydata->data2.bf8.kt7_0_spl_pkt_vec<<9)
                                                                |pkeydata->data1.bf8.kt7_0_spl_pkt_vec);
                }
                else
                {
                    if(!pkeydata->data2.bf8.kt7_0_msk_ev2pt_fld)
                    {
                        pkey->special_packet_vec = (((pkeydata->data2.bf8.kt7_0_spl_pkt_vec<<9)
                                                                     |pkeydata->data1.bf8.kt7_0_spl_pkt_vec)&0XFFFF);
                        pkey->mask |= CL_KEY_MASK_EV2PT;
                    }         
                }         
                break;
            }
            case 1:
            {       
                pkey->macmask = ((pkeydata->data4.bf8.kt7_1_msk_mac_da_fld<<3)
                                               |pkeydata->data3.bf8.kt7_1_msk_mac_da_fld);             
                if(pkey->macmask<48)
                {
                    pkey->mac_da[0] = (cs_int8)((pkeydata->data3.bf8.kt7_1_mac_da>>21)&0xFF);
                    pkey->mac_da[1] = (cs_int8)((pkeydata->data3.bf8.kt7_1_mac_da>>13)&0xFF);
                    pkey->mac_da[2] = (cs_int8)((pkeydata->data3.bf8.kt7_1_mac_da>>5)&0xFF);
                    pkey->mac_da[3] = (cs_int8)(((pkeydata->data3.bf8.kt7_1_mac_da<<3)
                                                                 |(pkeydata->data2.bf8.kt7_1_mac_da>>16))&0xFF);                 
                    pkey->mac_da[4] = (cs_int8)((pkeydata->data2.bf8.kt7_1_mac_da>>8)&0xFF);
                    pkey->mac_da[5] = (cs_int8)(pkeydata->data2.bf8.kt7_1_mac_da&0xFF);             
                    pkey->mask |= CL_KEY_MASK_MACDA;
                }
                pkey->spl_pkt_en = pkeydata->data4.bf8.kt7_1_spl_pkt_en;
                if(pkey->spl_pkt_en)
                {                 
                    pkey->special_packet_vec =  pkeydata->data4.bf8.kt7_1_spl_pkt_vec;  
                }
                else
                {
                    if(!pkeydata->data4.bf8.kt7_1_msk_ev2pt_fld)
                    {                 
                        pkey->special_packet_vec =  pkeydata->data4.bf8.kt7_1_spl_pkt_vec&0XFFFF;  
                        pkey->mask |= CL_KEY_MASK_EV2PT;
                    }         
                }         
                break;
            }
            case 2:
            {
                pkey->macmask = pkeydata->data6.bf8.kt7_2_msk_mac_da_fld;
                pkey->spl_pkt_en = pkeydata->data6.bf8.kt7_2_spl_pkt_en;
                             
                if(pkey->macmask<48)
                {
                    pkey->mac_da[0] = (cs_int8)((pkeydata->data6.bf8.kt7_2_mac_da>>2)&0xFF);
                    pkey->mac_da[1] = (cs_int8)(((pkeydata->data6.bf8.kt7_2_mac_da<<6)
                                                                 |(pkeydata->data5.bf8.kt7_2_mac_da>>26))&0xFF);
                    pkey->mac_da[2] = (cs_int8)((pkeydata->data5.bf8.kt7_2_mac_da>>18)&0xFF);
                    pkey->mac_da[3] = (cs_int8)((pkeydata->data5.bf8.kt7_2_mac_da>>10)&0xFF);                 
                    pkey->mac_da[4] = (cs_int8)((pkeydata->data5.bf8.kt7_2_mac_da>>2)&0xFF);
                    pkey->mac_da[5] = (cs_int8)(((pkeydata->data5.bf8.kt7_2_mac_da<<6)
                                                                 |pkeydata->data4.bf8.kt7_2_mac_da)&0xFF);             
                    pkey->mask |= CL_KEY_MASK_MACDA;
                }
                if(pkey->spl_pkt_en)
                {                 
                    pkey->special_packet_vec = ((pkeydata->data7.bf8.kt7_2_spl_pkt_vec<<15)
                                                                 |pkeydata->data6.bf8.kt7_2_spl_pkt_vec);
                }
                else
                {
                    if(!pkeydata->data7.bf8.kt7_2_msk_ev2pt_fld)
                    {
                        pkey->special_packet_vec = (((pkeydata->data7.bf8.kt7_2_spl_pkt_vec<<15)
                                                                     |pkeydata->data6.bf8.kt7_2_spl_pkt_vec)&0XFFFF);
                        pkey->mask |= CL_KEY_MASK_EV2PT;
                    }         
                }         
                break;
            }
            case 3:
            {
                pkey->spl_pkt_en = pkeydata->data8.bf8.kt7_3_spl_pkt_en;
                pkey->macmask = pkeydata->data8.bf8.kt7_3_msk_mac_da_fld;
                
                if(pkey->macmask<48)
                {
                    pkey->mac_da[0] = (cs_int8)((pkeydata->data8.bf8.kt7_3_mac_da>>15)&0xFF);
                    pkey->mac_da[1] = (cs_int8)((pkeydata->data8.bf8.kt7_3_mac_da>>7)&0xFF);
                    pkey->mac_da[2] = (cs_int8)(((pkeydata->data8.bf8.kt7_3_mac_da<<1)
                                                                 |(pkeydata->data7.bf8.kt7_3_mac_da>>24))&0xFF);
                    pkey->mac_da[3] = (cs_int8)((pkeydata->data7.bf8.kt7_3_mac_da>>16)&0xFF);                 
                    pkey->mac_da[4] = (cs_int8)((pkeydata->data7.bf8.kt7_3_mac_da>>8)&0xFF);
                    pkey->mac_da[5] = (cs_int8)(pkeydata->data7.bf8.kt7_3_mac_da&0xFF);             
                    pkey->mask |= CL_KEY_MASK_MACDA;
                }
                if(pkey->spl_pkt_en)
                {                 
                    pkey->special_packet_vec = ((pkeydata->data9.bf8.kt7_3_spl_pkt_vec<<2)
                                                                 |pkeydata->data8.bf8.kt7_3_spl_pkt_vec);
                }
                else
                {
                    if(!pkeydata->data9.bf8.kt7_3_msk_ev2pt_fld )
                    {
                        pkey->special_packet_vec = (((pkeydata->data9.bf8.kt7_3_spl_pkt_vec<<2)
                                                                     |pkeydata->data8.bf8.kt7_3_spl_pkt_vec)&0XFFFF);
                        pkey->mask |= CL_KEY_MASK_EV2PT;
                    }         
                }         
                break;
            }
            default:
                break;
        }    
        if (pkey->mask==0)
        {
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        } 
    }
    return CS_E_OK;
}
/**********************************************************************************
* function    : __cl_vlan_range_key_entry_get
* description :  indirect  get ce cl_vlan_range_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata:cl_vlan_range_key_t data[8]
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_vlan_range_key_entry_get( FE_LE_CE_KEY_DATA_T *pkeydata, cl_vlan_range_key_t *pdata)
{
    cl_vlan_range_key_t *pkey= NULL;
    cs_uint32 i;
  
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
  
    for (i = 0; i<8; i++)
    {  
        pkey = (cl_vlan_range_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_vlan_range_key_t));
    
        switch (i)
        {
            case 0:
            {               
                if(!pkeydata->data1.bf10.kt9_0_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data1.bf10.kt9_0_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }                 
                if(!pkeydata->data0.bf10.kt9_0_msk_bc_flag_fld)
                {
                    pkey->bc_flag = pkeydata->data0.bf10.kt9_0_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data1.bf10.kt9_0_msk_mc_flag_fld)
                {
                    pkey->mc_flag = pkeydata->data1.bf10.kt9_0_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }           
                if(!pkeydata->data0.bf10.kt9_0_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data0.bf10.kt9_0_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data0.bf10.kt9_0_msk_vlanid_fld)
                {
                    pkey->vlanid_hi = pkeydata->data0.bf10.kt9_0_vlanid_hi;
                    pkey->vlanid_lo = pkeydata->data0.bf10.kt9_0_vlanid_lo;
                    pkey->inner_vlan_en = pkeydata->data0.bf10.kt9_0_inner_vlan_en;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                } 
                break;
            }
            case 1:
            {         
                if(!pkeydata->data2.bf10.kt9_1_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data2.bf10.kt9_1_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }           
                if(!pkeydata->data2.bf10.kt9_1_msk_bc_flag_fld )
                {
                    pkey->bc_flag = pkeydata->data2.bf10.kt9_1_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data2.bf10.kt9_1_msk_mc_flag_fld )
                {
                    pkey->mc_flag = pkeydata->data2.bf10.kt9_1_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                } 
                if(!pkeydata->data2.bf10.kt9_1_msk_vlan_cnt_fld )
                {
                    pkey->vlan_count = pkeydata->data2.bf10.kt9_1_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data1.bf10.kt9_1_msk_vlanid_fld)
                {
                    pkey->vlanid_hi = pkeydata->data1.bf10.kt9_1_vlanid_hi;
                    pkey->vlanid_lo =  pkeydata->data1.bf10.kt9_1_vlanid_lo;
                    pkey->inner_vlan_en = pkeydata->data2.bf10.kt9_1_inner_vlan_en;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                break;
            }
            case 2:
            {
                if(!pkeydata->data3.bf10.kt9_2_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data3.bf10.kt9_2_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }           
                if(!pkeydata->data3.bf10.kt9_2_msk_bc_flag_fld )
                {
                    pkey->bc_flag = pkeydata->data3.bf10.kt9_2_bc_flag ;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data3.bf10.kt9_2_msk_mc_flag_fld )
                {
                    pkey->mc_flag = pkeydata->data3.bf10.kt9_2_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }  
                if(!pkeydata->data3.bf10.kt9_2_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data3.bf10.kt9_2_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data3.bf10.kt9_2_msk_vlanid_fld)
                {  
                    pkey->vlanid_hi = ((pkeydata->data3.bf10.kt9_2_vlanid_hi<<6)
                                                    |pkeydata->data2.bf10.kt9_2_vlanid_hi); 
                    pkey->vlanid_lo = pkeydata->data2.bf10.kt9_2_vlanid_lo;
                    pkey->inner_vlan_en = pkeydata->data3.bf10.kt9_2_inner_vlan_en;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                break;
            }
            case 3:
            {
                if(!pkeydata->data4.bf10.kt9_3_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data4.bf10.kt9_3_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }           
                if(!pkeydata->data4.bf10.kt9_3_msk_bc_flag_fld )
                {
                    pkey->bc_flag = pkeydata->data4.bf10.kt9_3_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data4.bf10.kt9_3_msk_mc_flag_fld)
                {
                    pkey->mc_flag = pkeydata->data4.bf10.kt9_3_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }       
                if(!pkeydata->data4.bf10.kt9_3_msk_vlan_cnt_fld )
                {
                    pkey->vlan_count = pkeydata->data4.bf10.kt9_3_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data4.bf10.kt9_3_msk_vlanid_fld )
                {
                    pkey->vlanid_hi = pkeydata->data4.bf10.kt9_3_vlanid_hi;
                    pkey->vlanid_lo = ((pkeydata->data4.bf10.kt9_3_vlanid_lo<<11)
                                                    |pkeydata->data3.bf10.kt9_3_vlanid_lo);
                    pkey->inner_vlan_en = pkeydata->data4.bf10.kt9_3_inner_vlan_en;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                break;
            }
            case 4:
            {
                if(!pkeydata->data6.bf10.kt9_4_msk_ev2pt_enc_fld)
                { 
                    pkey->ev2pt_enc = ((pkeydata->data6.bf10.kt9_4_ev2pt_enc<<2)
                                                       |pkeydata->data5.bf10.kt9_4_ev2pt_enc);
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }        
                if(!pkeydata->data5.bf10.kt9_4_msk_bc_flag_fld)
                {
                    pkey->bc_flag = pkeydata->data5.bf10.kt9_4_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data5.bf10.kt9_4_msk_mc_flag_fld)
                {
                    pkey->mc_flag = pkeydata->data5.bf10.kt9_4_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }          
                if(!pkeydata->data5.bf10.kt9_4_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data5.bf10.kt9_4_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data5.bf10.kt9_4_msk_vlanid_fld)
                {
                    pkey->vlanid_hi = pkeydata->data5.bf10.kt9_4_vlanid_hi;
                    pkey->vlanid_lo = ((pkeydata->data5.bf10.kt9_4_vlanid_lo<<4)
                                                     |pkeydata->data4.bf10.kt9_4_vlanid_lo);
                    pkey->inner_vlan_en = pkeydata->data5.bf10.kt9_4_inner_vlan_en;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                break;
            }
            case 5:
            {    
                if(!pkeydata->data7.bf10.kt9_5_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data7.bf10.kt9_5_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }        
                if(!pkeydata->data7.bf10.kt9_5_msk_bc_flag_fld)
                {
                    pkey->bc_flag = pkeydata->data7.bf10.kt9_5_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data7.bf10.kt9_5_msk_mc_flag_fld)
                {
                    pkey->mc_flag = pkeydata->data7.bf10.kt9_5_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }       
                if(!pkeydata->data6.bf10.kt9_5_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data6.bf10.kt9_5_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data6.bf10.kt9_5_msk_vlanid_fld)
                {
                    pkey->vlanid_hi = pkeydata->data6.bf10.kt9_5_vlanid_hi;
                    pkey->vlanid_lo = pkeydata->data6.bf10.kt9_5_vlanid_lo; 
                    pkey->inner_vlan_en = pkeydata->data7.bf10.kt9_5_inner_vlan_en;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }      
                break;
            }
            case 6:
            {        
                if(!pkeydata->data8.bf10.kt9_6_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data8.bf10.kt9_6_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }            
                if(!pkeydata->data8.bf10.kt9_6_msk_bc_flag_fld)
                {
                    pkey->bc_flag = pkeydata->data8.bf10.kt9_6_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data8.bf10.kt9_6_msk_mc_flag_fld)
                {
                    pkey->mc_flag = pkeydata->data8.bf10.kt9_6_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }
                if(!pkeydata->data8.bf10.kt9_6_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data8.bf10.kt9_6_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data8.bf10.kt9_6_msk_vlanid_fld)
                {
                    pkey->vlanid_hi = ((pkeydata->data8.bf10.kt9_6_vlanid_hi<<10)
                                                        |pkeydata->data7.bf10.kt9_6_vlanid_hi);
                    pkey->vlanid_lo =  pkeydata->data7.bf10.kt9_6_vlanid_lo;
                    pkey->inner_vlan_en = pkeydata->data8.bf10.kt9_6_inner_vlan_en;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                break;
            }
            case 7:
            {
                if(!pkeydata->data9.bf10.kt9_7_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data9.bf10.kt9_7_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }  
                if(!pkeydata->data9.bf10.kt9_7_msk_bc_flag_fld)
                {
                    pkey->bc_flag = pkeydata->data9.bf10.kt9_7_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data9.bf10.kt9_7_msk_mc_flag_fld)
                {
                    pkey->mc_flag = pkeydata->data9.bf10.kt9_7_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }
                if(!pkeydata->data9.bf10.kt9_7_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data9.bf10.kt9_7_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data9.bf10.kt9_7_msk_vlanid_fld)
                {
                    pkey->vlanid_hi =((pkeydata->data9.bf10.kt9_7_vlanid_hi<<3)
                                                  |pkeydata->data8.bf10.kt9_7_vlanid_hi);
                    pkey->vlanid_lo = pkeydata->data8.bf10.kt9_7_vlanid_lo;
                    pkey->inner_vlan_en = pkeydata->data9.bf10.kt9_7_inner_vlan_en;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                break;
            }
             default:
                break;
        }
        if (pkey->mask==0)
        {
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        } 
    }
    return CS_E_OK;
}

/**********************************************************************************
* function    : __cl_vlan_key_entry_get
* description :  indirect  get ce cl_vlan_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata:cl_vlan_key_t data[8]
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_vlan_key_entry_get(FE_LE_CE_KEY_DATA_T *pkeydata, cl_vlan_key_t *pdata)
{
    cl_vlan_key_t *pkey= NULL;
    cs_uint32 i;
  
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
 
    for (i = 0; i<8; i++)
    {  
        pkey = (cl_vlan_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_vlan_key_t));
    
        switch (i)
        {
            case 0:
            {
                if(!pkeydata->data1.bf9.kt8_0_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = (pkeydata->data1.bf9.kt8_0_tpid_enc<<1)|pkeydata->data0.bf9.kt8_0_tpid_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_TPID_ENC;
                }
                if(!pkeydata->data0.bf9.kt8_0_msk_ev2pt_enc_fld )
                {
                    pkey->ev2pt_enc = pkeydata->data0.bf9.kt8_0_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }
                if(!pkeydata->data0.bf9.kt8_0_msk_mac_da_flag_fld )
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data0.bf9.kt8_0_rsvd_mac_da_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG;
                }
                if(!pkeydata->data0.bf9.kt8_0_msk_bc_flag_fld )
                {
                    pkey->bc_flag = pkeydata->data0.bf9.kt8_0_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data0.bf9.kt8_0_msk_mc_flag_fld )
                {
                    pkey->mc_flag = pkeydata->data0.bf9.kt8_0_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }
                if(!pkeydata->data0.bf9.kt8_0_msk_p_tag_flag_fld )
                {
                    pkey->p_tag_flag = pkeydata->data0.bf9.kt8_0_p_tag_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_P_TAG_FLAG;
                }
                if(!pkeydata->data0.bf9.kt8_0_msk_vlan_cnt_fld )
                {
                    pkey->vlan_count = pkeydata->data0.bf9.kt8_0_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data0.bf9.kt8_0_msk_vlanid_fld )
                {
                    pkey->vlanid = pkeydata->data0.bf9.kt8_0_vlanid;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                if(!pkeydata->data0.bf9.kt8_0_msk_bc_llid_fld )
                {
                    pkey->bc_llid = pkeydata->data0.bf9.kt8_0_bc_llid;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_LLID;
                }
                break;
            }
            case 1:
            {
                if(!pkeydata->data2.bf9.kt8_1_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data2.bf9.kt8_1_tpid_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_TPID_ENC;
                }
                if(!pkeydata->data2.bf9.kt8_1_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = ((pkeydata->data2.bf9.kt8_1_ev2pt_enc <<3)
                                                      |pkeydata->data1.bf9.kt8_1_ev2pt_enc);
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }
                if(!pkeydata->data1.bf9.kt8_1_msk_mac_da_flag_fld )
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data1.bf9.kt8_1_rsvd_mac_da_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG;
                }
                if(!pkeydata->data1.bf9.kt8_1_msk_bc_flag_fld )
                {
                    pkey->bc_flag = pkeydata->data1.bf9.kt8_1_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data1.bf9.kt8_1_msk_mc_flag_fld )
                {
                    pkey->mc_flag = pkeydata->data1.bf9.kt8_1_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }
                if(!pkeydata->data1.bf9.kt8_1_msk_p_tag_flag_fld )
                {
                    pkey->p_tag_flag = pkeydata->data1.bf9.kt8_1_p_tag_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_P_TAG_FLAG;
                }
                if(!pkeydata->data1.bf9.kt8_1_msk_vlan_cnt_fld )
                {
                    pkey->vlan_count = pkeydata->data1.bf9.kt8_1_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data1.bf9.kt8_1_msk_vlanid_fld )
                {
                    pkey->vlanid = pkeydata->data1.bf9.kt8_1_vlanid;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                if(!pkeydata->data1.bf9.kt8_1_msk_bc_llid_fld )
                {
                    pkey->bc_llid = pkeydata->data1.bf9.kt8_1_bc_llid;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_LLID;
                }
                break;
            }
            case 2:
            {
                if(!pkeydata->data3.bf9.kt8_2_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data3.bf9.kt8_2_tpid_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_TPID_ENC;
                }
                if(!pkeydata->data3.bf9.kt8_2_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data3.bf9.kt8_2_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }
                if(!pkeydata->data2.bf9.kt8_2_msk_mac_da_flag_fld )
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data2.bf9.kt8_2_rsvd_mac_da_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG;
                }
                if(!pkeydata->data2.bf9.kt8_2_msk_bc_flag_fld )
                {
                    pkey->bc_flag = pkeydata->data2.bf9.kt8_2_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data2.bf9.kt8_2_msk_mc_flag_fld )
                {
                    pkey->mc_flag = pkeydata->data2.bf9.kt8_2_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }
                if(!pkeydata->data2.bf9.kt8_2_msk_p_tag_flag_fld )
                {
                    pkey->p_tag_flag = pkeydata->data2.bf9.kt8_2_p_tag_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_P_TAG_FLAG;
                }
                if(!pkeydata->data2.bf9.kt8_2_msk_vlan_cnt_fld )
                {
                    pkey->vlan_count = pkeydata->data2.bf9.kt8_2_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data2.bf9.kt8_2_msk_vlanid_fld )
                {
                    pkey->vlanid = pkeydata->data2.bf9.kt8_2_vlanid;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                if(!pkeydata->data2.bf9.kt8_2_msk_bc_llid_fld )
                {
                    pkey->bc_llid = pkeydata->data2.bf9.kt8_2_bc_llid;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_LLID;
                }
                break;
            }
            case 3:
            {
                if(!pkeydata->data4.bf9.kt8_3_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data4.bf9.kt8_3_tpid_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_TPID_ENC;
                }
                if(!pkeydata->data4.bf9.kt8_3_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data4.bf9.kt8_3_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }
                if(!pkeydata->data3.bf9.kt8_3_msk_mac_da_flag_fld )
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data3.bf9.kt8_3_rsvd_mac_da_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG;
                }
                if(!pkeydata->data3.bf9.kt8_3_msk_bc_flag_fld )
                {
                    pkey->bc_flag = pkeydata->data3.bf9.kt8_3_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data4.bf9.kt8_3_msk_mc_flag_fld )
                {
                    pkey->mc_flag = pkeydata->data3.bf9.kt8_3_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }
                if(!pkeydata->data3.bf9.kt8_3_msk_p_tag_flag_fld )
                {
                    pkey->p_tag_flag = pkeydata->data3.bf9.kt8_3_p_tag_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_P_TAG_FLAG;
                }
                if(!pkeydata->data3.bf9.kt8_3_msk_vlan_cnt_fld )
                {
                    pkey->vlan_count = pkeydata->data3.bf9.kt8_3_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data3.bf9.kt8_3_msk_vlanid_fld )
                {
                    pkey->vlanid = pkeydata->data3.bf9.kt8_3_vlanid;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                if(!pkeydata->data4.bf9.kt8_3_msk_bc_llid_fld )
                {
                    pkey->bc_llid = pkeydata->data4.bf9.kt8_3_bc_llid;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_LLID;
                }
                break;
            }
            case 4:
            {
                if(!pkeydata->data5.bf9.kt8_4_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data5.bf9.kt8_4_tpid_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_TPID_ENC;
                }
                if(!pkeydata->data5.bf9.kt8_4_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data5.bf9.kt8_4_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }
                if(!pkeydata->data4.bf9.kt8_4_msk_mac_da_flag_fld )
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data4.bf9.kt8_4_rsvd_mac_da_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG;
                }
                if(!pkeydata->data5.bf9.kt8_4_msk_bc_flag_fld )
                {
                    pkey->bc_flag = pkeydata->data5.bf9.kt8_4_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data5.bf9.kt8_4_msk_mc_flag_fld )
                {
                    pkey->mc_flag = pkeydata->data5.bf9.kt8_4_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }
                if(!pkeydata->data4.bf9.kt8_4_msk_p_tag_flag_fld )
                {
                    pkey->p_tag_flag = pkeydata->data4.bf9.kt8_4_p_tag_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_P_TAG_FLAG;
                }
                if(!pkeydata->data4.bf9.kt8_4_msk_vlan_cnt_fld )
                {
                    pkey->vlan_count = pkeydata->data4.bf9.kt8_4_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data4.bf9.kt8_4_msk_vlanid_fld )
                {
                    pkey->vlanid = pkeydata->data4.bf9.kt8_4_vlanid;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                if(!pkeydata->data5.bf9.kt8_4_msk_bc_llid_fld )
                {
                    pkey->bc_llid = pkeydata->data5.bf9.kt8_4_bc_llid;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_LLID;
                }
                break;
            }
            case 5:
            {
                if(!pkeydata->data6.bf9.kt8_5_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data6.bf9.kt8_5_tpid_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_TPID_ENC;
                }
                if(!pkeydata->data6.bf9.kt8_5_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data6.bf9.kt8_5_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }
                if(!pkeydata->data6.bf9.kt8_5_msk_mac_da_flag_fld )
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data6.bf9.kt8_5_rsvd_mac_da_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG;
                }
                if(!pkeydata->data6.bf9.kt8_5_msk_bc_flag_fld )
                {
                    pkey->bc_flag = pkeydata->data6.bf9.kt8_5_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data6.bf9.kt8_5_msk_mc_flag_fld )
                {
                    pkey->mc_flag = pkeydata->data6.bf9.kt8_5_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }
                if(!pkeydata->data6.bf9.kt8_5_msk_p_tag_flag_fld )
                {
                    pkey->p_tag_flag = pkeydata->data5.bf9.kt8_5_p_tag_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_P_TAG_FLAG;
                }
                if(!pkeydata->data5.bf9.kt8_5_msk_vlan_cnt_fld )
                {
                    pkey->vlan_count = pkeydata->data5.bf9.kt8_5_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data5.bf9.kt8_5_msk_vlanid_fld )
                {
                    pkey->vlanid = pkeydata->data5.bf9.kt8_5_vlanid;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                if(!pkeydata->data6.bf9.kt8_5_msk_bc_llid_fld )
                {
                    pkey->bc_llid = pkeydata->data6.bf9.kt8_5_bc_llid;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_LLID;
                }
                break;
            }
            case 6:
            {
                if(!pkeydata->data7.bf9.kt8_6_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data7.bf9.kt8_6_tpid_enc ;
                    pkey->mask |= CL_VLAN_KEY_MASK_TPID_ENC;
                }
                if(!pkeydata->data7.bf9.kt8_6_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data7.bf9.kt8_6_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }
                if(!pkeydata->data7.bf9.kt8_6_msk_mac_da_flag_fld)
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data7.bf9.kt8_6_rsvd_mac_da_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG;
                }
                if(!pkeydata->data7.bf9.kt8_6_msk_bc_flag_fld)
                {
                    pkey->bc_flag = pkeydata->data7.bf9.kt8_6_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data7.bf9.kt8_6_msk_mc_flag_fld)
                {
                    pkey->mc_flag = pkeydata->data7.bf9.kt8_6_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }
                if(!pkeydata->data7.bf9.kt8_6_msk_p_tag_flag_fld)
                {
                    pkey->p_tag_flag = pkeydata->data7.bf9.kt8_6_p_tag_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_P_TAG_FLAG;
                }
                if(!pkeydata->data7.bf9.kt8_6_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = ((pkeydata->data7.bf9.kt8_6_vlan_cnt<<1)
                                                      |pkeydata->data6.bf9.kt8_6_vlan_cnt);
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data6.bf9.kt8_6_msk_vlanid_fld)
                {
                    pkey->vlanid = pkeydata->data6.bf9.kt8_6_vlanid;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                if(!pkeydata->data7.bf9.kt8_6_msk_bc_llid_fld)
                {
                    pkey->bc_llid = pkeydata->data7.bf9.kt8_6_bc_llid;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_LLID;
                }
                break;
            }
            case 7:
            {
                if(!pkeydata->data8.bf9.kt8_7_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data8.bf9.kt8_7_tpid_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_TPID_ENC;
                }
                if(!pkeydata->data8.bf9.kt8_7_msk_ev2pt_enc_fld)
                {
                    pkey->ev2pt_enc = pkeydata->data8.bf9.kt8_7_ev2pt_enc;
                    pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
                }
                if(!pkeydata->data8.bf9.kt8_7_msk_mac_da_flag_fld)
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data8.bf9.kt8_7_rsvd_mac_da_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG;
                }
                if(!pkeydata->data8.bf9.kt8_7_msk_bc_flag_fld)
                {
                    pkey->bc_flag = pkeydata->data8.bf9.kt8_7_bc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
                }
                if(!pkeydata->data8.bf9.kt8_7_msk_mc_flag_fld)
                {
                    pkey->mc_flag = pkeydata->data8.bf9.kt8_7_mc_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
                }
                if(!pkeydata->data8.bf9.kt8_7_msk_p_tag_flag_fld)
                {
                    pkey->p_tag_flag = pkeydata->data8.bf9.kt8_7_p_tag_flag;
                    pkey->mask |= CL_VLAN_KEY_MASK_P_TAG_FLAG;
                }
                if(!pkeydata->data8.bf9.kt8_7_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data8.bf9.kt8_7_vlan_cnt;
                    pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
                }
                if(!pkeydata->data8.bf9.kt8_7_msk_vlanid_fld)
                {
                    pkey->vlanid = ((pkeydata->data8.bf9.kt8_7_vlanid<<11)
                                                |pkeydata->data7.bf9.kt8_7_vlanid);
                    pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
                }
                if(!pkeydata->data8.bf9.kt8_7_msk_bc_llid_fld)
                {
                    pkey->bc_llid = pkeydata->data8.bf9.kt8_7_bc_llid;
                    pkey->mask |= CL_VLAN_KEY_MASK_BC_LLID;
                }
                break;
            }
             default:
                break;
        }
        if (pkey->mask==0)
        {
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        } 
    }
    
    return CS_E_OK;
}

/**********************************************************************************
* function    : __cl_proto_key_entry_get
* description :  indirect  get ce proto_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata:cl_proto_key_t data[4]
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_proto_key_entry_get(FE_LE_CE_KEY_DATA_T *pkeydata,  cl_proto_key_t *pdata)
{
    cl_proto_key_t *pkey= NULL;
    cs_uint32 i;
  
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    for (i = 0; i<4; i++)
    {  
        pkey = (cl_proto_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_proto_key_t));
    
        switch (i)
        {
            case 3:
            {  
                pkey->spcl_hit_map = ((pkeydata->data8.bf11.kt10_3_spcl_hit_map<<4) |
                                                     pkeydata->data7.bf11.kt10_3_spcl_hit_map);
                if(pkey->spcl_hit_map)
                {
                    pkey->mask |= CL_KEY_MASK_SPECPKT;
                }
                if(pkeydata->data8.bf11.kt10_3_l4_src_hit_map)
                {
                    pkey->l4_src_hit_map = pkeydata->data8.bf11.kt10_3_l4_src_hit_map;
                    pkey->mask |= CL_KEY_MASK_L4SRC_RANGE;
                }
                if(pkeydata->data8.bf11.kt10_3_l4_dest_hit_map)
                {
                    pkey->l4_dest_hit_map = pkeydata->data8.bf11.kt10_3_l4_dest_hit_map;
                    pkey->mask |= CL_KEY_MASK_L4DST_RANGE;
                }
                if(!pkeydata->data9.bf11.kt10_3_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data9.bf11.kt10_3_tpid_enc;
                    pkey->mask |= CL_KEY_MASK_TPID;
                }
                if(!pkeydata->data9.bf11.kt10_3_msk_ev2pt_fld)
                {
                    pkey->ev2pt = pkeydata->data7.bf11.kt10_3_ev2pt;
                    pkey->mask |= CL_KEY_MASK_EV2PT;
                }
                if(!pkeydata->data9.bf11.kt10_3_msk_ippt_fld)
                {
                    pkey->ipprotocol = ((pkeydata->data9.bf11.kt10_3_ippt<<4)
                                                   |pkeydata->data8.bf11.kt10_3_ippt);
                    pkey->mask |= CL_KEY_MASK_IPPROTO;
                }
                if(!pkeydata->data9.bf11.kt10_3_msk_icmp_type_fld)
                {
                    pkey->icmp_type = pkeydata->data8.bf11.kt10_3_icmp_type;
                    pkey->mask |= CL_KEY_MASK_ICMP_TYPE;
                }
                if(!pkeydata->data9.bf11.kt10_3_msk_mac_da_low_fld)
                {
                    pkey->mac_da_low = pkeydata->data7.bf11.kt10_3_mac_da_low;
                    pkey->mask |= CL_KEY_MASK_MAC_DA_LOW;
                }
                if(!pkeydata->data9.bf11.kt10_3_msk_mac_da_flags_fld)
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data9.bf11.kt10_3_rsvd_mac_da_flag;
                    pkey->mcst_ip_flag = pkeydata->data9.bf11.kt10_3_mcst_ip_flag;
                    pkey->myMAC_flag = pkeydata->data9.bf11.kt10_3_mymac_flag;
                    pkey->mask |= CL_KEY_MASK_DA_FLAGS;
                }
                if(!pkeydata->data9.bf11.kt10_3_msk_prsr_len_excd_fld)
                {
                    pkey->prsr_len_excds = pkeydata->data9.bf11.kt10_3_prsr_len_excd;
                    pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
                }
                pkey->cfi_flag = pkeydata->data9.bf11.kt10_3_cfi_flg;
                break;
            }
            case 2:
            {  
                if(pkeydata->data5.bf11.kt10_2_spcl_hit_map)
                {
                    pkey->spcl_hit_map = pkeydata->data5.bf11.kt10_2_spcl_hit_map;
                    pkey->mask |= CL_KEY_MASK_SPECPKT;
                }
                if(pkeydata->data5.bf11.kt10_2_l4_src_hit_map)
                {
                    pkey->l4_src_hit_map = pkeydata->data5.bf11.kt10_2_l4_src_hit_map;
                    pkey->mask |= CL_KEY_MASK_L4SRC_RANGE;
                }
                if(pkeydata->data6.bf11.kt10_2_l4_dest_hit_map)
                {
                    pkey->l4_dest_hit_map = pkeydata->data6.bf11.kt10_2_l4_dest_hit_map;
                    pkey->mask |= CL_KEY_MASK_L4DST_RANGE;
                }
                if(!pkeydata->data7.bf11.kt10_2_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data6.bf11.kt10_2_tpid_enc;
                    pkey->mask |= CL_KEY_MASK_TPID;
                }
                if(!pkeydata->data6.bf11.kt10_2_msk_ev2pt_fld)
                {
                    pkey->ev2pt = ((pkeydata->data5.bf11.kt10_2_ev2pt<<8)
                                             |pkeydata->data4.bf11.kt10_2_ev2pt);
                    pkey->mask |= CL_KEY_MASK_EV2PT;
                }
                if(!pkeydata->data7.bf11.kt10_2_msk_ippt_fld)
                {
                    pkey->ipprotocol = pkeydata->data6.bf11.kt10_2_ippt;
                    pkey->mask |= CL_KEY_MASK_IPPROTO;
                }
                if(!pkeydata->data7.bf11.kt10_2_msk_icmp_type_fld)
                {
                    pkey->icmp_type = pkeydata->data6.bf11.kt10_2_icmp_type;
                    pkey->mask |= CL_KEY_MASK_ICMP_TYPE;
                }
                if(!pkeydata->data6.bf11.kt10_2_msk_mac_da_low_fld)
                {
                    pkey->mac_da_low = pkeydata->data5.bf11.kt10_2_mac_da_low;
                    pkey->mask |= CL_KEY_MASK_MAC_DA_LOW;
                }
                if(!pkeydata->data7.bf11.kt10_2_msk_mac_da_flags_fld)
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data6.bf11.kt10_2_rsvd_mac_da_flag;
                    pkey->mcst_ip_flag = pkeydata->data6.bf11.kt10_2_mcst_ip_flag;
                    pkey->myMAC_flag = pkeydata->data6.bf11.kt10_2_mymac_flag;
                    pkey->mask |= CL_KEY_MASK_DA_FLAGS;
                } 
                if(!pkeydata->data9.bf11.kt10_2_msk_prsr_len_excd_fld)
                {
                    pkey->prsr_len_excds = pkeydata->data9.bf11.kt10_2_prsr_len_excd;
                    pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
                }
                pkey->cfi_flag = pkeydata->data9.bf11.kt10_2_cfi_flg;
                break;
            }
            case 1:
            {  
                if(pkeydata->data3.bf11.kt10_1_spcl_hit_map)
                {
                    pkey->spcl_hit_map = pkeydata->data3.bf11.kt10_1_spcl_hit_map;
                    pkey->mask |= CL_KEY_MASK_SPECPKT;
                }
                if(pkeydata->data3.bf11.kt10_1_l4_src_hit_map)
                {
                    pkey->l4_src_hit_map = pkeydata->data3.bf11.kt10_1_l4_src_hit_map;
                    pkey->mask |= CL_KEY_MASK_L4SRC_RANGE;
                }
                if(pkeydata->data3.bf11.kt10_1_l4_dest_hit_map)
                {
                    pkey->l4_dest_hit_map = pkeydata->data3.bf11.kt10_1_l4_dest_hit_map;
                    pkey->mask |= CL_KEY_MASK_L4DST_RANGE;
                }
                if(!pkeydata->data4.bf11.kt10_1_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data4.bf11.kt10_1_tpid_enc;
                    pkey->mask |= CL_KEY_MASK_TPID;
                }
                if(!pkeydata->data4.bf11.kt10_1_msk_ev2pt_fld)
                {
                    pkey->ev2pt = pkeydata->data2.bf11.kt10_1_ev2pt;
                    pkey->mask |= CL_KEY_MASK_EV2PT;
                }
                if(!pkeydata->data4.bf11.kt10_1_msk_ippt_fld)
                {
                    pkey->ipprotocol = pkeydata->data4.bf11.kt10_1_ippt;
                    pkey->mask |= CL_KEY_MASK_IPPROTO;
                }
                if(!pkeydata->data4.bf11.kt10_1_msk_icmp_type_fld)
                {
                    pkey->icmp_type = ((pkeydata->data4.bf11.kt10_1_icmp_type<<4)
                                                   |pkeydata->data3.bf11.kt10_1_icmp_type);
                    pkey->mask |= CL_KEY_MASK_ICMP_TYPE;
                }
                if(!pkeydata->data4.bf11.kt10_1_msk_mac_da_low_fld)
                {
                    pkey->mac_da_low = ((pkeydata->data3.bf11.kt10_1_mac_da_low<<4)
                                                       |pkeydata->data2.bf11.kt10_1_mac_da_low);
                    pkey->mask |= CL_KEY_MASK_MAC_DA_LOW;
                }
                if(!pkeydata->data4.bf11.kt10_1_msk_mac_da_flags_fld)
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data4.bf11.kt10_1_rsvd_mac_da_flag;
                    pkey->mcst_ip_flag = pkeydata->data4.bf11.kt10_1_mcst_ip_flag;
                    pkey->myMAC_flag = pkeydata->data4.bf11.kt10_1_mymac_flag;
                    pkey->mask |= CL_KEY_MASK_DA_FLAGS;
                }
                if(!pkeydata->data9.bf11.kt10_1_msk_prsr_len_excd_fld)
                {
                    pkey->prsr_len_excds = pkeydata->data9.bf11.kt10_1_prsr_len_excd;
                    pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
                }
                pkey->cfi_flag = pkeydata->data9.bf11.kt10_1_cfi_flg;
                break;
            }
            case 0:
            {  
                if(pkeydata->data0.bf11.kt10_0_spcl_hit_map)
                {
                    pkey->spcl_hit_map = pkeydata->data0.bf11.kt10_0_spcl_hit_map;
                    pkey->mask |= CL_KEY_MASK_SPECPKT;
                }
                if(pkeydata->data1.bf11.kt10_0_l4_src_hit_map)
                {
                    pkey->l4_src_hit_map = pkeydata->data1.bf11.kt10_0_l4_src_hit_map;
                    pkey->mask |= CL_KEY_MASK_L4SRC_RANGE;
                }
                if(pkeydata->data1.bf11.kt10_0_l4_dest_hit_map)
                {
                    pkey->l4_dest_hit_map = pkeydata->data1.bf11.kt10_0_l4_dest_hit_map;
                    pkey->mask |= CL_KEY_MASK_L4DST_RANGE;
                }
                if(!pkeydata->data2.bf11.kt10_0_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data2.bf11.kt10_0_tpid_enc;
                    pkey->mask |= CL_KEY_MASK_TPID;
                }
                if(!pkeydata->data2.bf11.kt10_0_msk_ev2pt_fld)
                {
                    pkey->ev2pt = pkeydata->data0.bf11.kt10_0_ev2pt;
                    pkey->mask |= CL_KEY_MASK_EV2PT;
                }
                if(!pkeydata->data2.bf11.kt10_0_msk_ippt_fld)
                {
                    pkey->ipprotocol = pkeydata->data1.bf11.kt10_0_ippt;
                    pkey->mask |= CL_KEY_MASK_IPPROTO;
                }
                if(!pkeydata->data2.bf11.kt10_0_msk_icmp_type_fld)
                {
                    pkey->icmp_type = pkeydata->data1.bf11.kt10_0_icmp_type;
                    pkey->mask |= CL_KEY_MASK_ICMP_TYPE;
                }
                if(!pkeydata->data2.bf11.kt10_0_msk_mac_da_low_fld)
                {
                    pkey->mac_da_low = pkeydata->data0.bf11.kt10_0_mac_da_low;
                    pkey->mask |= CL_KEY_MASK_MAC_DA_LOW;
                }
                if(!pkeydata->data2.bf11.kt10_0_msk_mac_da_flags_fld)
                {
                    pkey->rsvd_mac_da_flag = pkeydata->data2.bf11.kt10_0_rsvd_mac_da_flag;
                    pkey->mcst_ip_flag = pkeydata->data2.bf11.kt10_0_mcst_ip_flag;
                    pkey->myMAC_flag = pkeydata->data2.bf11.kt10_0_mymac_flag;
                    pkey->mask |= CL_KEY_MASK_DA_FLAGS;
                }
                if(!pkeydata->data9.bf11.kt10_0_msk_prsr_len_excd_fld)
                {
                    pkey->prsr_len_excds = pkeydata->data9.bf11.kt10_0_prsr_len_excd;
                    pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
                }
                pkey->cfi_flag = pkeydata->data9.bf11.kt10_0_cfi_flg;
                break;
            }
            default:
                break;
        }
        if (pkey->mask==0)
        {
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        } 
    }
  
    return CS_E_OK;
}

/**********************************************************************************
* function    : __cl_mcst_key_entry_get
* description :  indirect  get ce mcst_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata:cl_mcst_key_t data[2]
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_mcst_key_entry_get(FE_LE_CE_KEY_DATA_T *pkeydata,  cl_mcst_key_t *pdata)
{
    cl_mcst_key_t *pkey= NULL;
    cs_uint32 i;
  
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    for (i = 0; i<2; i++)
    {  
        pkey = (cl_mcst_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_mcst_key_t));
    
        switch (i)
        {
            case 1:
            {   
                pkey->group_id = pkeydata->data9.bf12.kt11_1_group_id;
                pkey->group_id_vld =  pkeydata->data9.bf12.kt11_1_group_id_vld;
                pkey->source_addr_vld = pkeydata->data8.bf12.kt11_1_src_addr_vld;
     
               if(!pkeydata->data8.bf12.kt11_1_msk_addr_fld)
                {
                    pkey->addr_type = pkeydata->data8.bf12.kt11_1_addr_type;
                    pkey->addr[0] = (pkeydata->data8.bf12.kt11_1_addr>>16)&0xFF;
                    pkey->addr[1] = (pkeydata->data8.bf12.kt11_1_addr>>8)&0xFF;
                    pkey->addr[2] = pkeydata->data8.bf12.kt11_1_addr&0xFF;
                    pkey->addr[3] = (pkeydata->data7.bf12.kt11_1_addr>>24)&0xFF;
                    pkey->addr[4] = (pkeydata->data7.bf12.kt11_1_addr>>16)&0xFF;
                    pkey->addr[5] = (pkeydata->data7.bf12.kt11_1_addr>>8)&0xFF;
                    pkey->addr[6] = pkeydata->data7.bf12.kt11_1_addr&0xFF;
                    pkey->addr[7] = (pkeydata->data6.bf12.kt11_1_addr>>24)&0xFF;
                    pkey->addr[8] = (pkeydata->data6.bf12.kt11_1_addr>>16)&0xFF;
                    pkey->addr[9] = (pkeydata->data6.bf12.kt11_1_addr>>8)&0xFF;
                    pkey->addr[10] = pkeydata->data6.bf12.kt11_1_addr&0xFF;
                    pkey->addr[11] = (pkeydata->data5.bf12.kt11_1_addr>>24)&0xFF;
                    pkey->addr[12] = (pkeydata->data5.bf12.kt11_1_addr>>16)&0xFF;
                    pkey->addr[13] = (pkeydata->data5.bf12.kt11_1_addr>>8)&0xFF;
                    pkey->addr[14] = pkeydata->data5.bf12.kt11_1_addr&0xFF;
                    pkey->addr[15] = pkeydata->data4.bf12.kt11_1_addr;                  
                    pkey->mask |= CL_KEY_MASK_IPADDR;
                }
                if(!pkeydata->data9.bf12.kt11_1_msk_vlanid_fld)
                {
                    pkey->vlanId = ((pkeydata->data9.bf12.kt11_1_vlanid<<4)
                                             |pkeydata->data8.bf12.kt11_1_vlanid);
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }
                if(!pkeydata->data9.bf12.kt11_1_msk_prsr_len_excd_fld)
                {
                    pkey->prsr_len_excds = pkeydata->data9.bf12.kt11_1_prsr_len_excd;
                    pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
                }
                break;
            }
            case 0:
            {   
                pkey->group_id = pkeydata->data4.bf12.kt11_0_group_id ;
                pkey->group_id_vld = pkeydata->data4.bf12.kt11_0_group_id_vld;
                pkey->source_addr_vld = pkeydata->data4.bf12.kt11_0_src_addr_vld;
                          
                if(!pkeydata->data4.bf12.kt11_0_msk_addr_fld)
                {
                    pkey->addr_type = pkeydata->data4.bf12.kt11_0_addr_type;
                    pkey->addr[0] = (pkeydata->data3.bf12.kt11_0_addr>>24)&0xFF;
                    pkey->addr[1] = (pkeydata->data3.bf12.kt11_0_addr>>16)&0xFF;
                    pkey->addr[2] = (pkeydata->data3.bf12.kt11_0_addr>>8)&0xFF;                    
                    pkey->addr[3] = pkeydata->data3.bf12.kt11_0_addr&0xFF;                
                    pkey->addr[4] = (pkeydata->data2.bf12.kt11_0_addr>>24)&0xFF;
                    pkey->addr[5] = (pkeydata->data2.bf12.kt11_0_addr>>16)&0xFF;
                    pkey->addr[6] = (pkeydata->data2.bf12.kt11_0_addr>>8)&0xFF;
                    pkey->addr[7] = pkeydata->data2.bf12.kt11_0_addr&0xFF;
                    pkey->addr[8] = (pkeydata->data1.bf12.kt11_0_addr>>24)&0xFF;
                    pkey->addr[9] = (pkeydata->data1.bf12.kt11_0_addr>>16)&0xFF;
                    pkey->addr[10] = (pkeydata->data1.bf12.kt11_0_addr>>8)&0xFF;
                    pkey->addr[11] = pkeydata->data1.bf12.kt11_0_addr&0xFF;                 
                    pkey->addr[12] = (pkeydata->data0.bf12.kt11_0_addr>>24)&0xFF;
                    pkey->addr[13] = (pkeydata->data0.bf12.kt11_0_addr>>16)&0xFF;
                    pkey->addr[14] = (pkeydata->data0.bf12.kt11_0_addr>>8)&0xFF;
                    pkey->addr[15] = pkeydata->data0.bf12.kt11_0_addr&0xFF;                    
                    pkey->mask |= CL_KEY_MASK_IPADDR;
                }
                if(!pkeydata->data4.bf12.kt11_0_msk_vlanid_fld)
                {
                    pkey->vlanId = pkeydata->data4.bf12.kt11_0_vlanid;
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }
                if(!pkeydata->data9.bf12.kt11_0_msk_prsr_len_excd_fld)
                {
                    pkey->prsr_len_excds = pkeydata->data9.bf12.kt11_0_prsr_len_excd;
                    pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
                }
                break;
            }
            default:
                break;
        }
        if (pkey->mask==0)
        {
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        } 
    }
  
    return CS_E_OK;
}

/**********************************************************************************
* function    : __cl_mac_vlan_key_entry_get
* description :  indirect  get ce cl_mac_vlan_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata:cl_mac_vlan_key_t data[4]
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_mac_vlan_key_entry_get(FE_LE_CE_KEY_DATA_T *pkeydata,  cl_mac_vlan_key_t *pdata)
{
    cl_mac_vlan_key_t *pkey= NULL;
    cs_uint32 i;
  
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    for (i = 0; i<4; i++)
    {  
        pkey = (cl_mac_vlan_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_mac_vlan_key_t));
    
        switch (i)
        {
            case 3:
            {              
                if(!pkeydata->data2.bf6.kt5_3_msk_mac_addr_fld)
                {
                    pkey->mac_type = pkeydata->data1.bf6.kt5_3_mac_type;
                    pkey->mac[0] = (pkeydata->data1.bf6.kt5_3_mac_addr>>8)&0xFF;
                    pkey->mac[1] = pkeydata->data1.bf6.kt5_3_mac_addr&0xFF;
                    pkey->mac[2] = (pkeydata->data0.bf6.kt5_3_mac_addr>>24)&0xFF;
                    pkey->mac[3] = (pkeydata->data0.bf6.kt5_3_mac_addr>>16)&0xFF;
                    pkey->mac[4] = (pkeydata->data0.bf6.kt5_3_mac_addr>>8)&0xFF;
                    pkey->mac[5] = pkeydata->data0.bf6.kt5_3_mac_addr&0xFF;
                    pkey->mask |= CL_KEY_MASK_MACADDR;
                }
                if(!pkeydata->data2.bf6.kt5_3_msk_vlanid_fld)
                {
                    pkey->vlanid = pkeydata->data1.bf6.kt5_3_vlanid;
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }
                if(!pkeydata->data2.bf6.kt5_3_msk_802_1p_fld)
                {
                    pkey->dot1p = pkeydata->data2.bf6.kt5_3_vl_802_1p;
                    pkey->mask |= CL_KEY_MASK_DOT1P;
                }
                if(!pkeydata->data2.bf6.kt5_3_msk_p_tag_flag_fld)
                {
                    pkey->p_tag_flag = pkeydata->data1.bf6.kt5_3_p_tag_flag;
                    pkey->mask |= CL_KEY_MASK_P_TAG;
                }
                if(!pkeydata->data2.bf6.kt5_3_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data1.bf6.kt5_3_vlan_cnt;
                    pkey->mask |= CL_KEY_MASK_VLAN_COUNT;
                }
                pkey->age_timer_offset = pkeydata->data2.bf6.kt5_3_age_tmr_offset;
                break;
            }
            case 2:
            {                
                if(!pkeydata->data4.bf6.kt5_2_msk_mac_addr_fld)
                {
                    pkey->mac_type = pkeydata->data3.bf6.kt5_2_mac_type;
                    pkey->mac[0] = (pkeydata->data3.bf6.kt5_2_mac_addr>>22)&0xFF;
                    pkey->mac[1] = (pkeydata->data3.bf6.kt5_2_mac_addr>>14)&0xFF;
                    pkey->mac[2] = (pkeydata->data3.bf6.kt5_2_mac_addr>>6)&0xFF;
                    pkey->mac[3] = ((pkeydata->data3.bf6.kt5_2_mac_addr<<2)
                                               |(pkeydata->data2.bf6.kt5_2_mac_addr>>16))&0xFF;
                    pkey->mac[4] = (pkeydata->data2.bf6.kt5_2_mac_addr>>8)&0xFF;
                    pkey->mac[5] = pkeydata->data2.bf6.kt5_2_mac_addr&0xFF;
                    pkey->mask |= CL_KEY_MASK_MACADDR;
                }
                if(!pkeydata->data4.bf6.kt5_2_msk_vlanid_fld)
                {
                    pkey->vlanid = ((pkeydata->data4.bf6.kt5_2_vlanid<<1)
                                             |pkeydata->data3.bf6.kt5_2_vlanid);
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }
                if(!pkeydata->data4.bf6.kt5_2_msk_802_1p_fld)
                {
                    pkey->dot1p = pkeydata->data4.bf6.kt5_2_vl_802_1p;
                    pkey->mask |= CL_KEY_MASK_DOT1P;
                }
                if(!pkeydata->data4.bf6.kt5_2_msk_p_tag_flag_fld)
                {
                    pkey->p_tag_flag = pkeydata->data4.bf6.kt5_2_p_tag_flag;
                    pkey->mask |= CL_KEY_MASK_P_TAG;
                }
                if(!pkeydata->data4.bf6.kt5_2_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data4.bf6.kt5_2_vlan_cnt;
                    pkey->mask |= CL_KEY_MASK_VLAN_COUNT;
                }
                pkey->age_timer_offset = pkeydata->data4.bf6.kt5_2_age_tmr_offset;
                break;
            }
            case 1:
            {   
               if(!pkeydata->data6.bf6.kt5_1_msk_mac_addr_fld)
                {
                    pkey->mac_type = pkeydata->data6.bf6.kt5_1_mac_type;
                    pkey->mac[0] = (pkeydata->data6.bf6.kt5_1_mac_addr>>4)&0xFF;
                    pkey->mac[1] = ((pkeydata->data6.bf6.kt5_1_mac_addr<<4)
                                               |(pkeydata->data5.bf6.kt5_1_mac_addr>>28))&0xFF;
                    pkey->mac[2] = (pkeydata->data5.bf6.kt5_1_mac_addr>>20)&0xFF;
                    pkey->mac[3] = (pkeydata->data5.bf6.kt5_1_mac_addr>>12)&0xFF;
                    pkey->mac[4] = (pkeydata->data5.bf6.kt5_1_mac_addr>>4)&0xFF;
                    pkey->mac[5] = ((pkeydata->data5.bf6.kt5_1_mac_addr<<4)
                                               |pkeydata->data4.bf6.kt5_1_mac_addr)&0xFF;
                    pkey->mask |= CL_KEY_MASK_MACADDR;
                }
                if(!pkeydata->data7.bf6.kt5_1_msk_vlanid_fld)
                {
                    pkey->vlanid = pkeydata->data6.bf6.kt5_1_vlanid;
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }
                if(!pkeydata->data7.bf6.kt5_1_msk_802_1p_fld)
                {
                    pkey->dot1p = pkeydata->data6.bf6.kt5_1_vl_802_1p;
                    pkey->mask |= CL_KEY_MASK_DOT1P;
                }
                if(!pkeydata->data7.bf6.kt5_1_msk_p_tag_flag_fld)
                {
                    pkey->p_tag_flag = pkeydata->data6.bf6.kt5_1_p_tag_flag;
                    pkey->mask |= CL_KEY_MASK_P_TAG;
                }
                if(!pkeydata->data7.bf6.kt5_1_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data6.bf6.kt5_1_vlan_cnt;
                    pkey->mask |= CL_KEY_MASK_VLAN_COUNT;
                }
                pkey->age_timer_offset = pkeydata->data7.bf6.kt5_1_age_tmr_offset;
                break;
            }
            case 0:
            {                  
                if(!pkeydata->data9.bf6.kt5_0_msk_mac_addr_fld)
                {
                    pkey->mac_type = pkeydata->data8.bf6.kt5_0_mac_type;
                    pkey->mac[0] = (pkeydata->data8.bf6.kt5_0_mac_addr>>18)&0xFF;
                    pkey->mac[1] = (pkeydata->data8.bf6.kt5_0_mac_addr>>10)&0xFF;
                    pkey->mac[2] = (pkeydata->data8.bf6.kt5_0_mac_addr>>2)&0xFF;
                    pkey->mac[3] = ((pkeydata->data8.bf6.kt5_0_mac_addr<<6)
                                               |(pkeydata->data7.bf6.kt5_0_mac_addr>>16))&0xFF;
                    pkey->mac[4] = (pkeydata->data7.bf6.kt5_0_mac_addr>>8)&0xFF;
                    pkey->mac[5] = pkeydata->data7.bf6.kt5_0_mac_addr&0xFF;
                    pkey->mask |= CL_KEY_MASK_MACADDR;
                }
                if(!pkeydata->data9.bf6.kt5_0_msk_vlanid_fld)
                {
                    pkey->vlanid = ((pkeydata->data9.bf6.kt5_0_vlanid<<5)
                                             |pkeydata->data8.bf6.kt5_0_vlanid);
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }
                if(!pkeydata->data9.bf6.kt5_0_msk_802_1p_fld)
                {
                    pkey->dot1p = pkeydata->data9.bf6.kt5_0_vl_802_1p;
                    pkey->mask |= CL_KEY_MASK_DOT1P;
                }
                if(!pkeydata->data9.bf6.kt5_0_msk_p_tag_flag_fld)
                {
                    pkey->p_tag_flag = pkeydata->data9.bf6.kt5_0_p_tag_flag;
                    pkey->mask |= CL_KEY_MASK_P_TAG;
                }
                if(!pkeydata->data9.bf6.kt5_0_msk_vlan_cnt_fld)
                {
                    pkey->vlan_count = pkeydata->data9.bf6.kt5_0_vlan_cnt;
                    pkey->mask |= CL_KEY_MASK_VLAN_COUNT;
                }
                pkey->age_timer_offset = pkeydata->data9.bf6.kt5_0_age_tmr_offset;
                break;
            }
            default:
                break;
        }
        if (pkey->mask==0)
        {
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        } 
    }
  
    return CS_E_OK;
}

/**********************************************************************************
* function    : onu_aal_cl_mac_etype_key_entry_get
* description :  indirect  get ce cl_mac_etype_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata:cl_mac_etype_key_t data[4]
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_mac_etype_key_entry_get(FE_LE_CE_KEY_DATA_T *pkeydata,  cl_mac_etype_key_t *pdata)
{
    cl_mac_etype_key_t *pkey= NULL;
    cs_uint32 i;
  
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    for (i = 0; i<4; i++)
    {  
        pkey = (cl_mac_etype_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_mac_etype_key_t));
    
        switch (i)
        {
            case 3:
            {
                if(!pkeydata->data2.bf7.kt6_3_msk_mac_addr_fld)
                {
                    pkey->mac_type = pkeydata->data2.bf7.kt6_3_mac_type;
                    pkey->mac[0] = (pkeydata->data1.bf7.kt6_3_mac_addr>>8)&0xFF;
                    pkey->mac[1] = pkeydata->data1.bf7.kt6_3_mac_addr&0xFF;
                    pkey->mac[2] = (pkeydata->data0.bf7.kt6_3_mac_addr>>24)&0xFF;
                    pkey->mac[3] = (pkeydata->data0.bf7.kt6_3_mac_addr>>16)&0xFF;
                    pkey->mac[4] = (pkeydata->data0.bf7.kt6_3_mac_addr>>8)&0xFF;
                    pkey->mac[5] = pkeydata->data0.bf7.kt6_3_mac_addr&0xFF;
                    pkey->mask |= CL_KEY_MASK_MACADDR;
                }
                if(!pkeydata->data2.bf7.kt6_3_msk_ev2pt_fld)
                {
                    pkey->ev2pt = pkeydata->data1.bf7.kt6_3_ev2pt;
                    pkey->mask |= CL_KEY_MASK_EV2PT;
                }
                if(!pkeydata->data2.bf7.kt6_3_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data2.bf7.kt6_3_tpid_enc;
                    pkey->mask |= CL_KEY_MASK_TPID;
                }
                if(!pkeydata->data2.bf7.kt6_3_msk_mac_da_range_flag_fld)
                {
                    pkey->mac_da_range_flag = pkeydata->data2.bf7.kt6_3_mac_da_range_flag;
                    pkey->mask |= CL_KEY_MASK_MACDA_RANGE_FLAG;
                }
                pkey->age_timer_offset = pkeydata->data2.bf7.kt6_3_age_tmr_offset;
                break;
            }
            case 2:
            {
                pkey->age_timer_offset  = pkeydata->data4.bf7.kt6_2_age_tmr_offset;
                if(!pkeydata->data4.bf7.kt6_2_msk_mac_addr_fld)
                {
                    pkey->mac_type = pkeydata->data4.bf7.kt6_2_mac_type;
                    pkey->mac[0] = (pkeydata->data3.bf7.kt6_2_mac_addr>>23)&0xFF;
                    pkey->mac[1] = (pkeydata->data3.bf7.kt6_2_mac_addr>>15)&0xFF;
                    pkey->mac[2] = (pkeydata->data3.bf7.kt6_2_mac_addr>>7)&0xFF;
                    pkey->mac[3] = (((pkeydata->data3.bf7.kt6_2_mac_addr<<1)
                                                  |(pkeydata->data2.bf7.kt6_2_mac_addr>>16))&0xFF);
                    pkey->mac[4] = (pkeydata->data2.bf7.kt6_2_mac_addr>>8)&0xFF;
                    pkey->mac[5] = pkeydata->data2.bf7.kt6_2_mac_addr&0xFF;
                    pkey->mask |= CL_KEY_MASK_MACADDR;
                }
                if(!pkeydata->data4.bf7.kt6_2_msk_ev2pt_fld)
                {
                    pkey->ev2pt = ((pkeydata->data4.bf7.kt6_2_ev2pt<<1)
                                               |pkeydata->data3.bf7.kt6_2_ev2pt);
                    pkey->mask |= CL_KEY_MASK_EV2PT;
                }
                if(!pkeydata->data4.bf7.kt6_2_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data4.bf7.kt6_2_tpid_enc;
                    pkey->mask |= CL_KEY_MASK_TPID;
                }
                if(!pkeydata->data4.bf7.kt6_2_msk_mac_da_range_flag_fld)
                {
                    pkey->mac_da_range_flag = pkeydata->data4.bf7.kt6_2_mac_da_range_flag;
                    pkey->mask |= CL_KEY_MASK_MACDA_RANGE_FLAG;
                }
                break;
            }
            case 1:
            {
                pkey->age_timer_offset  = ((pkeydata->data7.bf7.kt6_1_age_tmr_offset<<1)
                                                         |pkeydata->data6.bf7.kt6_1_age_tmr_offset);
                if(!pkeydata->data7.bf7.kt6_1_msk_mac_addr_fld)
                {
                    pkey->mac_type = pkeydata->data6.bf7.kt6_1_mac_type;
                    pkey->mac[0] = (pkeydata->data6.bf7.kt6_1_mac_addr>>6)&0xFF;
                    pkey->mac[1] = (((pkeydata->data6.bf7.kt6_1_mac_addr<<2)
                                                  |(pkeydata->data5.bf7.kt6_1_mac_addr>>30))&0xFF);
                    pkey->mac[2] = (pkeydata->data5.bf7.kt6_1_mac_addr>>22)&0xFF;
                    pkey->mac[3] = (pkeydata->data5.bf7.kt6_1_mac_addr>>14)&0xFF;
                    pkey->mac[4] = (pkeydata->data5.bf7.kt6_1_mac_addr>>6)&0xFF;
                    pkey->mac[5] = (((pkeydata->data5.bf7.kt6_1_mac_addr<<2)
                                                  |pkeydata->data4.bf7.kt6_1_mac_addr)&0xFF);
                    pkey->mask |= CL_KEY_MASK_MACADDR;
                }
                if(!pkeydata->data7.bf7.kt6_1_msk_ev2pt_fld)
                {
                    pkey->ev2pt = pkeydata->data6.bf7.kt6_1_ev2pt;
                    pkey->mask |= CL_KEY_MASK_EV2PT;
                }
                if(!pkeydata->data7.bf7.kt6_1_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data7.bf7.kt6_1_tpid_enc;
                    pkey->mask |= CL_KEY_MASK_TPID;
                }
                if(!pkeydata->data7.bf7.kt6_1_msk_mac_da_range_flag_fld)
                {
                    pkey->mac_da_range_flag = pkeydata->data7.bf7.kt6_1_mac_da_range_flag;
                    pkey->mask |= CL_KEY_MASK_MACDA_RANGE_FLAG;
                }
                break;
            }
            case 0:
            {
                pkey->age_timer_offset  = pkeydata->data9.bf7.kt6_0_age_tmr_offset ;
                if(!pkeydata->data9.bf7.kt6_0_msk_mac_addr_fld)
                {
                    pkey->mac_type = pkeydata->data9.bf7.kt6_0_mac_type;

                    pkey->mac[0] = (pkeydata->data8.bf7.kt6_0_mac_addr>>21)&0xFF;
                    pkey->mac[1] = (pkeydata->data8.bf7.kt6_0_mac_addr>>13)&0xFF;
                    pkey->mac[2] = (pkeydata->data8.bf7.kt6_0_mac_addr>>5)&0xFF;
                    pkey->mac[3] = (((pkeydata->data8.bf7.kt6_0_mac_addr<<3)
                                                  |(pkeydata->data7.bf7.kt6_0_mac_addr>>16))&0xFF);
                    pkey->mac[4] = (pkeydata->data7.bf7.kt6_0_mac_addr>>8)&0xFF;
                    pkey->mac[5] = pkeydata->data7.bf7.kt6_0_mac_addr&0xFF;
                    pkey->mask |= CL_KEY_MASK_MACADDR;
                }
                if(!pkeydata->data9.bf7.kt6_0_msk_ev2pt_fld)
                {
                    pkey->ev2pt = ((pkeydata->data9.bf7.kt6_0_ev2pt<<3)
                                              |pkeydata->data8.bf7.kt6_0_ev2pt);
                    pkey->mask |= CL_KEY_MASK_EV2PT;
                }
                if(!pkeydata->data9.bf7.kt6_0_msk_tpid_enc_fld)
                {
                    pkey->tpid_enc = pkeydata->data9.bf7.kt6_0_tpid_enc;
                    pkey->mask |= CL_KEY_MASK_TPID;
                }
                if(!pkeydata->data9.bf7.kt6_0_msk_mac_da_range_flag_fld)
                {
                    pkey->mac_da_range_flag = pkeydata->data9.bf7.kt6_0_mac_da_range_flag;
                    pkey->mask |= CL_KEY_MASK_MACDA_RANGE_FLAG;
                }
                break;
            }
            default:
                break;
        }
        if (pkey->mask==0)
        {
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        } 
    }
  
    return CS_E_OK;
}
/**********************************************************************************
* function    : __cl_ipv4_long_key_entry_get
* description :  indirect  get ce ipv4_long_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata: cl_ipv4_long_key_t *
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_ipv4_long_key_entry_get(FE_LE_CE_KEY_DATA_T *pkeydata, cl_ipv4_long_key_t *pdata)
{

    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
 
    if(!pkeydata->data9.bf0.kt0_msk_spid_fld)
    {
        pdata->spid = pkeydata->data8.bf0.kt0_spid;
        pdata->mask |= CL_KEY_MASK_SPID;
    }
    if(!pkeydata->data9.bf0.kt0_msk_vlan_cnt_fld)
    {
        pdata->vlan_count = pkeydata->data8.bf0.kt0_vlan_cnt;
        pdata->mask |= CL_KEY_MASK_VLAN_COUNT;
    }
    if(!pkeydata->data9.bf0.kt0_msk_p_tag_fld)
    {
        pdata->p_tag_flag = pkeydata->data8.bf0.kt0_p_tag_flag;
        pdata->mask |= CL_KEY_MASK_P_TAG;
    }
    if(!pkeydata->data9.bf0.kt0_msk_tpid_enc_fld)
    {
        pdata->tpid_enc = pkeydata->data8.bf0.kt0_tpid_enc;
        pdata->mask |= CL_KEY_MASK_TPID;
    }
    if(!pkeydata->data9.bf0.kt0_msk_igmp_type_fld)
    {
        pdata->igmp_type = pkeydata->data8.bf0.kt0_igmp_type;
        pdata->mask |= CL_KEY_MASK_IGMP_TYPE;
    }
    if(!pkeydata->data9.bf0.kt0_msk_icmp_type_fld)
    {
        pdata->icmp_type = pkeydata->data8.bf0.kt0_icmp_type;
        pdata->mask |= CL_KEY_MASK_ICMP_TYPE;
    }
    if(!pkeydata->data9.bf0.kt0_msk_tcp_flg_fld)
    {
        pdata->tcp_flags = ((pkeydata->data8.bf0.kt0_tcp_flg<<4)|pkeydata->data7.bf0.kt0_tcp_flg);
        pdata->mask |= CL_KEY_MASK_TCP_FLAGS;
    }
    if(!pkeydata->data9.bf0.kt0_msk_l4_port_fld)
    {
        pdata->l4_port_type = pkeydata->data7.bf0.kt0_l4_port_type;
        pdata->l4_port = ((pkeydata->data7.bf0.kt0_l4_port<<13)|pkeydata->data6.bf0.kt0_l4_port);
        pdata->mask |= CL_KEY_MASK_L4PORT;
    }
    if(pkeydata->data6.bf0.kt0_ip_fragment_state != 3)
    {
        pdata->ip_fragment_state = pkeydata->data6.bf0.kt0_ip_fragment_state;
        pdata->mask |= CL_KEY_MASK_IPFRAGMENT;
    }
    if(!pkeydata->data9.bf0.kt0_msk_ip_opt_flg_fld)
    {
        pdata->ip_option_flag = pkeydata->data6.bf0.kt0_ip_opt_flg;
        pdata->mask |= CL_KEY_MASK_IPOPTION;
    }
    if(!pkeydata->data9.bf0.kt0_msk_dscp_fld)
    {
        pdata->ipdscp = pkeydata->data6.bf0.kt0_dscp_tos;
        pdata->mask |= CL_KEY_MASK_DSCP;
    }
    if(!pkeydata->data9.bf0.kt0_msk_ippt_fld)
    {
        pdata->ipprotocol = pkeydata->data6.bf0.kt0_ippt;
        pdata->mask |= CL_KEY_MASK_IPPROTO;
    }
    if(pkeydata->data9.bf0.kt0_msk_ip_sa_fld)
    {
        pdata->srcip = pkeydata->data5.bf0.kt0_ip_sa;
        pdata->srcipmask = pkeydata->data9.bf0.kt0_msk_ip_sa_fld;
        pdata->mask |= CL_KEY_MASK_IPSA;
    }
    if(pkeydata->data9.bf0.kt0_msk_ip_da_fld)
    {
        pdata->dstip = pkeydata->data4.bf0.kt0_ip_da;
        pdata->dstipmask = pkeydata->data9.bf0.kt0_msk_ip_da_fld;
        pdata->mask |= CL_KEY_MASK_IPDA;
    }
    if(!pkeydata->data8.bf0.kt0_msk_ev2pt_fld)
    {
        pdata->ev2pt = pkeydata->data3.bf0.kt0_ev2pt;
        pdata->mask |= CL_KEY_MASK_EV2PT;
    }
    if(!pkeydata->data8.bf0.kt0_msk_mac_sa_fld)
    {
        pdata->srcmac[0] = (cs_int8)((pkeydata->data3.bf0.kt0_mac_sa>>8)&0xFF);
        pdata->srcmac[1] = (cs_int8)(pkeydata->data3.bf0.kt0_mac_sa&0xFF);
        pdata->srcmac[2] = (cs_int8)((pkeydata->data2.bf0.kt0_mac_sa >>24)&0xFF);
        pdata->srcmac[3] = (cs_int8)((pkeydata->data2.bf0.kt0_mac_sa>>16)&0xFF);
        pdata->srcmac[4] = (cs_int8)((pkeydata->data2.bf0.kt0_mac_sa>>8)&0xFF);
        pdata->srcmac[5] = (cs_int8)(pkeydata->data2.bf0.kt0_mac_sa&0xFF);
        pdata->mask |= CL_KEY_MASK_MACSA;
    }
    if(!pkeydata->data8.bf0.kt0_msk_mac_da_fld)
    {
        pdata->dstmac[0] = (cs_int8)((pkeydata->data1.bf0.kt0_mac_da>>8)&0xFF);
        pdata->dstmac[1] = (cs_int8)(pkeydata->data1.bf0.kt0_mac_da&0xFF);
        pdata->dstmac[2] = (cs_int8)((pkeydata->data0.bf0.kt0_mac_da >>24)&0xFF);
        pdata->dstmac[3] = (cs_int8)((pkeydata->data0.bf0.kt0_mac_da>>16)&0xFF);
        pdata->dstmac[4] = (cs_int8)((pkeydata->data0.bf0.kt0_mac_da>>8)&0xFF);
        pdata->dstmac[5] = (cs_int8)(pkeydata->data0.bf0.kt0_mac_da&0xFF);
        pdata->mask |= CL_KEY_MASK_MACDA;
    }
    if(!pkeydata->data9.bf0.kt0_msk_prsr_len_excd_fld)
    {
        pdata->prsr_len_excds = pkeydata->data1.bf0.kt0_prsr_len_excd;
        pdata->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
    }
    if(!pkeydata->data8.bf0.kt0_msk_802_1p_fld)
    {
        pdata->dot1p = pkeydata->data1.bf0.kt0_vl_802_1p;
        pdata->mask |= CL_KEY_MASK_DOT1P;
    }
    if(!pkeydata->data8.bf0.kt0_msk_vlanid_fld )
    {
        pdata->vlanid = pkeydata->data1.bf0.kt0_vlanid;
        pdata->mask |= CL_KEY_MASK_VLANID;
    }
    if(pkeydata->data7.bf0.kt0_spcl_hit_map)
    {
        pdata->spcl_hit_map = pkeydata->data7.bf0.kt0_spcl_hit_map;
        pdata->mask |= CL_KEY_MASK_SPECPKT;
    }
    if(pkeydata->data7.bf0.kt0_l4_src_hit_map)
    {
        pdata->l4_src_hit_map = pkeydata->data7.bf0.kt0_l4_src_hit_map;
        pdata->mask |= CL_KEY_MASK_L4SRC_RANGE;
    }
    if(pkeydata->data7.bf0.kt0_l4_dest_hit_map)
    {
        pdata->l4_dest_hit_map = pkeydata->data7.bf0.kt0_l4_dest_hit_map;
        pdata->mask |= CL_KEY_MASK_L4DST_RANGE;
    }
    if (pdata->mask==0)
    {
        pdata->mask |= CL_KEY_MASK_ALWAYS_MATCH;
    } 
     
    return CS_E_OK;

}
/**********************************************************************************
* function    : __cl_ipv4_short_key_entry_get
* description :  indirect  get ce ipv4_long_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata: CL_ipv4_short_key_t data[4]
* return      : CS_E_OK    - succeed
*                  CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_ipv4_short_key_entry_get(FE_LE_CE_KEY_DATA_T *pkeydata, cl_ipv4_short_key_t *pdata)
{
    cl_ipv4_short_key_t *pkey= NULL;
    cs_uint32 i;
  
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    for (i = 0; i<4; i++)
    {  
        pkey = (cl_ipv4_short_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_ipv4_short_key_t));
    
        switch (i)
        {       
            case 3:
            {           
                if(pkeydata->data2.bf3.kt1_3_msk_ip_addr_fld)
                {
                    pkey->ip = pkeydata->data0.bf3.kt1_3_ip_addr;
                    pkey->ip_type = pkeydata->data2.bf3.kt1_3_ip_addr_type;
                    pkey->ipmask = pkeydata->data2.bf3.kt1_3_msk_ip_addr_fld; 
                    pkey->mask |= CL_KEY_MASK_IPADDR;
                }
                if(!pkeydata->data2.bf3.kt1_3_msk_dscp_tos_fld)
                {
                    pkey->dscp_tos = pkeydata->data1.bf3.kt1_3_dscp_tos; 
                    pkey->mask |= CL_KEY_MASK_DSCP;
                }
                if(!pkeydata->data2.bf3.kt1_3_msk_ippt_fld)
                {
                    pkey->ipprotocol = pkeydata->data1.bf3.kt1_3_ippt;
                    pkey->mask |= CL_KEY_MASK_IPPROTO;                   
                }
                if(!pkeydata->data2.bf3.kt1_3_msk_l4_port_fld)
                {
                    pkey->l4_port = pkeydata->data1.bf3.kt1_3_l4_port;
                    pkey->l4_port_type =  pkeydata->data2.bf3.kt1_3_l4_port_type;
                    pkey->mask |= CL_KEY_MASK_L4PORT;
                } 
                if(!pkeydata->data9.bf3.kt1_3_msk_prsr_len_excd_fld)
                {
                    pkey->prsr_len_excds = pkeydata->data9.bf3.kt1_3_prsr_len_excd;
                    pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;                   
                }
                break;
            }
            case 2:
            {                   
                if( pkeydata->data4.bf3.kt1_2_msk_ip_addr_fld)
                {
                    pkey->ip = ((pkeydata->data3.bf3.kt1_2_ip_addr<<21)
                                        |pkeydata->data2.bf3.kt1_2_ip_addr);
                    pkey->ip_type = pkeydata->data4.bf3.kt1_2_ip_addr_type;
                    pkey->ipmask = pkeydata->data4.bf3.kt1_2_msk_ip_addr_fld; 
                    pkey->mask |= CL_KEY_MASK_IPADDR;
                }
                if(!pkeydata->data4.bf3.kt1_2_msk_dscp_tos_fld)
                {
                    pkey->dscp_tos = pkeydata->data4.bf3.kt1_2_dscp_tos; 
                    pkey->mask |= CL_KEY_MASK_DSCP;
                }
                if(!pkeydata->data4.bf3.kt1_2_msk_ippt_fld)
                {
                    pkey->ipprotocol = ((pkeydata->data4.bf3.kt1_2_ippt<<5)
                                                   |pkeydata->data3.bf3.kt1_2_ippt);
                    pkey->mask |= CL_KEY_MASK_IPPROTO;                   
                }
                if(!pkeydata->data4.bf3.kt1_2_msk_l4_port_fld)
                {
                    pkey->l4_port = pkeydata->data3.bf3.kt1_2_l4_port;
                    pkey->l4_port_type =  pkeydata->data4.bf3.kt1_2_l4_port_type;
                    pkey->mask |= CL_KEY_MASK_L4PORT;
                }
                if(!pkeydata->data9.bf3.kt1_2_msk_prsr_len_excd_fld)
                {
                    pkey->prsr_len_excds = pkeydata->data9.bf3.kt1_2_prsr_len_excd;
                    pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;                   
                }
                break;
            }
            case 1:
            {                               
                if( pkeydata->data6.bf3.kt1_1_msk_ip_addr_fld)
                {
                    pkey->ip = ((pkeydata->data5.bf3.kt1_1_ip_addr<<10)
                                        |pkeydata->data4.bf3.kt1_1_ip_addr);
                    pkey->ip_type = pkeydata->data6.bf3.kt1_1_ip_addr_type;
                    pkey->ipmask = pkeydata->data6.bf3.kt1_1_msk_ip_addr_fld; 
                    pkey->mask |= CL_KEY_MASK_IPADDR;
                }
                if(!pkeydata->data7.bf3.kt1_1_msk_dscp_tos_fld)
                {
                    pkey->dscp_tos = pkeydata->data6.bf3.kt1_1_dscp_tos; 
                    pkey->mask |= CL_KEY_MASK_DSCP;
                }
                if(!pkeydata->data6.bf3.kt1_1_msk_ippt_fld)
                {
                    pkey->ipprotocol = pkeydata->data6.bf3.kt1_1_ippt;
                    pkey->mask |= CL_KEY_MASK_IPPROTO;                   
                }
                if(!pkeydata->data6.bf3.kt1_1_msk_l4_port_fld)
                {
                    pkey->l4_port = ((pkeydata->data6.bf3.kt1_1_l4_port<<10)
                                               |pkeydata->data5.bf3.kt1_1_l4_port);
                    pkey->l4_port_type =  pkeydata->data6.bf3.kt1_1_l4_port_type;
                    pkey->mask |= CL_KEY_MASK_L4PORT;
                }    
                if(!pkeydata->data9.bf3.kt1_1_msk_prsr_len_excd_fld)
                {
                    pkey->prsr_len_excds = pkeydata->data9.bf3.kt1_1_prsr_len_excd;
                    pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;                   
                }
                break;
            }
            case 0:
            {                 
                if( pkeydata->data9.bf3.kt1_0_msk_ip_addr_fld)
                {
                    pkey->ip = ((pkeydata->data8.bf3.kt1_0_ip_addr<<31)
                                        |pkeydata->data7.bf3.kt1_0_ip_addr);
                    pkey->ip_type = pkeydata->data9.bf3.kt1_0_ip_addr_type;
                    pkey->ipmask = pkeydata->data9.bf3.kt1_0_msk_ip_addr_fld; 
                    pkey->mask |= CL_KEY_MASK_IPADDR;
                }
                if(!pkeydata->data9.bf3.kt1_0_msk_dscp_tos_fld)
                {
                    pkey->dscp_tos = ((pkeydata->data9.bf3.kt1_0_dscp_tos<<7)
                                                 |pkeydata->data8.bf3.kt1_0_dscp_tos); 
                    pkey->mask |= CL_KEY_MASK_DSCP;
                }
                if(!pkeydata->data9.bf3.kt1_0_msk_ippt_fld)
                {
                    pkey->ipprotocol = pkeydata->data8.bf3.kt1_0_ippt;
                    pkey->mask |= CL_KEY_MASK_IPPROTO;                   
                }
                if(!pkeydata->data9.bf3.kt1_0_msk_l4_port_fld)
                {
                    pkey->l4_port = pkeydata->data8.bf3.kt1_0_l4_port;
                    pkey->l4_port_type =  pkeydata->data9.bf3.kt1_0_l4_port_type;
                    pkey->mask |= CL_KEY_MASK_L4PORT;
                } 
                if(!pkeydata->data9.bf3.kt1_0_msk_prsr_len_excd_fld)
                {
                    pkey->prsr_len_excds = pkeydata->data9.bf3.kt1_0_prsr_len_excd;
                    pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;                   
                }
                break;
            }
            default:
                break;
        }
        if (pkey->mask==0)
        {
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        } 
    }
  
    return CS_E_OK;
}

/**********************************************************************************
* function    : __cl_ipv6_long_key_entry0_get
* description :  indirect  get ce ipv6_long_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata: cl_ipv6_long_key_t *
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_ipv6_long_key_entry0_get(FE_LE_CE_KEY_DATA_T *pkeydata, cl_ipv6_long_key_t *pdata)
{
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(!pkeydata->data9.bf1.kt2_0_msk_ipv6_flow_label_fld)
    {
        pdata->flow_label = ((pkeydata->data9.bf1.kt2_0_ipv6_flow_label<<16)
                                         |pkeydata->data8.bf1.kt2_0_ipv6_flow_label);
        pdata->mask |= CL_KEY_MASK_FLOW_LABEL;
    }
    if(pkeydata->data8.bf1.kt2_0_msk_ip_da_fld)
    {
        pdata->ipv6_da[0] = (cs_uint16)(pkeydata->data3.bf1.kt2_0_ip_da>>16);
        pdata->ipv6_da[1] = (cs_uint16)(pkeydata->data3.bf1.kt2_0_ip_da&0xFFFF);        
        pdata->ipv6_da[2] = (cs_uint16)(pkeydata->data2.bf1.kt2_0_ip_da>>16);
        pdata->ipv6_da[3] = (cs_uint16)(pkeydata->data2.bf1.kt2_0_ip_da&0xFFFF);        
        pdata->ipv6_da[4] = (cs_uint16)(pkeydata->data1.bf1.kt2_0_ip_da>>16);
        pdata->ipv6_da[5] = (cs_uint16)(pkeydata->data1.bf1.kt2_0_ip_da&0xFFFF);
        pdata->ipv6_da[6] = (cs_uint16)(pkeydata->data0.bf1.kt2_0_ip_da>>16);
        pdata->ipv6_da[7] = (cs_uint16)(pkeydata->data0.bf1.kt2_0_ip_da&0xFFFF);           
        pdata->dstipmask = pkeydata->data8.bf1.kt2_0_msk_ip_da_fld; 
        pdata->mask |= CL_KEY_MASK_IPDA;
    }
    if(pkeydata->data8.bf1.kt2_0_msk_ip_sa_fld)
    {
        pdata->ipv6_sa[0] = (cs_uint16)(pkeydata->data7.bf1.kt2_0_ip_sa>>16);
        pdata->ipv6_sa[1] = (cs_uint16)(pkeydata->data7.bf1.kt2_0_ip_sa&0xFFFF);        
        pdata->ipv6_sa[2] = (cs_uint16)(pkeydata->data6.bf1.kt2_0_ip_sa>>16);
        pdata->ipv6_sa[3] = (cs_uint16)(pkeydata->data6.bf1.kt2_0_ip_sa&0xFFFF);        
        pdata->ipv6_sa[4] = (cs_uint16)(pkeydata->data5.bf1.kt2_0_ip_sa>>16);
        pdata->ipv6_sa[5] = (cs_uint16)(pkeydata->data5.bf1.kt2_0_ip_sa&0xFFFF);
        pdata->ipv6_sa[6] = (cs_uint16)(pkeydata->data4.bf1.kt2_0_ip_sa>>16);
        pdata->ipv6_sa[7] = (cs_uint16)(pkeydata->data4.bf1.kt2_0_ip_sa&0xFFFF);           
        pdata->srcipmask = pkeydata->data8.bf1.kt2_0_msk_ip_sa_fld; 
        pdata->mask |= CL_KEY_MASK_IPSA;
    }

    return CS_E_OK;
}

/**********************************************************************************
* function    : __cl_ipv6_long_key_entry1_get
* description :  indirect  get ce ipv6_long_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata: cl_ipv6_long_key_t *
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_ipv6_long_key_entry1_get(FE_LE_CE_KEY_DATA_T *pkeydata, cl_ipv6_long_key_t *pdata)
{

    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
 
    if(!pkeydata->data9.bf2.kt2_1_msk_spid_fld)
    {
        pdata->spid = pkeydata->data8.bf2.kt2_1_spid;
        pdata->mask |= CL_KEY_MASK_SPID;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_vlan_cnt_fld)
    {
        pdata->vlan_count = pkeydata->data8.bf2.kt2_1_vlan_cnt;
        pdata->mask |= CL_KEY_MASK_VLAN_COUNT;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_p_tag_fld)
    {
        pdata->p_tag_flag = pkeydata->data8.bf2.kt2_1_p_tag_flag;
        pdata->mask |= CL_KEY_MASK_P_TAG;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_tpid_enc_fld)
    {
        pdata->tpid_enc = pkeydata->data8.bf2.kt2_1_tpid_enc;
        pdata->mask |= CL_KEY_MASK_TPID;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_igmp_type_fld)
    {
        pdata->igmp_type = pkeydata->data8.bf2.kt2_1_igmp_type;
        pdata->mask |= CL_KEY_MASK_IGMP_TYPE;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_icmp_type_fld)
    {
        pdata->icmp_type = pkeydata->data8.bf2.kt2_1_icmp_type;
        pdata->mask |= CL_KEY_MASK_ICMP_TYPE;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_tcp_flg_fld)
    {
        pdata->tcp_flags = ((pkeydata->data8.bf2.kt2_1_tcp_flg<<4)|pkeydata->data7.bf2.kt2_1_tcp_flg);
        pdata->mask |= CL_KEY_MASK_TCP_FLAGS;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_l4_port_fld)
    {
        pdata->l4_port_type = pkeydata->data7.bf2.kt2_1_l4_port_type;
        pdata->l4_port = ((pkeydata->data7.bf2.kt2_1_l4_port<<13)|pkeydata->data6.bf2.kt2_1_l4_port);
        pdata->mask |= CL_KEY_MASK_L4PORT;
    }
    if(pkeydata->data6.bf2.kt2_1_ip_fragment_state != 3)
    {
        pdata->ip_fragment_state = pkeydata->data6.bf2.kt2_1_ip_fragment_state;
        pdata->mask |= CL_KEY_MASK_IPFRAGMENT;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_ip_opt_flg_fld)
    {
        pdata->ip_option_flag = pkeydata->data6.bf2.kt2_1_ip_opt_flg;
        pdata->mask |= CL_KEY_MASK_IPOPTION;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_dscp_fld)
    {
        pdata->ipdscp = pkeydata->data6.bf2.kt2_1_dscp_tos;
        pdata->mask |= CL_KEY_MASK_DSCP;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_ippt_fld)
    {
        pdata->ipprotocol = pkeydata->data6.bf2.kt2_1_ippt;
        pdata->mask |= CL_KEY_MASK_IPPROTO;
    } 
    if(!pkeydata->data8.bf2.kt2_1_msk_ev2pt_fld)
    {
        pdata->ev2pt = pkeydata->data3.bf2.kt2_1_ev2pt;
        pdata->mask |= CL_KEY_MASK_EV2PT;
    }
    if(!pkeydata->data8.bf2.kt2_1_msk_mac_sa_fld)
    {
        pdata->srcmac[0] = (cs_int8)((pkeydata->data3.bf2.kt2_1_mac_sa>>8)&0xFF);
        pdata->srcmac[1] = (cs_int8)(pkeydata->data3.bf2.kt2_1_mac_sa&0xFF);
        pdata->srcmac[2] = (cs_int8)((pkeydata->data2.bf2.kt2_1_mac_sa >>24)&0xFF);
        pdata->srcmac[3] = (cs_int8)((pkeydata->data2.bf2.kt2_1_mac_sa>>16)&0xFF);
        pdata->srcmac[4] = (cs_int8)((pkeydata->data2.bf2.kt2_1_mac_sa>>8)&0xFF);
        pdata->srcmac[5] = (cs_int8)(pkeydata->data2.bf2.kt2_1_mac_sa&0xFF);
        pdata->mask |= CL_KEY_MASK_MACSA;
    }
    if(!pkeydata->data8.bf2.kt2_1_msk_mac_da_fld)
    {
        pdata->dstmac[0] = (cs_int8)((pkeydata->data1.bf2.kt2_1_mac_da>>8)&0xFF);
        pdata->dstmac[1] = (cs_int8)(pkeydata->data1.bf2.kt2_1_mac_da&0xFF);
        pdata->dstmac[2] = (cs_int8)((pkeydata->data0.bf2.kt2_1_mac_da >>24)&0xFF);
        pdata->dstmac[3] = (cs_int8)((pkeydata->data0.bf2.kt2_1_mac_da>>16)&0xFF);
        pdata->dstmac[4] = (cs_int8)((pkeydata->data0.bf2.kt2_1_mac_da>>8)&0xFF);
        pdata->dstmac[5] = (cs_int8)(pkeydata->data0.bf2.kt2_1_mac_da&0xFF);
        pdata->mask |= CL_KEY_MASK_MACDA;
    }
    if(!pkeydata->data9.bf2.kt2_1_msk_prsr_len_excd_fld)
    {
        pdata->prsr_len_excds = pkeydata->data1.bf2.kt2_1_prsr_len_excd;
        pdata->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
    }
    if(!pkeydata->data8.bf2.kt2_1_msk_802_1p_fld)
    {
        pdata->dot1p = pkeydata->data1.bf2.kt2_1_vl_802_1p;
        pdata->mask |= CL_KEY_MASK_DOT1P;
    }
    if(!pkeydata->data8.bf2.kt2_1_msk_vlanid_fld )
    {
        pdata->vlanid = pkeydata->data1.bf2.kt2_1_vlanid;
        pdata->mask |= CL_KEY_MASK_VLANID;
    }
    if(pkeydata->data7.bf2.kt2_1_spcl_hit_map)
    {
        pdata->spcl_hit_map = pkeydata->data7.bf2.kt2_1_spcl_hit_map;
        pdata->mask |= CL_KEY_MASK_SPECPKT;
    }
    if(pkeydata->data7.bf2.kt2_1_l4_src_hit_map)
    {
        pdata->l4_src_hit_map = pkeydata->data7.bf2.kt2_1_l4_src_hit_map;
        pdata->mask |= CL_KEY_MASK_L4SRC_RANGE;
    }
    if(pkeydata->data7.bf2.kt2_1_l4_dest_hit_map)
    {
        pdata->l4_dest_hit_map = pkeydata->data7.bf2.kt2_1_l4_dest_hit_map;
        pdata->mask |= CL_KEY_MASK_L4DST_RANGE;
    }

    return CS_E_OK;

}

/**********************************************************************************
* function    : __cl_ipv6_short_key_entry_get
* description :  indirect  get ce ipv6_short_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata: cl_ipv6_short_key_t *
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_ipv6_short_key_entry_get(FE_LE_CE_KEY_DATA_T *pkeydata, cl_ipv6_short_key_t *pdata)
{
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if(!pkeydata->data6.bf4.kt3_0_msk_prsr_len_excd_fld)
    {
        pdata->prsr_len_excds = pkeydata->data6.bf4.kt3_0_prsr_len_excd;
        pdata->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
    }
    if(!pkeydata->data6.bf4.kt3_0_msk_ipv6_flow_label_fld)
    {
        pdata->flow_label = ((pkeydata->data6.bf4.kt3_0_ipv6_flow_label<<19)
                                         |pkeydata->data5.bf4.kt3_0_ipv6_flow_label);
        pdata->mask |= CL_KEY_MASK_FLOW_LABEL;
    }
    if(!pkeydata->data5.bf4.kt3_0_msk_dscp_tos_fld)
    {
        pdata->dscp_tos = pkeydata->data4.bf4.kt3_0_dscp_tos;
        pdata->mask |= CL_KEY_MASK_DSCP;
    }
    if(!pkeydata->data5.bf4.kt3_0_msk_ippt_fld)
    {
        pdata->ipprotocol = pkeydata->data4.bf4.kt3_0_ippt;
        pdata->mask |= CL_KEY_MASK_IPPROTO;
    }
    if(!pkeydata->data5.bf4.kt3_0_msk_l4_port_fld)
    {
        pdata->l4_port_type = pkeydata->data5.bf4.kt3_0_l4_port_type;
        pdata->l4_port = pkeydata->data4.bf4.kt3_0_l4_port;
        pdata->mask |= CL_KEY_MASK_L4PORT;
    }
    if(pkeydata->data5.bf4.kt3_0_msk_ip_addr_fld)
    {
        pdata->ipv6_addr[0] = (cs_uint16)(pkeydata->data3.bf4.kt3_0_ip_addr>>16);
        pdata->ipv6_addr[1] = (cs_uint16)(pkeydata->data3.bf4.kt3_0_ip_addr&0xFFFF);        
        pdata->ipv6_addr[2] = (cs_uint16)(pkeydata->data2.bf4.kt3_0_ip_addr>>16);
        pdata->ipv6_addr[3] = (cs_uint16)(pkeydata->data2.bf4.kt3_0_ip_addr&0xFFFF);        
        pdata->ipv6_addr[4] = (cs_uint16)(pkeydata->data1.bf4.kt3_0_ip_addr>>16);
        pdata->ipv6_addr[5] = (cs_uint16)(pkeydata->data1.bf4.kt3_0_ip_addr&0xFFFF);
        pdata->ipv6_addr[6] = (cs_uint16)(pkeydata->data0.bf4.kt3_0_ip_addr>>16);
        pdata->ipv6_addr[7] = (cs_uint16)(pkeydata->data0.bf4.kt3_0_ip_addr&0xFFFF);        
        pdata->ip_type = pkeydata->data5.bf4.kt3_0_ip_addr_type;
        pdata->ipmask = pkeydata->data5.bf4.kt3_0_msk_ip_addr_fld; 
        pdata->mask |= CL_KEY_MASK_IPADDR;
    }
    if (pdata->mask==0)
    {
        pdata->mask |= CL_KEY_MASK_ALWAYS_MATCH;
    } 
    
    return CS_E_OK;
}

/**********************************************************************************
* function    : __cl_mirror_key_entry_get
* description :  indirect  get ce mirror_key memory table
*
* input       :    FE_LE_CE_KEY_DATA_T  *pkeydata
*              
* output      : pdata: cl_mirror_key_t data[8]
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status __cl_mirror_key_entry_get(FE_LE_CE_KEY_DATA_T *pkeydata, cl_mirror_key_t *pdata)
{
    cl_mirror_key_t *pkey= NULL;
    cs_uint32 i;
  
    if((pdata==NULL)||(pkeydata==NULL))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    for (i = 0; i<8; i++)
    {  
        pkey = (cl_mirror_key_t *) ((cs_uint8*)pdata + i*sizeof(cl_mirror_key_t));
    
        switch (i)
        {
            case 0:
            {                        
                pkey->mirror_pkt_flag = pkeydata->data1.bf5.kt4_0_mirror_pkt_flag;
                
                if(!pkeydata->data0.bf5.kt4_0_msk_spid_fld)
                {
                    pkey->spid = pkeydata->data0.bf5.kt4_0_spid;
                    pkey->mask |= CL_KEY_MASK_SPID;
                }
                if(!pkeydata->data0.bf5.kt4_0_msk_vlanid_fld)
                {
                    pkey->vlanid = pkeydata->data0.bf5.kt4_0_vlanid;
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }      
                if(!pkeydata->data0.bf5.kt4_0_msk_mirror_group_id_fld)
                {
                    pkey->mirror_group_id = pkeydata->data0.bf5.kt4_0_mirror_group_id;
                    pkey->mask |= CL_KEY_MASK_MIRROR_GROUP;
                }     
                break;
            }
            case 1:
            {                       
                pkey->mirror_pkt_flag = pkeydata->data2.bf5.kt4_1_mirror_pkt_flag;
                
                if(!pkeydata->data1.bf5.kt4_1_msk_spid_fld)
                {
                    pkey->spid = pkeydata->data1.bf5.kt4_1_spid;
                    pkey->mask |= CL_KEY_MASK_SPID;
                }
                if(!pkeydata->data1.bf5.kt4_1_msk_vlanid_fld)
                {
                    pkey->vlanid = pkeydata->data1.bf5.kt4_1_vlanid;
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }
                if(!pkeydata->data1.bf5.kt4_1_msk_mirror_group_id_fld)
                {
                    pkey->mirror_group_id = pkeydata->data1.bf5.kt4_1_mirror_group_id;
                    pkey->mask |= CL_KEY_MASK_MIRROR_GROUP;
                }        
                break;
            }
            case 2:
            {        
                pkey->mirror_pkt_flag = pkeydata->data3.bf5.kt4_2_mirror_pkt_flag;
                
                if(!pkeydata->data2.bf5.kt4_2_msk_spid_fld)
                {
                    pkey->spid = pkeydata->data2.bf5.kt4_2_spid;
                    pkey->mask |= CL_KEY_MASK_SPID;
                }
                if(!pkeydata->data2.bf5.kt4_2_msk_vlanid_fld)
                {
                    pkey->vlanid = pkeydata->data2.bf5.kt4_2_vlanid;
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }  
                if(!pkeydata->data3.bf5.kt4_2_msk_mirror_group_id_fld)
                {
                    pkey->mirror_group_id = ((pkeydata->data3.bf5.kt4_2_mirror_group_id<<1)
                                                            |pkeydata->data2.bf5.kt4_2_mirror_group_id);
                    pkey->mask |= CL_KEY_MASK_MIRROR_GROUP;
                }           
                break;
            }
            case 3:
            {        
                pkey->mirror_pkt_flag = pkeydata->data4.bf5.kt4_3_mirror_pkt_flag;
                
                if(!pkeydata->data3.bf5.kt4_3_msk_spid_fld)
                {
                    pkey->spid = pkeydata->data3.bf5.kt4_3_spid;
                    pkey->mask |= CL_KEY_MASK_SPID;
                }
                if(!pkeydata->data4.bf5.kt4_3_msk_vlanid_fld)
                {
                    pkey->vlanid = ((pkeydata->data4.bf5.kt4_3_vlanid<<7)
                                             |pkeydata->data3.bf5.kt4_3_vlanid);
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }  
                if(!pkeydata->data4.bf5.kt4_3_msk_mirror_group_id_fld)
                {
                    pkey->mirror_group_id = pkeydata->data4.bf5.kt4_3_mirror_group_id;
                    pkey->mask |= CL_KEY_MASK_MIRROR_GROUP;
                }
                break;
            }
            case 4:
            {        
                pkey->mirror_pkt_flag = pkeydata->data6.bf5.kt4_4_mirror_pkt_flag;
                
                if(!pkeydata->data4.bf5.kt4_4_msk_spid_fld)
                {
                    pkey->spid = pkeydata->data4.bf5.kt4_4_spid;
                    pkey->mask |= CL_KEY_MASK_SPID;
                }
                if(!pkeydata->data5.bf5.kt4_4_msk_vlanid_fld)
                {
                    pkey->vlanid = pkeydata->data5.bf5.kt4_4_vlanid;
                    pkey->mask |= CL_KEY_MASK_VLANID;
                } 
                if(!pkeydata->data5.bf5.kt4_4_msk_mirror_group_id_fld)
                {
                    pkey->mirror_group_id = pkeydata->data5.bf5.kt4_4_mirror_group_id;
                    pkey->mask |= CL_KEY_MASK_MIRROR_GROUP;
                }           
                break;
            }
            case 5:
            {        
                pkey->mirror_pkt_flag = pkeydata->data7.bf5.kt4_5_mirror_pkt_flag;
                
                if(!pkeydata->data6.bf5.kt4_5_msk_spid_fld)
                {
                    pkey->spid = pkeydata->data6.bf5.kt4_5_spid;
                    pkey->mask |= CL_KEY_MASK_SPID;
                }
                if(!pkeydata->data6.bf5.kt4_5_msk_vlanid_fld)
                {
                    pkey->vlanid = pkeydata->data6.bf5.kt4_5_vlanid;
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }  
                if(!pkeydata->data6.bf5.kt4_5_msk_mirror_group_id_fld)
                {
                    pkey->mirror_group_id = pkeydata->data6.bf5.kt4_5_mirror_group_id;
                    pkey->mask |= CL_KEY_MASK_MIRROR_GROUP;
                }      
                break;
            }
            case 6:
            {        
                pkey->mirror_pkt_flag = pkeydata->data8.bf5.kt4_6_mirror_pkt_flag;
                
                if(!pkeydata->data7.bf5.kt4_6_msk_spid_fld)
                {
                    pkey->spid = pkeydata->data7.bf5.kt4_6_spid;
                    pkey->mask |= CL_KEY_MASK_SPID;
                }
                if(!pkeydata->data7.bf5.kt4_6_msk_vlanid_fld)
                {
                    pkey->vlanid = pkeydata->data7.bf5.kt4_6_vlanid;
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }     
                if(!pkeydata->data8.bf5.kt4_6_msk_mirror_group_id_fld)
                {
                    pkey->mirror_group_id = pkeydata->data7.bf5.kt4_6_mirror_group_id;
                    pkey->mask |= CL_KEY_MASK_MIRROR_GROUP;
                }    
                break;
            }
            case 7:
            {        
                pkey->mirror_pkt_flag = pkeydata->data9.bf5.kt4_7_mirror_pkt_flag;
                
                if(!pkeydata->data8.bf5.kt4_7_msk_spid_fld)
                {
                    pkey->spid = pkeydata->data8.bf5.kt4_7_spid;
                    pkey->mask |= CL_KEY_MASK_SPID;
                }
                if(!pkeydata->data9.bf5.kt4_7_msk_vlanid_fld)
                {
                    pkey->vlanid = ((pkeydata->data9.bf5.kt4_7_vlanid<<11)
                                             &pkeydata->data8.bf5.kt4_7_vlanid);
                    pkey->mask |= CL_KEY_MASK_VLANID;
                }
                if(!pkeydata->data9.bf5.kt4_7_msk_mirror_group_id_fld)
                {
                    pkey->mirror_group_id = pkeydata->data9.bf5.kt4_7_mirror_group_id;
                    pkey->mask |= CL_KEY_MASK_MIRROR_GROUP;
                }       
                break;
            }
             default:
                break;
        }
        if (pkey->mask==0)
        {
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        } 
    }
  
    return CS_E_OK;
}

//  Pullic
//===================================================================================================


/**********************************************************************************
* function    : cl_entry_valid_set
* description : write ce cl_entry_vld
* input       :      port_id (0-2)
*                    entryindex: 0-47 (MA 0-15 )
*                    status:  0,the cl entry invlaid;
*                             1,the cl entry valid;
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status  cl_entry_valid_set(cs_uint8 port_id, cs_uint8 entryindex, cs_boolean  status)
{
    cs_uint32 bitmap_low=0;
    cs_uint16 bitmap_high=0;

   if((entryindex>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ_PORT(FE_LE_CE_KEY_VLD1, port_id, cl_entry_vld, bitmap_high);
    REG_READ_PORT(FE_LE_CE_KEY_VLD0, port_id, bitmap_low); 
   
    if(entryindex>31)
    {
        if (status)
            SETBIT(bitmap_high, (entryindex-32));      /* enable rule_index */
        else
            CLRBIT(bitmap_high, (entryindex-32));
    }
    else
    {
        if (status)
            SETBIT(bitmap_low, entryindex);      /* enable rule_index */
        else
            CLRBIT(bitmap_low, entryindex);
    }
    REG_FIELD_WRITE_PORT(FE_LE_CE_KEY_VLD1, port_id, cl_entry_vld, bitmap_high);
    REG_WRITE_PORT(FE_LE_CE_KEY_VLD0, port_id, bitmap_low); 

    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_entry_valid_get
* description : read ce cl_entry_vld
* input       :      port_id (0-2)
*                    entryindex: 0-47 (MA 0-15 )
*
* output      :    status:  0,the cl entry invlaid;
*                           1,the cl entry valid;
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to read register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status cl_entry_valid_get(cs_uint8 port_id, cs_uint8 entryindex, cs_boolean *status)
{
    cs_uint32 bitmap_low;
    cs_uint16 bitmap_high;

    if((status ==NULL)||(entryindex>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ_PORT(FE_LE_CE_KEY_VLD1, port_id, cl_entry_vld, bitmap_high);
    REG_READ_PORT(FE_LE_CE_KEY_VLD0, port_id, bitmap_low); 
  
    if(entryindex>31)
    {
        if (BITON(bitmap_high, (entryindex-32)))
            *status = 1;
        else
            *status = 0;
    }
    else
    {
        if (BITON(bitmap_low, entryindex))
            *status = 1;
        else
            *status = 0;
    }
    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_rule_mode_set
* description : write ce  rule mode ctrl
* input       :      port_id (0-2)
*                    entryindex: 0-47 (MA 0-15 )
*                    status:  0,AND
*                             1,OR
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status  cl_rule_mode_set(cs_uint8 port_id, cs_uint8 entryindex, cs_boolean  status)
{
    cs_uint32 bitmap_low;
    cs_uint16 bitmap_high;

   if((entryindex>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ_PORT(FE_LE_CE_RULE_MODE_CTRL1, port_id, cl_rule_or_en, bitmap_high);
    REG_READ_PORT(FE_LE_CE_RULE_MODE_CTRL0, port_id, bitmap_low); 
   
    if(entryindex>31)
    {
        if (status)
            SETBIT(bitmap_high, (entryindex-32));      /*  OR  function */
        else
            CLRBIT(bitmap_high, (entryindex-32));
    }
    else
    {
        if (status)
            SETBIT(bitmap_low, entryindex);     /*  OR  function */
        else
            CLRBIT(bitmap_low, entryindex);
    }
    REG_FIELD_WRITE_PORT(FE_LE_CE_RULE_MODE_CTRL1, port_id, cl_rule_or_en, bitmap_high);
    REG_WRITE_PORT(FE_LE_CE_RULE_MODE_CTRL0, port_id, bitmap_low); 
   
    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_rule_mode_get
* description : read  ce  rule mode ctrl
* input       :      port_id (0-2)
*                    entryindex: 0-47 (MA 0-15 )
*
* output      :      status:  0,AND;
*                                 1,OR;
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to read register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status cl_rule_mode_get(cs_uint8 port_id, cs_uint8 entryindex, cs_boolean *status)
{
    cs_uint32 bitmap_low;
    cs_uint16 bitmap_high;


    if((status ==NULL)||(entryindex>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ_PORT(FE_LE_CE_RULE_MODE_CTRL1, port_id, cl_rule_or_en, bitmap_high);
    REG_READ_PORT(FE_LE_CE_RULE_MODE_CTRL0, port_id, bitmap_low); 
 
    if(entryindex>31)
    {
        if (BITON(bitmap_high, (entryindex-32)))
            *status = 1;
        else
            *status = 0;
    }
    else
    {
        if (BITON(bitmap_low, entryindex))
            *status = 1;
        else
            *status = 0;
    }
    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_spec_key_set
* description : indirect write ce  rule spec_key
* input       :    port_id (0-2)
*                  addr   (0-7)
*                  pSpecKeyCfg: cl_spec_key_cfg_t
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status cl_spec_key_set(cs_uint8 port_id, cs_uint8 addr, cl_spec_key_cfg_t * pSpecKeyCfg)
{
    FE_LE_CE_SPCL_KEY_MEM_DATA1_t      data1;         
    FE_LE_CE_SPCL_KEY_MEM_DATA0_t      data0;         
    cs_status ret = 0;

    if((pSpecKeyCfg==NULL)||(port_id>AAL_FE_MA_ID)||(addr>7))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    data1.bf.spcl_key_ms_vld = pSpecKeyCfg->spec_ms_vld;
    data1.bf.spcl_hdr_type   = pSpecKeyCfg->spec_hdr_type;
    data0.bf.spcl_key        = pSpecKeyCfg->spec_key;
    data0.bf.spcl_msk        = pSpecKeyCfg->spec_msk;

    ret = __cl_specl_key_table_access( port_id, LE_CL_TABLE_WRITE, addr, (cs_uint32 *)&data1, (cs_uint32 *)&data0);
    if (ret)
    {
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_spec_key_get
* description :  indirect  read ce  rule spec_key table
* input       :      port_id (0-2)
*                    addr(phy index): 0-7
* output      : pdata:cl_spec_key_cfg_t *
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to read register
* globals     : none
* others      :
*
*************************************************************************************/
cs_status cl_spec_key_get(cs_uint8 port_id, cs_uint8 addr, cl_spec_key_cfg_t * pSpecKeyCfg)
{
    FE_LE_CE_SPCL_KEY_MEM_DATA1_t      data1;         
    FE_LE_CE_SPCL_KEY_MEM_DATA0_t      data0;         
    cs_status ret = 0;

    if((pSpecKeyCfg==NULL)||(port_id>AAL_FE_MA_ID)||(addr>7))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    ret = __cl_specl_key_table_access( port_id, LE_CL_TABLE_READ, addr, (cs_uint32 *)&data1, (cs_uint32 *)&data0);
    if (ret)
    {
        return CS_E_ERROR;
    }
   
    pSpecKeyCfg->spec_key      = data0.bf.spcl_key;
    pSpecKeyCfg->spec_msk      = data0.bf.spcl_msk;
    pSpecKeyCfg->spec_ms_vld   = data1.bf.spcl_key_ms_vld;
    pSpecKeyCfg->spec_hdr_type = data1.bf.spcl_hdr_type;

    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_l4_key_set
* description : indirect write ce  rule L4_key
* input       :      port_id (0-2)
*                    index   (0-7)
*                    port_type 0:CL_L4_SRC_TCP;1:CL_L4_SRC_UDP.2 ,3
*                    pdata: cl_l4key_cfg_t
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : none
* others      :
*
*************************************************************************************/
cs_status cl_l4_key_set(cs_uint8 port_id, cs_uint8 index, cs_uint8 port_type, cl_l4key_cfg_t * pdata)
{
    FE_LE_CE_L4_PORT_KEY_MEM_DATA_t  l4port;
    FE_LE_CE_L4_CTRL_t               data;
    cs_status  ret     = 0;
    cs_uint8   addr    = 0;
    

    if((pdata ==NULL)||(index>7)||(port_type>CL_L4_DST_UDP)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    if(pdata->valid> 0x1)
        return CS_E_ERROR;
  
    if(pdata->L4PortLow > pdata->L4PortHigh )
        return CS_E_ERROR;

    data.wrd   = 0;
    l4port.wrd = 0;

    REG_READ_PORT(FE_LE_CE_L4_CTRL, port_id, data.wrd); 
              
    if(port_type == CL_L4_SRC_TCP)
    {
        addr = index;
        l4port.bf.l4_port_high = pdata->L4PortHigh;
        l4port.bf.l4_port_low  = pdata->L4PortLow;
        ret = __cl_l4port_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, (cs_uint32 *)&l4port);
       
        if(pdata->valid)
        {
            SETBIT(data.bf.tcp_sp_chk_vld, index);
        }
        else
        {
            CLRBIT(data.bf.tcp_sp_chk_vld, index);
        }
    }
    else if(port_type == CL_L4_SRC_UDP)
    {
        addr = index+16;
        l4port.bf.l4_port_high = pdata->L4PortHigh;
        l4port.bf.l4_port_low  = pdata->L4PortLow;
        ret = __cl_l4port_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, (cs_uint32 *)&l4port);
        
        if(pdata->valid)
        {
            SETBIT(data.bf.udp_sp_chk_vld, index);
        }
        else
        {
            CLRBIT(data.bf.udp_sp_chk_vld, index);
        }
    }
    else if(port_type == CL_L4_DST_TCP)
    {
        addr = index+8;
        l4port.bf.l4_port_high = pdata->L4PortHigh;
        l4port.bf.l4_port_low  = pdata->L4PortLow;
        ret = __cl_l4port_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, (cs_uint32 *)&l4port);

        if(pdata->valid)
        {
            SETBIT(data.bf.tcp_dp_chk_vld, index);
        }
        else
        {
            CLRBIT(data.bf.tcp_dp_chk_vld, index);
        }
    }
    else if(port_type == CL_L4_DST_UDP)
    {
        addr = index+24;         
        l4port.bf.l4_port_high = pdata->L4PortHigh;
        l4port.bf.l4_port_low  = pdata->L4PortLow;
        ret = __cl_l4port_key_table_access(port_id, LE_CL_TABLE_WRITE, addr, (cs_uint32 *)&l4port);
      
        if(pdata->valid)
        {
            SETBIT(data.bf.udp_dp_chk_vld, index);
        }
        else
        {
            CLRBIT(data.bf.udp_dp_chk_vld, index);
        }
    }  
    if (ret)
    {
        return CS_E_ERROR;
    }
    
    REG_WRITE_PORT(FE_LE_CE_L4_CTRL, port_id, data.wrd); 
    
    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_l4_key_get
* description : indirect read ce rule L4_key
* input       :      port_id (0-2)
*                    index   (0-7)
*                    port_type 0:CL_L4_SRC_TCP;1:CL_L4_SRC_UDP.2 ,3
*
* output      : pdata: cl_l4key_cfg_t
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to read register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status cl_l4_key_get(cs_uint8 port_id, cs_uint8 index, cs_uint8 port_type, cl_l4key_cfg_t * pdata)
{
    FE_LE_CE_L4_PORT_KEY_MEM_DATA_t  l4port;
    FE_LE_CE_L4_CTRL_t               data;
    cs_status ret  = 0;
    cs_uint8  addr = 0;


    if((pdata ==NULL)||(index>7)||(port_type>CL_L4_DST_UDP)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    data.wrd   = 0;
    l4port.wrd = 0;

    REG_READ_PORT(FE_LE_CE_L4_CTRL, port_id, data.wrd); 
    
    if(port_type == CL_L4_SRC_TCP)
    {
        addr = index;
        ret = __cl_l4port_key_table_access(port_id, LE_CL_TABLE_READ, addr, (cs_uint32 *)&l4port);

        if(BITON(data.bf.tcp_sp_chk_vld, index))
        {           
            pdata->valid = 1;     
        }
    }
    else if(port_type == CL_L4_SRC_UDP)
    {
        addr = index+16;
        ret = __cl_l4port_key_table_access(port_id, LE_CL_TABLE_READ, addr, (cs_uint32 *)&l4port);

        if(BITON(data.bf.udp_sp_chk_vld, index))
        {         
            pdata->valid = 1;      
        }
    }
    else if(port_type == CL_L4_DST_TCP)
    {
        addr = index+8;
        ret = __cl_l4port_key_table_access(port_id, LE_CL_TABLE_READ, addr, (cs_uint32 *)&l4port);

        if(BITON(data.bf.tcp_dp_chk_vld, index))
        {         
            pdata->valid = 1;      
        }
    }
    else if(port_type == CL_L4_DST_UDP)
    {
        addr = index+24;
        ret = __cl_l4port_key_table_access(port_id, LE_CL_TABLE_READ, addr, (cs_uint32 *)&l4port);
   
        if(BITON(data.bf.udp_dp_chk_vld, index))
        {     
            pdata->valid = 1;
        }          
    }
    if (ret)
    {
        return CS_E_ERROR;
    }
    
    pdata->L4PortHigh = l4port.bf.l4_port_high;
    pdata->L4PortLow  = l4port.bf.l4_port_low;

    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_mac_filter_aging_time_set
* description : indirect write ce  rule mac filter age table
* input       :      port_id (0-2)
*                    addr   (0-63)
*                    pmacageCfg: cl_mac_age_cfg_t *
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status cl_mac_filter_aging_time_set(cs_uint8 port_id, cs_uint8 addr, cl_mac_age_cfg_t * pmacageCfg)
{
    cs_status  ret       = 0;
    cs_uint32  max_value = 0;
    cs_uint64  temp64    = 0;
    cs_uint32  timer     = 0;
    FE_LE_CE_MAC_FLTR_AGNG_MEM_DATA_t value;

    if((pmacageCfg ==NULL)||(addr>CL_MAC_AGING_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(pmacageCfg->time>16200){
        pmacageCfg->time = 16200;
    }
    /*80MHZ for FPGA, 125M hz for asic,1000 cyc */
    /*timer*max*64/125=1us*/
    REG_READ_PORT(FE_LE_CE_MAC_FLTR_AGNG_TMR_CFG, port_id, timer); 
    temp64 = (1000000 * 125 * (cs_uint64)pmacageCfg->time) / (64 * (cs_uint64)timer);
    max_value = temp64; 
    g_mfilter_aging_time[port_id][addr] = pmacageCfg->time;
    
    value.wrd = 0;
    value.bf.max_value = max_value;
    value.bf.aging_vld = pmacageCfg->valid;
    ret = __cl_mac_filter_aging_table_access(port_id, LE_CL_TABLE_WRITE, addr, (cs_uint32 *)&value);

    return ret;    
}

/**********************************************************************************
* function    : cl_mac_filter_aging_time_get
* description :  indirect  read ce  rule mac filter age table
* input       :      port_id (0-2)
*                    addr(phy index): (0-63)
* output      : pdata:cl_mac_age_cfg_t *
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to read register
* globals     : none
* others      :
*
*************************************************************************************/
cs_status cl_mac_filter_aging_time_get(cs_uint8 port_id, cs_uint8 addr, cl_mac_age_cfg_t * pdata)
{
    cs_status ret =0;
    FE_LE_CE_MAC_FLTR_AGNG_MEM_DATA_t value;

    if((pdata ==NULL)||(addr>CL_MAC_AGING_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    ret = __cl_mac_filter_aging_table_access(port_id, LE_CL_TABLE_READ, addr, (cs_uint32 *)&value);
    if (ret)
    {
        return CS_E_ERROR;
    }
    pdata->time  = g_mfilter_aging_time[port_id][addr];
    pdata->valid = value.bf.aging_vld;

    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_ctrl_set
* description : set  ce ctrl cfg
*
* input       :      port_id (0-2)
*                    pdata:cl_ctrl_cfg_t *
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to set
* globals     : none
*
* others      :
*
*************************************************************************************/
cs_status cl_ctrl_set(cs_uint8 port_id, cl_ctrl_cfg_t *pdata)
{
    FE_LE_CE_CTRL_t msk;
    FE_LE_CE_CTRL_t data;
    FE_LE_ENGINE_CTRL_t enmsk;
    FE_LE_ENGINE_CTRL_t endata;


    if((pdata==NULL)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(pdata->mask ==0)
        return CS_E_OK;
    
    data.wrd   = 0;
    msk.wrd    = 0;
    enmsk.wrd  = 0;
    endata.wrd = 0;
       
    if(pdata->mask&CL_CFG_MASK_AGING_EN)
    {
        if(pdata->mac_filter_aging_en>1)
            return CS_E_ERROR;
        msk.bf.mac_filter_aging_en  = 1;
        data.bf.mac_filter_aging_en = pdata->mac_filter_aging_en;
    }
    if(pdata->mask&CL_CFG_MASK_CL_START)
    {
        if(pdata->cl_start_entry>CL_ENTRY_MAXNUM)
            return CS_E_ERROR;
        msk.bf.cl_start_entry  = 1;
        data.bf.cl_start_entry = pdata->cl_start_entry;
    }
    if(pdata->mask&CL_CFG_MASK_DENY_MATCH)
    {
        msk.bf.deny_no_type0_match  =1;
        msk.bf.deny_no_type1_match  =1;
        msk.bf.deny_no_type2_match  =1;
        msk.bf.deny_no_type3_match  =1;
        msk.bf.deny_no_type4_match  =1;
        msk.bf.deny_no_type5_match  =1;
        msk.bf.deny_no_type6_match  =1;
        msk.bf.deny_no_type7_match  =1;
        msk.bf.deny_no_type8_match  =1;
        msk.bf.deny_no_type9_match  =1;
        msk.bf.deny_no_type10_match =1;
        msk.bf.deny_no_type11_match =1;
        msk.bf.deny_no_type12_match =1;
        msk.bf.deny_no_type13_match =1;
        msk.bf.deny_no_type14_match =1;
        msk.bf.deny_no_type15_match =1;
        data.bf.deny_no_type0_match  = pdata->deny_no_type_match.s.type_ipv4_long_key;
        data.bf.deny_no_type1_match  = pdata->deny_no_type_match.s.type_ipv4_short_key;
        data.bf.deny_no_type2_match  = pdata->deny_no_type_match.s.type_ipv6_long_key;
        data.bf.deny_no_type3_match  = pdata->deny_no_type_match.s.type_ipv6_short_key;
        data.bf.deny_no_type4_match  = pdata->deny_no_type_match.s.type_mirror_key;
        data.bf.deny_no_type5_match  = pdata->deny_no_type_match.s.type_mac_vlan_key;
        data.bf.deny_no_type6_match  = pdata->deny_no_type_match.s.type_mac_etype_key;
        data.bf.deny_no_type7_match  = pdata->deny_no_type_match.s.type_ctrl_pkt_key;
        data.bf.deny_no_type8_match  = pdata->deny_no_type_match.s.type_vlan_key;
        data.bf.deny_no_type9_match  = pdata->deny_no_type_match.s.type_vlan_range_key;
        data.bf.deny_no_type10_match = pdata->deny_no_type_match.s.type_proto_key;
        data.bf.deny_no_type11_match = pdata->deny_no_type_match.s.type_mcast_key;
        data.bf.deny_no_type12_match = pdata->deny_no_type_match.s.type_mac_vlan_key_c;
        data.bf.deny_no_type13_match = pdata->deny_no_type_match.s.type_mac_etype_key_d;
        data.bf.deny_no_type14_match = pdata->deny_no_type_match.s.type_vlan_key_e;
        data.bf.deny_no_type15_match = pdata->deny_no_type_match.s.type_rsv_key_f;

    }
    if (msk.wrd)
        REG_MASK_WRITE_PORT(FE_LE_CE_CTRL, port_id, msk.wrd, data.wrd); 

    if(pdata->mask&CL_CFG_MASK_TOS_ECN_EN)
    {
        enmsk.bf.ce_tos_ecn_msk_en  = 1;
        endata.bf.ce_tos_ecn_msk_en = pdata->tos_ecn_msk_en;
    }
    if(pdata->mask&CL_CFG_MASK_CE_CFI_FLAG)
    {
        enmsk.bf.ce_msk_cfi_flg  = 1;
        endata.bf.ce_msk_cfi_flg = pdata->ce_msk_cfi_flg;
    }
    if(pdata->mask&CL_CFG_MASK_CE_MAX_NUM)
    {
        enmsk.bf.dbg_ce_max_entry  = 1;
        endata.bf.dbg_ce_max_entry = pdata->dbg_ce_max_entry;
    }
    if (enmsk.wrd)
        REG_MASK_WRITE_PORT(FE_LE_ENGINE_CTRL, port_id, enmsk.wrd, endata.wrd); 

    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_ctrl_get
* description : get  ce ctrl cfg
* input       :    port_id (0-2)
*
* output      : pdata:cl_ctrl_cfg_t *
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to read register
* globals     : none
* others      :
*
*************************************************************************************/
cs_status cl_ctrl_get(cs_uint8 port_id, cl_ctrl_cfg_t *pdata)
{
    FE_LE_CE_CTRL_t data;
    FE_LE_ENGINE_CTRL_t endata;

    if((pdata==NULL)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_READ_PORT(FE_LE_CE_CTRL, port_id, data.wrd); 
    REG_READ_PORT(FE_LE_ENGINE_CTRL, port_id, endata.wrd); 

    pdata->dbg_ce_max_entry    = endata.bf.dbg_ce_max_entry;
    pdata->tos_ecn_msk_en      = endata.bf.ce_tos_ecn_msk_en;
    pdata->ce_msk_cfi_flg      = endata.bf.ce_msk_cfi_flg;
    pdata->mac_filter_aging_en = data.bf.mac_filter_aging_en;
    pdata->cl_start_entry      = data.bf.cl_start_entry;
   
    pdata->deny_no_type_match.s.type_ipv4_long_key   = data.bf.deny_no_type0_match;
    pdata->deny_no_type_match.s.type_ipv4_short_key  = data.bf.deny_no_type1_match;
    pdata->deny_no_type_match.s.type_ipv6_long_key   = data.bf.deny_no_type2_match;
    pdata->deny_no_type_match.s.type_ipv6_short_key  = data.bf.deny_no_type3_match;
    pdata->deny_no_type_match.s.type_mirror_key      = data.bf.deny_no_type4_match;
    pdata->deny_no_type_match.s.type_mac_vlan_key    = data.bf.deny_no_type5_match;
    pdata->deny_no_type_match.s.type_mac_etype_key   = data.bf.deny_no_type6_match;
    pdata->deny_no_type_match.s.type_ctrl_pkt_key    = data.bf.deny_no_type7_match;
    pdata->deny_no_type_match.s.type_vlan_key        = data.bf.deny_no_type8_match;
    pdata->deny_no_type_match.s.type_vlan_range_key  = data.bf.deny_no_type9_match;
    pdata->deny_no_type_match.s.type_proto_key       = data.bf.deny_no_type10_match;
    pdata->deny_no_type_match.s.type_mcast_key       = data.bf.deny_no_type11_match;
    pdata->deny_no_type_match.s.type_mac_vlan_key_c  = data.bf.deny_no_type12_match;
    pdata->deny_no_type_match.s.type_mac_etype_key_d = data.bf.deny_no_type13_match;
    pdata->deny_no_type_match.s.type_vlan_key_e      = data.bf.deny_no_type14_match;
    pdata->deny_no_type_match.s.type_rsv_key_f       = data.bf.deny_no_type15_match;

    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_default_rule_set
* description :  indirect  write ce fib memory table for default result
*
* input       :    port_id (0-2)
*                  pdata:CL_fib_data_t *  mask(Fib fields mask)
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : none
* others      : addr(phy index): 384 (MA 128 ) default result
*
*************************************************************************************/
cs_status cl_default_rule_set(cs_uint8 port_id, cl_fib_data_t *pdata)
{
    FE_LE_CE_FIB_DATA_T  fibdata;
    cs_uint16 addr =0;
    cs_status ret =0;

    if((pdata==NULL)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    if(port_id == AAL_FE_MA_ID)
        addr = 128;
    else
        addr = 384;

    memset( &fibdata, 0, sizeof(FE_LE_CE_FIB_DATA_T));

    /*read register */
    ret = __cl_fib_table_access(port_id, LE_CL_TABLE_READ, addr, &fibdata);
    if (ret)
    {
        return CS_E_ERROR;
    }
    if(pdata->mask&CL_RSLT_MASK_PERMIT)
    {
        fibdata.data2.bf.permit = pdata->permit;
        //  fibdata.data2.bf.permit_filter_dis = pdata->permit_filter_dis;
    }
    if(pdata->mask&CL_RSLT_MASK_RATE_LIMITER)
    {
        fibdata.data2.bf.flow_id_sel     = pdata->flowId_sel;
        fibdata.data2.bf.flow_id         = pdata->flowID;
        fibdata.data2.bf.rate_lmt_bypass = pdata->rate_limiter_bypass;
    }
    if(pdata->mask&CL_RSLT_MASK_DPID)
    {
        if(pdata->dpid > 7 )
            return CS_E_ERROR;
      
        fibdata.data2.bf.dpid               = pdata->dpid;
        fibdata.data2.bf.mirror_en          = pdata->cl_mirror_en;
        fibdata.data2.bf.mirror_pkt_src_sel = pdata->cl_mirror_src_sel;
    }
    if(pdata->mask&CL_RSLT_MASK_DSCP)
    {
        if(pdata->dscp_sel > 3 )
            return CS_E_ERROR;
        fibdata.data2.bf.dscp_sel = pdata->dscp_sel;
        if(pdata->dscp > 63 )
            return CS_E_ERROR;
        fibdata.data2.bf.dscp = pdata->dscp>>4;
        fibdata.data1.bf.dscp = pdata->dscp;
    }
    if(pdata->mask&CL_RSLT_MASK_VLAN)
    {
        if(pdata->inner_vlan_cmd > 3 )
            return CS_E_ERROR;
        fibdata.data1.bf.inner_vlan_cmd = pdata->inner_vlan_cmd;
        if(pdata->inner_tpid_sel > 7 )
            return CS_E_ERROR;
        fibdata.data1.bf.inner_tpid_sel = pdata->inner_tpid_sel;
        if(pdata->inner_vlanid > 4095 )
            return CS_E_ERROR;
        fibdata.data1.bf.inner_vlanid = pdata->inner_vlanid;
        if(pdata->top_vlanid_sel > 1 )
            return CS_E_ERROR;
        fibdata.data1.bf.top_vlanid_sel = pdata->top_vlanid_sel;
        if(pdata->top_vlan_cmd > 3 )
            return CS_E_ERROR;
        fibdata.data1.bf.top_vlanid_cmd = pdata->top_vlan_cmd;
        if(pdata->top_tpid_sel > 7 )
            return CS_E_ERROR;
        fibdata.data1.bf.top_tpid_sel = pdata->top_tpid_sel;
        if(pdata->top_vlanid > 4095 )
            return CS_E_ERROR;
        fibdata.data1.bf.top_vlanid = pdata->top_vlanid>>10;
        fibdata.data0.bf.top_vlanid = pdata->top_vlanid;
        if(pdata->eg_chk_vlan_sel > 3 )
            return CS_E_ERROR;
        fibdata.data1.bf.egr_chk_vlan_sel = pdata->eg_chk_vlan_sel;
    }
    if(pdata->mask&CL_RSLT_MASK_8021P)
    {
        if(pdata->inner_8021p_sel > 4 )
            return CS_E_ERROR;
        fibdata.data0.bf.inner_802_1p_sel = pdata->inner_8021p_sel;
        if(pdata->inner_8021p > 7 )
            return CS_E_ERROR;
        fibdata.data0.bf.inner_802_1p = pdata->inner_8021p;
        if(pdata->top_8021p_sel > 4 )
            return CS_E_ERROR;
        fibdata.data0.bf.top_802_1p_sel = pdata->top_8021p_sel;
        if(pdata->top_8021p > 7 )
            return CS_E_ERROR;
        fibdata.data0.bf.top_802_1p = pdata->top_8021p;
    }
    if(pdata->mask&CL_RSLT_MASK_COS)
    {
       if(pdata->cos_sel > 3 )
            return CS_E_ERROR;
        fibdata.data0.bf.cos_sel = pdata->cos_sel;

        if(pdata->cos > 7 )
            return CS_E_ERROR;
    
        fibdata.data0.bf.cos = pdata->cos;
    }
    if(pdata->mask&CL_RSLT_MASK_L2_LEARN)
    {
        fibdata.data0.bf.learn_dis = pdata->learn_dis;
        fibdata.data0.bf.l2_sa_limit_fwd_permit = pdata->l2_limit_permit;
    }

     /*write register */
    ret = __cl_fib_table_access(port_id, LE_CL_TABLE_WRITE, addr, &fibdata);
    if (ret)
    {
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_default_rule_get
* description :  indirect  read fib memory table for default result
* input       :    port_id (0-2)
*
* output      : pdata:CL_fib_data_t *
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to read register
* globals     : none
* others      :   addr(phy index): 384 (MA 128 ) default result
*
*************************************************************************************/
cs_status cl_default_rule_get(cs_uint8 port_id, cl_fib_data_t *pdata)
{
    FE_LE_CE_FIB_DATA_T  fibdata;
    cs_uint16 addr =0;
    cs_status ret =0;

    if((pdata==NULL)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    if(port_id == AAL_FE_MA_ID)
        addr = 128;
    else
        addr = 384;
    memset( &fibdata, 0, sizeof(FE_LE_CE_FIB_DATA_T));
    /*read register */
    ret = __cl_fib_table_access(port_id, LE_CL_TABLE_READ, addr, &fibdata);
    if (ret)
    {
        return CS_E_ERROR;
    }

    pdata->permit = fibdata.data2.bf.permit;
    pdata->rate_limiter_bypass = fibdata.data2.bf.rate_lmt_bypass;
    pdata->flowId_sel =  fibdata.data2.bf.flow_id_sel;
    pdata->flowID = fibdata.data2.bf.flow_id;
    pdata->dpid = fibdata.data2.bf.dpid;
    pdata->cl_mirror_en = fibdata.data2.bf.mirror_en;
    pdata->cl_mirror_src_sel = fibdata.data2.bf.mirror_pkt_src_sel;
    pdata->dscp_sel = fibdata.data2.bf.dscp_sel;
    pdata->dscp =  (fibdata.data2.bf.dscp<<4)|fibdata.data1.bf.dscp;
    pdata->inner_vlan_cmd = fibdata.data1.bf.inner_vlan_cmd;
    pdata->inner_tpid_sel = fibdata.data1.bf.inner_tpid_sel;
    pdata->inner_vlanid = fibdata.data1.bf.inner_vlanid;
    pdata->top_vlanid_sel = fibdata.data1.bf.top_vlanid_sel;
    pdata->top_vlan_cmd = fibdata.data1.bf.top_vlanid_cmd;
    pdata->top_tpid_sel = fibdata.data1.bf.top_tpid_sel;
    pdata->top_vlanid = (fibdata.data1.bf.top_vlanid<<10)|fibdata.data0.bf.top_vlanid;
    pdata->eg_chk_vlan_sel = fibdata.data1.bf.egr_chk_vlan_sel;
    pdata->inner_8021p_sel =  fibdata.data0.bf.inner_802_1p_sel;
    pdata->inner_8021p = fibdata.data0.bf.inner_802_1p;
    pdata->top_8021p_sel = fibdata.data0.bf.top_802_1p_sel;
    pdata->top_8021p = fibdata.data0.bf.top_802_1p;
    pdata->cos_sel =  fibdata.data0.bf.cos_sel;
    pdata->cos = fibdata.data0.bf.cos;
    pdata->learn_dis = fibdata.data0.bf.learn_dis;
    pdata->l2_limit_permit = fibdata.data0.bf.l2_sa_limit_fwd_permit;
  
    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_key_entry_set
* description :  indirect  set ce cl_key_entry  memory table
*
* input       :      port_id (0-2)
*                    addr(phy table entry index): 0-47 (MA 0-15 )
*                    pdata: cl_key_entry_t
* output      : 
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to set register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status cl_key_entry_set(cs_uint8 port_id, cs_uint16 addr, cl_key_entry_t *pdata)
{
    cs_status ret =0;
 
    
    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {    
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
   
    switch (pdata->keytype)
    {
        case CL_CTRL_PKT_KEY:
        {  
            ret = __cl_ctrl_pkt_key_entry_set( port_id, addr, &(pdata->u.cl_ctrl_pkt_key[0]));    
            break;
        }
        case CL_IPV4_LONG_KEY:
        {  
            ret = __cl_ipv4_long_key_entry_set( port_id, addr, &(pdata->u.cl_ipv4_long_key));
            break;
        }
        case CL_IPV4_SHORT_KEY:
        {  
            ret = __cl_ipv4_short_key_entry_set( port_id, addr, &(pdata->u.cl_ipv4_short_key[0]));    
            break;
        }
        case CL_IPV6_LONG_KEY:
        {  
            ret = __cl_ipv6_long_key_entry_set( port_id, addr, &(pdata->u.cl_ipv6_long_key));
            break;
        }
        case CL_IPV6_SHORT_KEY:
        {  
            ret = __cl_ipv6_short_key_entry_set( port_id, addr, &(pdata->u.cl_ipv6_short_key));
            break;
        }
        case CL_MIRROR_KEY:
        {  
            ret = __cl_mirror_key_entry_set( port_id, addr, &(pdata->u.cl_mirror_key[0]));
            break;
        }
        case CL_MAC_VLAN_KEY:
        case CL_MAC_VLAN_KEY_C:
        {  
            ret = __cl_mac_vlan_key_entry_set( port_id, pdata->keytype, addr, &(pdata->u.cl_mac_vlan_key[0]));
            break;
        }
        case CL_MAC_ETYPE_KEY:
        case CL_MAC_ETYPE_KEY_D:   
        {  
            ret = __cl_mac_etype_key_entry_set( port_id, pdata->keytype, addr, &(pdata->u.cl_mac_etype_key[0]));
            break;
        }
        case CL_VLAN_KEY:
        case CL_VLAN_KEY_E:   
        {  
            ret = __cl_vlan_key_entry_set( port_id, pdata->keytype, addr, &(pdata->u.cl_vlan_key[0]));
            break;
        }
        case CL_VLAN_RANGE_KEY:   
        {  
            ret = __cl_vlan_range_key_entry_set( port_id, addr, &(pdata->u.cl_vlan_range_key[0]));
            break;
        }
        case CL_PROTO_KEY:
        {  
            ret = __cl_proto_key_entry_set( port_id, addr, &(pdata->u.cl_proto_key[0]));
            break;
        }
        case CL_MCST_KEY:
        {  
            ret = __cl_mcst_key_entry_set( port_id, addr, &(pdata->u.cl_mcst_key[0]));
            break;
        }
        default:
            break;
    }
    if(ret)
    {
        AAL_INF_LOG("key_entry set error!\n");
        return CS_E_ERROR;
    }
  
    ret = cl_entry_valid_set( port_id, addr, pdata->entryvalid);    
    if(ret)
    {
        AAL_INF_LOG("entry_valid set error!\n");
        return CS_E_ERROR;
    }
    if(pdata->keytype == CL_IPV6_LONG_KEY)
    {
        ret = cl_entry_valid_set( port_id, (addr+1), pdata->entryvalid);    
        if(ret)
        {
            AAL_INF_LOG("entry_valid set error!\n");
            return CS_E_ERROR;
        }
    }
               
    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_key_entry_get
* description :  indirect  get ce cl_key_entry  memory table
*
* input       :    port_id (0-2)
*                  addr(phy table entry index): 0-47 (MA 0-15 )
*
* output      : pdata: cl_key_entry_t
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to get register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status cl_key_entry_get(cs_uint8 port_id, cs_uint16 addr, cl_key_entry_t *pdata)
{
    FE_LE_CE_KEY_DATA_T keydata;
    cs_status   ret    = 0;
    cs_boolean  status = 0;

    if((pdata==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));
    /*read register */
    ret = __cl_key_table_access(port_id, LE_CL_TABLE_READ, addr, &keydata);
    if (ret)
    {  
        AAL_INF_LOG("read register error!\n");
        return CS_E_ERROR;
    }
    pdata->keytype = keydata.data9.bf12.kt_cl_key_type;
    
    switch (pdata->keytype)
    {
        case CL_IPV4_LONG_KEY:
        {
            ret = __cl_ipv4_long_key_entry_get(&keydata, &(pdata->u.cl_ipv4_long_key));     
            break;
        }
        case CL_IPV4_SHORT_KEY:
        {
            ret = __cl_ipv4_short_key_entry_get(&keydata, &(pdata->u.cl_ipv4_short_key[0]));     
            break;
        }
        case CL_IPV6_LONG_KEY:
        {
            // addr must be the first entry of long key
            if(port_id == AAL_FE_MA_ID)
            {
                if(addr>14)
                    return CS_E_ERROR;
            }
            else
            {
                if(addr>46)
                    return CS_E_ERROR;
            }
            ret += __cl_ipv6_long_key_entry0_get(&keydata, &(pdata->u.cl_ipv6_long_key)); 
            memset( &keydata, 0, sizeof(FE_LE_CE_KEY_DATA_T));
            /*read register */
            ret += __cl_key_table_access(port_id, LE_CL_TABLE_READ, (addr+1), &keydata);
            if(keydata.data9.bf12.kt_cl_key_type!=CL_IPV6_LONG_KEY)
            {  
                AAL_INF_LOG("The second entry is not IPV6 long key!\n");
                return CS_E_ERROR;
            }
            ret += __cl_ipv6_long_key_entry1_get(&keydata, &(pdata->u.cl_ipv6_long_key)); 
            if(pdata->u.cl_ipv6_long_key.mask ==0)
            {
                pdata->u.cl_ipv6_long_key.mask |= CL_KEY_MASK_ALWAYS_MATCH;
            }
            break;
        }
        case CL_IPV6_SHORT_KEY:
        {
            ret = __cl_ipv6_short_key_entry_get(&keydata, &(pdata->u.cl_ipv6_short_key));     
            break;
        }
        case CL_MIRROR_KEY:
        {
            ret = __cl_mirror_key_entry_get(&keydata, &(pdata->u.cl_mirror_key[0]));     
            break;
        }
        case CL_MAC_VLAN_KEY:
        case CL_MAC_VLAN_KEY_C:
        {
            ret = __cl_mac_vlan_key_entry_get(&keydata, &pdata->u.cl_mac_vlan_key[0]);        
            break;
        }
        case CL_MAC_ETYPE_KEY:
        case CL_MAC_ETYPE_KEY_D:
        {
            ret = __cl_mac_etype_key_entry_get(&keydata, &pdata->u.cl_mac_etype_key[0]);        
            break;
        }
        case CL_CTRL_PKT_KEY:
        {
            ret = __cl_ctrl_pkt_key_entry_get(&keydata, &pdata->u.cl_ctrl_pkt_key[0]);        
            break;
        }
        case CL_VLAN_KEY:
        case CL_VLAN_KEY_E:
        {
            ret = __cl_vlan_key_entry_get(&keydata, &pdata->u.cl_vlan_key[0]);        
            break;
        }
        case CL_VLAN_RANGE_KEY:
        {
            ret = __cl_vlan_range_key_entry_get(&keydata, &pdata->u.cl_vlan_range_key[0]);        
            break;
        }
        case CL_PROTO_KEY:
        {
            ret = __cl_proto_key_entry_get(&keydata, &pdata->u.cl_proto_key[0]);        
            break;
        }
        case CL_MCST_KEY:
        {
            ret = __cl_mcst_key_entry_get(&keydata, &pdata->u.cl_mcst_key[0]);        
            break;
        }
        default:
            break;
    }
    if (ret)
    {
        AAL_INF_LOG("cl_key_entry_get error!\n");
        return CS_E_ERROR;
    }
    ret = cl_entry_valid_get( port_id, addr, &status);
    if (ret)
    {
        AAL_INF_LOG("cl_entry_valid_get error!\n");
        return CS_E_ERROR;
    }
    pdata->entryvalid = status;
        
    return CS_E_OK;
}
/**********************************************************************************
* function    : cl_fib_table_set
* description :  indirect  write ce fib memory table
*
* input       :      port_id (0-2)
*                    addr(phy index): 0-384 (MA 0-128 ) 384/128 default result
*                    pdata:cl_fib_data_t *
* output      : none
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to write register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status cl_fib_table_set(cs_uint8 port_id, cs_uint16 addr, cl_fib_data_t *pdata)
{
    FE_LE_CE_FIB_DATA_T  fibdata;
    cs_status ret =0;

    if((pdata==NULL)||(addr>CL_FIB_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    memset( &fibdata, 0, sizeof(FE_LE_CE_FIB_DATA_T));
 
    fibdata.data2.bf.valid = pdata->valid;
    fibdata.data2.bf.permit_filter_dis = pdata->permit_filter_dis;
    fibdata.data2.bf.permit = pdata->permit;
    fibdata.data2.bf.permit_priority = pdata->permit_pri;

    if(pdata->mask&CL_RSLT_MASK_DPID)
    {
        if(pdata->dpid > 7 )
            return CS_E_ERROR;
        fibdata.data2.bf.dpid = pdata->dpid;
        fibdata.data2.bf.mirror_en = pdata->cl_mirror_en;
        fibdata.data2.bf.mirror_pkt_src_sel = pdata->cl_mirror_src_sel;
        fibdata.data2.bf.dpid_rslt_valid = 1;
    }
    if(pdata->mask&CL_RSLT_MASK_DSCP)
    {
        if(pdata->dscp_sel > 3 )
            return CS_E_ERROR;
        fibdata.data2.bf.dscp_sel = pdata->dscp_sel;
        if(pdata->dscp > 63 )
            return CS_E_ERROR;
        fibdata.data2.bf.dscp = pdata->dscp>>4;
        fibdata.data1.bf.dscp = pdata->dscp;
        fibdata.data2.bf.dscp_rslt_valid = 1;
    }
    if(pdata->mask&CL_RSLT_MASK_VLAN)
    {
        if(pdata->inner_vlan_cmd > 3 )
            return CS_E_ERROR;
        fibdata.data1.bf.inner_vlan_cmd = pdata->inner_vlan_cmd;
        if(pdata->inner_tpid_sel > 7 )
            return CS_E_ERROR;
        fibdata.data1.bf.inner_tpid_sel = pdata->inner_tpid_sel;
        if(pdata->inner_vlanid > 4095 )
            return CS_E_ERROR;
        fibdata.data1.bf.inner_vlanid = pdata->inner_vlanid;
        if(pdata->top_vlanid_sel > 1 )
            return CS_E_ERROR;
        fibdata.data1.bf.top_vlanid_sel = pdata->top_vlanid_sel;
        if(pdata->top_vlan_cmd > 3 )
            return CS_E_ERROR;
        fibdata.data1.bf.top_vlanid_cmd = pdata->top_vlan_cmd;
        if(pdata->top_tpid_sel > 7 )
            return CS_E_ERROR;
        fibdata.data1.bf.top_tpid_sel = pdata->top_tpid_sel;
        if(pdata->top_vlanid > 4095 )
            return CS_E_ERROR;
        fibdata.data1.bf.top_vlanid = pdata->top_vlanid>>10;
        fibdata.data0.bf.top_vlanid = pdata->top_vlanid;
        if(pdata->eg_chk_vlan_sel > 3 )
            return CS_E_ERROR;
        fibdata.data1.bf.egr_chk_vlan_sel = pdata->eg_chk_vlan_sel;
        fibdata.data1.bf.vlan_rslt_valid = 1;
    }
    if(pdata->mask&CL_RSLT_MASK_8021P)
    {
        if(pdata->inner_8021p_sel > 4 )
            return CS_E_ERROR;
        fibdata.data0.bf.inner_802_1p_sel = pdata->inner_8021p_sel;
        if(pdata->inner_8021p > 7 )
            return CS_E_ERROR;
        fibdata.data0.bf.inner_802_1p = pdata->inner_8021p;
        if(pdata->top_8021p_sel > 4 )
            return CS_E_ERROR;
        fibdata.data0.bf.top_802_1p_sel = pdata->top_8021p_sel;
        if(pdata->top_8021p > 7 )
            return CS_E_ERROR;
        fibdata.data0.bf.top_802_1p = pdata->top_8021p;
        fibdata.data0.bf.dot1p_rslt_valid = 1;
    }
    if(pdata->mask&CL_RSLT_MASK_COS)
    {
       if(pdata->cos_sel > 3 )
            return CS_E_ERROR;
        fibdata.data0.bf.cos_sel = pdata->cos_sel;

        if(pdata->cos > 7 )
            return CS_E_ERROR;
      
        fibdata.data0.bf.cos = pdata->cos;
        fibdata.data0.bf.cos_rslt_valid = 1;
    }
    if(pdata->mask&CL_RSLT_MASK_L2_LEARN)
    {
        fibdata.data0.bf.learn_dis = pdata->learn_dis;
        fibdata.data0.bf.l2_sa_limit_fwd_permit = pdata->l2_limit_permit;
        fibdata.data0.bf.l2_rslt_valid = 1;
    }
    if(pdata->mask&CL_RSLT_MASK_RATE_LIMITER)
    {
        fibdata.data2.bf.flow_id_sel = pdata->flowId_sel;
        fibdata.data2.bf.flow_id = pdata->flowID;
        fibdata.data2.bf.rate_lmt_bypass = pdata->rate_limiter_bypass;
        fibdata.data2.bf.rate_lmt_rslt_valid = 1;
    }
      
     /*write register */
    ret = __cl_fib_table_access(port_id, LE_CL_TABLE_WRITE, addr, &fibdata);

    return ret;
}

/**********************************************************************************
* function    : cl_fib_table_get
* description :  indirect  read fib memory table
* input       :      port_id (0-2)
*                    addr(phy index): 0-384 (MA 0-127 )
* output      : pdata:cl_fib_data_t *
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to read register
* globals     : none
* others      :
*
*************************************************************************************/
cs_status cl_fib_table_get(cs_uint8 port_id, cs_uint16 addr, cl_fib_data_t *pdata)
{
    FE_LE_CE_FIB_DATA_T  fibdata;
    cs_status ret =0;

    if((pdata==NULL)||(addr>CL_FIB_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    memset( &fibdata, 0, sizeof(FE_LE_CE_FIB_DATA_T));
    /*read register */
    ret = __cl_fib_table_access(port_id, LE_CL_TABLE_READ, addr, &fibdata);
    if (ret)
    {
        return CS_E_ERROR;
    }
    
    pdata->valid =  fibdata.data2.bf.valid;
    pdata->permit_filter_dis = fibdata.data2.bf.permit_filter_dis;
    pdata->permit = fibdata.data2.bf.permit;
    pdata->permit_pri = fibdata.data2.bf.permit_priority;
    if(fibdata.data2.bf.dpid_rslt_valid )
    {
        pdata->dpid = fibdata.data2.bf.dpid;
        pdata->cl_mirror_en = fibdata.data2.bf.mirror_en;
        pdata->cl_mirror_src_sel = fibdata.data2.bf.mirror_pkt_src_sel;
        pdata->mask |= CL_RSLT_MASK_DPID;
    }
    if(fibdata.data1.bf.vlan_rslt_valid)
    {
        pdata->inner_vlan_cmd = fibdata.data1.bf.inner_vlan_cmd;
        pdata->inner_tpid_sel = fibdata.data1.bf.inner_tpid_sel;
        pdata->inner_vlanid = fibdata.data1.bf.inner_vlanid;
        pdata->top_vlanid_sel = fibdata.data1.bf.top_vlanid_sel;
        pdata->top_vlan_cmd = fibdata.data1.bf.top_vlanid_cmd;
        pdata->top_tpid_sel = fibdata.data1.bf.top_tpid_sel;
        pdata->top_vlanid = (fibdata.data1.bf.top_vlanid<<10)|fibdata.data0.bf.top_vlanid;
        pdata->eg_chk_vlan_sel = fibdata.data1.bf.egr_chk_vlan_sel;
        pdata->mask |= CL_RSLT_MASK_VLAN;
    }
    if(fibdata.data2.bf.dscp_rslt_valid)
    {
        pdata->dscp_sel = fibdata.data2.bf.dscp_sel;
        pdata->dscp =  (fibdata.data2.bf.dscp<<4)|fibdata.data1.bf.dscp;
        pdata->mask |= CL_RSLT_MASK_DSCP;
    }
    if(fibdata.data0.bf.dot1p_rslt_valid)
    {
        pdata->inner_8021p_sel =  fibdata.data0.bf.inner_802_1p_sel;
        pdata->inner_8021p = fibdata.data0.bf.inner_802_1p;
        pdata->top_8021p_sel = fibdata.data0.bf.top_802_1p_sel;
        pdata->top_8021p = fibdata.data0.bf.top_802_1p;
        pdata->mask |= CL_RSLT_MASK_8021P;
    }
    if(fibdata.data0.bf.cos_rslt_valid)
    {
        pdata->cos_sel =  fibdata.data0.bf.cos_sel;
        pdata->cos = fibdata.data0.bf.cos;
        pdata->mask |= CL_RSLT_MASK_COS;
    }
    if(fibdata.data0.bf.l2_rslt_valid)
    {
        pdata->learn_dis = fibdata.data0.bf.learn_dis;
        pdata->l2_limit_permit = fibdata.data0.bf.l2_sa_limit_fwd_permit;
        pdata->mask |= CL_RSLT_MASK_L2_LEARN;
    }
    if(fibdata.data2.bf.rate_lmt_rslt_valid)
    {
        pdata->rate_limiter_bypass = fibdata.data2.bf.rate_lmt_bypass;
        pdata->flowId_sel =  fibdata.data2.bf.flow_id_sel;
        pdata->flowID = fibdata.data2.bf.flow_id;
        pdata->mask |= CL_RSLT_MASK_RATE_LIMITER;
    }

    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_entry_link_list_set
* description :  indirect  set ce_key_link_list_table  memory table
*
* input       :      port_id (0-2)
*                    addr(phy table entry index): 0-47 (MA 0-15 )
*                    cl_next_entry: 0-47 (MA 0-15 )
* output      :
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to set register
* globals     : none

* others      :
*
*************************************************************************************/
cs_status cl_entry_link_list_set(cs_uint8 port_id, cs_uint8 addr, cs_uint8 cl_next_entry)
{
    cs_status ret =0;
    FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA_t data;
     
    if((cl_next_entry>CL_ENTRY_MAXNUM)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {    
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    data.wrd = 0;
      
    data.bf.cl_next_entry = cl_next_entry;
        /*write register */
    ret = __cl_key_link_list_table_write(port_id, addr, data);
    if (ret)
    {
        return CS_E_ERROR;
    }
    return CS_E_OK;
}

/**********************************************************************************
* function    : cl_entry_link_list_get
* description :  indirect  read ce_key_link_list_table memory table
* input       :      port_id (0-2)
*                    addr(phy table entry index): 0-47 (MA 0-15 )
* output      : cl_next_entry:cs_uint8 *
* return      : CS_E_OK    - succeed
*               CS_E_ERROR - fail to read register
* globals     : none
* others      :
*
*************************************************************************************/
cs_status cl_entry_link_list_get(cs_uint8 port_id, cs_uint8 addr, cs_uint8 *cl_next_entry)
{
    cs_status ret =0;
    FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA_t data;
     
    if((cl_next_entry==NULL)||(addr>CL_ENTRY_MAXNUM)||(port_id>AAL_FE_MA_ID))
    {    
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
   
    data.wrd = 0;
    /*read register */
    ret = __cl_key_link_list_table_read(port_id, addr, &data);
    if (ret)
    {
        return CS_E_ERROR;
    }
    
    *cl_next_entry = data.bf.cl_next_entry;
    
    return CS_E_OK;
}

void cls_util_init(void)
{
    cs_status ret;
    
    ret = cs_mutex_init(&aal_keylist_mutex, "aal keylist mutex",0);
    if(ret!=0)
    {
        cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", ret, __FILE__, __LINE__);
    }

    ret = cs_mutex_init(&aal_clstbl_mutex, "aal cls mutex",0);
    if(ret!=0)
    {
        cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", ret, __FILE__, __LINE__);
    }

    ret = cs_mutex_init(&aal_fibtbl_mutex, "aal fib mutex",0);
    if(ret!=0)
    {
        cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", ret, __FILE__, __LINE__);
    }
    ret = cs_mutex_init(&aal_spectbl_mutex, "aal spectb mutex",0);
    if(ret!=0)
    {
        cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", ret, __FILE__, __LINE__);
    }
    ret = cs_mutex_init(&aal_l4port_mutex, "aal l4port mutex",0);
    if(ret!=0)
    {
        cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", ret, __FILE__, __LINE__);
    }
    ret = cs_mutex_init(&aal_filter_mutex, "aal filter mutex",0);
    if(ret!=0)
    {
        cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", ret, __FILE__, __LINE__);
    }

    return;
}
/*  end of file */

