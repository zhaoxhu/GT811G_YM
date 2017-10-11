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
#include "aal_l2.h"
#include "aal_util.h"


/*****************************************************************************/
/*                    Local Functions                                        */
/*****************************************************************************/

FE_TOP_t *top_reg          = &(g_lynxd_reg_ptr->fe.top);
#define  LE_REG_BASE        ((char*)&(g_lynxd_reg_ptr->fe.le))
#define  LE_PORT_WIDTH      (0x400)
#define  PORT_BASE(port_id) (FE_LE_t *)(LE_REG_BASE + port_id* LE_PORT_WIDTH)


typedef struct{

    FE_LE_VE_VLAN_TABLE_MEM_DATA3_t  data3;
    FE_LE_VE_VLAN_TABLE_MEM_DATA2_t  data2;
    FE_LE_VE_VLAN_TABLE_MEM_DATA1_t  data1;
    FE_LE_VE_VLAN_TABLE_MEM_DATA0_t  data0;

}FE_LE_VE_VLAN_TABLE_DATA_T;

cs_uint32 aal_vlan_mutex;
cs_uint32 aal_fdb_mutex;
cs_uint32 aal_sppkt_mutex;


/*****************************************************************************
 *
 *             Functions Implementations
 *
 ****************************************************************************/
static cs_status __sppkt_table_read(cs_uint8 port_id, cs_uint8 pkt_type, FE_TOP_PRT_SP_PKT_TABLE_DATA_t *data)
{
    cs_uint16 i = 0;
    cs_status   ret = CS_E_OK;
    FE_TOP_PRT_SP_PKT_TABLE_ACCESS_t  tbl_ac;

    
    tbl_ac.wrd = top_reg->PRT_SP_PKT_TABLE_ACCESS.wrd;
    tbl_ac.bf.access = 1;
    tbl_ac.bf.address = 21 * port_id  + pkt_type;
    tbl_ac.bf.rbw = 0; /**< read */
    
    cs_mutex_lock(aal_sppkt_mutex);

    top_reg->PRT_SP_PKT_TABLE_ACCESS.wrd = tbl_ac.wrd;
    
    while (1) {
        if (0 == top_reg->PRT_SP_PKT_TABLE_ACCESS.bf.access) {
            data->wrd = top_reg->PRT_SP_PKT_TABLE_DATA.wrd;
            break;
        }

        if (++i > 2000) 
        {
            AAL_CRI_LOG("__vlan_table_read timeout. [%s %d]\n",  __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
            break;
        }
    }

    cs_mutex_unlock(aal_sppkt_mutex);
    return ret;
}


static cs_status __sppkt_table_write(cs_uint8 port_id, cs_uint8 pkt_type, FE_TOP_PRT_SP_PKT_TABLE_DATA_t *data)
{
    cs_uint16 i = 0;
    cs_status   ret = CS_E_OK;
    FE_TOP_PRT_SP_PKT_TABLE_ACCESS_t  tbl_ac;

    
    tbl_ac.wrd = top_reg->PRT_SP_PKT_TABLE_ACCESS.wrd;
    tbl_ac.bf.access = 1;
    tbl_ac.bf.address = 21 * port_id  + pkt_type;
    tbl_ac.bf.rbw = 1; /**< write */
    
    cs_mutex_lock(aal_sppkt_mutex);

    top_reg->PRT_SP_PKT_TABLE_DATA.wrd = data->wrd;
    top_reg->PRT_SP_PKT_TABLE_ACCESS.wrd = tbl_ac.wrd;

    i=0;
    while (1) {
        if (0 == top_reg->PRT_SP_PKT_TABLE_ACCESS.bf.access) {
            break;
        }

        if (++i > 2000) 
        {
            AAL_CRI_LOG("__vlan_table_read timeout. [%s %d]\n",  __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
            break;
        }
    }

    cs_mutex_unlock(aal_sppkt_mutex);
    return ret;
}

static cs_status __fdb_read_acc(cs_uint8 index, FE_TOP_L2E_TABLE_MEM_DATA2_t *data2, 
    FE_TOP_L2E_TABLE_MEM_DATA1_t *data1, FE_TOP_L2E_TABLE_MEM_DATA0_t *data0)
{
    FE_TOP_L2E_TABLE_MEM_ACCESS_t tmp = top_reg->L2E_TABLE_MEM_ACCESS;
    cs_uint16 i = 0;
    cs_status   ret = CS_E_OK;
     
    if (index > 0x3f)
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    if (!data0 || !data1 || !data2)
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    cs_mutex_lock(aal_fdb_mutex);
       
    tmp.bf.access = 1;
    tmp.bf.rbw = 0;
    tmp.bf.address = index;

    top_reg->L2E_TABLE_MEM_ACCESS = tmp;

    while (1) {
        if (0 == top_reg->L2E_TABLE_MEM_ACCESS.bf.access) {
            *data2 = top_reg->L2E_TABLE_MEM_DATA2;
            *data1 = top_reg->L2E_TABLE_MEM_DATA1;
            *data0 = top_reg->L2E_TABLE_MEM_DATA0;
            
            break;
        }

        if (++i > 2000) 
        {
            AAL_CRI_LOG("__fdb_read_acc timeout. [%s %d]\n",  __FUNCTION__, __LINE__);

            ret = CS_E_ERROR;
            break;
        }
    }

    cs_mutex_unlock(aal_fdb_mutex);
    return ret;
}


static cs_status __fdb_write_acc(cs_uint8 index, FE_TOP_L2E_TABLE_MEM_DATA2_t data2, 
    FE_TOP_L2E_TABLE_MEM_DATA1_t data1, FE_TOP_L2E_TABLE_MEM_DATA0_t data0)
{   
    FE_TOP_L2E_TABLE_MEM_ACCESS_t tmp = top_reg->L2E_TABLE_MEM_ACCESS;
    cs_uint16 i = 0;

    cs_status   ret = CS_E_OK;

    if (index > 0x3f) 
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    cs_mutex_lock(aal_fdb_mutex);
  
    top_reg->L2E_TABLE_MEM_DATA2 = data2;
    top_reg->L2E_TABLE_MEM_DATA1 = data1;
    top_reg->L2E_TABLE_MEM_DATA0 = data0;
    
    tmp.bf.access = 1;
    tmp.bf.rbw = 1;
    tmp.bf.address = index;

    top_reg->L2E_TABLE_MEM_ACCESS = tmp;

    while (1) {
        if (0 == top_reg->L2E_TABLE_MEM_ACCESS.bf.access) {
            break;
        }

        if (++i > 2000) {
            AAL_CRI_LOG("__fdb_write_acc timeout. [%s %d]\n",  __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
            break;
        }       
    }

    cs_mutex_unlock(aal_fdb_mutex);
    
    return ret;
}


static cs_status __vlan_table_read(cs_uint8 port_id, cs_uint8 index, FE_LE_VE_VLAN_TABLE_DATA_T *data)
{
    cs_uint16 i = 0;
    FE_LE_t *base = PORT_BASE(port_id);
    cs_status   ret = CS_E_OK;
    FE_LE_VE_VLAN_TABLE_MEM_ACCESS_t temp = base->VE_VLAN_TABLE_MEM_ACCESS;

    cs_mutex_lock(aal_vlan_mutex);
      
    temp.bf.access = 1;
    temp.bf.rbw = 0;
    temp.bf.address = index;

    base->VE_VLAN_TABLE_MEM_ACCESS = temp;
    while (1) {
        if (0 == base->VE_VLAN_TABLE_MEM_ACCESS.bf.access) {
            data->data0 = base->VE_VLAN_TABLE_MEM_DATA0;
            data->data1 = base->VE_VLAN_TABLE_MEM_DATA1;
            data->data2 = base->VE_VLAN_TABLE_MEM_DATA2;
            data->data3 = base->VE_VLAN_TABLE_MEM_DATA3;

            break;
        }

        if (++i > 2000) 
        {
            AAL_CRI_LOG("__vlan_table_read timeout. [%s %d]\n",  __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
            break;
        }
    }

    cs_mutex_unlock(aal_vlan_mutex);
      
    return ret;
}


static cs_status __vlan_table_write(cs_uint8 port_id, cs_uint8 index, FE_LE_VE_VLAN_TABLE_DATA_T *data)
{
    FE_LE_t *base = PORT_BASE(port_id);
    FE_LE_VE_VLAN_TABLE_MEM_ACCESS_t temp = base->VE_VLAN_TABLE_MEM_ACCESS;
    cs_uint16 i = 0;
    cs_status   ret = CS_E_OK;

    cs_mutex_lock(aal_vlan_mutex);
    
    base->VE_VLAN_TABLE_MEM_DATA0 = data->data0;
    base->VE_VLAN_TABLE_MEM_DATA1 = data->data1;
    base->VE_VLAN_TABLE_MEM_DATA2 = data->data2;
    base->VE_VLAN_TABLE_MEM_DATA3 = data->data3;



    temp.bf.access = 1;
    temp.bf.rbw = 1;
    temp.bf.address = index;

    base->VE_VLAN_TABLE_MEM_ACCESS = temp;
    while (1) {
        if (0 == base->VE_VLAN_TABLE_MEM_ACCESS.bf.access) {
            break;
        }

        if (++i > 2000) {
            AAL_CRI_LOG("__vlan_table_write timeout. [%s %d]\n",  __FUNCTION__, __LINE__);
            ret = CS_E_ERROR;
            break;
        }
    }

    cs_mutex_unlock(aal_vlan_mutex);
    
    return ret;
}


//  Pullic
//============================================================================

cs_status aal_pkt_fe_ctrl_set( CS_IN cs_aal_port_id_t  port, CS_IN cs_aal_pkt_fe_ctrl_msk_t cfg_msk, CS_IN cs_aal_pkt_fe_ctrl_t *cfg)
{
    FE_LE_ENGINE_CTRL_t lectrl_mask;
    FE_LE_ENGINE_CTRL_t lectrl_data;
    FE_LE_PE_CTRL_t pe_mask;
    FE_LE_PE_CTRL_t pe_data;
    cs_aal_fe_instance_t fe_id  = 0;
    
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if (cfg_msk.u32 == 0)
        return CS_E_OK;
    
    fe_id = FE_PORT(port);

    pe_mask.wrd = 0;
    pe_data.wrd = 0;
    lectrl_mask.wrd = 0;
    lectrl_data.wrd = 0;

    if (cfg_msk.s.pe_ge_en)
    {   
        if (cfg->pe_ge_en > 1)
            return CS_E_PARAM;
        pe_mask.bf.pe_ge_en = FILL_MSK;
        pe_data.bf.pe_ge_en = cfg->pe_ge_en;
    }
    if (cfg_msk.s.pe_pon_en)
    {   
        if (cfg->pe_pon_en > 1)
            return CS_E_PARAM;
        pe_mask.bf.pe_pon_en = FILL_MSK;
        pe_data.bf.pe_pon_en = cfg->pe_pon_en;
    }
    if (cfg_msk.s.pe_cpu_en)
    {   
        if (cfg->pe_cpu_en > 1)
            return CS_E_PARAM;
        pe_mask.bf.pe_cpu_en = FILL_MSK;
        pe_data.bf.pe_cpu_en = cfg->pe_cpu_en;
    }
    if (cfg_msk.s.pe_mii0_en)
    {   
        if (cfg->pe_mii0_en > 1)
            return CS_E_PARAM;
        pe_mask.bf.pe_mii0_en = FILL_MSK;
        pe_data.bf.pe_mii0_en = cfg->pe_mii0_en;
    }
    if (cfg_msk.s.pe_mii1_en)
    {   
        if (cfg->pe_mii1_en > 1)
            return CS_E_PARAM;
        pe_mask.bf.pe_mii1_en = FILL_MSK;
        pe_data.bf.pe_mii1_en = cfg->pe_mii1_en;
    }
    if (cfg_msk.s.ingress_pkt_len)
    {   
        if (cfg->ingress_pkt_len > AAL_TO_TE_INCOME)
            return CS_E_PARAM;
        pe_mask.bf.ingress_pkt_len = FILL_MSK;
        pe_data.bf.ingress_pkt_len = cfg->ingress_pkt_len;
    }
    if (cfg_msk.s.inner_def_dot1p)
    {   
        if (cfg->inner_def_dot1p > 7)
            return CS_E_PARAM;
        pe_mask.bf.inner_default_8021p = FILL_MSK;
        pe_data.bf.inner_default_8021p = cfg->inner_def_dot1p;
    }
    if (cfg_msk.s.top_def_8021p)
    {   
        if (cfg->top_def_8021p > 7)
            return CS_E_PARAM;
        pe_mask.bf.top_default_8021p = FILL_MSK;
        pe_data.bf.top_default_8021p = cfg->top_def_8021p;
    }
    if (cfg_msk.s.txpad_en)
    {   
        if (cfg->txpad_en > 1)
            return CS_E_PARAM;
        pe_mask.bf.txpad_en = FILL_MSK;
        pe_data.bf.txpad_en = cfg->txpad_en;
    }
    if (cfg_msk.s.stomp_crc)
    {   
        if (cfg->stomp_crc > AAL_FCS_CALC_CRC32)
            return CS_E_PARAM;
        pe_mask.bf.stomp_crc = FILL_MSK;
        pe_data.bf.stomp_crc = cfg->stomp_crc;
    }
    if (cfg_msk.s.keep_fcs)
    {   
        if (cfg->keep_fcs > 1)
            return CS_E_PARAM;
        pe_mask.bf.keep_fcs = FILL_MSK;
        pe_data.bf.keep_fcs = cfg->keep_fcs;
    }
    if (cfg_msk.s.loop_dpid_deny)
    {   
        if (cfg->loop_dpid_deny > 1)
            return CS_E_PARAM;
        lectrl_mask.bf.l2_loop_loop_deny = FILL_MSK;
        lectrl_data.bf.l2_loop_loop_deny = cfg->loop_dpid_deny;
    }
    if (cfg_msk.s.loop_mac_chk)
    {   
        if (cfg->loop_mac_chk > 1)
            return CS_E_PARAM;
        lectrl_mask.bf.mac_adr_loop_chk_en = FILL_MSK;
        lectrl_data.bf.mac_adr_loop_chk_en = cfg->loop_mac_chk;
    }
    if (cfg_msk.s.loop_vlan_chk)
    {   
        if (cfg->loop_vlan_chk > 1)
            return CS_E_PARAM;
        lectrl_mask.bf.loop_vlan_chk_en = FILL_MSK;
        lectrl_data.bf.loop_vlan_chk_en = cfg->loop_vlan_chk;
    }
    if (cfg_msk.s.loop_deny)
    {   
        if (cfg->loop_deny > 1)
            return CS_E_PARAM;
        lectrl_mask.bf.loop_deny = FILL_MSK;
        lectrl_data.bf.loop_deny = cfg->loop_deny;
    }

    if (pe_mask.wrd)
        REG_MASK_WRITE_PORT(FE_LE_PE_CTRL, fe_id, pe_mask.wrd, pe_data.wrd); 
    if (lectrl_mask.wrd)
        REG_MASK_WRITE_PORT(FE_LE_ENGINE_CTRL, fe_id, lectrl_mask.wrd, lectrl_data.wrd); 

    return CS_E_OK;
}


cs_status aal_pkt_fe_ctrl_get( CS_IN cs_aal_port_id_t  port, CS_OUT cs_aal_pkt_fe_ctrl_t *cfg )
{   
    FE_LE_ENGINE_CTRL_t  lectrl_data;
    FE_LE_PE_CTRL_t      pe_data;
    cs_aal_fe_instance_t fe_id  = 0;
    
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);

    REG_READ_PORT(FE_LE_PE_CTRL, fe_id, pe_data.wrd); 
    REG_READ_PORT(FE_LE_ENGINE_CTRL, fe_id, lectrl_data.wrd); 

    cfg->pe_ge_en = pe_data.bf.pe_ge_en;
    cfg->pe_pon_en = pe_data.bf.pe_pon_en;
    cfg->pe_cpu_en = pe_data.bf.pe_cpu_en;
    cfg->pe_mii0_en = pe_data.bf.pe_mii0_en;
    cfg->pe_mii1_en = pe_data.bf.pe_mii1_en;
    cfg->ingress_pkt_len = pe_data.bf.ingress_pkt_len;
    cfg->inner_def_dot1p = pe_data.bf.inner_default_8021p;
    cfg->top_def_8021p = pe_data.bf.top_default_8021p;
    cfg->txpad_en = pe_data.bf.txpad_en;
    cfg->stomp_crc = pe_data.bf.stomp_crc;
    cfg->keep_fcs =  pe_data.bf.keep_fcs;
    cfg->loop_dpid_deny = lectrl_data.bf.l2_loop_loop_deny;
    cfg->loop_mac_chk = lectrl_data.bf.mac_adr_loop_chk_en;
    cfg->loop_vlan_chk = lectrl_data.bf.loop_vlan_chk_en;
    cfg->loop_deny =  lectrl_data.bf.loop_deny;
  
    return CS_E_OK;
}


cs_status aal_pkt_special_udf_set(
    CS_IN cs_aal_port_id_t          port,
    CS_IN cs_aal_pkt_spec_udf_msk_t msk,
    CS_IN cs_aal_pkt_spec_udf_t     *cfg
)
{
    cs_aal_fe_instance_t fe_id  = 0;
    FE_LE_PP_SPCL_PKT_MAC_0_ADDR1_t temp1 ;
    FE_LE_PP_SPCL_PKT_MAC_0_ADDR1_t temp1_msk;
    FE_LE_PP_SPCL_PKT_MAC_0_ADDR0_t temp0 ;
    FE_LE_PP_SPCL_PKT_MAC_1_ADDR1_t temp3 ;
    FE_LE_PP_SPCL_PKT_MAC_1_ADDR1_t temp3_msk;
    FE_LE_PP_SPCL_PKT_MAC_1_ADDR0_t temp2 ;
    FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR1_t temp5;
    FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR0_t temp4;
   
    if ((NULL == cfg) || (port>AAL_PORT_ID_MII1)) {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if (msk.u32 == 0)
        return CS_E_OK;

    fe_id = FE_PORT(port);
     
    temp1_msk.wrd = 0;
    temp1.wrd     = 0;
    temp0.wrd     = 0;
    temp3_msk.wrd = 0;
    temp3.wrd     = 0;
    temp2.wrd     = 0;
    temp5.wrd     = 0;
    temp4.wrd     = 0;
    
    /*  UDF 0 */
    if (msk.s.udf0_etype) 
    {
        REG_FIELD_WRITE_PORT(FE_LE_PP_SPCL_EV2PT_CFG_0, fe_id, ev2pt_0, cfg->udf0_etype);   
    }

    if (msk.s.udf0_chk_da) 
    {
        temp1_msk.bf.mac_cmp_en = FILL_MSK;
        temp1.bf.mac_cmp_en = cfg->udf0_chk_da;         
    }
    
    if (msk.s.udf0_da) 
    {    
        temp1_msk.bf.mac_addr = FILL_MSK;
        temp1.bf.mac_addr = ((cfg->udf0_da.addr[0] << 8) | cfg->udf0_da.addr[1]);
        temp0.bf.mac_addr = ((cfg->udf0_da.addr[2] << 24) |(cfg->udf0_da.addr[3] << 16) |
                              (cfg->udf0_da.addr[4] << 8) |cfg->udf0_da.addr[5]);
        REG_WRITE_PORT(FE_LE_PP_SPCL_PKT_MAC_0_ADDR0, fe_id, temp0.wrd);     
    }
    
    if(temp1_msk.wrd)
    {
        REG_MASK_WRITE_PORT(FE_LE_PP_SPCL_PKT_MAC_0_ADDR1, fe_id, temp1_msk.wrd, temp1.wrd);
    }

    /** UDF 1 */
    if (msk.s.udf1_etype) 
    {
        REG_FIELD_WRITE_PORT(FE_LE_PP_SPCL_EV2PT_CFG_0, fe_id, ev2pt_1, cfg->udf1_etype);   
    }
    
    if (msk.s.udf1_chk_da) 
    {
        temp3_msk.bf.mac_cmp_en = FILL_MSK;
        temp3.bf.mac_cmp_en = cfg->udf1_chk_da;         
    }
    
    if (msk.s.udf1_da) 
    {    
        temp3_msk.bf.mac_addr = FILL_MSK;
        temp3.bf.mac_addr = ((cfg->udf1_da.addr[0] << 8) | cfg->udf1_da.addr[1]);
        temp2.bf.mac_addr = ((cfg->udf1_da.addr[2] << 24) |(cfg->udf1_da.addr[3] << 16) |
                              (cfg->udf1_da.addr[4] << 8) |cfg->udf1_da.addr[5]);
        REG_WRITE_PORT(FE_LE_PP_SPCL_PKT_MAC_1_ADDR0, fe_id, temp2.wrd);     
    }

    if(temp3_msk.wrd)
    {
        REG_MASK_WRITE_PORT(FE_LE_PP_SPCL_PKT_MAC_1_ADDR1, fe_id, temp3_msk.wrd, temp3.wrd);
    }
      
    /** UDF 2 */
    if (msk.s.udf2_etype) 
    {
        REG_FIELD_WRITE_PORT(FE_LE_PP_SPCL_EV2PT_CFG_1, fe_id, ev2pt_2, cfg->udf2_etype);   
    }
    
    /** UDF 3 */
    if (msk.s.udf3_etype) 
    {
        REG_FIELD_WRITE_PORT(FE_LE_PP_SPCL_EV2PT_CFG_1, fe_id, ev2pt_3, cfg->udf3_etype);   
    }
    
    /** UDF 4 */
    if (msk.s.udf4_etype) 
    {
        REG_FIELD_WRITE_PORT(FE_LE_PP_SPCL_EV2PT_CFG_2, fe_id, ev2pt_4, cfg->udf4_etype);   
    }
    
    /** HELLO */
    if (msk.s.hello_da) 
    {
        temp5.bf.mac_addr = ((cfg->hello_da.addr[0] << 8)|cfg->hello_da.addr[1]);
        temp4.bf.mac_addr = (cfg->hello_da.addr[2] << 24) + (cfg->hello_da.addr[3] << 16) + (cfg->hello_da.addr[4] << 8) + cfg->hello_da.addr[5];
        REG_WRITE_PORT(FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR0, fe_id, temp4.wrd);
        REG_FIELD_WRITE_PORT(FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR1, fe_id, mac_addr, temp5.bf.mac_addr);
    }
    
    return CS_E_OK;
    
}

cs_status aal_pkt_special_udf_get(
    CS_IN  cs_aal_port_id_t           port,
    CS_OUT cs_aal_pkt_spec_udf_t      *cfg
)
{    
    cs_aal_fe_instance_t fe_id  = 0;
    cs_uint32            temp0  = 0;
    cs_uint16            temp1  = 0;
     
    if ((NULL == cfg) || (port>AAL_PORT_ID_MII1)) {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
    
     /*  UDF 0 */
    
    REG_FIELD_READ_PORT(FE_LE_PP_SPCL_EV2PT_CFG_0, fe_id, ev2pt_0, cfg->udf0_etype);   
    REG_FIELD_READ_PORT(FE_LE_PP_SPCL_PKT_MAC_0_ADDR1, fe_id, mac_cmp_en, cfg->udf0_chk_da);

    REG_FIELD_READ_PORT(FE_LE_PP_SPCL_PKT_MAC_0_ADDR1, fe_id, mac_addr, temp1); 
    REG_READ_PORT(FE_LE_PP_SPCL_PKT_MAC_0_ADDR0, fe_id, temp0);   

    cfg->udf0_da.addr[0] = temp1 >> 8;
    cfg->udf0_da.addr[1] = temp1 & 0xff;
    cfg->udf0_da.addr[2] = temp0 >> 24;
    cfg->udf0_da.addr[3] = (temp0 >> 16) & 0xff;
    cfg->udf0_da.addr[4] = (temp0 >> 8) & 0xff;
    cfg->udf0_da.addr[5] = temp0 & 0xff;

    /** UDF 1 */

    REG_FIELD_READ_PORT(FE_LE_PP_SPCL_EV2PT_CFG_0, fe_id, ev2pt_1, cfg->udf1_etype);   
    REG_FIELD_READ_PORT(FE_LE_PP_SPCL_PKT_MAC_1_ADDR1, fe_id, mac_cmp_en, cfg->udf1_chk_da);

    REG_FIELD_READ_PORT(FE_LE_PP_SPCL_PKT_MAC_1_ADDR1, fe_id, mac_addr, temp1); 
    REG_READ_PORT(FE_LE_PP_SPCL_PKT_MAC_1_ADDR0, fe_id, temp0);   

    cfg->udf1_da.addr[0] = temp1 >> 8;
    cfg->udf1_da.addr[1] = temp1 & 0xff;
    cfg->udf1_da.addr[2] = temp0 >> 24;
    cfg->udf1_da.addr[3] = (temp0 >> 16) & 0xff;
    cfg->udf1_da.addr[4] = (temp0 >> 8) & 0xff;
    cfg->udf1_da.addr[5] = temp0 & 0xff;
    
    /** UDF 2 */

    REG_FIELD_READ_PORT(FE_LE_PP_SPCL_EV2PT_CFG_1, fe_id, ev2pt_2, cfg->udf2_etype);   

    /** UDF 3 */

    REG_FIELD_READ_PORT(FE_LE_PP_SPCL_EV2PT_CFG_1, fe_id, ev2pt_3, cfg->udf3_etype);   
    
    /** UDF 4 */

    REG_FIELD_READ_PORT(FE_LE_PP_SPCL_EV2PT_CFG_2, fe_id, ev2pt_4, cfg->udf4_etype);   

    
    /** HELLO */

    REG_FIELD_READ_PORT(FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR1, fe_id, mac_addr, temp1);
    REG_READ_PORT(FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR0, fe_id, temp0);
          
    cfg->hello_da.addr[0] = temp1 >> 8;
    cfg->hello_da.addr[1] = temp1 & 0xff;
    cfg->hello_da.addr[2] = temp0 >> 24;
    cfg->hello_da.addr[3] = (temp0 >> 16) & 0xff;
    cfg->hello_da.addr[4] = (temp0 >> 8) & 0xff;
    cfg->hello_da.addr[5] = temp0 & 0xff;


    return CS_E_OK;

}


cs_status aal_special_pkt_behavior_set(
    CS_IN cs_aal_port_id_t             port,
    CS_IN cs_aal_pkt_type_t            pkt_type,
    CS_IN cs_aal_spec_pkt_ctrl_msk_t   cfg_msk,
    CS_IN cs_aal_spec_pkt_ctrl_t       *cfg
)
{
    FE_TOP_PRT_SP_PKT_TABLE_DATA_t    tbl_data;
    FE_TOP_GE_PORT_CTRL_2_t     mask;
    FE_TOP_GE_PORT_CTRL_2_t     data;
    cs_aal_spec_pkt_ctrl_msk_t  temp;

    if (pkt_type > AAL_PKT_TYPE_MAX ||(port>AAL_PORT_ID_MII1)||(!cfg)) 
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if (cfg_msk.u32 == 0)
        return CS_E_OK;

    if (cfg_msk.s.permit)
    {  
        mask.wrd = 0;
        data.wrd = 0;

        SETBIT(mask.bf.sp_pkt_permit, pkt_type); 
        if(cfg->permit)
            SETBIT(data.bf.sp_pkt_permit, pkt_type); 
        else
            CLRBIT(data.bf.sp_pkt_permit, pkt_type);
       
        switch(port)
        {
            case AAL_PORT_ID_GE:
                REG_MASK_WRITE(FE_TOP_GE_PORT_CTRL_2, mask.wrd, data.wrd);
                break;

            case AAL_PORT_ID_PON:
                REG_MASK_WRITE(FE_TOP_PON_PORT_CTRL_2, mask.wrd, data.wrd);
                break;

            case AAL_PORT_ID_CPU:
                REG_MASK_WRITE(FE_TOP_CPU_PORT_CTRL_2, mask.wrd, data.wrd);  
                break;

            case AAL_PORT_ID_MII0:
                REG_MASK_WRITE(FE_TOP_MII0_PORT_CTRL_2, mask.wrd, data.wrd);
                break;

            case AAL_PORT_ID_MII1:
                REG_MASK_WRITE(FE_TOP_MII1_PORT_CTRL_2, mask.wrd, data.wrd);  
                break;

            default:
                return CS_E_PARAM;
        }
    }
    
    temp.u32 = cfg_msk.u32;    
    temp.s.permit = 0;
    if (temp.u32)
    {     
        __sppkt_table_read(port, pkt_type, &tbl_data);
          
        if (cfg_msk.s.pri)
        {  
            tbl_data.bf.sp_pkt_pri_sel = cfg->pri;
        }
        if (cfg_msk.s.bypass_plc)
        {  
            tbl_data.bf.sp_policer_bypass = cfg->bypass_plc;
        }
        if (cfg_msk.s.lmt_fwd)
        {  
            tbl_data.bf.sp_sa_limit_fwd_permit = cfg->lmt_fwd;
        }
        if (cfg_msk.s.learning)
        {  
            tbl_data.bf.sp_learn_dis = !cfg->learning;
        }
        if (cfg_msk.s.cos)
        {  
            tbl_data.bf.sp_cos  = cfg->cos;
        }
        if (cfg_msk.s.dpid)
        {            
            SPEC_PORT_TO_DPID(cfg->dpid.dst_op, cfg->dpid.dpid, tbl_data.bf.sp_dpid);
        }
        if (cfg_msk.s.rate_limit)
        {  
            tbl_data.bf.sp_flow_en = cfg->flow_en;
            tbl_data.bf.sp_flow_id = cfg->flow_id;
        }
        if (cfg_msk.s.mirror)
        {  
            tbl_data.bf.sp_mirror_en = cfg->mirror_en;
            tbl_data.bf.sp_mirror_pkt_src_sel = cfg->mirror_src_sel;
        }
        
        /**
         * 21 entries for each valid port:
         * Address 0-20 belongs to table for GE port.  
         * Address 21-41belongs to table for Pon port. 
         * Address 42-62 belongs to table for Cpu port.
         * Address 63-83 belongs to table for MII0 port.
         * Address 84-104 belongs to table for MII1 port.
         */
        __sppkt_table_write(port, pkt_type, &tbl_data);
    }
      
    return (CS_E_OK);

}


cs_status aal_special_pkt_behavior_get(
    CS_IN  cs_aal_port_id_t          port,
    CS_IN  cs_aal_pkt_type_t         pkt_type,
    CS_OUT cs_aal_spec_pkt_ctrl_t    *cfg
)
{
    cs_status ret= CS_E_OK;
    FE_TOP_PRT_SP_PKT_TABLE_DATA_t    tbl_data;
    FE_TOP_GE_PORT_CTRL_2_t data;


    if (pkt_type > AAL_PKT_TYPE_MAX 
        || (port>AAL_PORT_ID_MII1)
        || NULL == cfg) {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return (CS_E_PARAM);
    }

    data.wrd = 0;
    switch(port)
    {
        case AAL_PORT_ID_GE:
            REG_READ(FE_TOP_GE_PORT_CTRL_2, data.wrd);
            break;

        case AAL_PORT_ID_PON:
            REG_READ(FE_TOP_PON_PORT_CTRL_2, data.wrd);
            break;

        case AAL_PORT_ID_CPU:
            REG_READ(FE_TOP_CPU_PORT_CTRL_2, data.wrd);  
            break;

        case AAL_PORT_ID_MII0:
            REG_READ(FE_TOP_MII0_PORT_CTRL_2, data.wrd);
            break;

        case AAL_PORT_ID_MII1:
            REG_READ(FE_TOP_MII1_PORT_CTRL_2, data.wrd);  
            break;

        default:
            return CS_E_PARAM;
    }

    cfg->permit = BITON(data.bf.sp_pkt_permit,pkt_type)?1:0;
        
    
    /**
     * Address 0-20 belongs to table for GE port. 
     * Address 21-41belongs to table for Pon port. 
     * Address 42-62 belongs to table for Cpu port.
     * Address 63-83 belongs to table for MII0 port.
     * Address 84-104 belongs to table for MII1 port.
     */
    ret = __sppkt_table_read(port, pkt_type, &tbl_data);
     
    cfg->pri        = tbl_data.bf.sp_pkt_pri_sel;
    cfg->bypass_plc = tbl_data.bf.sp_policer_bypass;
    cfg->lmt_fwd    = tbl_data.bf.sp_sa_limit_fwd_permit;
    cfg->learning   = !tbl_data.bf.sp_learn_dis;
    cfg->cos        = tbl_data.bf.sp_cos;
    SPEC_DPID_TO_PORT(tbl_data.bf.sp_dpid, cfg->dpid.dst_op, cfg->dpid.dpid);
    cfg->flow_en    = tbl_data.bf.sp_flow_en;
    cfg->flow_id    = tbl_data.bf.sp_flow_id;
    cfg->mirror_en  = tbl_data.bf.sp_mirror_en;
    cfg->mirror_src_sel = tbl_data.bf.sp_mirror_pkt_src_sel ;

    return ret;

}


cs_status aal_port_dft_cfg_set( CS_IN cs_aal_port_id_t port, CS_IN cs_aal_port_dft_msk_t cfg_msk, CS_IN cs_aal_port_dft_cfg_t *cfg)
{
    FE_TOP_GE_PORT_DFLT_CFG_0_t mask0;
    FE_TOP_GE_PORT_DFLT_CFG_0_t data0;
    FE_TOP_GE_PORT_DFLT_CFG_1_t mask1;
    FE_TOP_GE_PORT_DFLT_CFG_1_t data1;


    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if (cfg_msk.u32 == 0)
        return CS_E_OK;

    mask0.wrd = 0;
    data0.wrd = 0;
    mask1.wrd = 0;
    data1.wrd = 0;

    if (cfg_msk.s.vlanid)
    {
        if (cfg->vlanid > 4095)
            return CS_E_PARAM;

        mask0.bf.vlanid = FILL_MSK;
        data0.bf.vlanid = cfg->vlanid;
    }

    if (cfg_msk.s.cos)
    {
        if (cfg->cos > 7)
            return CS_E_PARAM;

        mask0.bf.cos = FILL_MSK;
        data0.bf.cos = cfg->cos;
    }

    if (cfg_msk.s.dot1p)
    {
        if (cfg->dot1p > 7)
            return CS_E_PARAM;

        mask0.bf.Eight021p = FILL_MSK;
        data0.bf.Eight021p = cfg->dot1p;
    }

    if (cfg_msk.s.dscp)
    {
        if (cfg->dscp > 63)
            return CS_E_PARAM;

        mask0.bf.dscp = FILL_MSK;
        data0.bf.dscp = cfg->dscp;
    }

    if (cfg_msk.s.tpid)
    {
        mask1.bf.tpid = FILL_MSK;
        data1.bf.tpid = cfg->tpid;
    }

    switch (port)
    {
        case AAL_PORT_ID_GE:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_GE_PORT_DFLT_CFG_0, mask0.wrd, data0.wrd);
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_GE_PORT_DFLT_CFG_1, mask1.wrd, data1.wrd);
            break;

        case AAL_PORT_ID_PON:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_PON_PORT_DFLT_CFG_0, mask0.wrd, data0.wrd);
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_PON_PORT_DFLT_CFG_1, mask1.wrd, data1.wrd);
            break;

        case AAL_PORT_ID_CPU:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_CPU_PORT_DFLT_CFG_0, mask0.wrd, data0.wrd);
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_CPU_PORT_DFLT_CFG_1, mask1.wrd, data1.wrd);
            break;

        case AAL_PORT_ID_MII0:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_MII0_PORT_DFLT_CFG_0, mask0.wrd, data0.wrd);
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_MII0_PORT_DFLT_CFG_1, mask1.wrd, data1.wrd);
            break;

        case AAL_PORT_ID_MII1:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_MII1_PORT_DFLT_CFG_0, mask0.wrd, data0.wrd);
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_MII1_PORT_DFLT_CFG_1, mask1.wrd, data1.wrd);
            break;

        default:
            return CS_E_PARAM;
            break;
    }

    
    return CS_E_OK;
}


cs_status aal_port_dft_cfg_get( CS_IN cs_aal_port_id_t port, CS_OUT cs_aal_port_dft_cfg_t *cfg )
{
    cs_uint32 data0 = 0;
    cs_uint32 data1 = 0;
    FE_TOP_GE_PORT_DFLT_CFG_0_t *pdata0 = (FE_TOP_GE_PORT_DFLT_CFG_0_t *)&data0;
    FE_TOP_GE_PORT_DFLT_CFG_1_t *pdata1 = (FE_TOP_GE_PORT_DFLT_CFG_1_t *)&data1;


    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    switch (port)
    {
        case AAL_PORT_ID_GE:
            REG_READ(FE_TOP_GE_PORT_DFLT_CFG_0, data0);
            REG_READ(FE_TOP_GE_PORT_DFLT_CFG_1, data1);
            break;

        case AAL_PORT_ID_PON:
            REG_READ(FE_TOP_PON_PORT_DFLT_CFG_0, data0);
            REG_READ(FE_TOP_PON_PORT_DFLT_CFG_1, data1);
            break;

        case AAL_PORT_ID_CPU:
            REG_READ(FE_TOP_CPU_PORT_DFLT_CFG_0, data0);
            REG_READ(FE_TOP_CPU_PORT_DFLT_CFG_1, data1);
            break;

        case AAL_PORT_ID_MII0:
            REG_READ(FE_TOP_MII0_PORT_DFLT_CFG_0, data0);
            REG_READ(FE_TOP_MII0_PORT_DFLT_CFG_1, data1);
            break;

        case AAL_PORT_ID_MII1:
            REG_READ(FE_TOP_MII1_PORT_DFLT_CFG_0, data0);
            REG_READ(FE_TOP_MII1_PORT_DFLT_CFG_1, data1);
            break;

        default:
            return CS_E_PARAM;
            break;
    }

    cfg->vlanid = pdata0->bf.vlanid;
    cfg->cos    = pdata0->bf.cos;
    cfg->dot1p  = pdata0->bf.Eight021p;
    cfg->dscp   = pdata0->bf.dscp;
    cfg->tpid   = pdata1->bf.tpid;

    return CS_E_OK;
}


cs_status aal_port_cfg_set( CS_IN cs_aal_port_id_t port, CS_IN cs_aal_port_msk_t cfg_msk, CS_IN cs_aal_port_cfg_t *cfg )
{
    FE_TOP_GE_PORT_CTRL_0_t mask0;
    FE_TOP_GE_PORT_CTRL_0_t data0;
    FE_TOP_GE_PORT_CTRL_1_t mask1;
    FE_TOP_GE_PORT_CTRL_1_t data1;


    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if (cfg_msk.u32 == 0)
        return CS_E_OK;
    
    mask0.wrd = 0;
    data0.wrd = 0;
    mask1.wrd = 0;
    data1.wrd = 0;

    if (cfg_msk.s.cpu_header_pkt_type)
    {
        if (cfg->cpu_header_pkt_type > AAL_CPU_HEADINFO_SPEK_TYPE)
            return CS_E_PARAM;

        mask0.bf.cpu_header_pkt_type = FILL_MSK;
        data0.bf.cpu_header_pkt_type = cfg->cpu_header_pkt_type;
    }

    if (cfg_msk.s.oam_lpbk_en)
    {
        if (cfg->oam_lpbk_en > 1)
            return CS_E_PARAM;
        
        mask0.bf.oam_lpbk_en = FILL_MSK;
        data0.bf.oam_lpbk_en = cfg->oam_lpbk_en;
    }

    if (cfg_msk.s.dbg_da_sa_swap_en)
    {
        if (cfg->dbg_da_sa_swap_en > 1)
            return CS_E_PARAM;

        mask0.bf.dbg_da_sa_swap_en = FILL_MSK;
        data0.bf.dbg_da_sa_swap_en = cfg->dbg_da_sa_swap_en;
    }

    if (cfg_msk.s.ipv4_dscp_mask)
    {
        if (cfg->ipv4_dscp_mask > 0x3f)
            return CS_E_PARAM;

        mask0.bf.ipv4_dscp_mask = FILL_MSK;
        data0.bf.ipv4_dscp_mask = cfg->ipv4_dscp_mask;
    }

    if (cfg_msk.s.ipv6_tc_mask)
    {
        if (cfg->ipv6_tc_mask > 0x3f)
            return CS_E_PARAM;

        mask0.bf.ipv6_tc_mask = FILL_MSK;
        data0.bf.ipv6_tc_mask = cfg->ipv6_tc_mask;
    }

    if (cfg_msk.s.rx_def_vid_swap)
    {
        if (cfg->rx_def_vid_swap > 1)
            return CS_E_PARAM;

        mask0.bf.vlan_en = FILL_MSK;
        data0.bf.vlan_en = cfg->rx_def_vid_swap;
    }

    if (cfg_msk.s.tx_def_vid_pop)
    {
        if (cfg->tx_def_vid_pop > 1)
            return CS_E_PARAM;

        mask0.bf.untagged_vlan_en = FILL_MSK;
        data0.bf.untagged_vlan_en = cfg->tx_def_vid_pop;
    }

    if (cfg_msk.s.uuc_dpid)
    {   
        mask1.bf.uuc_dpid = FILL_MSK;     
        SPEC_PORT_TO_DPID(cfg->uuc_dpid.dst_op, cfg->uuc_dpid.dpid, data1.bf.uuc_dpid);
    }

    if (cfg_msk.s.uuc_fwd_cos)
    {
        if (cfg->uuc_fwd_cos > 7)
            return CS_E_PARAM;

        mask1.bf.uuc_fwd_cos = FILL_MSK;
        data1.bf.uuc_fwd_cos = cfg->uuc_fwd_cos;
    }

    if (cfg_msk.s.bc_dpid)
    {
        mask1.bf.bc_dpid = FILL_MSK;
        SPEC_PORT_TO_DPID(cfg->bc_dpid.dst_op, cfg->bc_dpid.dpid, data1.bf.bc_dpid);
    }

    if (cfg_msk.s.bc_fwd_cos)
    {
        if (cfg->bc_fwd_cos > 7)
            return CS_E_PARAM;

        mask1.bf.bc_fwd_cos = FILL_MSK;
        data1.bf.bc_fwd_cos = cfg->bc_fwd_cos;
    }

    if (cfg_msk.s.mc_dpid)
    {
        mask1.bf.mc_dpid = FILL_MSK;
        SPEC_PORT_TO_DPID(cfg->mc_dpid.dst_op, cfg->mc_dpid.dpid, data1.bf.mc_dpid);
    }

    if (cfg_msk.s.mc_fwd_cos)
    {
        if (cfg->mc_fwd_cos > 7)
            return CS_E_PARAM;

        mask1.bf.mc_fwd_cos = FILL_MSK;
        data1.bf.mc_fwd_cos = cfg->mc_fwd_cos;
    }


    switch (port)
    {
        case AAL_PORT_ID_GE:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_GE_PORT_CTRL_0, mask0.wrd, data0.wrd);
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_GE_PORT_CTRL_1, mask1.wrd, data1.wrd);
            break;

        case AAL_PORT_ID_PON:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_PON_PORT_CTRL_0, mask0.wrd, data0.wrd);
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_PON_PORT_CTRL_1, mask1.wrd, data1.wrd);
            break;

        case AAL_PORT_ID_CPU:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_CPU_PORT_CTRL_0, mask0.wrd, data0.wrd);
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_CPU_PORT_CTRL_1, mask1.wrd, data1.wrd);   
            break;

        case AAL_PORT_ID_MII0:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_MII0_PORT_CTRL_0, mask0.wrd, data0.wrd);
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_MII0_PORT_CTRL_1, mask1.wrd, data1.wrd);  
            break;

        case AAL_PORT_ID_MII1:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_MII1_PORT_CTRL_0, mask0.wrd, data0.wrd);  
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_MII1_PORT_CTRL_1, mask1.wrd, data1.wrd);  
            break;

        default:
            return CS_E_ERROR;
            break;
    }

    return CS_E_OK;
}


cs_status aal_port_cfg_get( CS_IN cs_aal_port_id_t port, CS_OUT cs_aal_port_cfg_t *cfg )
{
    cs_uint32 data0 = 0;
    cs_uint32 data1 = 0;
    FE_TOP_GE_PORT_CTRL_0_t *pdata0 = (FE_TOP_GE_PORT_CTRL_0_t *)&data0;
    FE_TOP_GE_PORT_CTRL_1_t *pdata1 = (FE_TOP_GE_PORT_CTRL_1_t *)&data1;


    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    switch(port)
    {
        case AAL_PORT_ID_GE:
            REG_READ(FE_TOP_GE_PORT_CTRL_0, data0);
            REG_READ(FE_TOP_GE_PORT_CTRL_1, data1);
            break;

        case AAL_PORT_ID_PON:
            REG_READ(FE_TOP_PON_PORT_CTRL_0, data0);
            REG_READ(FE_TOP_PON_PORT_CTRL_1, data1);
            break;

        case AAL_PORT_ID_CPU:
            REG_READ(FE_TOP_CPU_PORT_CTRL_0, data0);  
            REG_READ(FE_TOP_CPU_PORT_CTRL_1, data1);  
            break;

        case AAL_PORT_ID_MII0:
            REG_READ(FE_TOP_MII0_PORT_CTRL_0, data0);
            REG_READ(FE_TOP_MII0_PORT_CTRL_1, data1);
            break;

        case AAL_PORT_ID_MII1:
            REG_READ(FE_TOP_MII1_PORT_CTRL_0, data0);  
            REG_READ(FE_TOP_MII1_PORT_CTRL_1, data1);  
            break;

        default:
            return CS_E_PARAM;
    }
    
    cfg->cpu_header_pkt_type = pdata0->bf.cpu_header_pkt_type;
    cfg->oam_lpbk_en =  pdata0->bf.oam_lpbk_en;
    cfg->dbg_da_sa_swap_en = pdata0->bf.dbg_da_sa_swap_en;
    cfg->ipv4_dscp_mask = pdata0->bf.ipv4_dscp_mask;
    cfg->ipv6_tc_mask = pdata0->bf.ipv6_tc_mask;
    cfg->rx_def_vid_swap = pdata0->bf.vlan_en;
    cfg->tx_def_vid_pop = pdata0->bf.untagged_vlan_en;
    SPEC_DPID_TO_PORT(pdata1->bf.uuc_dpid, cfg->uuc_dpid.dst_op, cfg->uuc_dpid.dpid);
    cfg->uuc_fwd_cos = pdata1->bf.uuc_fwd_cos;
    SPEC_DPID_TO_PORT(pdata1->bf.bc_dpid, cfg->bc_dpid.dst_op, cfg->bc_dpid.dpid);
    cfg->bc_fwd_cos = pdata1->bf.bc_fwd_cos;
    SPEC_DPID_TO_PORT(pdata1->bf.mc_dpid, cfg->mc_dpid.dst_op, cfg->mc_dpid.dpid);
    cfg->mc_fwd_cos = pdata1->bf.mc_fwd_cos;
    
    return CS_E_OK;
}


cs_status aal_fdb_cfg_set( CS_IN cs_aal_fdb_msk_t cfg_msk, CS_IN cs_aal_fdb_cfg_t *cfg)
{
    FE_TOP_L2E_LRN_AGING_CTRL_0_t mask0;
    FE_TOP_L2E_LRN_AGING_CTRL_0_t data0;
    FE_TOP_L2E_LRN_AGING_CTRL_1_t mask1;
    FE_TOP_L2E_LRN_AGING_CTRL_1_t data1;
    FE_TOP_L2E_FRWD_CTRL_t fwd_mask;
    FE_TOP_L2E_FRWD_CTRL_t fwd_data;
    

    if (!cfg)
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if (cfg_msk.u32 == 0)
        return CS_E_OK;

    mask0.wrd = 0;
    data0.wrd = 0;
    mask1.wrd = 0;
    data1.wrd = 0;
    fwd_mask.wrd = 0;
    fwd_data.wrd = 0;

    if (cfg_msk.s.lrn_mode)
    {
        if (cfg->learn_mode > 1)
            return CS_E_PARAM;

        mask0.bf.learn_mode = FILL_MSK;
        data0.bf.learn_mode = cfg->learn_mode;
    }

    if (cfg_msk.s.aging_en)
    {
        if (cfg->aging_en > 1)
            return CS_E_PARAM;

        mask0.bf.age_en = FILL_MSK;
        data0.bf.age_en = cfg->aging_en;
    }

    if (cfg_msk.s.station_move_enable)
    {
        if (cfg->station_move_en > 1)
            return CS_E_PARAM;

        mask0.bf.station_move_en = FILL_MSK;
        data0.bf.station_move_en = cfg->station_move_en;
    }

    if (cfg_msk.s.station_move_drop)
    {
        if (cfg->station_move_drop > 1)
            return CS_E_PARAM;

        fwd_mask.bf.stn_mov_drop_en = FILL_MSK;
        fwd_data.bf.stn_mov_drop_en = cfg->station_move_drop;
    }
        
    if (cfg_msk.s.aging_cntr)
    {
        if (cfg->aging_cntr > 0xfff)
            return CS_E_PARAM;
    
        mask1.bf.age_start_val = FILL_MSK;
        data1.bf.age_start_val = cfg->aging_cntr;
    }

    if (cfg_msk.s.st_loop_thrshld)
    {
        if (cfg->st_loop_thrshld > 0xfff)
            return CS_E_PARAM;
        
        mask1.bf.age_stm_delta_val = FILL_MSK;
        data1.bf.age_stm_delta_val = cfg->st_loop_thrshld;
    }

    if (cfg_msk.s.st_loop_detect_drop)
    {
        if (cfg->st_loop_detect_drop  > 1)
            return CS_E_PARAM;

        fwd_mask.bf.st_loop_det_drop = FILL_MSK;
        fwd_data.bf.st_loop_det_drop = cfg->st_loop_detect_drop;
    }

    if (mask0.wrd)
        REG_MASK_WRITE(FE_TOP_L2E_LRN_AGING_CTRL_0, mask0.wrd, data0.wrd);

    if (mask1.wrd)
        REG_MASK_WRITE(FE_TOP_L2E_LRN_AGING_CTRL_1, mask1.wrd, data1.wrd); 

    if (fwd_mask.wrd)
        REG_MASK_WRITE(FE_TOP_L2E_FRWD_CTRL, fwd_mask.wrd, fwd_data.wrd); 
       

    /* setting this config at last */
    if (cfg_msk.s.aging_intvl)
    {
        if (!cfg->aging_intvl)
            return CS_E_PARAM;
        REG_WRITE(FE_TOP_L2E_AGING_CFG, cfg->aging_intvl);
    }

    return CS_E_OK;
}


cs_status aal_fdb_cfg_get( CS_OUT cs_aal_fdb_cfg_t *cfg )
{
    FE_TOP_L2E_LRN_AGING_CTRL_0_t data0;
    FE_TOP_L2E_LRN_AGING_CTRL_1_t data1;
    FE_TOP_L2E_FRWD_CTRL_t fwd_data;
    cs_uint32 aging_time = 0;

    if (!cfg)
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    REG_READ(FE_TOP_L2E_LRN_AGING_CTRL_0, data0.wrd);
    REG_READ(FE_TOP_L2E_LRN_AGING_CTRL_1, data1.wrd);
    REG_READ(FE_TOP_L2E_FRWD_CTRL, fwd_data.wrd);
    REG_READ(FE_TOP_L2E_AGING_CFG, aging_time);
    cfg->learn_mode          = data0.bf.learn_mode;
    cfg->aging_en            = data0.bf.age_en;
    cfg->station_move_en     = data0.bf.station_move_en;
    cfg->station_move_drop   = fwd_data.bf.stn_mov_drop_en;
    cfg->aging_cntr          = data1.bf.age_start_val;
    cfg->aging_intvl         = aging_time;
    cfg->st_loop_detect_drop = fwd_data.bf.st_loop_det_drop;
    cfg->st_loop_thrshld     = data1.bf.age_stm_delta_val;

    return CS_E_OK;
}


cs_status aal_fdb_entry_set( CS_IN cs_uint8 idx, CS_IN cs_aal_fdb_entry_t *fdb )
{
    FE_TOP_L2E_TABLE_MEM_DATA0_t data0;
    FE_TOP_L2E_TABLE_MEM_DATA1_t data1;
    FE_TOP_L2E_TABLE_MEM_DATA2_t data2;

    if (idx >= 64 || !fdb)
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if (fdb->mac_vld > 1 || fdb->dot1p > 7 || fdb->vid > 4095 || fdb->permit_sa_mii1 > 1 || fdb->permit_da_mii1 > 1 ||
        fdb->permit_sa_mii0 > 1 || fdb->permit_da_mii0 > 1 || fdb->permit_sa_pon > 1 || fdb->permit_da_pon > 1 ||
        fdb->permit_sa_cpu > 1 || fdb->permit_da_cpu > 1 || fdb->permit_sa_ge > 1 || fdb->permit_da_ge > 1 ||
        fdb->static_flag > 1 || fdb->vld > 1 || fdb->dot1p_vld > 1 || fdb->vid_vld > 1)
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    data0.wrd = 0;
    data1.wrd = 0;
    data2.wrd = 0;

    data0.bf.mac_addr   = (fdb->mac.addr[2] << 24) | (fdb->mac.addr[3] << 16) | (fdb->mac.addr[4] << 8) | (fdb->mac.addr[5]);
    data1.bf.mac_addr   = (fdb->mac.addr[0] << 8) | fdb->mac.addr[1];
    data1.bf.mac_vld    = fdb->mac_vld;
    data1.bf.dot1p      = fdb->dot1p;
    data1.bf.vid        = fdb->vid;
    data2.bf.age_cnt    = fdb->aging_status;
    data2.bf.mii1_sa_pd = fdb->permit_sa_mii1;
    data2.bf.mii1_da_pd = fdb->permit_da_mii1;
    data2.bf.mii0_sa_pd = fdb->permit_sa_mii0;
    data2.bf.mii0_da_pd = fdb->permit_da_mii0;
    data2.bf.cpu_sa_pd  = fdb->permit_sa_cpu;
    data2.bf.cpu_da_pd  = fdb->permit_da_cpu;
    data2.bf.pon_sa_pd  = fdb->permit_sa_pon;
    data2.bf.pon_da_pd  = fdb->permit_da_pon;
    data2.bf.ge_sa_pd   = fdb->permit_sa_ge;
    data2.bf.ge_da_pd   = fdb->permit_da_ge;
    data2.bf.pid        = fdb->port_id;
    data2.bf.static_dyn = fdb->static_flag;
    data2.bf.valid      = fdb->vld;
    data2.bf.dot1p_vld  = fdb->dot1p_vld;
    data2.bf.vlanid_vld = fdb->vid_vld;


    return __fdb_write_acc(idx, data2, data1, data0);
}


cs_status aal_fdb_entry_get( CS_IN cs_uint8 idx, CS_OUT cs_aal_fdb_entry_t *fdb)
{
    FE_TOP_L2E_TABLE_MEM_DATA0_t data0;
    FE_TOP_L2E_TABLE_MEM_DATA1_t data1;
    FE_TOP_L2E_TABLE_MEM_DATA2_t data2;

    if (idx >= 64 || !fdb)
        return CS_E_PARAM;

    if (__fdb_read_acc(idx, &data2, &data1, &data0) != CS_E_OK)
        return CS_E_ERROR;

    fdb->mac.addr[0]    = data1.bf.mac_addr >> 8;
    fdb->mac.addr[1]    = data1.bf.mac_addr & 0xff;
    fdb->mac.addr[2]    = (data0.bf.mac_addr >> 24 & 0xff);
    fdb->mac.addr[3]    = (data0.bf.mac_addr >> 16 & 0xff);
    fdb->mac.addr[4]    = (data0.bf.mac_addr >> 8 & 0xff);
    fdb->mac.addr[5]    = data0.bf.mac_addr & 0xff;

    fdb->mac_vld        = data1.bf.mac_vld;
    fdb->dot1p          = data1.bf.dot1p;
    fdb->vid            = data1.bf.vid;
    fdb->aging_status   = data2.bf.age_cnt;
    fdb->permit_sa_mii1 = data2.bf.mii1_sa_pd;
    fdb->permit_da_mii1 = data2.bf.mii1_da_pd;
    fdb->permit_sa_mii0 = data2.bf.mii0_sa_pd;
    fdb->permit_da_mii0 = data2.bf.mii0_da_pd;
    fdb->permit_sa_cpu  = data2.bf.cpu_sa_pd;
    fdb->permit_da_cpu  = data2.bf.cpu_da_pd;
    fdb->permit_sa_pon  = data2.bf.pon_sa_pd;
    fdb->permit_da_pon  = data2.bf.pon_da_pd;
    fdb->permit_sa_ge   = data2.bf.ge_sa_pd;
    fdb->permit_da_ge   = data2.bf.ge_da_pd;
    fdb->port_id        = data2.bf.pid;
    fdb->static_flag    = data2.bf.static_dyn;
    fdb->vld            = data2.bf.valid;
    fdb->dot1p_vld      = data2.bf.dot1p_vld;
    fdb->vid_vld        = data2.bf.vlanid_vld;

    return CS_E_OK;
}


cs_status aal_ge_vlan_mac_limit_entry_set(CS_IN cs_aal_fdb_vlan_mac_t idx, CS_IN cs_aal_vlan_mac_entry_t * vlan_mac_entry )
{  
    FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_0_t  cfg0;
    FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_1_t  cfg1;
    FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_2_t  cfg2;
    FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_3_t  cfg3;
    
    if (idx > AAL_FDB_GE_VLN_CFG3 || !vlan_mac_entry)
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if((vlan_mac_entry->vld >1)||(vlan_mac_entry->mac_limit >64)||(vlan_mac_entry->mac_limited_vlan >4095))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    cfg0.wrd = 0;
    cfg1.wrd = 0;
    cfg2.wrd = 0;
    cfg3.wrd = 0;
        
    if(idx == AAL_FDB_GE_VLN_CFG0)
    {
        cfg0.bf.vlan_limit_valid = vlan_mac_entry->vld;
        cfg0.bf.mac_limit = vlan_mac_entry->mac_limit;
        cfg0.bf.mac_limited_vlan = vlan_mac_entry->mac_limited_vlan; 
        REG_WRITE(FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_0, cfg0.wrd);
    }
    else if(idx == AAL_FDB_GE_VLN_CFG1)
    {
        cfg1.bf.vlan_limit_valid = vlan_mac_entry->vld;
        cfg1.bf.mac_limit = vlan_mac_entry->mac_limit;
        cfg1.bf.mac_limited_vlan = vlan_mac_entry->mac_limited_vlan; 
        REG_WRITE(FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_1, cfg1.wrd);
    }
    else if(idx == AAL_FDB_GE_VLN_CFG2)
    {
        cfg2.bf.vlan_limit_valid = vlan_mac_entry->vld;
        cfg2.bf.mac_limit = vlan_mac_entry->mac_limit;
        cfg2.bf.mac_limited_vlan = vlan_mac_entry->mac_limited_vlan; 
        REG_WRITE(FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_2, cfg2.wrd);
    }
    else if(idx == AAL_FDB_GE_VLN_CFG3)
    {
        cfg3.bf.vlan_limit_valid = vlan_mac_entry->vld;
        cfg3.bf.mac_limit = vlan_mac_entry->mac_limit;
        cfg3.bf.mac_limited_vlan = vlan_mac_entry->mac_limited_vlan; 
        REG_WRITE(FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_3, cfg3.wrd);
    }
    
     return CS_E_OK;
}


cs_status aal_ge_vlan_mac_limit_entry_get( CS_IN cs_aal_fdb_vlan_mac_t idx, CS_OUT cs_aal_vlan_mac_entry_t *vlan_mac_entry)
{  
    FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_0_t  cfg0;
    FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_1_t  cfg1;
    FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_2_t  cfg2;
    FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_3_t  cfg3;
    
    
    if (idx > AAL_FDB_GE_VLN_CFG3 || !vlan_mac_entry)
        return CS_E_PARAM;

    cfg0.wrd = 0;
    cfg1.wrd = 0;
    cfg2.wrd = 0;
    cfg3.wrd = 0;
        
    if(idx == AAL_FDB_GE_VLN_CFG0)
    {
        REG_READ(FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_0, cfg0.wrd);
        vlan_mac_entry->vld = cfg0.bf.vlan_limit_valid;
        vlan_mac_entry->mac_limit = cfg0.bf.mac_limit;
        vlan_mac_entry->mac_limited_vlan = cfg0.bf.mac_limited_vlan;       
    }
    else if(idx == AAL_FDB_GE_VLN_CFG1)
    {
        REG_READ(FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_1, cfg1.wrd);
        vlan_mac_entry->vld = cfg1.bf.vlan_limit_valid;
        vlan_mac_entry->mac_limit = cfg1.bf.mac_limit;
        vlan_mac_entry->mac_limited_vlan = cfg1.bf.mac_limited_vlan;       
    }
    else if(idx == AAL_FDB_GE_VLN_CFG2)
    {
        REG_READ(FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_2, cfg2.wrd);
        vlan_mac_entry->vld = cfg2.bf.vlan_limit_valid;
        vlan_mac_entry->mac_limit = cfg2.bf.mac_limit;
        vlan_mac_entry->mac_limited_vlan = cfg2.bf.mac_limited_vlan;       
    }
    else if(idx == AAL_FDB_GE_VLN_CFG3)
    {
        REG_READ(FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_3, cfg3.wrd);
        vlan_mac_entry->vld = cfg3.bf.vlan_limit_valid;
        vlan_mac_entry->mac_limit = cfg3.bf.mac_limit;
        vlan_mac_entry->mac_limited_vlan = cfg3.bf.mac_limited_vlan;       
    }
    
     return CS_E_OK;    
}


cs_status aal_fdb_port_cfg_set( CS_IN cs_aal_port_id_t port, CS_IN cs_aal_fdb_port_msk_t cfg_msk, CS_IN cs_aal_fdb_port_cfg_t *cfg)
{
    FE_TOP_GE_PORT_CTRL_0_t       mask0;
    FE_TOP_GE_PORT_CTRL_0_t       data0;
    FE_TOP_L2E_LRN_AGING_CTRL_0_t aging_mask;
    FE_TOP_L2E_LRN_AGING_CTRL_0_t aging_data;
    FE_TOP_L2E_FRWD_CTRL_t        fwd_mask;
    FE_TOP_L2E_FRWD_CTRL_t        fwd_data;
    FE_TOP_L2E_LEARN_CFG_0_t      lrn_mask0;
    FE_TOP_L2E_LEARN_CFG_0_t      lrn_data0;
    FE_TOP_L2E_LEARN_CFG_1_t      lrn_mask1;
    FE_TOP_L2E_LEARN_CFG_1_t      lrn_data1;


    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if (cfg_msk.u32 == 0)
        return CS_E_OK;
    
    mask0.wrd      = 0;
    data0.wrd      = 0;
    aging_mask.wrd = 0;
    aging_data.wrd = 0;
    fwd_mask.wrd   = 0;
    fwd_data.wrd   = 0;
    lrn_mask0.wrd  = 0;
    lrn_data0.wrd  = 0;
    lrn_mask1.wrd  = 0;
    lrn_data1.wrd  = 0;

    if (cfg_msk.s.learn_dis)
    {
        if (cfg->learn_dis > 1)
            return CS_E_PARAM;

        mask0.bf.block_pkt_l2_lrn = FILL_MSK;
        data0.bf.block_pkt_l2_lrn = cfg->learn_dis;
    }

    if (cfg_msk.s.learn_lmt_ovrwrt)
    {
        if (cfg->learn_lmt_ovrwrt > 1)
            return CS_E_PARAM;
    
        mask0.bf.l2_learn_lru_en = FILL_MSK;
        data0.bf.l2_learn_lru_en = cfg->learn_lmt_ovrwrt;
    }

    if (cfg_msk.s.learn_dpid_cpu_dis)
    {
        if (cfg->learn_dpid_cpu_dis > 1)
            return CS_E_PARAM;
            
        mask0.bf.block_dpid_cpu_lrn = FILL_MSK;
        data0.bf.block_dpid_cpu_lrn = cfg->learn_dpid_cpu_dis;
    }

    if (cfg_msk.s.learn_dpid_drop_dis)
    {
        if (cfg->learn_dpid_drop_dis > 1)
            return CS_E_PARAM;

        mask0.bf.block_dpid_drop_lrn = FILL_MSK;
        data0.bf.block_dpid_drop_lrn = cfg->learn_dpid_drop_dis;
    }

    if (cfg_msk.s.pkt_fwd_dis)
    {
        if (cfg->pkt_fwd_dis > 1)
            return CS_E_PARAM;

        mask0.bf.block_pkt_fwd = FILL_MSK;
        data0.bf.block_pkt_fwd = cfg->pkt_fwd_dis;
    }

    if (cfg_msk.s.learn_vlan_sel)
    {
        if (cfg->learn_vlan_sel > AAL_FDB_VLAN_LRN_RX_INNER)
            return CS_E_PARAM;

        mask0.bf.l2_vlan_sel = FILL_MSK;
        data0.bf.l2_vlan_sel = cfg->learn_vlan_sel;
    }

    if (cfg_msk.s.lookup_vlan_sel)
    {
        if (cfg->lookup_vlan_sel > AAL_FDB_VLAN_LKUP_RX_INNER)
            return CS_E_PARAM;
        
        mask0.bf.l2_fwd_vlan_sel = FILL_MSK;
        data0.bf.l2_fwd_vlan_sel = cfg->lookup_vlan_sel;
    }
        
    if (cfg_msk.s.sa_drop_en)
    {
        if (cfg->sa_drop_en > 1)
            return CS_E_PARAM;

        mask0.bf.sa_drop_en = FILL_MSK;
        data0.bf.sa_drop_en = cfg->sa_drop_en;
    }
    
    switch (port)
    {
        case AAL_PORT_ID_GE:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_GE_PORT_CTRL_0, mask0.wrd, data0.wrd);
            break;

        case AAL_PORT_ID_PON:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_PON_PORT_CTRL_0, mask0.wrd, data0.wrd);
            break;

        case AAL_PORT_ID_CPU:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_CPU_PORT_CTRL_0, mask0.wrd, data0.wrd);  
            break;

        case AAL_PORT_ID_MII0:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_MII0_PORT_CTRL_0, mask0.wrd, data0.wrd);  
            break;

        case AAL_PORT_ID_MII1:
            if (mask0.wrd)
                REG_MASK_WRITE(FE_TOP_MII1_PORT_CTRL_0, mask0.wrd, data0.wrd);  
            break;

        default:
            return CS_E_ERROR;
            break;
    }


    switch (port)
    {
        case AAL_PORT_ID_GE:
            if (cfg_msk.s.learn_mac_vld)
            {
                if (cfg->learn_mac_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_mac_vld_ge = FILL_MSK;
                aging_data.bf.learn_mac_vld_ge = cfg->learn_mac_vld;
            }

            if (cfg_msk.s.learn_vlan_vld)
            {
                if (cfg->learn_vlan_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_vlan_vld_ge = FILL_MSK;
                aging_data.bf.learn_vlan_vld_ge = cfg->learn_vlan_vld;
            }

            if (cfg_msk.s.learn_dot1p_vld)
            {
                if (cfg->learn_dot1p_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_dot1p_vld_ge = FILL_MSK;
                aging_data.bf.learn_dot1p_vld_ge = cfg->learn_dot1p_vld;
            }

            if (cfg_msk.s.sa_lmt_drop_en)
            {
                if (cfg->sa_lmt_drop_en > 1)
                    return CS_E_PARAM;
                fwd_mask.bf.sa_lrn_max_limit_drop_en_ge = FILL_MSK;
                fwd_data.bf.sa_lrn_max_limit_drop_en_ge = cfg->sa_lmt_drop_en;
            }

            if (cfg_msk.s.lookup_mac_vld)
            {
                if (cfg->lookup_mac_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_mac_vld_ge = FILL_MSK;
                fwd_data.bf.fwd_mac_vld_ge = cfg->lookup_mac_vld;
            }

            if (cfg_msk.s.lookup_vlan_vld)
            {
                if (cfg->lookup_vlan_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_vlan_vld_ge = FILL_MSK;
                fwd_data.bf.fwd_vlan_vld_ge = cfg->lookup_vlan_vld;
            }

            if (cfg_msk.s.lookup_dot1p_vld)
            {
                if (cfg->lookup_dot1p_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_dot1p_vld_ge = FILL_MSK;
                fwd_data.bf.fwd_dot1p_vld_ge = cfg->lookup_dot1p_vld;
            }

            if (cfg_msk.s.mac_match_ctrl)
            {
                if (cfg->mac_match_ctrl > AAL_FDB_PERMIT_DA_SA)
                    return CS_E_PARAM;

                aging_mask.bf.ge_permit = FILL_MSK;
                aging_data.bf.ge_permit = cfg->mac_match_ctrl;
            }
            
            if (cfg_msk.s.sa_max_limit)
            {
                if (cfg->sa_max_limit > 0x40)
                    return CS_E_PARAM;
                lrn_mask0.bf.ge_sa_max_learn_limit = FILL_MSK;
                lrn_data0.bf.ge_sa_max_learn_limit = cfg->sa_max_limit;
            }
            break;

        case AAL_PORT_ID_PON:
            if (cfg_msk.s.learn_mac_vld)
            {
                if (cfg->learn_mac_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_mac_vld_pon = FILL_MSK;
                aging_data.bf.learn_mac_vld_pon = cfg->learn_mac_vld;
            }

            if (cfg_msk.s.learn_vlan_vld)
            {
                if (cfg->learn_vlan_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_vlan_vld_pon = FILL_MSK;
                aging_data.bf.learn_vlan_vld_pon = cfg->learn_vlan_vld;
            }

            if (cfg_msk.s.learn_dot1p_vld)
            {
                if (cfg->learn_dot1p_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_dot1p_vld_pon = FILL_MSK;
                aging_data.bf.learn_dot1p_vld_pon = cfg->learn_dot1p_vld;
            }

            if (cfg_msk.s.sa_lmt_drop_en)
            {
                if (cfg->sa_lmt_drop_en > 1)
                    return CS_E_PARAM;
                fwd_mask.bf.sa_lrn_max_limit_drop_en_pon = FILL_MSK;
                fwd_data.bf.sa_lrn_max_limit_drop_en_pon = cfg->sa_lmt_drop_en;
            }

            if (cfg_msk.s.lookup_mac_vld)
            {
                if (cfg->lookup_mac_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_mac_vld_pon = FILL_MSK;
                fwd_data.bf.fwd_mac_vld_pon = cfg->lookup_mac_vld;
            }

            if (cfg_msk.s.lookup_vlan_vld)
            {
                if (cfg->lookup_vlan_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_vlan_vld_pon = FILL_MSK;
                fwd_data.bf.fwd_vlan_vld_pon = cfg->lookup_vlan_vld;
            }

            if (cfg_msk.s.lookup_dot1p_vld)
            {
                if (cfg->lookup_dot1p_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_dot1p_vld_pon = FILL_MSK;
                fwd_data.bf.fwd_dot1p_vld_pon = cfg->lookup_dot1p_vld;
            }

            if (cfg_msk.s.mac_match_ctrl)
            {
                if (cfg->mac_match_ctrl > AAL_FDB_PERMIT_DA_SA)
                    return CS_E_PARAM;

                aging_mask.bf.pon_permit = FILL_MSK;
                aging_data.bf.pon_permit = cfg->mac_match_ctrl;
            }
            
            if (cfg_msk.s.sa_max_limit)
            {
                if (cfg->sa_max_limit > 0x40)
                    return CS_E_PARAM;
                lrn_mask0.bf.pon_sa_max_learn_limit = FILL_MSK;
                lrn_data0.bf.pon_sa_max_learn_limit = cfg->sa_max_limit;
            }
            break;    

        case AAL_PORT_ID_CPU:
            if (cfg_msk.s.learn_mac_vld)
            {
                if (cfg->learn_mac_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_mac_vld_cpu = FILL_MSK;
                aging_data.bf.learn_mac_vld_cpu = cfg->learn_mac_vld;
            }

            if (cfg_msk.s.learn_vlan_vld)
            {
                if (cfg->learn_vlan_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_vlan_vld_cpu = FILL_MSK;
                aging_data.bf.learn_vlan_vld_cpu = cfg->learn_vlan_vld;
            }

            if (cfg_msk.s.learn_dot1p_vld)
            {
                if (cfg->learn_dot1p_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_dot1p_vld_cpu = FILL_MSK;
                aging_data.bf.learn_dot1p_vld_cpu = cfg->learn_dot1p_vld;
            }

            if (cfg_msk.s.sa_lmt_drop_en)
            {
                if (cfg->sa_lmt_drop_en > 1)
                    return CS_E_PARAM;
                fwd_mask.bf.sa_lrn_max_limit_drop_en_cpu = FILL_MSK;
                fwd_data.bf.sa_lrn_max_limit_drop_en_cpu = cfg->sa_lmt_drop_en;
            }

            if (cfg_msk.s.lookup_mac_vld)
            {
                if (cfg->lookup_mac_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_mac_vld_cpu = FILL_MSK;
                fwd_data.bf.fwd_mac_vld_cpu = cfg->lookup_mac_vld;
            }

            if (cfg_msk.s.lookup_vlan_vld)
            {
                if (cfg->lookup_vlan_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_vlan_vld_cpu = FILL_MSK;
                fwd_data.bf.fwd_vlan_vld_cpu = cfg->lookup_vlan_vld;
            }

            if (cfg_msk.s.lookup_dot1p_vld)
            {
                if (cfg->lookup_dot1p_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_dot1p_vld_cpu = FILL_MSK;
                fwd_data.bf.fwd_dot1p_vld_cpu = cfg->lookup_dot1p_vld;
            }

            if (cfg_msk.s.mac_match_ctrl)
            {
                if (cfg->mac_match_ctrl > AAL_FDB_PERMIT_DA_SA)
                    return CS_E_PARAM;

                aging_mask.bf.cpu_permit = FILL_MSK;
                aging_data.bf.cpu_permit = cfg->mac_match_ctrl;
            }
            
            if (cfg_msk.s.sa_max_limit)
            {
                if (cfg->sa_max_limit > 0x40)
                    return CS_E_PARAM;
                lrn_mask0.bf.cpu_sa_max_learn_limit = FILL_MSK;
                lrn_data0.bf.cpu_sa_max_learn_limit = cfg->sa_max_limit;
            }
            break;

        case AAL_PORT_ID_MII0:
            if (cfg_msk.s.learn_mac_vld)
            {
                if (cfg->learn_mac_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_mac_vld_mii0 = FILL_MSK;
                aging_data.bf.learn_mac_vld_mii0 = cfg->learn_mac_vld;
            }

            if (cfg_msk.s.learn_vlan_vld)
            {
                if (cfg->learn_vlan_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_vlan_vld_mii0 = FILL_MSK;
                aging_data.bf.learn_vlan_vld_mii0 = cfg->learn_vlan_vld;
            }

            if (cfg_msk.s.learn_dot1p_vld)
            {
                if (cfg->learn_dot1p_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_dot1p_vld_mii0 = FILL_MSK;
                aging_data.bf.learn_dot1p_vld_mii0 = cfg->learn_dot1p_vld;
            }

            if (cfg_msk.s.sa_lmt_drop_en)
            {
                if (cfg->sa_lmt_drop_en > 1)
                    return CS_E_PARAM;
                fwd_mask.bf.sa_lrn_max_limit_drop_en_mii0 = FILL_MSK;
                fwd_data.bf.sa_lrn_max_limit_drop_en_mii0 = cfg->sa_lmt_drop_en;
            }

            if (cfg_msk.s.lookup_mac_vld)
            {
                if (cfg->lookup_mac_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_mac_vld_mii0 = FILL_MSK;
                fwd_data.bf.fwd_mac_vld_mii0 = cfg->lookup_mac_vld;
            }

            if (cfg_msk.s.lookup_vlan_vld)
            {
                if (cfg->lookup_vlan_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_vlan_vld_mii0 = FILL_MSK;
                fwd_data.bf.fwd_vlan_vld_mii0 = cfg->lookup_vlan_vld;
            }

            if (cfg_msk.s.lookup_dot1p_vld)
            {
                if (cfg->lookup_dot1p_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_dot1p_vld_mii0 = FILL_MSK;
                fwd_data.bf.fwd_dot1p_vld_mii0 = cfg->lookup_dot1p_vld;
            }

            if (cfg_msk.s.mac_match_ctrl)
            {
                if (cfg->mac_match_ctrl > AAL_FDB_PERMIT_DA_SA)
                    return CS_E_PARAM;

                aging_mask.bf.mii0_permit = FILL_MSK;
                aging_data.bf.mii0_permit = cfg->mac_match_ctrl;
            }
            
            if (cfg_msk.s.sa_max_limit)
            {
                if (cfg->sa_max_limit > 0x40)
                    return CS_E_PARAM;
                lrn_mask0.bf.mii0_sa_max_learn_limit = FILL_MSK;
                lrn_data0.bf.mii0_sa_max_learn_limit = cfg->sa_max_limit;
            }
            break;

        case AAL_PORT_ID_MII1:
            if (cfg_msk.s.learn_mac_vld)
            {
                if (cfg->learn_mac_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_mac_vld_mii1 = FILL_MSK;
                aging_data.bf.learn_mac_vld_mii1 = cfg->learn_mac_vld;
            }

            if (cfg_msk.s.learn_vlan_vld)
            {
                if (cfg->learn_vlan_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_vlan_vld_mii1 = FILL_MSK;
                aging_data.bf.learn_vlan_vld_mii1 = cfg->learn_vlan_vld;
            }

            if (cfg_msk.s.learn_dot1p_vld)
            {
                if (cfg->learn_dot1p_vld > 1)
                    return CS_E_PARAM;
                aging_mask.bf.learn_dot1p_vld_mii1 = FILL_MSK;
                aging_data.bf.learn_dot1p_vld_mii1 = cfg->learn_dot1p_vld;
            }

            if (cfg_msk.s.sa_lmt_drop_en)
            {
                if (cfg->sa_lmt_drop_en > 1)
                    return CS_E_PARAM;
                fwd_mask.bf.sa_lrn_max_limit_drop_en_mii1 = FILL_MSK;
                fwd_data.bf.sa_lrn_max_limit_drop_en_mii1 = cfg->sa_lmt_drop_en;
            }

            if (cfg_msk.s.lookup_mac_vld)
            {
                if (cfg->lookup_mac_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_mac_vld_mii1 = FILL_MSK;
                fwd_data.bf.fwd_mac_vld_mii1 = cfg->lookup_mac_vld;
            }

            if (cfg_msk.s.lookup_vlan_vld)
            {
                if (cfg->lookup_vlan_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_vlan_vld_mii1 = FILL_MSK;
                fwd_data.bf.fwd_vlan_vld_mii1 = cfg->lookup_vlan_vld;
            }

            if (cfg_msk.s.lookup_dot1p_vld)
            {
                if (cfg->lookup_dot1p_vld > 1)
                    return CS_E_PARAM;

                fwd_mask.bf.fwd_dot1p_vld_mii1 = FILL_MSK;
                fwd_data.bf.fwd_dot1p_vld_mii1 = cfg->lookup_dot1p_vld;
            }

            if (cfg_msk.s.mac_match_ctrl)
            {
                if (cfg->mac_match_ctrl > AAL_FDB_PERMIT_DA_SA)
                    return CS_E_PARAM;

                aging_mask.bf.mii1_permit = FILL_MSK;
                aging_data.bf.mii1_permit = cfg->mac_match_ctrl;
            }
            
            if (cfg_msk.s.sa_max_limit)
            {
                if (cfg->sa_max_limit > 0x40)
                    return CS_E_PARAM;
                lrn_mask1.bf.mii1_sa_max_learn_limit = FILL_MSK;
                lrn_data1.bf.mii1_sa_max_learn_limit = cfg->sa_max_limit;
            }
            break;

        default:
            break;
    }

    if (fwd_mask.wrd)
        REG_MASK_WRITE(FE_TOP_L2E_FRWD_CTRL, fwd_mask.wrd, fwd_data.wrd);  
    
    if (lrn_mask0.wrd)
        REG_MASK_WRITE(FE_TOP_L2E_LEARN_CFG_0, lrn_mask0.wrd, lrn_data0.wrd);  

    if (lrn_mask1.wrd)    
        REG_MASK_WRITE(FE_TOP_L2E_LEARN_CFG_1, lrn_mask1.wrd, lrn_data1.wrd);  
    
    if (aging_mask.wrd)
        REG_MASK_WRITE(FE_TOP_L2E_LRN_AGING_CTRL_0, aging_mask.wrd, aging_data.wrd);  
        
    return CS_E_OK;
}


cs_status aal_fdb_port_cfg_get( CS_IN cs_aal_port_id_t port, CS_OUT cs_aal_fdb_port_cfg_t *cfg )
{
    cs_uint32 data0 = 0;
    FE_TOP_GE_PORT_CTRL_0_t *pdata0 = (FE_TOP_GE_PORT_CTRL_0_t *)&data0;
    FE_TOP_L2E_LRN_AGING_CTRL_0_t aging_mask;
    FE_TOP_L2E_LRN_AGING_CTRL_0_t aging_data;
    FE_TOP_L2E_FRWD_CTRL_t   fwd_mask;
    FE_TOP_L2E_FRWD_CTRL_t   fwd_data;
    FE_TOP_L2E_LEARN_CFG_0_t lrn_mask0;
    FE_TOP_L2E_LEARN_CFG_0_t lrn_data0;
    FE_TOP_L2E_LEARN_CFG_1_t lrn_mask1;
    FE_TOP_L2E_LEARN_CFG_1_t lrn_data1;


    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    aging_mask.wrd = 0;
    aging_data.wrd = 0;
    fwd_mask.wrd   = 0;
    fwd_data.wrd   = 0;
    lrn_mask0.wrd  = 0;
    lrn_data0.wrd  = 0;
    lrn_mask1.wrd  = 0;
    lrn_data1.wrd  = 0;

    REG_READ(FE_TOP_L2E_FRWD_CTRL, fwd_data.wrd);
    REG_READ(FE_TOP_L2E_LEARN_CFG_0, lrn_data0.wrd);  
    REG_READ(FE_TOP_L2E_LEARN_CFG_1, lrn_data1.wrd); 
    REG_READ(FE_TOP_L2E_LRN_AGING_CTRL_0, aging_data.wrd);  

    switch(port)
    {
        case AAL_PORT_ID_GE:
            REG_READ(FE_TOP_GE_PORT_CTRL_0, data0);
            cfg->learn_mac_vld = aging_data.bf.learn_mac_vld_ge;       
            cfg->learn_vlan_vld = aging_data.bf.learn_vlan_vld_ge;  
            cfg->learn_dot1p_vld = aging_data.bf.learn_dot1p_vld_ge;
            cfg->sa_lmt_drop_en = fwd_data.bf.sa_lrn_max_limit_drop_en_ge;   
            cfg->lookup_mac_vld = fwd_data.bf.fwd_mac_vld_ge;
            cfg->lookup_vlan_vld = fwd_data.bf.fwd_vlan_vld_ge;
            cfg->lookup_dot1p_vld = fwd_data.bf.fwd_dot1p_vld_ge;
            cfg->mac_match_ctrl = aging_data.bf.ge_permit;            
            cfg->sa_max_limit = lrn_data0.bf.ge_sa_max_learn_limit; 
            break;

        case AAL_PORT_ID_PON:
            REG_READ(FE_TOP_PON_PORT_CTRL_0, data0);
            cfg->learn_mac_vld = aging_data.bf.learn_mac_vld_pon;       
            cfg->learn_vlan_vld = aging_data.bf.learn_vlan_vld_pon;  
            cfg->learn_dot1p_vld = aging_data.bf.learn_dot1p_vld_pon;
            cfg->sa_lmt_drop_en = fwd_data.bf.sa_lrn_max_limit_drop_en_pon;   
            cfg->lookup_mac_vld = fwd_data.bf.fwd_mac_vld_pon;
            cfg->lookup_vlan_vld = fwd_data.bf.fwd_vlan_vld_pon;
            cfg->lookup_dot1p_vld = fwd_data.bf.fwd_dot1p_vld_pon;
            cfg->mac_match_ctrl = aging_data.bf.pon_permit;            
            cfg->sa_max_limit = lrn_data0.bf.pon_sa_max_learn_limit;
            break;

        case AAL_PORT_ID_CPU:
            REG_READ(FE_TOP_CPU_PORT_CTRL_0, data0);  
            cfg->learn_mac_vld = aging_data.bf.learn_mac_vld_cpu;       
            cfg->learn_vlan_vld = aging_data.bf.learn_vlan_vld_cpu;  
            cfg->learn_dot1p_vld = aging_data.bf.learn_dot1p_vld_cpu;
            cfg->sa_lmt_drop_en = fwd_data.bf.sa_lrn_max_limit_drop_en_cpu;   
            cfg->lookup_mac_vld = fwd_data.bf.fwd_mac_vld_cpu;
            cfg->lookup_vlan_vld = fwd_data.bf.fwd_vlan_vld_cpu;
            cfg->lookup_dot1p_vld = fwd_data.bf.fwd_dot1p_vld_cpu;
            cfg->mac_match_ctrl = aging_data.bf.cpu_permit;            
            cfg->sa_max_limit = lrn_data0.bf.cpu_sa_max_learn_limit;
            break;

        case AAL_PORT_ID_MII0:
            REG_READ(FE_TOP_MII0_PORT_CTRL_0, data0);
            cfg->learn_mac_vld = aging_data.bf.learn_mac_vld_mii0;       
            cfg->learn_vlan_vld = aging_data.bf.learn_vlan_vld_mii0;  
            cfg->learn_dot1p_vld = aging_data.bf.learn_dot1p_vld_mii0;
            cfg->sa_lmt_drop_en = fwd_data.bf.sa_lrn_max_limit_drop_en_mii0;   
            cfg->lookup_mac_vld = fwd_data.bf.fwd_mac_vld_mii0;
            cfg->lookup_vlan_vld = fwd_data.bf.fwd_vlan_vld_mii0;
            cfg->lookup_dot1p_vld = fwd_data.bf.fwd_dot1p_vld_mii0;
            cfg->mac_match_ctrl = aging_data.bf.mii0_permit;            
            cfg->sa_max_limit = lrn_data0.bf.mii0_sa_max_learn_limit;        
            break;

        case AAL_PORT_ID_MII1:
            REG_READ(FE_TOP_MII1_PORT_CTRL_0, data0);  
            cfg->learn_mac_vld = aging_data.bf.learn_mac_vld_mii1;       
            cfg->learn_vlan_vld = aging_data.bf.learn_vlan_vld_mii1;  
            cfg->learn_dot1p_vld = aging_data.bf.learn_dot1p_vld_mii1;
            cfg->sa_lmt_drop_en = fwd_data.bf.sa_lrn_max_limit_drop_en_mii1;   
            cfg->lookup_mac_vld = fwd_data.bf.fwd_mac_vld_mii1;
            cfg->lookup_vlan_vld = fwd_data.bf.fwd_vlan_vld_mii1;
            cfg->lookup_dot1p_vld = fwd_data.bf.fwd_dot1p_vld_mii1;
            cfg->mac_match_ctrl = aging_data.bf.mii1_permit;            
            cfg->sa_max_limit = lrn_data1.bf.mii1_sa_max_learn_limit;        
            break;

        default:
            return CS_E_PARAM;
    }

    cfg->learn_dis = pdata0->bf.block_pkt_l2_lrn;
    cfg->learn_lmt_ovrwrt = pdata0->bf.l2_learn_lru_en;
    cfg->learn_dpid_cpu_dis = pdata0->bf.block_dpid_cpu_lrn;
    cfg->learn_dpid_drop_dis = pdata0->bf.block_dpid_drop_lrn;
    cfg->pkt_fwd_dis = pdata0->bf.block_pkt_fwd;
    cfg->learn_vlan_sel =  pdata0->bf.l2_vlan_sel;
    cfg->lookup_vlan_sel = pdata0->bf.l2_fwd_vlan_sel;        
    cfg->sa_drop_en = pdata0->bf.sa_drop_en;
    
    return CS_E_OK;
}


cs_status aal_fdb_status_get( CS_OUT cs_aal_fdb_status_t *cfg )
{
    cs_uint32 aging_idx = 0;
    FE_TOP_L2E_LEARN_STS_0_t data0;
    FE_TOP_L2E_LEARN_STS_1_t data1;
    FE_TOP_L2E_LEARN_STS_2_t data2;
    FE_TOP_L2E_STN_MOV_IDX_STS_t stn_data;

    if (!cfg)
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    REG_READ(FE_TOP_L2E_LEARN_STS_0, data0.wrd);
    REG_READ(FE_TOP_L2E_LEARN_STS_1, data1.wrd);
    REG_READ(FE_TOP_L2E_LEARN_STS_2, data2.wrd);
    REG_READ(FE_TOP_L2E_STN_MOV_IDX_STS, stn_data.wrd);
    REG_READ(FE_TOP_L2E_AGING_STS, aging_idx);
     
    cfg->aging_idx    = aging_idx;
    cfg->lrn_cnt_mii1 = data1.bf.mii1_learn_cnt;
    cfg->lrn_cnt_mii0 = data0.bf.mii0_learn_cnt;
    cfg->lrn_cnt_cpu  = data0.bf.cpu_learn_cnt;
    cfg->lrn_cnt_pon  = data0.bf.pon_learn_cnt;
    cfg->lrn_cnt_ge   = data0.bf.ge_learn_cnt;

    cfg->ge_vln0_lrn_cnt   = data2.bf.ge_vln0_learn_cnt;
    cfg->ge_vln1_lrn_cnt   = data2.bf.ge_vln1_learn_cnt;
    cfg->ge_vln2_lrn_cnt   = data2.bf.ge_vln2_learn_cnt;
    cfg->ge_vln3_lrn_cnt   = data2.bf.ge_vln3_learn_cnt;
    
    cfg->station_move_idx_mii1 = stn_data.bf.mii1_station_move_index;
    cfg->station_move_idx_mii0 = stn_data.bf.mii0_station_move_index;
    cfg->station_move_idx_cpu  = stn_data.bf.cpu_station_move_index;
    cfg->station_move_idx_pon  = stn_data.bf.pon_station_move_index;
    cfg->station_move_idx_ge   = stn_data.bf.ge_station_move_index;
    
    return CS_E_OK;
}

cs_status aal_fe_int_enable(cs_aal_fe_int_msk_t mask)
{
    cs_uint32 int_en;
    
    REG_READ(FE_TOP_STS_INTENABLE, int_en);
    int_en |= mask.u32[0];
    REG_WRITE(FE_TOP_STS_INTENABLE, int_en);
    
    REG_READ(FE_TOP_STS_INTENABLE_1, int_en);
    int_en |= mask.u32[1];
    REG_WRITE(FE_TOP_STS_INTENABLE_1, int_en);
    
    return CS_E_OK; 
}

cs_status aal_fe_int_disable(cs_aal_fe_int_msk_t mask)
{
    cs_uint32 int_en;
    REG_READ(FE_TOP_STS_INTENABLE, int_en);
    int_en &= (~mask.u32[0]);
    REG_WRITE(FE_TOP_STS_INTENABLE, int_en);
    
    REG_READ(FE_TOP_STS_INTENABLE_1, int_en);
    int_en &= (~mask.u32[1]);
    REG_WRITE(FE_TOP_STS_INTENABLE_1, int_en);
    
    return CS_E_OK; 
}

cs_status aal_fe_int_en_get(cs_aal_fe_int_msk_t *mask)
{
    cs_uint32 int_en;
    
    if(NULL == mask){
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_READ(FE_TOP_STS_INTENABLE, int_en);
    mask->u32[0] = int_en;
    REG_READ(FE_TOP_STS_INTENABLE_1, int_en);
    mask->u32[1] = int_en;

    return CS_E_OK;
}

cs_status aal_fe_int_status_get(cs_aal_fe_int_msk_t* mask)
{

    cs_uint32 int_src;
    
    if(NULL == mask){
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    REG_READ(FE_TOP_STS_INTERRUPT, int_src);
    mask->u32[0] = int_src;
    
    REG_READ(FE_TOP_STS_INTERRUPT_1, int_src);
    mask->u32[1] = int_src;
    
    return CS_E_OK; 
}

cs_status aal_fe_int_status_clr(cs_aal_fe_int_msk_t mask)
{

    REG_WRITE(FE_TOP_STS_INTERRUPT, mask.u32[0]); /*write 1 to clear*/
    REG_WRITE(FE_TOP_STS_INTERRUPT_1, mask.u32[1]); /*write 1 to clear*/

    return CS_E_OK; 
}

#if 0
cs_status aal_fe_int_cfg_set( CS_IN cs_aal_fe_int_msk_t evt_msk, CS_IN cs_aal_fe_int_cfg_t *event )
{
    FE_TOP_STS_INTENABLE_t mask0;
    FE_TOP_STS_INTENABLE_t data0;

    
    if (!event)
        return CS_E_PARAM;

    if ((evt_msk.u32[0] == 0)&&(evt_msk.u32[1] == 0))
        return CS_E_OK;

    mask0.wrd = 0;
    data0.wrd = 0;
    
    if (evt_msk.s.evt_pkt_len_mismatch_err_ge)
    {
        if (event->evt_pkt_len_mismatch_err_ge_en > 1)
            return CS_E_PARAM;

        mask0.bf.ge_pp_pkt_len_mismatch_err_intE = FILL_MSK;
        data0.bf.ge_pp_pkt_len_mismatch_err_intE = event->evt_pkt_len_mismatch_err_ge_en;
    }

    if (evt_msk.s.evt_pkt_len_mismatch_err_pon)
    {
        if (event->evt_pkt_len_mismatch_err_pon_en > 1)
            return CS_E_PARAM;

        mask0.bf.pon_pp_pkt_len_mismatch_err_intE = FILL_MSK;
        data0.bf.pon_pp_pkt_len_mismatch_err_intE = event->evt_pkt_len_mismatch_err_pon_en;
    }

    if (evt_msk.s.evt_pkt_len_mismatch_err_ma)
    {
        if (event->evt_pkt_len_mismatch_err_ma_en > 1)
            return CS_E_PARAM;

        mask0.bf.ma_pp_pkt_len_mismatch_err_intE = FILL_MSK;
        data0.bf.ma_pp_pkt_len_mismatch_err_intE = event->evt_pkt_len_mismatch_err_ma_en;
    }

    if (evt_msk.s.evt_pkt_len_runt_err_ge)
    {
        if (event->evt_pkt_len_runt_err_ge_en > 1)
            return CS_E_PARAM;

        mask0.bf.ge_pp_pkt_len_runt_err_intE = FILL_MSK;
        data0.bf.ge_pp_pkt_len_runt_err_intE = event->evt_pkt_len_runt_err_ge_en;
    }

    if (evt_msk.s.evt_pkt_len_runt_err_pon)
    {
        if (event->evt_pkt_len_runt_err_pon_en > 1)
            return CS_E_PARAM;

        mask0.bf.pon_pp_pkt_len_runt_err_intE = FILL_MSK;
        data0.bf.pon_pp_pkt_len_runt_err_intE = event->evt_pkt_len_runt_err_pon_en;
    }

    if (evt_msk.s.evt_pkt_len_runt_err_ma)
    {
        if (event->evt_pkt_len_runt_err_ma_en > 1)
            return CS_E_PARAM;

        mask0.bf.ma_pp_pkt_len_runt_err_intE = FILL_MSK;
        data0.bf.ma_pp_pkt_len_runt_err_intE = event->evt_pkt_len_runt_err_ma_en;
    }

    if (evt_msk.s.evt_ce_mac_filter_aged_out_ge)
    {
        if (event->evt_ce_mac_filter_aged_out_ge_en > 1)
            return CS_E_PARAM;

        mask0.bf.ce_ge_mac_filter_aged_out_intE = FILL_MSK;
        data0.bf.ce_ge_mac_filter_aged_out_intE = event->evt_ce_mac_filter_aged_out_ge_en;
    }

    if (evt_msk.s.evt_ce_mac_filter_aged_out_pon)
    {
        if (event->evt_ce_mac_filter_aged_out_pon_en > 1)
            return CS_E_PARAM;

        mask0.bf.ce_pon_mac_filter_aged_out_intE = FILL_MSK;
        data0.bf.ce_pon_mac_filter_aged_out_intE = event->evt_ce_mac_filter_aged_out_pon_en;
    }

    if (evt_msk.s.evt_ce_mac_filter_aged_out_ma)
    {
        if (event->evt_ce_mac_filter_aged_out_ma_en > 1)
            return CS_E_PARAM;

        mask0.bf.ce_ma_mac_filter_aged_out_intE = FILL_MSK;
        data0.bf.ce_ma_mac_filter_aged_out_intE = event->evt_ce_mac_filter_aged_out_ma_en;
    }

    if (evt_msk.s.evt_pkt_len_err_ge)
    {
        if (event->evt_pkt_len_err_ge_en > 1)
            return CS_E_PARAM;

        mask0.bf.ge_pkt_len_err_intE = FILL_MSK;
        data0.bf.ge_pkt_len_err_intE = event->evt_pkt_len_err_ge_en;
    }

    if (evt_msk.s.evt_pkt_len_err_pon)
    {
        if (event->evt_pkt_len_err_pon_en > 1)
            return CS_E_PARAM;

        mask0.bf.pon_pkt_len_err_intE = FILL_MSK;
        data0.bf.pon_pkt_len_err_intE = event->evt_pkt_len_err_pon_en;
    }

    if (evt_msk.s.evt_pkt_len_err_ma)
    {
        if (event->evt_pkt_len_err_ma_en > 1)
            return CS_E_PARAM;

        mask0.bf.ma_pkt_len_err_intE = FILL_MSK;
        data0.bf.ma_pkt_len_err_intE = event->evt_pkt_len_err_ma_en;
    }

    if (evt_msk.s.evt_lat_fifo_overflow_ge)
    {
        if (event->evt_lat_fifo_overflow_ge_en > 1)
            return CS_E_PARAM;

        mask0.bf.lat_fifo_ge_overflow_intE = FILL_MSK;
        data0.bf.lat_fifo_ge_overflow_intE = event->evt_lat_fifo_overflow_ge_en;
    }

    if (evt_msk.s.evt_lat_fifo_overflow_pon)
    {
        if (event->evt_lat_fifo_overflow_pon_en > 1)
            return CS_E_PARAM;

        mask0.bf.lat_fifo_pon_overflow_intE = FILL_MSK;
        data0.bf.lat_fifo_pon_overflow_intE = event->evt_lat_fifo_overflow_pon_en;
    }

    if (evt_msk.s.evt_lat_fifo_overflow_ma)
    {
        if (event->evt_lat_fifo_overflow_ma_en > 1)
            return CS_E_PARAM;

        mask0.bf.lat_fifo_ma_overflow_intE = FILL_MSK;
        data0.bf.lat_fifo_ma_overflow_intE = event->evt_lat_fifo_overflow_ma_en;
    }

    if (evt_msk.s.evt_fdb_entry_aged_out)
    {
        if (event->evt_fdb_entry_aged_out_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_entry_aged_out_intE = FILL_MSK;
        data0.bf.l2_entry_aged_out_intE = event->evt_fdb_entry_aged_out_en;
    }

    if (evt_msk.s.evt_fdb_no_avail_dyn_entry_learn)
    {
        if (event->evt_fdb_no_avail_dyn_entry_learn_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_no_avail_dyn_entry_learn_intE = FILL_MSK;
        data0.bf.l2_no_avail_dyn_entry_learn_intE = event->evt_fdb_no_avail_dyn_entry_learn_en;
    }

    if (evt_msk.s.evt_fdb_ovr_max_limit_ge)
    {
        if (event->evt_fdb_ovr_max_limit_ge_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_ovr_max_limit_ge_intE = FILL_MSK;
        data0.bf.l2_ovr_max_limit_ge_intE = event->evt_fdb_ovr_max_limit_ge_en;
    }
      
    if (evt_msk.s.evt_fdb_ovr_max_limit_pon)
    {
        if (event->evt_fdb_ovr_max_limit_pon_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_ovr_max_limit_pon_intE = FILL_MSK;
        data0.bf.l2_ovr_max_limit_pon_intE = event->evt_fdb_ovr_max_limit_pon_en;
    }

    if (evt_msk.s.evt_fdb_ovr_max_limit_cpu)
    {
        if (event->evt_fdb_ovr_max_limit_cpu_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_ovr_max_limit_cpu_intE = FILL_MSK;
        data0.bf.l2_ovr_max_limit_cpu_intE = event->evt_fdb_ovr_max_limit_cpu_en;
    }

    if (evt_msk.s.evt_fdb_ovr_max_limit_mii0)
    {
        if (event->evt_fdb_ovr_max_limit_mii0_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_ovr_max_limit_mii0_intE = FILL_MSK;
        data0.bf.l2_ovr_max_limit_mii0_intE = event->evt_fdb_ovr_max_limit_mii0_en;
    }
    
    if (evt_msk.s.evt_fdb_ovr_max_limit_mii1)
    {
        if (event->evt_fdb_ovr_max_limit_mii1_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_ovr_max_limit_mii1_intE = FILL_MSK;
        data0.bf.l2_ovr_max_limit_mii1_intE = event->evt_fdb_ovr_max_limit_mii1_en;
    }

    if (evt_msk.s.evt_fdb_stn_mov_ge)
    {
        if (event->evt_fdb_stn_mov_ge_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_stn_mov_ge_intE = FILL_MSK;
        data0.bf.l2_stn_mov_ge_intE = event->evt_fdb_stn_mov_ge_en;
    }

    if (evt_msk.s.evt_fdb_stn_mov_pon)
    {
        if (event->evt_fdb_stn_mov_pon_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_stn_mov_pon_intE = FILL_MSK;
        data0.bf.l2_stn_mov_pon_intE = event->evt_fdb_stn_mov_pon_en;
    }

    if (evt_msk.s.evt_fdb_stn_mov_cpu)
    {
        if (event->evt_fdb_stn_mov_cpu_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_stn_mov_cpu_intE = FILL_MSK;
        data0.bf.l2_stn_mov_cpu_intE = event->evt_fdb_stn_mov_cpu_en;
    }

    if (evt_msk.s.evt_fdb_stn_mov_mii0)
    {
        if (event->evt_fdb_stn_mov_mii0_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_stn_mov_mii0_intE = FILL_MSK;
        data0.bf.l2_stn_mov_mii0_intE = event->evt_fdb_stn_mov_mii0_en;
    }

    if (evt_msk.s.evt_fdb_stn_mov_mii1)
    {
        if (event->evt_fdb_stn_mov_mii1_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_stn_mov_mii1_intE = FILL_MSK;
        data0.bf.l2_stn_mov_mii1_intE = event->evt_fdb_stn_mov_mii1_en;
    }

    if (evt_msk.s.evt_fdb_learn_ge)
    {
        if (event->evt_fdb_learn_ge_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_learn_ge_intE = FILL_MSK;
        data0.bf.l2_learn_ge_intE = event->evt_fdb_learn_ge_en;
    }

    if (evt_msk.s.evt_fdb_learn_pon)
    {
        if (event->evt_fdb_learn_pon_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_learn_pon_intE = FILL_MSK;
        data0.bf.l2_learn_pon_intE = event->evt_fdb_learn_pon_en;
    }

    if (evt_msk.s.evt_fdb_learn_cpu)
    {
        if (event->evt_fdb_learn_cpu_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_learn_cpu_intE = FILL_MSK;
        data0.bf.l2_learn_cpu_intE = event->evt_fdb_learn_cpu_en;
    }

    if (evt_msk.s.evt_fdb_learn_mii0)
    {
        if (event->evt_fdb_learn_mii0_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_learn_mii0_intE = FILL_MSK;
        data0.bf.l2_learn_mii0_intE = event->evt_fdb_learn_mii0_en;
    }

    if (evt_msk.s.evt_fdb_learn_mii1)
    {
        if (event->evt_fdb_learn_mii1_en > 1)
            return CS_E_PARAM;

        mask0.bf.l2_learn_mii1_intE = FILL_MSK;
        data0.bf.l2_learn_mii1_intE = event->evt_fdb_learn_mii1_en;
    }

    REG_MASK_WRITE(FE_TOP_STS_INTENABLE, mask0.wrd, data0.wrd);
        
    if (evt_msk.s.evt_fdb_stn_mov_loop_det_drop)
    {
        if (event->evt_fdb_stn_mov_loop_det_drop_en > 1)
            return CS_E_PARAM;

        REG_FIELD_WRITE(FE_TOP_STS_INTENABLE_1, l2_stn_mov_loop_det_drop_intE, event->evt_fdb_stn_mov_loop_det_drop_en);   
    }
        
    return CS_E_OK;
}


cs_status aal_fe_int_cfg_get( CS_OUT cs_aal_fe_int_cfg_t *event )
{
    FE_TOP_STS_INTENABLE_t  data0;

    if (!event)
        return CS_E_PARAM;
    
    data0.wrd = 0;

    REG_READ(FE_TOP_STS_INTENABLE, data0.wrd);
    REG_FIELD_READ(FE_TOP_STS_INTENABLE_1, l2_stn_mov_loop_det_drop_intE, event->evt_fdb_stn_mov_loop_det_drop_en);   
    
    event->evt_pkt_len_mismatch_err_ge_en = data0.bf.ge_pp_pkt_len_mismatch_err_intE;
    event->evt_pkt_len_mismatch_err_pon_en = data0.bf.pon_pp_pkt_len_mismatch_err_intE;
    event->evt_pkt_len_mismatch_err_ma_en = data0.bf.ma_pp_pkt_len_mismatch_err_intE;
    event->evt_pkt_len_runt_err_ge_en = data0.bf.ge_pp_pkt_len_runt_err_intE;
    event->evt_pkt_len_runt_err_pon_en = data0.bf.pon_pp_pkt_len_runt_err_intE;
    event->evt_pkt_len_runt_err_ma_en = data0.bf.ma_pp_pkt_len_runt_err_intE;
    event->evt_ce_mac_filter_aged_out_ge_en = data0.bf.ce_ge_mac_filter_aged_out_intE;
    event->evt_ce_mac_filter_aged_out_pon_en = data0.bf.ce_pon_mac_filter_aged_out_intE;  
    event->evt_ce_mac_filter_aged_out_ma_en = data0.bf.ce_ma_mac_filter_aged_out_intE;
    event->evt_pkt_len_err_ge_en = data0.bf.ge_pkt_len_err_intE;
    event->evt_pkt_len_err_pon_en = data0.bf.pon_pkt_len_err_intE;
    event->evt_pkt_len_err_ma_en = data0.bf.ma_pkt_len_err_intE;
    event->evt_lat_fifo_overflow_ge_en = data0.bf.lat_fifo_ge_overflow_intE;
    event->evt_lat_fifo_overflow_pon_en = data0.bf.lat_fifo_pon_overflow_intE;
    event->evt_lat_fifo_overflow_ma_en = data0.bf.lat_fifo_ma_overflow_intE;
    event->evt_fdb_entry_aged_out_en = data0.bf.l2_entry_aged_out_intE;
    event->evt_fdb_no_avail_dyn_entry_learn_en = data0.bf.l2_no_avail_dyn_entry_learn_intE;
    event->evt_fdb_ovr_max_limit_ge_en = data0.bf.l2_ovr_max_limit_ge_intE;
    event->evt_fdb_ovr_max_limit_pon_en = data0.bf.l2_ovr_max_limit_pon_intE;
    event->evt_fdb_ovr_max_limit_cpu_en = data0.bf.l2_ovr_max_limit_cpu_intE;
    event->evt_fdb_ovr_max_limit_mii0_en = data0.bf.l2_ovr_max_limit_mii0_intE;
    event->evt_fdb_ovr_max_limit_mii1_en = data0.bf.l2_ovr_max_limit_mii1_intE;
    event->evt_fdb_stn_mov_ge_en = data0.bf.l2_stn_mov_ge_intE;
    event->evt_fdb_stn_mov_pon_en =  data0.bf.l2_stn_mov_pon_intE;
    event->evt_fdb_stn_mov_cpu_en = data0.bf.l2_stn_mov_cpu_intE;
    event->evt_fdb_stn_mov_mii0_en = data0.bf.l2_stn_mov_mii0_intE;
    event->evt_fdb_stn_mov_mii1_en = data0.bf.l2_stn_mov_mii1_intE;
    event->evt_fdb_learn_ge_en = data0.bf.l2_learn_ge_intE;
    event->evt_fdb_learn_pon_en = data0.bf.l2_learn_pon_intE;
    event->evt_fdb_learn_cpu_en = data0.bf.l2_learn_cpu_intE;
    event->evt_fdb_learn_mii0_en = data0.bf.l2_learn_mii0_intE;
    event->evt_fdb_learn_mii1_en = data0.bf.l2_learn_mii1_intE;
      
    return CS_E_OK;
}

cs_status aal_fe_int_status_get( CS_OUT cs_aal_fe_int_status_t *event_staus )
{
    FE_TOP_STS_INTERRUPT_t  data0;

    if (!event_staus)
        return CS_E_PARAM;
    
    data0.wrd = 0;

    REG_READ(FE_TOP_STS_INTERRUPT, data0.wrd);
    REG_FIELD_READ(FE_TOP_STS_INTERRUPT_1, l2_stn_mov_loop_det_drop_intI, event_staus->evt_fdb_stn_mov_loop_det_drop_sts);   
    
    event_staus->evt_pkt_len_mismatch_err_ge_sts = data0.bf.ge_pp_pkt_len_mismatch_err_intI;
    event_staus->evt_pkt_len_mismatch_err_pon_sts = data0.bf.pon_pp_pkt_len_mismatch_err_intI;
    event_staus->evt_pkt_len_mismatch_err_ma_sts = data0.bf.ma_pp_pkt_len_mismatch_err_intI;
    event_staus->evt_pkt_len_runt_err_ge_sts = data0.bf.ge_pp_pkt_len_runt_err_intI;
    event_staus->evt_pkt_len_runt_err_pon_sts = data0.bf.pon_pp_pkt_len_runt_err_intI;
    event_staus->evt_pkt_len_runt_err_ma_sts = data0.bf.ma_pp_pkt_len_runt_err_intI;
    event_staus->evt_ce_mac_filter_aged_out_ge_sts = data0.bf.ce_ge_mac_filter_aged_out_intI;
    event_staus->evt_ce_mac_filter_aged_out_pon_sts = data0.bf.ce_pon_mac_filter_aged_out_intI;  
    event_staus->evt_ce_mac_filter_aged_out_ma_sts = data0.bf.ce_ma_mac_filter_aged_out_intI;
    event_staus->evt_pkt_len_err_ge_sts = data0.bf.ge_pkt_len_err_intI;
    event_staus->evt_pkt_len_err_pon_sts = data0.bf.pon_pkt_len_err_intI;
    event_staus->evt_pkt_len_err_ma_sts = data0.bf.ma_pkt_len_err_intI;
    event_staus->evt_lat_fifo_overflow_ge_sts = data0.bf.lat_fifo_ge_overflow_intI;
    event_staus->evt_lat_fifo_overflow_pon_sts = data0.bf.lat_fifo_pon_overflow_intI;
    event_staus->evt_lat_fifo_overflow_ma_sts = data0.bf.lat_fifo_ma_overflow_intI;
    event_staus->evt_fdb_entry_aged_out_sts = data0.bf.l2_entry_aged_out_intI;
    event_staus->evt_fdb_no_avail_dyn_entry_learn_sts = data0.bf.l2_no_avail_dyn_entry_learn_intI;
    event_staus->evt_fdb_ovr_max_limit_ge_sts = data0.bf.l2_ovr_max_limit_ge_intI;
    event_staus->evt_fdb_ovr_max_limit_pon_sts = data0.bf.l2_ovr_max_limit_pon_intI;
    event_staus->evt_fdb_ovr_max_limit_cpu_sts = data0.bf.l2_ovr_max_limit_cpu_intI;
    event_staus->evt_fdb_ovr_max_limit_mii0_sts = data0.bf.l2_ovr_max_limit_mii0_intI;
    event_staus->evt_fdb_ovr_max_limit_mii1_sts = data0.bf.l2_ovr_max_limit_mii1_intI;
    event_staus->evt_fdb_stn_mov_ge_sts = data0.bf.l2_stn_mov_ge_intI;
    event_staus->evt_fdb_stn_mov_pon_sts =  data0.bf.l2_stn_mov_pon_intI;
    event_staus->evt_fdb_stn_mov_cpu_sts = data0.bf.l2_stn_mov_cpu_intI;
    event_staus->evt_fdb_stn_mov_mii0_sts = data0.bf.l2_stn_mov_mii0_intI;
    event_staus->evt_fdb_stn_mov_mii1_sts = data0.bf.l2_stn_mov_mii1_intI;
    event_staus->evt_fdb_learn_ge_sts = data0.bf.l2_learn_ge_intI;
    event_staus->evt_fdb_learn_pon_sts = data0.bf.l2_learn_pon_intI;
    event_staus->evt_fdb_learn_cpu_sts = data0.bf.l2_learn_cpu_intI;
    event_staus->evt_fdb_learn_mii0_sts = data0.bf.l2_learn_mii0_intI;
    event_staus->evt_fdb_learn_mii1_sts = data0.bf.l2_learn_mii1_intI;
      
    return CS_E_OK;
}

cs_status aal_fe_int_status_clr(void)
{
    cs_uint32  data = 0xffffffff;

    REG_WRITE(FE_TOP_STS_INTERRUPT, data);
    REG_WRITE(FE_TOP_STS_INTERRUPT_1, data);   

    return CS_E_OK;
}
#endif

cs_status aal_vlan_cfg_set( CS_IN cs_aal_port_id_t port, CS_IN cs_aal_vlan_cfg_msk_t msk, CS_IN cs_aal_vlan_cfg_t *cfg )
{
    FE_LE_ENGINE_TPID_CFG_0_t mask0;
    FE_LE_ENGINE_TPID_CFG_0_t data0;
    FE_LE_ENGINE_TPID_CFG_1_t mask1;
    FE_LE_ENGINE_TPID_CFG_1_t data1;
    FE_LE_ENGINE_CTRL_t lectrl_mask;
    FE_LE_ENGINE_CTRL_t lectrl_data;
    cs_aal_fe_instance_t fe_id = 0;
    
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if (msk.u32 == 0)
        return CS_E_OK;
    
    fe_id = FE_PORT(port);

    mask0.wrd = 0;
    data0.wrd = 0;
    mask1.wrd = 0;
    data1.wrd = 0;
    lectrl_mask.wrd = 0;
    lectrl_data.wrd = 0;

    if (msk.s.ingress_inner_tpid)
    {
        mask0.bf.ingr_inner_tpid = FILL_MSK;
        data0.bf.ingr_inner_tpid = cfg->ingress_inner_tpid;
    }

    if (msk.s.ingress_outer_tpid1)
    {
        mask0.bf.ingr_outer_tpid1 = FILL_MSK;
        data0.bf.ingr_outer_tpid1 = cfg->ingress_outer_tpid1;
    }
    
    if (msk.s.ingress_outer_tpid2)
    {
        mask1.bf.ingr_outer_tpid2 = FILL_MSK;
        data1.bf.ingr_outer_tpid2 = cfg->ingress_outer_tpid2;
    }

    if (msk.s.parse_inner_8100)
    {  
        if (cfg->parse_inner_8100 > 1)
            return CS_E_PARAM;
         
        SETBIT(lectrl_mask.bf.parser_tpid_valid, 0); 
        if(cfg->parse_inner_8100)
            SETBIT(lectrl_data.bf.parser_tpid_valid, 0); 
        else
            CLRBIT(lectrl_data.bf.parser_tpid_valid, 0);
    }

    if (msk.s.parse_outer_88a8)
    {  
        if (cfg->parse_outer_88a8 > 1)
            return CS_E_PARAM;
        SETBIT(lectrl_mask.bf.parser_tpid_valid, 1); 
        if(cfg->parse_outer_88a8)
            SETBIT(lectrl_data.bf.parser_tpid_valid, 1); 
        else
            CLRBIT(lectrl_data.bf.parser_tpid_valid, 1);
    }

    if (msk.s.parse_inner_tpid)
    {  
        if (cfg->parse_inner_tpid > 1)
            return CS_E_PARAM;
        SETBIT(lectrl_mask.bf.parser_tpid_valid, 3); 
        if(cfg->parse_inner_tpid)
            SETBIT(lectrl_data.bf.parser_tpid_valid, 3); 
        else
            CLRBIT(lectrl_data.bf.parser_tpid_valid, 3);
    }

    if (msk.s.parse_outer_tpid1)
    {  
        if (cfg->parse_outer_tpid1 > 1)
            return CS_E_PARAM;
        SETBIT(lectrl_mask.bf.parser_tpid_valid, 2); 
        if(cfg->parse_outer_tpid1)
            SETBIT(lectrl_data.bf.parser_tpid_valid, 2); 
        else
            CLRBIT(lectrl_data.bf.parser_tpid_valid, 2);
    }

    if (msk.s.parse_outer_tpid2)
    { 
        if (cfg->parse_outer_tpid2 > 1)
            return CS_E_PARAM;
        SETBIT(lectrl_mask.bf.parser_tpid_valid, 4); 
        if(cfg->parse_outer_tpid2)
            SETBIT(lectrl_data.bf.parser_tpid_valid, 4); 
        else
            CLRBIT(lectrl_data.bf.parser_tpid_valid, 4);
    }
   
    if (msk.s.deny_inner_8100)
    {  
        if (cfg->deny_inner_8100 > 1)
            return CS_E_PARAM;
        SETBIT(lectrl_mask.bf.tag_filter_deny, 0); 
        if(cfg->deny_inner_8100)
            SETBIT(lectrl_data.bf.tag_filter_deny, 0); 
        else
            CLRBIT(lectrl_data.bf.tag_filter_deny, 0);
    }

    if (msk.s.deny_outer_88a8)
    { 
        if (cfg->deny_outer_88a8 > 1)
            return CS_E_PARAM;
        SETBIT(lectrl_mask.bf.tag_filter_deny, 1); 
        if(cfg->deny_outer_88a8)
            SETBIT(lectrl_data.bf.tag_filter_deny, 1); 
        else
            CLRBIT(lectrl_data.bf.tag_filter_deny, 1);
    }

    if (msk.s.deny_inner_tpid)
    {  
        if (cfg->deny_inner_tpid > 1)
            return CS_E_PARAM;
        SETBIT(lectrl_mask.bf.tag_filter_deny, 3); 
        if(cfg->deny_inner_tpid)
            SETBIT(lectrl_data.bf.tag_filter_deny, 3); 
        else
            CLRBIT(lectrl_data.bf.tag_filter_deny, 3);
    }

    if (msk.s.deny_outer_tpid1)
    {
        if (cfg->deny_outer_tpid1 > 1)
            return CS_E_PARAM;
        SETBIT(lectrl_mask.bf.tag_filter_deny, 2); 
        if(cfg->deny_outer_tpid1)
            SETBIT(lectrl_data.bf.tag_filter_deny, 2); 
        else
            CLRBIT(lectrl_data.bf.tag_filter_deny, 2);
    }

    if (msk.s.deny_outer_tpid2)
    { 
        if (cfg->deny_outer_tpid2 > 1)
            return CS_E_PARAM;
        SETBIT(lectrl_mask.bf.tag_filter_deny, 4); 
        if(cfg->deny_outer_tpid2)
            SETBIT(lectrl_data.bf.tag_filter_deny, 4); 
        else
            CLRBIT(lectrl_data.bf.tag_filter_deny, 4);
    }

    if (msk.s.ingress_mapping_flag)
    {
        if (cfg->ingress_mapping_flag > 1)
            return CS_E_PARAM;
        lectrl_mask.bf.dbg_ingr_vlan_map_en = FILL_MSK;
        lectrl_data.bf.dbg_ingr_vlan_map_en = cfg->ingress_mapping_flag;
    }

    if (msk.s.top_vlan_l2_en)
    {   
        if (cfg->top_vlan_l2_en > 1)
            return CS_E_PARAM;
        lectrl_mask.bf.top_vlan_l2_en = FILL_MSK;
        lectrl_data.bf.top_vlan_l2_en = cfg->top_vlan_l2_en;
    }

    if (mask0.wrd)
        REG_MASK_WRITE_PORT(FE_LE_ENGINE_TPID_CFG_0, fe_id, mask0.wrd, data0.wrd);

    if (mask1.wrd)
        REG_MASK_WRITE_PORT(FE_LE_ENGINE_TPID_CFG_1, fe_id, mask1.wrd, data1.wrd); 

    if (lectrl_mask.wrd)
        REG_MASK_WRITE_PORT(FE_LE_ENGINE_CTRL, fe_id, lectrl_mask.wrd, lectrl_data.wrd); 
    
    return CS_E_OK;
}


cs_status aal_vlan_cfg_get( CS_IN cs_aal_port_id_t port, CS_IN cs_aal_vlan_cfg_t *cfg )
{   
    FE_LE_ENGINE_TPID_CFG_0_t data0;
    FE_LE_ENGINE_TPID_CFG_1_t data1;
    FE_LE_ENGINE_CTRL_t lectrl_data;
    cs_aal_fe_instance_t fe_id = 0;
    
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);

    REG_READ_PORT(FE_LE_ENGINE_TPID_CFG_0, fe_id, data0.wrd);
    REG_READ_PORT(FE_LE_ENGINE_TPID_CFG_1, fe_id, data1.wrd); 
    REG_READ_PORT(FE_LE_ENGINE_CTRL, fe_id, lectrl_data.wrd); 

    cfg->ingress_inner_tpid   = data0.bf.ingr_inner_tpid;
    cfg->ingress_outer_tpid1  = data0.bf.ingr_outer_tpid1;
    cfg->ingress_outer_tpid2  = data1.bf.ingr_outer_tpid2;
    cfg->ingress_mapping_flag = lectrl_data.bf.dbg_ingr_vlan_map_en;
    cfg->top_vlan_l2_en       = lectrl_data.bf.top_vlan_l2_en;

    cfg->parse_inner_8100  = BITON(lectrl_data.bf.parser_tpid_valid,0)?1:0;
    cfg->parse_outer_88a8  = BITON(lectrl_data.bf.parser_tpid_valid,1)?1:0;
    cfg->parse_inner_tpid  = BITON(lectrl_data.bf.parser_tpid_valid,3)?1:0;
    cfg->parse_outer_tpid1 = BITON(lectrl_data.bf.parser_tpid_valid,2)?1:0;
    cfg->parse_outer_tpid2 = BITON(lectrl_data.bf.parser_tpid_valid,4)?1:0;

    cfg->deny_inner_8100   = BITON(lectrl_data.bf.tag_filter_deny,0)?1:0;
    cfg->deny_outer_88a8   = BITON(lectrl_data.bf.tag_filter_deny,1)?1:0;
    cfg->deny_inner_tpid   = BITON(lectrl_data.bf.tag_filter_deny,3)?1:0;
    cfg->deny_outer_tpid1  = BITON(lectrl_data.bf.tag_filter_deny,2)?1:0;
    cfg->deny_outer_tpid2  = BITON(lectrl_data.bf.tag_filter_deny,4)?1:0;
  
    return CS_E_OK;
}


cs_status aal_vlan_entry_set( CS_IN cs_aal_port_id_t port, CS_IN cs_aal_vlan_idx_t idx, CS_IN cs_aal_vlan_entry_t *cfg)
{
    FE_LE_VE_VLAN_TABLE_DATA_T data;
    cs_uint32 igr_bitmap = 0;
    cs_uint32 egr_bitmap = 0;
    cs_uint32 map_bitmap = 0;
    cs_status ret   = 0;
    cs_aal_fe_instance_t  fe_id = 0;
    cs_uint8  addr  = 0;
    cs_uint8  keyid = 0;
    
    if ((port>AAL_PORT_ID_MII1)||(idx>AAL_VLAN_IDX_DEF_EGRESS)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
 
    if ((cfg->vid_lo > cfg->vid_hi) ||(cfg->vid_lo > 0xfff) ||(cfg->vid_hi > 0xfff) ||( cfg->permit_ge > 1) || (cfg->permit_pon >1)||
       (cfg->permit_mii0 >1)||(cfg->permit_mii1>1)||(cfg->ingress_filter>1)||(cfg->egress_filter>1)||(cfg->mapping_flag>1))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
    addr  = idx/4;
    keyid = idx%4;

    data.data0.wrd = 0;
    data.data1.wrd = 0;
    data.data2.wrd = 0;
    data.data3.wrd = 0;
     
    /*read register */
    ret = __vlan_table_read(fe_id, addr, &data);
    if (ret)
    {
        return CS_E_ERROR;
    }
    
    switch (keyid)
    {
        case 0:
        {
            data.data0.bf.e0_vlanid_hi   = cfg->vid_hi;
            data.data0.bf.e0_vlanid_lo   = cfg->vid_lo;
            data.data0.bf.e0_permit_ge   = cfg->permit_ge;
            data.data0.bf.e0_permit_pon  = cfg->permit_pon;
            data.data0.bf.e0_permit_mii0 = cfg->permit_mii0;
            data.data0.bf.e0_permit_mii1 = cfg->permit_mii1;
            break;
        }
        case 1:
        {
            data.data1.bf.e1_vlanid_hi   = cfg->vid_hi;
            data.data1.bf.e1_vlanid_lo   = cfg->vid_lo>>4;
            data.data0.bf.e1_vlanid_lo   = cfg->vid_lo&0xf;
            data.data1.bf.e1_permit_ge   = cfg->permit_ge;
            data.data1.bf.e1_permit_pon  = cfg->permit_pon;
            data.data1.bf.e1_permit_mii0 = cfg->permit_mii0;
            data.data1.bf.e1_permit_mii1 = cfg->permit_mii1;
            break;
        }
        case 2:
        {
            data.data2.bf.e2_vlanid_hi   = cfg->vid_hi;
            data.data2.bf.e2_vlanid_lo   = cfg->vid_lo>>8;
            data.data1.bf.e2_vlanid_lo   = cfg->vid_lo&0xff;
            data.data2.bf.e2_permit_ge   = cfg->permit_ge;
            data.data2.bf.e2_permit_pon  = cfg->permit_pon;
            data.data2.bf.e2_permit_mii0 = cfg->permit_mii0;
            data.data2.bf.e2_permit_mii1 = cfg->permit_mii1;
            break;
        }
        case 3:
        {
            data.data3.bf.e3_vlanid_hi   = cfg->vid_hi;
            data.data2.bf.e3_vlanid_lo   = cfg->vid_lo;        
            data.data3.bf.e3_permit_ge   = cfg->permit_ge;
            data.data3.bf.e3_permit_pon  = cfg->permit_pon;
            data.data3.bf.e3_permit_mii0 = cfg->permit_mii0;
            data.data3.bf.e3_permit_mii1 = cfg->permit_mii1;
            break;
        }      
        default:
            break;
    }
            
    /*write register */
    ret = __vlan_table_write(fe_id, addr, &data);
    if (ret)
    {
        return CS_E_ERROR;
    }
    
    if(idx<AAL_VLAN_IDX_DEF_INGRESS)
    {
             /*read register */
        REG_READ_PORT(FE_LE_VE_INGRESS_VLAN_TBL_VLD_CFG, fe_id, igr_bitmap);
        REG_READ_PORT(FE_LE_VE_EGRESS_VLAN_TBL_VLD_CFG, fe_id, egr_bitmap);
        REG_READ_PORT(FE_LE_VE_MAP_VLAN_TBL_CFG, fe_id, map_bitmap);
      
        if (cfg->ingress_filter)
            SETBIT(igr_bitmap, idx);      
        else
            CLRBIT(igr_bitmap, idx);

        if (cfg->egress_filter)
            SETBIT(egr_bitmap, idx);      
        else
            CLRBIT(egr_bitmap, idx);

        if (cfg->mapping_flag)
            SETBIT(map_bitmap, idx);      
        else
            CLRBIT(map_bitmap, idx);
                    
          /*write register */
        REG_WRITE_PORT(FE_LE_VE_INGRESS_VLAN_TBL_VLD_CFG, fe_id, igr_bitmap);
        REG_WRITE_PORT(FE_LE_VE_EGRESS_VLAN_TBL_VLD_CFG, fe_id, egr_bitmap);
        REG_WRITE_PORT(FE_LE_VE_MAP_VLAN_TBL_CFG, fe_id, map_bitmap);      
    }

    return CS_E_OK;
}


cs_status aal_vlan_entry_get( CS_IN cs_aal_port_id_t port, CS_IN cs_aal_vlan_idx_t idx, CS_OUT cs_aal_vlan_entry_t *cfg)
{
    FE_LE_VE_VLAN_TABLE_DATA_T data;
    cs_uint32 igr_bitmap = 0;
    cs_uint32 egr_bitmap = 0;
    cs_uint32 map_bitmap = 0;
    cs_aal_fe_instance_t  fe_id = 0;
    cs_status ret   = 0;
    cs_uint8  addr  = 0;
    cs_uint8  keyid = 0;
    
    if ((port>AAL_PORT_ID_MII1)||(idx>AAL_VLAN_IDX_DEF_EGRESS)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
    addr  = idx/4;
    keyid = idx%4;

    data.data0.wrd = 0;
    data.data1.wrd = 0;
    data.data2.wrd = 0;
    data.data3.wrd = 0;
    
    /*read register */
    ret = __vlan_table_read(fe_id, addr, &data);
    if (ret)
    {
        return CS_E_ERROR;
    }
    
    switch (keyid)
    {
        case 0:
        {
            cfg->vid_hi      = data.data0.bf.e0_vlanid_hi;  
            cfg->vid_lo      = data.data0.bf.e0_vlanid_lo;  
            cfg->permit_ge   = data.data0.bf.e0_permit_ge;  
            cfg->permit_pon  = data.data0.bf.e0_permit_pon; 
            cfg->permit_mii0 = data.data0.bf.e0_permit_mii0;
            cfg->permit_mii1 = data.data0.bf.e0_permit_mii1;           
            break;
        }
        case 1:
        {
            cfg->vid_hi      = data.data1.bf.e1_vlanid_hi;
            cfg->vid_lo      = (data.data1.bf.e1_vlanid_lo<<4)|data.data0.bf.e1_vlanid_lo;
            cfg->permit_ge   = data.data1.bf.e1_permit_ge;
            cfg->permit_pon  = data.data1.bf.e1_permit_pon;
            cfg->permit_mii0 = data.data1.bf.e1_permit_mii0;
            cfg->permit_mii1 = data.data1.bf.e1_permit_mii1;
            break;
        }
        case 2:
        {
            cfg->vid_hi      = data.data2.bf.e2_vlanid_hi;
            cfg->vid_lo      = (data.data2.bf.e2_vlanid_lo<<8)|data.data1.bf.e2_vlanid_lo;           
            cfg->permit_ge   = data.data2.bf.e2_permit_ge;
            cfg->permit_pon  = data.data2.bf.e2_permit_pon;
            cfg->permit_mii0 = data.data2.bf.e2_permit_mii0;
            cfg->permit_mii1 = data.data2.bf.e2_permit_mii1;
            break;
        }
        case 3:
        {
            cfg->vid_hi      = data.data3.bf.e3_vlanid_hi;
            cfg->vid_lo      = data.data2.bf.e3_vlanid_lo;        
            cfg->permit_ge   = data.data3.bf.e3_permit_ge;
            cfg->permit_pon  = data.data3.bf.e3_permit_pon;
            cfg->permit_mii0 = data.data3.bf.e3_permit_mii0;
            cfg->permit_mii1 = data.data3.bf.e3_permit_mii1;
            break;
        }      
        default:
            break;
    }
    
    if(idx<AAL_VLAN_IDX_DEF_INGRESS)
    {
             /*read register */
        REG_READ_PORT(FE_LE_VE_INGRESS_VLAN_TBL_VLD_CFG, fe_id, igr_bitmap);
        REG_READ_PORT(FE_LE_VE_EGRESS_VLAN_TBL_VLD_CFG, fe_id, egr_bitmap);
        REG_READ_PORT(FE_LE_VE_MAP_VLAN_TBL_CFG, fe_id, map_bitmap);
      
        cfg->ingress_filter = BITON(igr_bitmap,idx)?1:0;
        cfg->egress_filter  = BITON(egr_bitmap,idx)?1:0;
        cfg->mapping_flag   = BITON(map_bitmap,idx)?1:0;    
    }
    
    return CS_E_OK;
}


cs_status aal_mirror_cfg_set(
    CS_IN cs_aal_mirr_msk_t cfg_msk,
    CS_IN cs_aal_mirr_cfg_t *cfg
)   
{
    FE_TOP_MIRROR_CTRL_t    mirr_msk;    
    FE_TOP_MIRROR_CTRL_t    mirr_ctrl;     

    if (!cfg)
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if (cfg_msk.u32 == 0)
        return CS_E_OK;

    mirr_msk.wrd  = 0;
    mirr_ctrl.wrd = 0;

    if (cfg_msk.s.drr_ge_weight)
    {
        mirr_msk.bf.drr_ge_weight  = FILL_MSK;
        mirr_ctrl.bf.drr_ge_weight = cfg->drr_ge_weight;
    }
    if (cfg_msk.s.drr_pon_weight)
    {
        mirr_msk.bf.drr_pon_weight  = FILL_MSK;
        mirr_ctrl.bf.drr_pon_weight = cfg->drr_pon_weight;
    }
    if (cfg_msk.s.drr_ma_weight)
    {
        mirr_msk.bf.drr_ma_weight  = FILL_MSK;
        mirr_ctrl.bf.drr_ma_weight = cfg->drr_ma_weight;
    }
    if (cfg_msk.s.drr_weight_base)
    {
        if (cfg->drr_weight_base > AAL_MIRR_WEIGHT_BASE_256B)
            return CS_E_PARAM;
        mirr_msk.bf.drr_weight_base  = FILL_MSK;
        mirr_ctrl.bf.drr_weight_base = cfg->drr_weight_base;
    }
    if (cfg_msk.s.enhance_drr)
    {
        if (cfg->enhance_drr > 1)
            return CS_E_PARAM;

        mirr_msk.bf.drr_delay_opt  = FILL_MSK;
        mirr_ctrl.bf.drr_delay_opt = cfg->enhance_drr;
    }
    
    if (mirr_msk.wrd)
        REG_MASK_WRITE(FE_TOP_MIRROR_CTRL, mirr_msk.wrd, mirr_ctrl.wrd);

    if (cfg_msk.s.l2_mirror_groupid)
    {
        if (cfg->l2_mirror_groupid > 31)
            return CS_E_PARAM;
        REG_FIELD_WRITE(FE_TOP_L2E_FRWD_CTRL, l2_mirror_group_id, cfg->l2_mirror_groupid);
      
    }
    if (cfg_msk.s.gl_mirror_en)
    {
        if (cfg->gl_mirror_en > 1)
            return CS_E_PARAM;
        REG_FIELD_WRITE(GLOBAL_CTRL, mir_enable, cfg->gl_mirror_en);
    }
    
    return CS_E_OK;
}


cs_status aal_mirror_cfg_get(
    CS_OUT cs_aal_mirr_cfg_t *cfg
)
{  
    FE_TOP_MIRROR_CTRL_t    mirr_ctrl;     

    if (!cfg)
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    REG_READ(FE_TOP_MIRROR_CTRL, mirr_ctrl.wrd);
    REG_FIELD_READ(FE_TOP_L2E_FRWD_CTRL, l2_mirror_group_id, cfg->l2_mirror_groupid);
    REG_FIELD_READ(GLOBAL_CTRL, mir_enable, cfg->gl_mirror_en);
    REG_FIELD_READ(FE_TOP_L2E_LRN_AGING_CTRL_0, sw_l2_pkt_mirror_en, cfg->l2_mirror_en);

    cfg->drr_ge_weight = mirr_ctrl.bf.drr_ge_weight;
    cfg->drr_pon_weight = mirr_ctrl.bf.drr_pon_weight;
    cfg->drr_ma_weight = mirr_ctrl.bf.drr_ma_weight;
    cfg->drr_weight_base = mirr_ctrl.bf.drr_weight_base;
    cfg->enhance_drr = mirr_ctrl.bf.drr_delay_opt;
      
    return CS_E_OK;
}


cs_status aal_mirror_port_cfg_set(
    CS_IN cs_aal_port_id_t             port,
    CS_IN cs_aal_mirror_port_cfg_msk_t cfg_msk,
    CS_IN cs_aal_mirror_port_cfg_t     *cfg
)
{  
    FE_TOP_GE_PORT_CTRL_1_t mask1;
    FE_TOP_GE_PORT_CTRL_1_t data1;
    FE_TOP_GE_PORT_CTRL_2_t mask2;
    FE_TOP_GE_PORT_CTRL_2_t data2;
    

    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if (cfg_msk.u32 == 0)
        return CS_E_OK;
    
    mask1.wrd = 0;
    data1.wrd = 0;
    mask2.wrd = 0;
    data2.wrd = 0;

    if (cfg_msk.s.umc_mirror_en)
    {
        if (cfg->umc_mirror_en > 1)
            return CS_E_PARAM;
        
        mask1.bf.umc_mirror_en = FILL_MSK;
        data1.bf.umc_mirror_en = cfg->umc_mirror_en;
    }
    if (cfg_msk.s.umc_mirror_src_sel)
    {
        if (cfg->umc_mirror_src_sel > AAL_MIRR_SEL_AFTER_PE)
            return CS_E_PARAM;
        
        mask1.bf.umc_mirror_src_sel = FILL_MSK;
        data1.bf.umc_mirror_src_sel = cfg->umc_mirror_src_sel;
    }
    if (cfg_msk.s.umc_mirror_groupid)
    {
        if (cfg->umc_mirror_groupid > 31)
            return CS_E_PARAM;
        
        mask1.bf.umc_mirror_groupid = FILL_MSK;
        data1.bf.umc_mirror_groupid = cfg->umc_mirror_groupid;
    }
    if (cfg_msk.s.uuc_mirror_en)
    {
        if (cfg->uuc_mirror_en > 1)
            return CS_E_PARAM;
        
        mask1.bf.uuc_mirror_en = FILL_MSK;
        data1.bf.uuc_mirror_en = cfg->uuc_mirror_en;
    }
    if (cfg_msk.s.uuc_mirror_src_sel)
    {
        if (cfg->uuc_mirror_src_sel > AAL_MIRR_SEL_AFTER_PE)
            return CS_E_PARAM;
        
        mask1.bf.uuc_mirror_src_sel = FILL_MSK;
        data1.bf.uuc_mirror_src_sel = cfg->uuc_mirror_src_sel;
    }
    if (cfg_msk.s.uuc_mirror_groupid)
    {
        if (cfg->uuc_mirror_groupid > 31)
            return CS_E_PARAM;
        
        mask1.bf.uuc_mirror_groupid = FILL_MSK;
        data1.bf.uuc_mirror_groupid = cfg->uuc_mirror_groupid;
    }
    if (cfg_msk.s.bc_mirror_en)
    {
        if (cfg->bc_mirror_en > 1)
            return CS_E_PARAM;
        
        mask2.bf.bc_mirror_en = FILL_MSK;
        data2.bf.bc_mirror_en = cfg->bc_mirror_en;
    }
    if (cfg_msk.s.bc_mirror_src_sel)
    {
        if (cfg->bc_mirror_src_sel > AAL_MIRR_SEL_AFTER_PE)
            return CS_E_PARAM;
        
        mask2.bf.bc_mirror_src_sel = FILL_MSK;
        data2.bf.bc_mirror_src_sel = cfg->bc_mirror_src_sel;
    }
    if (cfg_msk.s.bc_mirror_groupid)
    {
        if (cfg->bc_mirror_groupid > 31)
            return CS_E_PARAM;
        
        mask2.bf.bc_mirror_groupid = FILL_MSK;
        data2.bf.bc_mirror_groupid = cfg->bc_mirror_groupid;
    }

    switch (port)
    {
        case AAL_PORT_ID_GE:
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_GE_PORT_CTRL_1, mask1.wrd, data1.wrd);
            if (mask2.wrd)
                REG_MASK_WRITE(FE_TOP_GE_PORT_CTRL_2, mask2.wrd, data2.wrd);
            break;

        case AAL_PORT_ID_PON:
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_PON_PORT_CTRL_1, mask1.wrd, data1.wrd);
            if (mask2.wrd)
                REG_MASK_WRITE(FE_TOP_PON_PORT_CTRL_2, mask2.wrd, data2.wrd);
            break;

        case AAL_PORT_ID_CPU:
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_CPU_PORT_CTRL_1, mask1.wrd, data1.wrd);
            if (mask2.wrd)
                REG_MASK_WRITE(FE_TOP_CPU_PORT_CTRL_2, mask2.wrd, data2.wrd);   
            break;

        case AAL_PORT_ID_MII0:
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_MII0_PORT_CTRL_1, mask1.wrd, data1.wrd);
            if (mask2.wrd)
                REG_MASK_WRITE(FE_TOP_MII0_PORT_CTRL_2, mask2.wrd, data2.wrd);  
            break;

        case AAL_PORT_ID_MII1:
            if (mask1.wrd)
                REG_MASK_WRITE(FE_TOP_MII1_PORT_CTRL_1, mask1.wrd, data1.wrd);  
            if (mask2.wrd)
                REG_MASK_WRITE(FE_TOP_MII1_PORT_CTRL_2, mask2.wrd, data2.wrd);  
            break;

        default:
            return CS_E_ERROR;
            break;
    }
     
    return CS_E_OK;
}


cs_status aal_mirror_port_cfg_get(
    CS_IN  cs_aal_port_id_t         port,
    CS_OUT cs_aal_mirror_port_cfg_t *cfg
)
{   
    cs_uint32 data1 = 0;
    cs_uint32 data2 = 0;
    FE_TOP_GE_PORT_CTRL_1_t *pdata1 = (FE_TOP_GE_PORT_CTRL_1_t *)&data1;
    FE_TOP_GE_PORT_CTRL_2_t *pdata2 = (FE_TOP_GE_PORT_CTRL_2_t *)&data2;


    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    switch(port)
    {
        case AAL_PORT_ID_GE:
            REG_READ(FE_TOP_GE_PORT_CTRL_1, data1);
            REG_READ(FE_TOP_GE_PORT_CTRL_2, data2);
            break;

        case AAL_PORT_ID_PON:
            REG_READ(FE_TOP_PON_PORT_CTRL_1, data1);
            REG_READ(FE_TOP_PON_PORT_CTRL_2, data2);
            break;

        case AAL_PORT_ID_CPU:
            REG_READ(FE_TOP_CPU_PORT_CTRL_1, data1);  
            REG_READ(FE_TOP_CPU_PORT_CTRL_2, data2);  
            break;

        case AAL_PORT_ID_MII0:
            REG_READ(FE_TOP_MII0_PORT_CTRL_1, data1);
            REG_READ(FE_TOP_MII0_PORT_CTRL_2, data2);
            break;

        case AAL_PORT_ID_MII1:
            REG_READ(FE_TOP_MII1_PORT_CTRL_1, data1);  
            REG_READ(FE_TOP_MII1_PORT_CTRL_2, data2);  
            break;

        default:
            return CS_E_PARAM;
    }

    cfg->umc_mirror_en = pdata1->bf.umc_mirror_en;
    cfg->umc_mirror_src_sel = pdata1->bf.umc_mirror_src_sel;
    cfg->umc_mirror_groupid = pdata1->bf.umc_mirror_groupid;
    cfg->uuc_mirror_en = pdata1->bf.uuc_mirror_en;
    cfg->uuc_mirror_src_sel = pdata1->bf.uuc_mirror_src_sel;
    cfg->uuc_mirror_groupid = pdata1->bf.uuc_mirror_groupid;
    cfg->bc_mirror_en = pdata2->bf.bc_mirror_en;
    cfg->bc_mirror_src_sel = pdata2->bf.bc_mirror_src_sel;
    cfg->bc_mirror_groupid = pdata2->bf.bc_mirror_groupid;
   
    return CS_E_OK;
}


cs_status aal_redirect_cfg_set(
    CS_IN cs_aal_port_id_t             port,
    CS_IN cs_aal_redirect_cfg_msk_t    cfg_msk,
    CS_IN cs_aal_redirect_cfg_t        *cfg
)
{  
    FE_LE_PR_DPID_PRMS_CTRL_t mask, data;
    cs_aal_fe_instance_t fe_id = 0;

   
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    if (cfg_msk.u32 == 0)
        return CS_E_OK;

    fe_id = FE_PORT(port);
        
    mask.wrd = 0;
    data.wrd = 0;

    if(cfg_msk.s.dpid_redirect1_en)
    {
        if (cfg->dpid_redirect1_en > 1)
            return CS_E_PARAM;
        
        mask.bf.dpid_redirect1_en = FILL_MSK;
        data.bf.dpid_redirect1_en = cfg->dpid_redirect1_en;
    }
    if(cfg_msk.s.redirect1_fwd_dpid)
    {      
        mask.bf.redirect1_fwd_dpid = FILL_MSK;
        SPEC_PORT_TO_DPID(cfg->redirect1_fwd_dpid.dst_op, cfg->redirect1_fwd_dpid.dpid, data.bf.redirect1_fwd_dpid);
    }
    if(cfg_msk.s.redirect1_original_dpid)
    {
        mask.bf.redirect1_compare_dpid = FILL_MSK;
        SPEC_PORT_TO_DPID(cfg->redirect1_original_dpid.dst_op, cfg->redirect1_original_dpid.dpid, data.bf.redirect1_compare_dpid);
    }
    if(cfg_msk.s.dpid_redirect0_en)
    {
        if (cfg->dpid_redirect0_en > 1)
            return CS_E_PARAM;
        
        mask.bf.dpid_redirect0_en = FILL_MSK;
        data.bf.dpid_redirect0_en = cfg->dpid_redirect0_en;
    }
    if(cfg_msk.s.redirect0_fwd_dpid)
    {
        mask.bf.redirect0_fwd_dpid = FILL_MSK;
        SPEC_PORT_TO_DPID(cfg->redirect0_fwd_dpid.dst_op, cfg->redirect0_fwd_dpid.dpid, data.bf.redirect0_fwd_dpid);
    }
    if(cfg_msk.s.redirect0_original_dpid)
    {
        mask.bf.redirect0_compare_dpid = FILL_MSK;
        SPEC_PORT_TO_DPID(cfg->redirect0_original_dpid.dst_op, cfg->redirect0_original_dpid.dpid, data.bf.redirect0_compare_dpid);
    }
    
    REG_MASK_WRITE_PORT(FE_LE_PR_DPID_PRMS_CTRL, fe_id, mask.wrd, data.wrd);
    
    return CS_E_OK;
}


cs_status aal_redirect_cfg_get(
    CS_IN  cs_aal_port_id_t        port,
    CS_OUT cs_aal_redirect_cfg_t   *cfg
)
{  
    FE_LE_PR_DPID_PRMS_CTRL_t data;
    cs_aal_fe_instance_t fe_id = 0;
     
    
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
     
    REG_READ_PORT(FE_LE_PR_DPID_PRMS_CTRL, fe_id, data.wrd);
    
    cfg->dpid_redirect1_en = data.bf.dpid_redirect1_en;
    SPEC_DPID_TO_PORT(data.bf.redirect1_fwd_dpid, cfg->redirect1_fwd_dpid.dst_op, cfg->redirect1_fwd_dpid.dpid);
    SPEC_DPID_TO_PORT(data.bf.redirect1_compare_dpid, cfg->redirect1_original_dpid.dst_op, cfg->redirect1_original_dpid.dpid);
    
    cfg->dpid_redirect0_en = data.bf.dpid_redirect0_en;
    SPEC_DPID_TO_PORT(data.bf.redirect0_fwd_dpid, cfg->redirect0_fwd_dpid.dst_op, cfg->redirect0_fwd_dpid.dpid);
    SPEC_DPID_TO_PORT(data.bf.redirect0_compare_dpid, cfg->redirect0_original_dpid.dst_op, cfg->redirect0_original_dpid.dpid);
       
    return CS_E_OK;
}


cs_status aal_debug_ctrl_set(
    CS_IN cs_aal_debug_ctrl_msk_t cfg_msk,
    CS_IN cs_aal_debug_ctrl_t *cfg
)
{
    if (!cfg)
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    if (cfg_msk.u32 == 0)
        return CS_E_OK;
    
    if(cfg_msk.s.ge_mask_rxmac_drop_en)
    {
        if (cfg->ge_mask_rxmac_drop_en > 1)
            return CS_E_PARAM;
        REG_FIELD_WRITE_PORT(FE_LE_PE_CTRL, AAL_FE_GE_ID, mask_rxmac_drop, cfg->ge_mask_rxmac_drop_en);
    }
    if(cfg_msk.s.pon_mask_rxmac_drop_en)
    {
        if (cfg->pon_mask_rxmac_drop_en > 1)
            return CS_E_PARAM;
        REG_FIELD_WRITE_PORT(FE_LE_PE_CTRL, AAL_FE_PON_ID, mask_rxmac_drop, cfg->pon_mask_rxmac_drop_en);
    }
    if(cfg_msk.s.ma_mask_rxmac_drop_en)
    {
        if (cfg->ma_mask_rxmac_drop_en > 1)
            return CS_E_PARAM;
        REG_FIELD_WRITE_PORT(FE_LE_PE_CTRL, AAL_FE_MA_ID, mask_rxmac_drop, cfg->ma_mask_rxmac_drop_en);
    }
    if(cfg_msk.s.dbg_cntr_clr_on_read)
    {
        if (cfg->dbg_cntr_clr_on_read > 1)
            return CS_E_PARAM;
        REG_FIELD_WRITE(FE_TOP_MISC_CTRL, dbg_cntr_clr_on_read, cfg->dbg_cntr_clr_on_read);
    }
    
    return CS_E_OK;
}


cs_status aal_debug_ctrl_get(
    CS_OUT cs_aal_debug_ctrl_t *cfg
)
{
    if (!cfg)
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ_PORT(FE_LE_PE_CTRL, AAL_FE_GE_ID, mask_rxmac_drop, cfg->ge_mask_rxmac_drop_en);
    REG_FIELD_READ_PORT(FE_LE_PE_CTRL, AAL_FE_PON_ID, mask_rxmac_drop, cfg->pon_mask_rxmac_drop_en);
    REG_FIELD_READ_PORT(FE_LE_PE_CTRL, AAL_FE_MA_ID, mask_rxmac_drop, cfg->ma_mask_rxmac_drop_en);
    REG_FIELD_READ(FE_TOP_MISC_CTRL, dbg_cntr_clr_on_read, cfg->dbg_cntr_clr_on_read);
     
    return CS_E_OK;
}


cs_status aal_debug_fe_pkt_count_get(
    CS_OUT cs_aal_pkt_fe_cnt_sts_t  *cfg
)
{  
    if (!cfg)
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    REG_READ(FE_TOP_MIR_0_DROP_CNT, cfg->pkt_drop_cnt_ge_fe);
    REG_READ(FE_TOP_MIR_1_DROP_CNT, cfg->pkt_drop_cnt_pon_fe);  
    REG_READ(FE_TOP_MIR_2_DROP_CNT, cfg->pkt_drop_cnt_ma_fe);  
    REG_READ_PORT(FE_LE_ENGINE_INGRESS_PKT_CNT, AAL_FE_GE_ID, cfg->pkt_cnt_ingress_ge);
    REG_READ_PORT(FE_LE_ENGINE_INGRESS_PKT_CNT, AAL_FE_PON_ID, cfg->pkt_cnt_ingress_pon);
    REG_READ_PORT(FE_LE_ENGINE_INGRESS_PKT_CNT, AAL_FE_MA_ID, cfg->pkt_cnt_ingress_ma);
    REG_READ_PORT(FE_LE_ENGINE_EGRESS_PKT_CNT, AAL_FE_GE_ID, cfg->pkt_cnt_egress_ge);
    REG_READ_PORT(FE_LE_ENGINE_EGRESS_PKT_CNT, AAL_FE_PON_ID, cfg->pkt_cnt_egress_pon);
    REG_READ_PORT(FE_LE_ENGINE_EGRESS_PKT_CNT, AAL_FE_MA_ID, cfg->pkt_cnt_egress_ma);

    return CS_E_OK;
}


cs_status aal_debug_pkt_rsltn_state_get(
    CS_IN  cs_aal_port_id_t           port,
    CS_OUT cs_aal_pkt_rsltn_state_t   *cfg
)
{   
    FE_LE_PR_DEBUG_STS_t  data;  
    cs_aal_fe_instance_t fe_id = 0;
     
    
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
    
    REG_READ_PORT(FE_LE_PR_DEBUG_STS, fe_id, data.wrd);

    cfg->vlan_rsltn_st  = data.bf.vlan_rsltn_st;
    cfg->dscp_rsltn_st  = data.bf.dscp_rsltn_st;
    cfg->cos_rsltn_st   = data.bf.cos_rsltn_st;
    cfg->dot1p_rsltn_st = data.bf.Eight021p_rsltn_st;
    cfg->dpid_rsltn_st  = data.bf.dpid_rsltn_st;
  
    return CS_E_OK;
}


cs_status aal_debug_vlan_cmd_err_count_get(
    CS_IN  cs_aal_port_id_t              port,
    CS_OUT cs_aal_pe_vlan_cmd_err_cnt_t  *cfg
)
{
    FE_LE_PE_VLAN_CMD_ERR_CNT_t  data;
    cs_aal_fe_instance_t fe_id = 0;
     
    
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
    
    REG_READ_PORT(FE_LE_PE_VLAN_CMD_ERR_CNT, fe_id, data.wrd);
    cfg->Pop_no_top_vlan_cnt    = data.bf.cnt&0xf;
    cfg->Swap_np_top_vlan_cnt   = (data.bf.cnt>>4)&0xf;
    cfg->Pop_no_inner_vlan_cnt  = (data.bf.cnt>>8)&0xf;
    cfg->swap_no_inner_vlan_cnt = (data.bf.cnt>>12)&0xf;
    cfg->Push_no_inner_vlan_cnt = (data.bf.cnt>>16)&0xf;
    cfg->Runt_pkt_cnt           = (data.bf.cnt>>20)&0xf;

    return CS_E_OK;
}


cs_status aal_debug_pp_info_capture_get(
    CS_IN  cs_aal_port_id_t              port,
    CS_OUT cs_aal_pp_info_capture_t      *cfg
)
{
    cs_aal_fe_instance_t fe_id = 0;
     
    
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);
     
    REG_READ_PORT(FE_LE_PE_PR_INTERFACE_CAPTURE_0, fe_id, cfg->pp_ni_header_capture_0);
    REG_READ_PORT(FE_LE_PE_PR_INTERFACE_CAPTURE_1, fe_id, cfg->pp_ni_header_capture_1);
      
    return CS_E_OK;
}


cs_status aal_debug_pr_info_capture_get(
    CS_IN  cs_aal_port_id_t              port,
    CS_OUT cs_aal_pr_pe_info_capture_t   *cfg
)
{  
    cs_aal_fe_instance_t fe_id = 0;
     
    
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);
    
    return CS_E_OK;
}


cs_status aal_debug_drop_src_capture_get(
    CS_IN  cs_aal_port_id_t              port,
    CS_OUT cs_aal_fe_drop_src_capture_t  *cfg
)
{
    cs_aal_fe_instance_t fe_id = 0;
     
    
    if ((port>AAL_PORT_ID_MII1)||(!cfg))
    {
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);
    
    return CS_E_OK;
}


cs_status l2_init(void)
{   
    FE_TOP_GE_PORT_CTRL_1_t mask1;
    FE_TOP_GE_PORT_CTRL_1_t data1;
    FE_LE_ENGINE_CTRL_t engine_mask;
    FE_LE_ENGINE_CTRL_t engine_data;
    FE_LE_PE_CTRL_t   pe_ctrl_mask, pe_ctrl_data;
    cs_aal_fdb_msk_t  fdb_cfg_mask;
    cs_aal_fdb_cfg_t  fdb_cfg_data;
    cs_aal_fdb_port_msk_t fdb_port_mask;
    cs_aal_fdb_port_cfg_t fdb_port_data;
    cs_aal_port_id_t   port;
    cs_aal_fdb_entry_t fdb;
    cs_uint8    i = 0;
    cs_status ret = 0;
    cs_aal_pkt_type_t         pkt_type;
    cs_aal_spec_pkt_ctrl_msk_t  sp_msk;
    cs_aal_spec_pkt_ctrl_t  cpu_spctrl;
    cs_aal_spec_pkt_ctrl_t  sp_ctrl[] = {
       /* Note: when DPID == 7({2, 0}), Learning,lmt_fwd,cos,bypass_plc,flow,mirror, permit will be ignored */
       /*       they will be from CL_RSLT or others                                                         */
        {1 ,  0,  0, 1, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_NORMAL */ 
        {23,  0,  0, 0, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_BPDU   */
        {28,  0,  0, 0, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_8021X  */
        {17,  0,  0, 0, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_IEEE_RSRVD1 */
        {2,  0,  0, 0, 6, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_MYMAC  */
        {16,  0,  0, 1, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_MC_A  */
        {15,  0,  0, 1, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_MC_B  */
        {14,  0,  0, 1, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_MC_C  */
        {21,  0,  0, 1, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_IGMP  */
        {24,  0,  0, 1, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_ARP   */
        {31,  0,  0, 0, 7, {0, 2}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_OAM   */
        {25,  0,  0, 0, 0, {1, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_MPCP  */
        {13,  0,  0, 1, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_DHCP  */
        {20,  0,  0, 1, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_PPPoE_DIS  */
        {19,  0,  0, 1, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_PPPoE_Session  */
        {18,  0,  0, 0, 6, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_HELLO  */
        {22,  0,  0, 1, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_AAL_PKT_IPV6NDP  */
        {3,   0,  0, 0, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_ALL_PKT_UDF0  */
        {4,   0,  0, 0, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_ALL_PKT_UDF1  */
        {5,   0,  0, 0, 0, {2, 0}, 0, 0, 0, 0, 1},   /**< ONU_ALL_PKT_UDF2  */
        {6,   0,  0, 0, 0, {2, 0}, 0, 0, 0, 0, 1}    /**< ONU_ALL_PKT_UDF3  */
    };

 
     ret = cs_mutex_init(&aal_fdb_mutex, "aal fdb mutex",0);
     if(ret!=0)
     {
         cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", ret, __FILE__, __LINE__);
     }
     
     ret = cs_mutex_init(&aal_vlan_mutex, "aal vlan mutex",0);
     if(ret!=0)
     {
         cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", ret, __FILE__, __LINE__);
     }
     
     ret = cs_mutex_init(&aal_sppkt_mutex, "aal sppkt mutex",0);
     if(ret!=0)
     {
         cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", ret, __FILE__, __LINE__);
     }
    // PORT defaulf setting.
    mask1.wrd = 0;
    data1.wrd = 0;

    mask1.bf.uuc_dpid = FILL_MSK;   
    data1.bf.uuc_dpid = AAL_DPID_PON;

    REG_MASK_WRITE(FE_TOP_GE_PORT_CTRL_1, mask1.wrd, data1.wrd);
    
    mask1.wrd = 0;
    data1.wrd = 0;

    mask1.bf.uuc_dpid = FILL_MSK;   
    data1.bf.uuc_dpid = AAL_DPID_GE;

    REG_MASK_WRITE(FE_TOP_PON_PORT_CTRL_1, mask1.wrd, data1.wrd);

    mask1.wrd = 0;
    data1.wrd = 0;

    mask1.bf.uuc_dpid    = FILL_MSK;   
    mask1.bf.uuc_fwd_cos = FILL_MSK;  
    data1.bf.uuc_dpid    = AAL_DPID_PON;
    data1.bf.uuc_fwd_cos = 0;

    REG_MASK_WRITE(FE_TOP_CPU_PORT_CTRL_1, mask1.wrd, data1.wrd);

    mask1.wrd = 0;
    data1.wrd = 0;

    mask1.bf.uuc_dpid    = FILL_MSK;   
    mask1.bf.uuc_fwd_cos = FILL_MSK;  
    data1.bf.uuc_dpid    = AAL_DPID_MII0;
    data1.bf.uuc_fwd_cos = 0;

    REG_MASK_WRITE(FE_TOP_MII0_PORT_CTRL_1, mask1.wrd, data1.wrd);

    mask1.wrd = 0;
    data1.wrd = 0;

    mask1.bf.uuc_dpid    = FILL_MSK;   
    mask1.bf.uuc_fwd_cos = FILL_MSK;  
    data1.bf.uuc_dpid    = AAL_DPID_MII1;
    data1.bf.uuc_fwd_cos = 0;

    REG_MASK_WRITE(FE_TOP_MII1_PORT_CTRL_1, mask1.wrd, data1.wrd);

    // special pkt defaulf setting
    sp_msk.u32 = 0xffffffff;
    sp_msk.s.permit = 0;
    sp_msk.s.rsvd   = 0;
    for (port = AAL_PORT_ID_GE; port <= AAL_PORT_ID_MII1; ++port) {

        for (pkt_type = AAL_PKT_NORMAL; pkt_type <= AAL_PKT_TYPE_MAX; ++pkt_type) {

            if (port == AAL_PORT_ID_CPU && pkt_type != AAL_PKT_NORMAL) {
                cpu_spctrl = sp_ctrl[pkt_type];
                
                /** CPU--> PON :
                * now it is not used now, because CPU tx will  bypass FE
                */                
                cpu_spctrl.dpid.dst_op = AAL_SPEC_DST_PORT;
                cpu_spctrl.dpid.dpid   = AAL_PORT_ID_PON;
                
                ret = aal_special_pkt_behavior_set(port, 
                                                    pkt_type, 
                                                    sp_msk,
                                                    &cpu_spctrl);
            }
            else {
#if 0                
                // upstream to cpu/MII0.MII1 cos = 3, downstream to cpu/MII0.MII1 cos =7.
                // BM will mapping cos if work in NTT mode.
                if((sp_ctrl[pkt_type].dpid.dst_op == AAL_SPEC_DST_PORT)&&
                    (sp_ctrl[pkt_type].dpid.dpid >= AAL_PORT_ID_CPU)&&(port<=AAL_PORT_ID_PON))
                {
                    sp_ctrl[pkt_type].cos = (port == AAL_PORT_ID_PON)?7:3;
                }
#endif                
                // only GE port enable learning
                if(port != AAL_PORT_ID_GE)
                {
                    sp_ctrl[pkt_type].learning = 0;
                }
                /* only in PON port, OAM,802.1X bypass plc*/
                if((port == AAL_PORT_ID_PON)&&
                    ((pkt_type == AAL_PKT_OAM)||(pkt_type == AAL_PKT_8021X)))
                {
                    sp_ctrl[pkt_type].bypass_plc = 1;
                }
                
                ret = aal_special_pkt_behavior_set(port, 
                                                    pkt_type, 
                                                    sp_msk,
                                                    &sp_ctrl[pkt_type]);
            }

            if (CS_E_OK != ret) {
                cs_halt("special pkt init FAILED\n");
            }
        }
    }
     
    // vlan Tag Identification defaulf setting.
    engine_mask.wrd = 0;
    engine_data.wrd = 0;
    
    engine_mask.bf.mac_adr_loop_chk_en = FILL_MSK;   
    engine_mask.bf.parser_tpid_valid   = FILL_MSK;

    engine_data.bf.mac_adr_loop_chk_en = 1;
    engine_data.bf.parser_tpid_valid   = 0x3;
    
    for(i=0; i<3; i++)
    {
        REG_MASK_WRITE_PORT(FE_LE_ENGINE_CTRL, i, engine_mask.wrd, engine_data.wrd);
    }
   
    /*  fdb configuration */
    fdb_cfg_mask.u32 = 0;
    memset(&fdb_cfg_data, 0, sizeof(cs_aal_fdb_cfg_t));

    fdb_port_mask.u32 = 0;
    memset(&fdb_port_data, 0, sizeof(cs_aal_fdb_port_cfg_t));

    /* do global layer-ii configuration */
    fdb_cfg_mask.s.aging_en = 1;
    fdb_cfg_data.aging_en   = 1;                             /* enable hardware aging */

    fdb_cfg_mask.s.aging_cntr = 1;
    fdb_cfg_data.aging_cntr = 0xfff;
    fdb_cfg_mask.s.aging_intvl = 1;
    fdb_cfg_data.aging_intvl = 143062;   /* 300sec */
    
    (void)aal_fdb_cfg_set(fdb_cfg_mask, &fdb_cfg_data);
      

    /* do port based layer-ii configuration */
    fdb_port_mask.s.sa_max_limit = 1;
    fdb_port_data.sa_max_limit   = 64;

    fdb_port_mask.s.learn_dot1p_vld = 1;
    fdb_port_data.learn_dot1p_vld   = 0;   /* disable dot1p learning */

    fdb_port_mask.s.learn_lmt_ovrwrt = 1;
    fdb_port_data.learn_lmt_ovrwrt   = 0;  /* don't replace mac address if atu is full */

    fdb_port_mask.s.learn_dis = 1;
    fdb_port_data.learn_dis   = 1;         /* disable learning for all ports */

    for (port=AAL_PORT_ID_GE; port<=AAL_PORT_ID_MII1; port++)
    {
        (void)aal_fdb_port_cfg_set(port, fdb_port_mask, &fdb_port_data);       
    }

    /* special initialization for port fdb */
    fdb_port_mask.u32 = 0;
    memset(&fdb_port_data, 0, sizeof(cs_aal_fdb_port_cfg_t));

    fdb_port_mask.s.learn_dis = 1;
    fdb_port_data.learn_dis   = 0;       /* enable learning for giga port */

    (void)aal_fdb_port_cfg_set(AAL_PORT_ID_GE, fdb_port_mask, &fdb_port_data);

       
    /* fdb initialization */
    memset(&fdb, 0, sizeof(fdb));
    fdb.vld = 1;
    for (i=0; i<64; i++)
    {
        if (aal_fdb_entry_set(i, &fdb) != CS_E_OK)
        {    
            cs_halt("fdb entry init FAILED\n");
        }
    }

    /* work-around for 'under size CRC wrong' */
    pe_ctrl_mask.wrd = 0;
    pe_ctrl_data.wrd = 0;
    
    pe_ctrl_mask.bf.txpad_en = FILL_MSK;
    pe_ctrl_mask.bf.dbg_pe_drop_undrsz_pkt_disable = FILL_MSK;
    pe_ctrl_data.bf.txpad_en = 1;
    pe_ctrl_data.bf.dbg_pe_drop_undrsz_pkt_disable = 0;
   
    REG_MASK_WRITE_PORT(FE_LE_PE_CTRL, AAL_FE_PON_ID, pe_ctrl_mask.wrd, pe_ctrl_data.wrd);
    REG_MASK_WRITE_PORT(FE_LE_PE_CTRL, AAL_FE_MA_ID, pe_ctrl_mask.wrd, pe_ctrl_data.wrd);
    pe_ctrl_mask.bf.ingress_pkt_len = FILL_MSK;
    pe_ctrl_data.bf.ingress_pkt_len = 0;
    REG_MASK_WRITE_PORT(FE_LE_PE_CTRL, AAL_FE_GE_ID, pe_ctrl_mask.wrd, pe_ctrl_data.wrd);

     
    return CS_E_OK;
}

/*  end of file */

