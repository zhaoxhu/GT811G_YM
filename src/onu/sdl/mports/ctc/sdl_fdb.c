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

#include "sdl_fdb.h"
#include "sdl_mac_filterbind_util.h"
#include "aal_l2.h"
#include "sdl_init.h"
#include "sdl_init.h"
#include "sdl.h"
#include "rtk_api.h"
#include "rtk_api_ext.h"
#include "rtk_error.h"


/*Private Macro Definition*/
#define __FDB_STATIC_ENTRY_MAC_MAX      (64)
#define __FDB_ENTRY_HW_INDEX_MAX        (2112)
#define __FDB_AGING_TIME_MAX            (458)

#define __FDB_MAC_LIMIT_MAX             (255)



extern sdl_init_cfg_t sdl_int_cfg;

sdl_fdb_port_cfg_t g_fdb_port_cfg[UNI_PORT_MAX];
cs_uint32   g_fdb_aging_time = 0;

typedef struct{
    cs_sdl_fdb_entry_t    entry[__FDB_STATIC_ENTRY_MAC_MAX];
    cs_uint64              valid_map;
} __fdb_static_entry_t ;

__fdb_static_entry_t  __fdb_static_entry_table[UNI_PORT_MAX];      


#if 1
//added by zhuxh
extern cs_uint16 __fdb_static_entry_num;
extern cs_sdl_fdb_entry_t __fdb_static_entry[64];
int convertHex(char num, cs_uint8 *hex)
{
    typedef struct 
    {
        char low;
        char high;
        cs_uint8 base;
    } edge_t;

    static edge_t hex_edge[] = 
    {
        { '0', '9', 0 },
        { 'a', 'f', 10 },
        { 'A', 'F', 10 }
    };

    int i;

    for(i=0; i<sizeof(hex_edge)/sizeof(edge_t); i++)
    {
        if(num >= hex_edge[i].low && num <= hex_edge[i].high)
        {
            *hex = num - hex_edge[i].low + hex_edge[i].base;
            return 0;
        }
    }

    return -1;
}

int get_mac_addr(char *num_array, cs_uint8 *mac)
{
    int i;
    cs_uint8 one, two;

    if(strlen(num_array) != 12)
    {
        diag_printf("\nInvalid mac address!\n");
        return -1;
    }

    for(i=0; i<6; i++)
    {
        if( convertHex(num_array[2*i], &two) || convertHex(num_array[2*i+1], &one) )
            return -1;

        mac[i] = two*16 + one;
    }

    return 0;
}

//删除第一个元素后，此元素后面的所有元素向前移动一个单位
// 因为此list 的删除方法是长度减1，事实上删除的是最后一个元素
static int fdb_static_list_del_update(cs_sdl_fdb_entry_t *pFdb_entry, int entry_len)
{
	//入口规则检查
	if(NULL == pFdb_entry)
	{
		cs_printf("in %s, line:%d, arg check err!\n");
		return CS_E_ERROR;
	}

	if(entry_len <= 0)
	{
		cs_printf("in %s, line:%d, arg check err!\n");
		return CS_E_ERROR;
	}
	//只有一个元素时，清除此元素并退出
	if(1 == entry_len)
	{
		memset(&pFdb_entry[0], 0, sizeof(cs_sdl_fdb_entry_t));
		
		return CS_E_OK;
	}
	
	
	//第一个元素以后的所有元素均向前移动一个单位
	cs_sdl_fdb_entry_t *fdb_entry_buf = NULL;
	int buf_len = 0;
	buf_len = sizeof(cs_sdl_fdb_entry_t)*(entry_len-1);
	fdb_entry_buf = (cs_sdl_fdb_entry_t *)malloc(buf_len);
	if(NULL == fdb_entry_buf)
	{
		cs_printf("in %s, line:%d, malloc err!\n");
		return CS_E_ERROR;
	}
	
	memcpy(fdb_entry_buf, &pFdb_entry[1], buf_len);
	memcpy(&pFdb_entry[0], fdb_entry_buf, buf_len);
	free(fdb_entry_buf);
	fdb_entry_buf = NULL;
	//最后一个元素清零
	memset(&pFdb_entry[entry_len-1], 0, sizeof(cs_sdl_fdb_entry_t));
	
	//结束退出
	return CS_E_OK;
}

int fdb_static_list_add(char *mac, int port, int vlan_id)
{
	int ret = 0;
	cs_status  rc = 0;
	cs_callback_context_t    context;

    if (__fdb_static_entry_num >= sdl_int_cfg.max_static_mac_entry)
	{
        diag_printf("ERROR: entry table is full now\n");
        ret = CS_E_ERROR;
		goto end;
    }

    if(get_mac_addr(mac, __fdb_static_entry[__fdb_static_entry_num].mac.addr))
    {
    	cs_printf("input mac address error\n");
        ret = CS_E_ERROR;
		goto end;
    }
    else
    {
	    __fdb_static_entry[__fdb_static_entry_num].vlan_id = vlan_id;
	    __fdb_static_entry[__fdb_static_entry_num].port = port;
	    __fdb_static_entry[__fdb_static_entry_num].type = SDL_FDB_ENTRY_STATIC;
		rc = epon_request_onu_fdb_entry_add(context, 0, 0, &__fdb_static_entry[__fdb_static_entry_num]);
        if (rc) {
            diag_printf("epon_request_onu_fdb_static_entry_add failed! rc :%d\n",rc);
        }
	    __fdb_static_entry_num++;
    }
	ret = CS_E_OK;
	
end: 
    return ret;       
}

int fdb_static_list_del(char *mac, int vlan_id)
{
	int ret = CS_E_OK;
    cs_uint16  ii = 0;
	cs_status  rc = 0;
	cs_uint8       addr[CS_MACADDR_LEN];
	cs_callback_context_t    context;

    if(get_mac_addr(mac, addr))
    {
    	cs_printf("input mac address error\n");
        ret = CS_E_ERROR;
		goto end;
    }
	for(ii = 0; ii <(__fdb_static_entry_num ) ;ii++)
	{
		if((!memcmp(addr,__fdb_static_entry[ii].mac.addr,6)) && (__fdb_static_entry[ii].vlan_id == vlan_id))
		{
			 rc = epon_request_onu_fdb_entry_del(context, 0, 0,  
		        &__fdb_static_entry[ii].mac, __fdb_static_entry[ii].vlan_id);
		    if (rc) 
			{
		        diag_printf("epon_request_onu_fdb_static_entry_del failed error is %d\n",rc);
		    }
			else
			{
				cs_printf("delete %02x:%02x:%02x:%02x:%02x:%02x static fdb entry success\n",__fdb_static_entry[ii].mac.addr[0],
																				__fdb_static_entry[ii].mac.addr[1],
																				__fdb_static_entry[ii].mac.addr[2],
																				__fdb_static_entry[ii].mac.addr[3],
																				__fdb_static_entry[ii].mac.addr[4],
																				__fdb_static_entry[ii].mac.addr[5]);
				#if 1
				if(CS_E_OK != fdb_static_list_del_update(&__fdb_static_entry[ii], __fdb_static_entry_num - ii))
				{
					ret = CS_E_ERROR;
					goto end;
				}
				#endif
				__fdb_static_entry_num--;
				
				ret = CS_E_OK;
				goto end;
			}
		}
	}
	ret = CS_E_ERROR;
	cs_printf("mac not found!\n");
	
end:
		return ret;   
}
#endif

cs_status fdb_static_entry_show(void)
{
    cs_uint8 i, j;

    cs_printf("\n\n-----------SW Static FDB Table---------------\n\n");
    for(i=0; i<UNI_PORT_MAX; i++){
        cs_uint8 index = 0;
        cs_printf("PortID: %d---\n", i+1);
        for(j=0; j<__FDB_STATIC_ENTRY_MAC_MAX; j++)
        {
            if(__fdb_static_entry_table[i].valid_map & (((cs_uint64)1)<<j)){
                cs_printf("index : %d\n", ++index);
                cs_printf("vlan  : %d\n", __fdb_static_entry_table[i].entry[j].vlan_id);
                cs_printf("mac   : %02x:%02x:%02x:%02x:%02x:%02x\n",
                        __fdb_static_entry_table[i].entry[j].mac.addr[0],
                        __fdb_static_entry_table[i].entry[j].mac.addr[1],
                        __fdb_static_entry_table[i].entry[j].mac.addr[2],
                        __fdb_static_entry_table[i].entry[j].mac.addr[3],
                        __fdb_static_entry_table[i].entry[j].mac.addr[4],
                        __fdb_static_entry_table[i].entry[j].mac.addr[5]);
            }
        }
        cs_printf("\n");
    }
    
    return CS_E_OK;
}

/*check entry is added per ports*/
static cs_boolean fdb_static_entry_find(cs_port_id_t port_id, cs_mac_t *addr, cs_uint8 *index)
{
    cs_uint8 i;
    
    *index = __FDB_STATIC_ENTRY_MAC_MAX;
    
    for(i=0; i<__FDB_STATIC_ENTRY_MAC_MAX; i++){
        if((__fdb_static_entry_table[port_id].valid_map & (((cs_uint64)1)<<i))
            && (!memcmp(__fdb_static_entry_table[port_id].entry[i].mac.addr, addr, 6))){
            *index = i;
            return EPON_TRUE;
        }
    }
    
    return EPON_FALSE;
}

/*check entry is added globaly*/
static cs_boolean fdb_static_entry_check(cs_mac_t *addr)
{
    cs_uint8 port_id;
    cs_uint8 index;
    
    for(port_id=0; port_id<UNI_PORT_MAX; port_id++){
        if(fdb_static_entry_find(port_id, addr, &index))
            return EPON_TRUE;            
    }
    
    return EPON_FALSE;
}

static cs_status fdb_static_entry_idx_get(cs_port_id_t port_id, cs_uint8 *index)
{
    cs_uint8 i;
    
    *index = __FDB_STATIC_ENTRY_MAC_MAX;
    
    for(i=0; i<__FDB_STATIC_ENTRY_MAC_MAX; i++){
        if(!(__fdb_static_entry_table[port_id].valid_map & (((cs_uint64)1) << i ))){
            *index = i;
            return CS_E_OK;
        }
    }
    
    return CS_E_NOT_FOUND;
}

/* Check if it is a valid Unicast mac addr 
 * CS_E_OK  - valid
 * CS_E_ERROR - invalid
 */
static cs_status __fdb_check_mac(cs_mac_t *mac)
{
    cs_uint8 zero_mac[6] = {0, 0, 0, 0, 0, 0};

    /* if all '0', invalid */
    if(!memcmp(mac, zero_mac, sizeof(cs_mac_t))) {
        SDL_MIN_LOG("The MAC address is empty!\n");
        return CS_E_ERROR;
    }

    /* if multi-cast, invalid */
    if(mac->addr[0] & 0x1) {
        SDL_MIN_LOG("The MAC address is multi-cast address!\n");
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}
/* workaround for BUG 29184,make 802.1x packet to be known MC packet*/
static cs_status __fdb_8021x_static_mac(void)
{
    rtk_mac_t       rtk_mac;
    rtk_portmask_t  rtk_portmask;
    rtk_api_ret_t   rtk_ret  = 0;


    rtk_mac.octet[0]= 0x01;
    rtk_mac.octet[1]= 0x80;
    rtk_mac.octet[2]= 0xc2;
    rtk_mac.octet[3]= 0x00;
    rtk_mac.octet[4]= 0x00;
    rtk_mac.octet[5]= 0x03;
  
    rtk_portmask.bits[0]= 0x40; /* uplink port*/
           /* set to HW */
    rtk_ret = rtk_l2_mcastAddr_add(&rtk_mac, 0 ,0, rtk_portmask);
    if(RT_ERR_OK != rtk_ret)
    {
        SDL_MIN_LOG("rtk_l2_mcastAddr_add return %d. FILE: %s, LINE: %d", rtk_ret, __FILE__, __LINE__);
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

cs_status epon_request_onu_fdb_full_behavior_set (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_fdb_full_behavior_t    full_behavior
)
{
    return CS_E_NOT_SUPPORT;
}

cs_status epon_request_onu_fdb_full_behavior_get (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_fdb_full_behavior_t   *full_behavior
)
{
    return CS_E_NOT_SUPPORT;
}

cs_status epon_request_onu_fdb_learn_mode_set (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_fdb_learn_mode_t       lrn_mode
)
{
    return CS_E_NOT_SUPPORT;
}


cs_status epon_request_onu_fdb_learn_mode_get (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_fdb_learn_mode_t      *lrn_mode
)
{
    *lrn_mode = SDL_FDB_SHARED_VLAN_LEARNING;
    
    return CS_E_OK;
}

cs_status epon_request_onu_fdb_mac_limit_set (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_uint32                     mac_num
)
{
    cs_uint32               static_mac_num;
    cs_uint32               dynamic_limit = 0;
    rtk_api_ret_t           rtk_ret = 0;
    cs_status               rt = CS_E_OK;

    UNI_PORT_CHECK(port_id);
    
    if((mac_num>__FDB_MAC_LIMIT_MAX) && (mac_num != __FDB_NO_MAC_LIMIT)){
        SDL_MIN_LOG("mac_num %d is invalid\n", mac_num);
        rt = CS_E_PARAM;
        goto END;
    }

#if 0    
    if(g_fdb_port_cfg[port_id-1].mac_limit == mac_num){
        SDL_MIN_LOG("mac_num %d is same as before\n", mac_num);
        rt = CS_E_OK;
        goto END;
    }
#endif
    
    static_mac_num = g_fdb_port_cfg[port_id-1].static_mac_num;
    
    /*mac_num = 0xffff: Learning and pass all*/
    if(0xffff==mac_num){
        /*Set limit number > lookup table number(eg,2113),
          learn over action is meanless, ie,action is forward.                                                               作用的, 意即action是forward.   
        */
        rtk_ret = rtk_l2_limitLearningCnt_set(port_id-1, RTK_MAX_NUM_OF_LEARN_LIMIT + 1);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("rtk_l2_limitLearningCnt_set return %d\n", rtk_ret);
            rt = CS_E_ERROR;
            goto END;
        }
    }
    else{ /*mac_num = (0-0xff)*/
        if (sdl_int_cfg.fdb_limit_include_static) {
            if (static_mac_num > mac_num) {
                SDL_MIN_LOG("static entry number %d is larger than dynamic limit %d\n", static_mac_num, mac_num);
                rt = CS_E_PARAM;
                goto END;
            } else {
                dynamic_limit = mac_num - static_mac_num; /* '0' is valid */
            }
        } 
        else {
            dynamic_limit = mac_num;
        }

        if((dynamic_limit + static_mac_num) > sdl_int_cfg.max_fdb_entry) {
            SDL_MIN_LOG("mac_num exceed the max FDB entry!\n");
            rt = CS_E_PARAM;
            goto END;
        }
    
        rtk_ret = rtk_l2_limitLearningCnt_set(port_id-1, dynamic_limit);
        if(RT_ERR_OK != rtk_ret) {
            SDL_MIN_LOG("rtk_l2_limitLearningCnt_set return %d\n", rtk_ret);
            rt = CS_E_ERROR;
            goto END;
        }
        
        /*added by wujm@2016-08-10,learn over action is droping*/
        rtk_ret = rtk_l2_limitLearningCntAction_set(RTK_WHOLE_SYSTEM, LIMIT_LEARN_CNT_ACTION_DROP);
    }
    
    /* then, clear all dynamic entires for this port */
    rt = epon_request_onu_fdb_entry_clr_per_port(context, device_id, llidport, SDL_FDB_CLR_DYNAMIC, port_id);
    if (rt) {
        SDL_MIN_LOG("epon_request_onu_fdb_entry_clr_per_port return %d!\n", rt);
        goto END;
    }
        
    g_fdb_port_cfg[port_id-1].mac_limit = mac_num;

END:
    return rt;
}



cs_status epon_request_onu_fdb_mac_limit_get (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_port_id_t                  port_id,
    CS_OUT cs_uint32                     *mac_num
)
{
    if(NULL==mac_num) {
        SDL_MIN_LOG("mac_num is NULL pointer\n");
        return CS_E_PARAM;
    }

    UNI_PORT_CHECK(port_id);
        
    *mac_num = g_fdb_port_cfg[port_id-1].mac_limit;
    
    return CS_E_OK;
}

cs_status epon_request_onu_fdb_age_set (
    CS_IN cs_callback_context_t          context,
    CS_IN cs_int32                       device_id,
    CS_IN cs_int32                       llidport,
    CS_IN cs_uint32                      aging_time
)
{
    rtk_port_t      portid;
    rtk_enable_t    enable;
    rtk_api_ret_t   rtk_ret = 0;
    cs_status       rt = CS_E_OK;

    if (aging_time > sdl_int_cfg.fdb_aging_max_time) {
        SDL_MIN_LOG("aging_time %d is invalid\n", aging_time);
        rt = CS_E_PARAM;
        goto END;
    }
    
    if(g_fdb_aging_time == aging_time){
       SDL_MIN_LOG("aging_time %d is same as before\n", aging_time);
       rt = CS_E_OK;
       goto END;
    }

    for(portid=0; portid<UNI_PORT_MAX; portid++){
        enable = (aging_time != 0);
        rtk_ret = rtk_l2_agingEnable_set(portid, enable);
        if (RT_ERR_OK != rtk_ret) {
            SDL_MIN_LOG("rtk_l2_agingEnable_set return %d\n", rtk_ret);
            rt = CS_E_ERROR;
            goto END;
        }
    }
    
    /*set aging time*/
    rtk_ret = rtk_l2_aging_set(aging_time);
    if (RT_ERR_OK != rtk_ret) {
        SDL_MIN_LOG("rtk_l2_aging_set return %d!\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }
    
    g_fdb_aging_time = aging_time;
     
END:
    return rt;
    
}

cs_status epon_request_onu_fdb_age_get (
    CS_IN  cs_callback_context_t                   context,
    CS_IN  cs_int32                                device_id,
    CS_IN  cs_int32                                llidport,
    CS_OUT cs_uint32                               *aging_time
)
{    
    if(NULL==aging_time) {
        SDL_MIN_LOG("aging_time is NULL pointer\n");
        return CS_E_PARAM;
    }
   
    *aging_time = g_fdb_aging_time;
    
    return CS_E_OK;
    
}

cs_status epon_request_onu_mac_learn_set(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_port_id_t            port_id,
    CS_IN cs_sdl_fdb_mac_learn_t  status
)
{
#if 1
	//modefied by zhuxh
    rtk_api_ret_t   rtk_ret  = 0;
    cs_status rt = CS_E_OK;
    
    UNI_PORT_CHECK(port_id);


    if (status > SDL_FDB_MAC_LEARN_ENABLE) {
        SDL_MIN_LOG("In %s, status %d is not supported\n", __FUNCTION__, status);
        rt = CS_E_PARAM;
        goto END;
    }
    
    if (status == g_fdb_port_cfg[port_id-1].lrn_en) {
        SDL_MIN_LOG("In %s, status %d is the same as before!\n", __FUNCTION__, status);
        rt = CS_E_OK;
        goto END;
    }
    
    if (status == SDL_FDB_MAC_LEARN_DISABLE) {
        /*MAC limit counter is set to 0*/
        rtk_ret = rtk_l2_limitLearningCnt_set(port_id-1, 0);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("In function:%s,line:%d invoke rtk_l2_limitLearningCnt_set fail!\n",__FUNCTION__, __LINE__);
            rt = CS_E_ERROR;
            goto END;
        }
        
        /*learn over action is forwarding*/
        rtk_ret = rtk_l2_limitLearningCntAction_set(RTK_WHOLE_SYSTEM, LIMIT_LEARN_CNT_ACTION_DROP);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("In function:%s,line:%d invoke rtk_l2_limitLearningCnt_set fail!\n",__FUNCTION__, __LINE__);
            rt = CS_E_ERROR;
            goto END;
        }
        
        /*when disable mac-learn,should clear all dynamic entires */
        rt = epon_request_onu_fdb_entry_clr_per_port(context, device, llidport, SDL_FDB_CLR_DYNAMIC, port_id);
        if(rt){
            SDL_MIN_LOG("In function:%s,line:%d invoke epon_request_onu_uc_l2_entry_clr fail!\n",__FUNCTION__, __LINE__);
            goto END;
        }
        
        g_fdb_port_cfg[port_id-1].lrn_en = SDL_FDB_MAC_LEARN_DISABLE;
        
    }
    else{
        /*learn over action is droping*/
        rtk_ret = rtk_l2_limitLearningCntAction_set(RTK_WHOLE_SYSTEM, LIMIT_LEARN_CNT_ACTION_FORWARD);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("In function:%s,line:%d invoke rtk_l2_limitLearningCnt_set fail!\n",__FUNCTION__, __LINE__);
            rt = CS_E_ERROR;
            goto END;
        }
        g_fdb_port_cfg[port_id-1].lrn_en = SDL_FDB_MAC_LEARN_ENABLE;
        
        /*when enable mac learn, mac limit should take effect*/
        rt = epon_request_onu_fdb_mac_limit_set(context, device, llidport, port_id, g_fdb_port_cfg[port_id-1].mac_limit);
        if(rt){
            SDL_MIN_LOG("In function:%s,line:%d invoke epon_request_onu_uc_l2_entry_clr fail!\n",__FUNCTION__, __LINE__);
            goto END;
        }
    }
    
END:
    return rt;
#endif
    return CS_E_NOT_SUPPORT;
    
}

cs_status epon_request_onu_mac_learn_get(
    CS_IN  cs_callback_context_t   context,
    CS_IN  cs_int32                device,
    CS_IN  cs_int32                llidport,
    CS_IN  cs_port_id_t            port_id,
    CS_OUT cs_sdl_fdb_mac_learn_t  *status
)
{
    if(NULL == status) {
        SDL_MIN_LOG("In %s, status is NULL pointer\n", __FUNCTION__);
        return CS_E_PARAM;
    }

    UNI_PORT_CHECK(port_id);
	#if 0
    *status = SDL_FDB_MAC_LEARN_ENABLE;
	#else
	//added by zhuxh
	*status = g_fdb_port_cfg[port_id-1].lrn_en;
	#endif
    return CS_E_OK;
}

static void __onu_fdb_entry_clr_uplink_port (
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_int32                device_id,
    CS_IN   cs_int32                llidport,
    CS_IN   cs_sdl_fdb_clr_mode_t   clr_mode
)
{
    rtk_l2_ucastAddr_t l2_data;
    rtk_uint32 address = 0;
    rtk_api_ret_t rtn  = RT_ERR_OK;

    /* Only flush dynamic entries on the Realtek service port */
    
    if(clr_mode == SDL_FDB_CLR_STATIC){
        SDL_MIN_LOG("Do not support service port static MAC adrress flush \n");
        return;
    }
    
    while(1){
        rtn = rtk_l2_addr_next_get(READMETHOD_NEXT_L2UCSPA, SWITCH_UPLINK_PORT, &address, &l2_data);   
        if (rtn != RT_ERR_OK){
            SDL_MIN_LOG("pord: %d, address: %d.\n", portid, address);
            break;
        }
        address++;

        if(1 != l2_data.is_static)
        {
            rtn = rtk_l2_addr_del(&l2_data.mac, &l2_data);
            
            if(rtn != RT_ERR_OK)
            {
                SDL_MIN_LOG("rtk_l2_addr_del return %d.\n", rtn);
            }
        }
    }
    
    return;    
}


cs_status epon_request_onu_fdb_entry_clr (
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_int32                device_id,
    CS_IN   cs_int32                llidport,
    CS_IN   cs_sdl_fdb_clr_mode_t   clr_mode
)
{
    cs_port_id_t portid;
    cs_status rt = CS_E_OK;

    /* Uni port MAC flush */
    for(portid=CS_UNI_PORT_ID1; portid<=UNI_PORT_MAX; portid++){
        rt =  epon_request_onu_fdb_entry_clr_per_port(context, device_id, llidport, clr_mode, portid);
        if(rt){
            SDL_MIN_LOG("epon_request_onu_fdb_entry_clr_per_port return %d\n", rt);
        }
    }

    /* Service port MAC flush */
    __onu_fdb_entry_clr_uplink_port(context, device_id, llidport, clr_mode);
    
    return rt;
    
}

cs_status epon_request_onu_fdb_entry_clr_per_port (
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_int32                device_id,
    CS_IN   cs_int32                llidport,
    CS_IN   cs_sdl_fdb_clr_mode_t   clr_mode,
    CS_IN   cs_port_id_t            portid
)
{
    rtk_l2_ucastAddr_t l2_data;
    rtk_uint32 address = 0;
    rtk_api_ret_t rtn  = RT_ERR_OK;
    cs_boolean rt = FALSE;
    cs_uint8 index;
         
    UNI_PORT_CHECK(portid);

    if(clr_mode > SDL_FDB_CLR_STATIC){
        SDL_MIN_LOG("invalid clr_mode (%d).\n", clr_mode);
        return CS_E_ERROR;
    }
    
    while(1){
        rtn = rtk_l2_addr_next_get(READMETHOD_NEXT_L2UCSPA, portid-1, &address, &l2_data);   
        if (rtn != RT_ERR_OK){
            SDL_MIN_LOG("pord: %d, address: %d.\n", portid, address);
            break;
        }
        address++;

        switch(clr_mode)
        {
            case SDL_FDB_CLR_DYNAMIC:
                 if(1 != l2_data.is_static)
                 {
                      rtn = rtk_l2_addr_del(&l2_data.mac, &l2_data);
                      if(rtn != RT_ERR_OK){
                          SDL_MIN_LOG("rtk_l2_addr_del return %d.\n", rtn);
                      }
                 }
                 break;

             case SDL_FDB_CLR_STATIC:
                if(1 == l2_data.is_static)
                {
                    /* do not delete the MAC address that is not in software static MAC list */
                    rt = fdb_static_entry_find(portid-1, (cs_mac_t*)&l2_data.mac.octet[0], &index);
                    if(rt){
                        /* Clear the corresponding static mac entry table */
                        memset(&__fdb_static_entry_table[portid-1].entry[index], 0, sizeof(cs_sdl_fdb_entry_t));
                        __fdb_static_entry_table[portid - 1].valid_map &= ~(((cs_uint64)0x1)<<index);
                        g_fdb_port_cfg[portid-1].static_mac_num--;
                        
                        rtn = rtk_l2_addr_del(&l2_data.mac, &l2_data);
                        
                        if(rtn != RT_ERR_OK){
                            SDL_MIN_LOG("rtk_l2_addr_del return %d.\n", rtn);
                        }
                    }
                }
                break;
                
                case SDL_FDB_CLR_BOTH:
                    if(1 == l2_data.is_static)
                    {
                        /* do not delete the MAC address that is not in software static MAC list */
                        rt = fdb_static_entry_find(portid -1, (cs_mac_t*)&l2_data.mac.octet[0], &index);
                        if(rt){
                           /* Clear the corresponding static mac entry table */
                            memset(&__fdb_static_entry_table[portid-1].entry[index], 0, sizeof(cs_sdl_fdb_entry_t));
                            __fdb_static_entry_table[portid - 1].valid_map &= ~(((cs_uint64)0x1)<<index);
                            g_fdb_port_cfg[portid-1].static_mac_num--;
                            
                            rtn = rtk_l2_addr_del(&l2_data.mac, &l2_data);
                    
                            if(rtn!=RT_ERR_OK){
                                SDL_MIN_LOG("rtk_l2_addr_del return %d.\n", rtn);
                            }
                        }
                    }else
                    {
                        rtn = rtk_l2_addr_del(&l2_data.mac, &l2_data);
                        if(rtn != RT_ERR_OK){
                            SDL_MIN_LOG("rtk_l2_addr_del return %d.\n", rtn);
                        }
                    }
                    break;
                    
                default:
                    /* will not come here logically */
                    break;  
          }
    }
    
    /*For security reason, double check the SW Static table*/
    if((SDL_FDB_CLR_STATIC == clr_mode) || (SDL_FDB_CLR_BOTH == clr_mode)){
    
        if(__fdb_static_entry_table[portid-1].valid_map)
        {
            memset(&__fdb_static_entry_table[portid-1].entry[0], 0, __FDB_STATIC_ENTRY_MAC_MAX*sizeof(cs_sdl_fdb_entry_t));
            __fdb_static_entry_table[portid-1].valid_map = 0;

            SDL_MIN_LOG("Software static MAC address table not synchronized with hardware\n");
        }

        /* Clear the static MAC address number */
        g_fdb_port_cfg[portid-1].static_mac_num = 0;

        /*After setting, re-config the mac limit,Dynamic entries will be cleared there */
        
        if(sdl_int_cfg.fdb_limit_include_static){
            rt = epon_request_onu_fdb_mac_limit_set(context, 
                                                    device_id,
                                                    llidport, 
                                                     portid, 
                                                 g_fdb_port_cfg[portid -1].mac_limit);
            if(rt){
                SDL_MIN_LOG("epon_request_onu_fdb_mac_limit_set return %d!\n", rt);
                return rt;
            }
        }
    }
    
    return CS_E_OK;    
}

cs_status epon_request_onu_fdb_entry_add(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_sdl_fdb_entry_t            *entry
)
{
    rtk_mac_t          mac;
    rtk_port_t         portid;
    rtk_l2_ucastAddr_t l2_data;
    cs_uint8           index;
    rtk_api_ret_t      rtk_ret = 0;
    cs_status          rt = CS_E_OK;

    if(NULL==entry){
        SDL_MIN_LOG("param is NULL pointer\n",);
        return CS_E_PARAM;
    }
    
    UNI_PORT_CHECK(entry->port);
    VID_CHECK(entry->vlan_id);
    
    if(entry->type != SDL_FDB_ENTRY_STATIC){
        SDL_MIN_LOG("Entry type is wrong!\n");
        return CS_E_NOT_SUPPORT;
    }
    
    /* Check if it is a valid Unicast mac addr */
    rt = __fdb_check_mac(&entry->mac);
    if(rt){
        SDL_MIN_LOG("In %s(%d) error MAC!\n", __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    /*Check whether it is already added*/
    portid = entry->port-1;
    rt = fdb_static_entry_check(&entry->mac);
    if(rt){
        SDL_MIN_LOG("ERROR: Static mac entry has already been added!\n");
        return CS_E_PARAM;
    }
    
    rt = fdb_static_entry_idx_get(portid, &index);
    if(rt){
        SDL_MIN_LOG("ERROR: SW Static entry table is full!\n");
        return CS_E_RESOURCE;
    }
    
    if(g_fdb_port_cfg[portid].static_mac_num >= sdl_int_cfg.max_static_mac_entry){
        SDL_MIN_LOG("ERROR: Static mac entry exceed! max num %d\n", sdl_int_cfg.max_static_mac_entry);
        return CS_E_PARAM;
    }
        
    /* if static MAC number is larger than MAC limit number, 
     * ONU just accept or keep the front static entries, 
     * number is equal to MAC limit number. 
     * In this case, there will be no dynamic entry available.
     */   
    if(sdl_int_cfg.fdb_limit_include_static && 
        (g_fdb_port_cfg[portid].static_mac_num >= g_fdb_port_cfg[portid].mac_limit)){
        SDL_MIN_LOG("WARNING: Static table exceeds mac limit %d now\n"
                    "Remaining entries will be ignored\n",
                    g_fdb_port_cfg[portid].mac_limit);
        return CS_E_PARAM;
    }
    
    memcpy(&mac, &entry->mac, sizeof(rtk_mac_t));
    
    memset(&l2_data, 0, sizeof(rtk_l2_ucastAddr_t));
    l2_data.is_static = entry->type;
    l2_data.cvid = entry->vlan_id;
    l2_data.port= entry->port - 1;
    l2_data.ivl = 0;
    l2_data.fid = 0;
    l2_data.efid = 0;
    
    rtk_ret = rtk_l2_addr_add(&mac, &l2_data);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_l2_addr_add return %d!\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }
    
    memcpy(&__fdb_static_entry_table[portid].entry[index], entry, sizeof(cs_sdl_fdb_entry_t));

    __fdb_static_entry_table[portid].valid_map |=  ((cs_uint64)1) << index;
    
    g_fdb_port_cfg[portid].static_mac_num = g_fdb_port_cfg[portid].static_mac_num + 1;
    
    /*After setting, re-config the mac limit,Dynamic entries will be cleared there */
    
    if(sdl_int_cfg.fdb_limit_include_static){
        rt = epon_request_onu_fdb_mac_limit_set(context, 
                                                device_id,
                                                llidport, 
                                                entry->port, 
                                                g_fdb_port_cfg[portid].mac_limit);
        if(rt){
            SDL_MIN_LOG("epon_request_onu_fdb_mac_limit_set return %d\n", rt);
            goto END;
        }
}
    
END:
    return rt;
}

cs_status epon_request_onu_fdb_entry_del(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_mac_t                      *mac,
    CS_IN  cs_uint16                     vlan
)
{
    rtk_mac_t          rtk_mac;
    rtk_l2_ucastAddr_t l2_data;
    cs_port_id_t       portid;
    cs_uint8           index;
    rtk_api_ret_t      rtk_ret = 0;
    cs_status          rt = CS_E_OK;

    if((NULL==mac)) {
        SDL_MIN_LOG("In %s(%d) error params!\n");
        return CS_E_PARAM;
    }
    
    VID_CHECK(vlan);

    /* Check if it is a valid Unicast mac addr */
    if(__fdb_check_mac(mac)){
        SDL_MIN_LOG("error MAC!\n");
        return CS_E_PARAM;
    }
    
    memcpy(&rtk_mac, mac, sizeof(rtk_mac_t));
    memset(&l2_data, 0, sizeof(rtk_l2_ucastAddr_t));
    l2_data.cvid = vlan;
    l2_data.fid = 0;
    l2_data.efid = 0;
    l2_data.ivl = 0;

    /*Firstly get the portid*/
    rtk_ret = rtk_l2_addr_get(&rtk_mac, &l2_data);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_l2_addr_get return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }
    portid = l2_data.port;
    
    if(!fdb_static_entry_find(portid, mac, &index)){
        SDL_MIN_LOG("ERROR: Static mac entry is not found!\n");
        return CS_E_OK;
    }
    
    rtk_ret = rtk_l2_addr_del(&rtk_mac, &l2_data);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_l2_addr_set return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }

    memset(&__fdb_static_entry_table[portid].entry[index], 0, sizeof(cs_sdl_fdb_entry_t));
    __fdb_static_entry_table[portid].valid_map &= ~(((cs_uint64)0x1)<<index);
    
    g_fdb_port_cfg[portid].static_mac_num = g_fdb_port_cfg[portid].static_mac_num - 1;
    
    /* after setting, re-config the mac limit Dynamic entries will be cleared there */
    if(sdl_int_cfg.fdb_limit_include_static){
        rt = epon_request_onu_fdb_mac_limit_set(context, 
            device_id, llidport, l2_data.port+1, g_fdb_port_cfg[portid].mac_limit);
        if(rt){
            SDL_MIN_LOG("In function:%s,line:%d invoke epon_request_onu_fdb_mac_limit_set fail!\n",__FUNCTION__, __LINE__);
            goto END;
        }
    }

END:
    return rt;
}

cs_status epon_request_onu_fdb_entry_get(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_mac_t                      *mac,                  
    CS_IN  cs_uint16                     vlan,
    CS_OUT cs_sdl_fdb_entry_t            *entry
)
{
    rtk_mac_t          rtk_mac;
    rtk_l2_ucastAddr_t l2_data;
    rtk_api_ret_t      rtk_ret = 0;
    cs_status          rt = CS_E_OK;

    if((NULL==mac) || (NULL==entry) || (vlan>4095)) {
        SDL_MIN_LOG("In %s(%d) error params!\n",__FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    /* Check if it is a valid Unicast mac addr */
    if(__fdb_check_mac(mac)){
        SDL_MIN_LOG("In %s(%d) error MAC!\n", __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    memcpy(&rtk_mac, mac, sizeof(rtk_mac_t));
    memset(&l2_data, 0, sizeof(rtk_l2_ucastAddr_t));
    l2_data.cvid = vlan;
    l2_data.fid = 0;
    l2_data.efid = 0;
    l2_data.ivl = 0;

    rtk_ret = rtk_l2_addr_get(&rtk_mac, &l2_data);
    if(RT_ERR_OK != rtk_ret){
       SDL_MIN_LOG("rtk_l2_addr_get return %d\n", rtk_ret);
       rt = CS_E_ERROR;
       goto END;
    }
    
    memcpy(&entry->mac, &l2_data.mac, sizeof(cs_mac_t));
    entry->vlan_id = l2_data.cvid;
    entry->port = P2L_PORT(l2_data.port);     
    entry->type = l2_data.is_static ? SDL_FDB_ENTRY_STATIC : SDL_FDB_ENTRY_DYNAMIC; 
    
 END:   
    return rt;
}

cs_status epon_request_onu_fdb_entry_getnext( 
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_sdl_fdb_entry_get_mode_t   mode,
    CS_IN  cs_uint16                     offset,
    CS_OUT cs_uint16                     *next,
    CS_OUT cs_sdl_fdb_entry_t            *entry
)
{
    rtk_l2_ucastAddr_t l2_data;
    rtk_uint32 address = offset;
    rtk_api_ret_t rtk_ret = RT_ERR_OK;

    if((NULL==next) || (NULL==entry)) {
        SDL_MIN_LOG("null pointer\n");
        return CS_E_PARAM;
    }
    
    if(mode > SDL_FDB_ENTRY_GET_MODE_ALL){
        SDL_MIN_LOG("Error Get mode.\n");
        return CS_E_PARAM;
    }

    while(1){
        rtk_ret = rtk_l2_addr_next_get(READMETHOD_NEXT_L2UC, 0, &address, &l2_data);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("rtk_l2_addr_next_get return %d.\n", rtk_ret);
            return CS_E_NOT_FOUND;
        }
        
        if(((SDL_FDB_ENTRY_GET_MODE_DYNAMIC == mode) && (1 == l2_data.is_static)) ||
            ((SDL_FDB_ENTRY_GET_MODE_STATIC == mode) && (0 == l2_data.is_static))){
            address++;
            continue;
        }
        
        memset(entry, 0, sizeof(cs_sdl_fdb_entry_t)); 
        memcpy(&entry->mac, &l2_data.mac, sizeof(cs_mac_t));  
        entry->vlan_id = l2_data.cvid;
        entry->port = P2L_PORT(l2_data.port);    
        entry->type = l2_data.is_static ? SDL_FDB_ENTRY_STATIC : SDL_FDB_ENTRY_DYNAMIC;
        
        break;
    }
    
    *next = address + 1;
    
    return CS_E_OK;
    
}


cs_status epon_request_onu_fdb_entry_getnext_per_port( 
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_port_id_t                  port_id,
    CS_IN  cs_sdl_fdb_entry_get_mode_t   mode,
    CS_IN  cs_uint16                     offset,
    CS_OUT cs_uint16                     *next,
    CS_OUT cs_sdl_fdb_entry_t            *entry
)
{
    rtk_l2_ucastAddr_t l2_data;
    rtk_uint32 address = offset;
    rtk_api_ret_t rtk_ret = RT_ERR_OK;
    
    if((NULL==next) || (NULL==entry)) {
        SDL_MIN_LOG("null pointer\n");
        return CS_E_PARAM;
    }
    
    if(mode>SDL_FDB_ENTRY_GET_MODE_ALL) {
        SDL_MIN_LOG("Mode is not supported\n");
        return CS_E_PARAM;
    }  
    
    if(offset>__FDB_ENTRY_HW_INDEX_MAX) {
        SDL_MIN_LOG("Offset is beyond max value\n");
        return CS_E_PARAM;
    }     
    
    UNI_PORT_CHECK(port_id);
    
    if(mode == SDL_FDB_ENTRY_GET_MODE_STATIC){
        for(address=offset; address<__FDB_STATIC_ENTRY_MAC_MAX; address++){
            if(!(__fdb_static_entry_table[port_id-1].valid_map & (((cs_uint64)1 )<<address)))
                continue;
                
            memset(entry, 0, sizeof(cs_sdl_fdb_entry_t)); 
            memcpy(&entry->mac, &__fdb_static_entry_table[port_id-1].entry[address].mac.addr[0], sizeof(cs_mac_t));
            entry->vlan_id = __fdb_static_entry_table[port_id-1].entry[address].vlan_id;
            entry->port = port_id;                                      
            entry->type = SDL_FDB_ENTRY_STATIC;
            
            *next = address + 1;
            
            return CS_E_OK;
        }  
        
        if(address == __FDB_STATIC_ENTRY_MAC_MAX){
            SDL_MIN_LOG("port id: %d, no static entry is found\n", port_id);
            
            *next = __FDB_STATIC_ENTRY_MAC_MAX;
            
            return CS_E_NOT_FOUND;
        }
        
    }
    
    while(1){
        rtk_ret = rtk_l2_addr_next_get(READMETHOD_NEXT_L2UCSPA, port_id-1, &address, &l2_data);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("rtk_l2_addr_next_get return %d\n", rtk_ret);
            return CS_E_NOT_FOUND;
        }
        
        if(((SDL_FDB_ENTRY_GET_MODE_DYNAMIC == mode) && (1 == l2_data.is_static)) ||
            ((SDL_FDB_ENTRY_GET_MODE_STATIC == mode) && (0 == l2_data.is_static))){
            address++;
            continue;
        }
            
        memset(entry, 0, sizeof(cs_sdl_fdb_entry_t)); 
        memcpy(&entry->mac, &l2_data.mac, sizeof(cs_mac_t));
        entry->vlan_id = l2_data.cvid;
        entry->port = port_id;                                      
        entry->type = l2_data.is_static ? SDL_FDB_ENTRY_STATIC : SDL_FDB_ENTRY_DYNAMIC;
        
        break;
    }
    
    *next = address + 1;
 
    return CS_E_OK;
}


/* added by wujm@2016-08-04,sa block */
cs_status epon_request_onu_fdb_block_entry_add(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device_id,
    CS_IN  cs_int32                  llidport,
    CS_IN  cs_sdl_fdb_entry_t *entry,
    CS_IN  sdl_mac_block_type_t      type)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_l2_ucastAddr_t l2_addr_entry;
    rtk_mac_t mac;
    int i = 0;
    cs_status          rt = CS_E_OK;

    memset(&l2_addr_entry, 0, sizeof(l2_addr_entry));


    if(NULL==entry){
        SDL_MIN_LOG("param is NULL pointer\n",);
        return CS_E_PARAM;
    }
    
    UNI_PORT_CHECK(entry->port);
    VID_CHECK(entry->vlan_id);
    
    if(entry->type != SDL_FDB_ENTRY_STATIC){
        SDL_MIN_LOG("Entry type is wrong!\n");
        return CS_E_NOT_SUPPORT;
    }
    
    /* Check if it is a valid Unicast mac addr */
    rt = __fdb_check_mac(&entry->mac);
    if(rt){
        SDL_MIN_LOG("In %s(%d) error MAC!\n", __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    for (i=0; i<6; i++)
    {
        mac.octet[i] = entry->mac.addr[i];
    }

    l2_addr_entry.fid  = 0;
    l2_addr_entry.efid = 0;
    /* added by wujm@2016-08-04,note :add entry to one port,but take effect on all ports */
    l2_addr_entry.port = 0;
    if(SA_BLOCK == type)
    {
        l2_addr_entry.sa_block  = 1;
    }
    else if(DA_BLOCK == type)
    {
        l2_addr_entry.da_block  = 1;
    }
    else if(SA_DA_BOTH_BLOCK == type)
    {
        l2_addr_entry.sa_block  = 1;
        l2_addr_entry.da_block  = 1;
    }
    
    l2_addr_entry.auth      = 0;
    l2_addr_entry.is_static = 1;
    
    memcpy(&l2_addr_entry.mac, &mac, sizeof(mac));
        
    rtn = rtk_l2_addr_add(&mac, &l2_addr_entry);
    if (rtn)
        rt = CS_E_ERROR;
    else
        rt = CS_E_OK;

    return rt;
}    

cs_status epon_request_onu_fdb_block_entry_del(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_mac_t                      *mac,
    CS_IN  cs_uint16                     vlan
)
{
    rtk_mac_t          rtk_mac;
    rtk_l2_ucastAddr_t l2_data;
    //cs_port_id_t       portid;
    //cs_uint8           index;
    rtk_api_ret_t      rtk_ret = 0;
    cs_status          rt = CS_E_OK;

    if((NULL==mac)) {
        SDL_MIN_LOG("In %s(%d) error params!\n");
        return CS_E_PARAM;
    }
    
    VID_CHECK(vlan);

    /* Check if it is a valid Unicast mac addr */
    if(__fdb_check_mac(mac)){
        SDL_MIN_LOG("error MAC!\n");
        return CS_E_PARAM;
    }
    
    memcpy(&rtk_mac, mac, sizeof(rtk_mac_t));
    memset(&l2_data, 0, sizeof(rtk_l2_ucastAddr_t));
    //l2_data.cvid = vlan;
    l2_data.fid = 0;
    l2_data.efid = 0;
    l2_data.ivl = 0;
#if 0
    /*Firstly get the portid*/
    rtk_ret = rtk_l2_addr_get(&rtk_mac, &l2_data);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_l2_addr_get return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }
    portid = l2_data.port;
    
    if(!fdb_static_entry_find(portid, mac, &index)){
        SDL_MIN_LOG("ERROR: Static mac entry is not found!\n");
        return CS_E_OK;
    }
#endif    
    rtk_ret = rtk_l2_addr_del(&rtk_mac, &l2_data);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_l2_addr_set return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }

#if 0
    memset(&__fdb_static_entry_table[portid].entry[index], 0, sizeof(cs_sdl_fdb_entry_t));
    __fdb_static_entry_table[portid].valid_map &= ~(((cs_uint64)0x1)<<index);
    
    g_fdb_port_cfg[portid].static_mac_num = g_fdb_port_cfg[portid].static_mac_num - 1;
    
    /* after setting, re-config the mac limit Dynamic entries will be cleared there */
    if(sdl_int_cfg.fdb_limit_include_static){
        rt = epon_request_onu_fdb_mac_limit_set(context, 
            device_id, llidport, l2_data.port+1, g_fdb_port_cfg[portid].mac_limit);
        if(rt){
            SDL_MIN_LOG("In function:%s,line:%d invoke epon_request_onu_fdb_mac_limit_set fail!\n",__FUNCTION__, __LINE__);
            goto END;
        }
    }
#endif

END:
    return rt;
}

cs_status epon_request_onu_fdb_block_entry_get(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_mac_t                      *mac,                  
    CS_IN  cs_uint16                     vlan,
    CS_OUT cs_sdl_fdb_entry_t            *entry,
    CS_OUT sdl_mac_block_type_t      *type
)
{
    rtk_mac_t          rtk_mac;
    rtk_l2_ucastAddr_t l2_data;
    rtk_api_ret_t      rtk_ret = 0;
    cs_status          rt = CS_E_OK;

    if((NULL==mac) || (NULL==entry) || (NULL==type) || (vlan>4095)) {
        SDL_MIN_LOG("In %s(%d) error params!\n",__FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    /* Check if it is a valid Unicast mac addr */
    if(__fdb_check_mac(mac)){
        SDL_MIN_LOG("In %s(%d) error MAC!\n", __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    memcpy(&rtk_mac, mac, sizeof(rtk_mac_t));
    memset(&l2_data, 0, sizeof(rtk_l2_ucastAddr_t));
    l2_data.cvid = vlan;
    l2_data.fid = 0;
    l2_data.efid = 0;
    l2_data.ivl = 0;

    rtk_ret = rtk_l2_addr_get(&rtk_mac, &l2_data);
    if(RT_ERR_OK != rtk_ret){
       SDL_MIN_LOG("rtk_l2_addr_get return %d\n", rtk_ret);
       rt = CS_E_ERROR;
       goto END;
    }
    
    if(l2_data.sa_block == 1)
    {
        *type |= SA_BLOCK;
    }

    if(l2_data.da_block == 1)
    {
        *type |= DA_BLOCK;
    }

    if(0 == *type )
    {
       SDL_MIN_LOG("this mac is not blocked\n");
       rt = CS_E_ERROR;
       goto END;
    }
    
    memcpy(&entry->mac, &l2_data.mac, sizeof(cs_mac_t));
    entry->vlan_id = l2_data.cvid;
    entry->port = P2L_PORT(l2_data.port);     
    entry->type = l2_data.is_static ? SDL_FDB_ENTRY_STATIC : SDL_FDB_ENTRY_DYNAMIC;   
    
 END:   
    return rt;
}

cs_status epon_request_onu_fdb_block_entry_getnext( 
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_uint16                     offset,
    CS_OUT cs_uint16                     *next,
    CS_OUT cs_sdl_fdb_entry_t            *entry,
    CS_OUT sdl_mac_block_type_t *type
)
{
    rtk_l2_ucastAddr_t l2_data;
    rtk_uint32 address = offset;
    rtk_api_ret_t rtk_ret = RT_ERR_OK;

    if((NULL==next) || (NULL==entry) || (NULL==type)) {
        SDL_MIN_LOG("null pointer\n");
        return CS_E_PARAM;
    }

    *type = 0;
    
    while(1){
        rtk_ret = rtk_l2_addr_next_get(READMETHOD_NEXT_L2UC, 0, &address, &l2_data);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("rtk_l2_addr_next_get return %d.\n", rtk_ret);
            return CS_E_NOT_FOUND;
        }

        if(l2_data.sa_block == 1)
        {
            *type |= SA_BLOCK;
        }

        if(l2_data.da_block == 1)
        {
            *type |= DA_BLOCK;
        }

        if(0 == *type )
        {
           address++;
            continue;
        }

        memset(entry, 0, sizeof(cs_sdl_fdb_entry_t)); 
        memcpy(&entry->mac, &l2_data.mac, sizeof(cs_mac_t));  
        entry->vlan_id = l2_data.cvid;
        entry->port = P2L_PORT(l2_data.port);    
        entry->type = l2_data.is_static ? SDL_FDB_ENTRY_STATIC : SDL_FDB_ENTRY_DYNAMIC;
        
        break;
    }
    
    *next = address + 1;
    
    return CS_E_OK;
    
}

#if 1
//added by zhuxh
cs_status epon_request_onu_fdb_entry_get_byindex( 
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_sdl_fdb_entry_get_mode_t   mode,
    CS_IN  cs_uint16                     offset,
    CS_OUT cs_sdl_fdb_entry_t            *entry,
    CS_OUT cs_uint16                     *next
)
{
    rtk_l2_ucastAddr_t l2_data;
    rtk_uint32 address = offset;
    rtk_api_ret_t rtk_ret = RT_ERR_OK;

    if((NULL==next) || (NULL==entry)) {
        SDL_MIN_LOG("null pointer\n");
        return CS_E_PARAM;
    }
    
    if(mode > SDL_FDB_ENTRY_GET_MODE_ALL){
        SDL_MIN_LOG("Error Get mode.\n");
        return CS_E_PARAM;
    }

    while(1){
        rtk_ret = rtk_l2_addr_next_get(READMETHOD_NEXT_L2UC, 0, &address, &l2_data);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("rtk_l2_addr_next_get return %d.\n", rtk_ret);
            return CS_E_NOT_FOUND;
        }
        
        if(((SDL_FDB_ENTRY_GET_MODE_DYNAMIC == mode) && (1 == l2_data.is_static)) ||
            ((SDL_FDB_ENTRY_GET_MODE_STATIC == mode) && (0 == l2_data.is_static))){
            address++;
            continue;
        }
        
        memset(entry, 0, sizeof(cs_sdl_fdb_entry_t)); 
        memcpy(&entry->mac, &l2_data.mac, sizeof(cs_mac_t));  
        entry->vlan_id = l2_data.cvid;
        entry->port = P2L_PORT(l2_data.port);    
        entry->type = l2_data.is_static ? SDL_FDB_ENTRY_STATIC : SDL_FDB_ENTRY_DYNAMIC;

		#if 1
		//add
		if(CS_UPLINK_PORT == entry->port)
		{
			entry->port = UNI_PORT_MAX+1;
		}
		#endif
        break;
    }
    
    *next = address + 1;
    
    return CS_E_OK;
    
}
#endif

#ifdef HAVE_USER_MAC_TRACE
cs_status epon_request_onu_fdb_port_get(
	CS_IN	cs_callback_context_t		context,
	CS_IN	cs_int32					device_id,
	CS_IN	cs_int32					llidport,
	CS_IN	cs_mac_t					*mac,
	CS_OUT	cs_port_id_t				*port
	)
{
	rtk_mac_t			rtk_mac;
	rtk_l2_ucastAddr_t	l2_data;
	rtk_api_ret_t		rtk_ret = 0;
	cs_status			rt = CS_E_OK;

	if((NULL==mac)) {
		SDL_MIN_LOG("In %s(%d) error params!\n",__FUNCTION__, __LINE__);
		return CS_E_PARAM;
	}

	/* Check if it is a valid Unicast mac addr */
	if(__fdb_check_mac(mac)){
		SDL_MIN_LOG("In %s(%d) error MAC!\n", __FUNCTION__, __LINE__);
		return CS_E_PARAM;
	}

	memcpy(&rtk_mac, mac, sizeof(rtk_mac_t));
	memset(&l2_data, 0, sizeof(rtk_l2_ucastAddr_t));
	l2_data.cvid = 0;
	l2_data.fid = 0;
	l2_data.efid = 0;
	l2_data.ivl = 0;

	rtk_ret = rtk_l2_addr_get(&rtk_mac, &l2_data);
	if(RT_ERR_OK != rtk_ret){
		SDL_MIN_LOG("rtk_l2_addr_get return %d\n", rtk_ret);
		rt = CS_E_ERROR;
		goto END;
	}

	*port = P2L_PORT(l2_data.port);

	END:
	return rt;
}

cs_status 
onu_mac_locate(
	CS_IN		cs_uint8 	*mac, 
	CS_OUT		cs_uint8	*onuport
	)
{
	//入口规则检查
	if(NULL == mac)
	{
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		cs_printf("(NULL == mac)\n");
		return CS_E_ERROR;
	}
	if(NULL == onuport)
	{
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		cs_printf("(NULL == onuport)\n");
		return CS_E_ERROR;
	}

	cs_status ret = CS_E_ERROR;
	cs_port_id_t port_id;
	cs_callback_context_t context;
	cs_mac_t mac_search;

	memcpy(mac_search.addr, mac, CS_MACADDR_LEN);
	if(CS_E_OK == epon_request_onu_fdb_port_get(context, 0, 0, &mac_search, &port_id))
	{
		//找到，返回成功
		*onuport = port_id;
		ret = CS_E_OK;
	}
	else
	{
		//未找到，返回失败
		ret = CS_E_ERROR;
	}

	return ret;
}

cs_status
locateUserMac(
	CS_IN	cs_uint8	*mac, 
	CS_OUT	cs_uint8	*onuslot, 
	CS_OUT	cs_uint8	*onuport, 
	CS_OUT	cs_uint8	*subsw, 
	CS_OUT	cs_uint8	*sw_mac,
	CS_OUT	cs_uint32	*sw_port
	)
{
	//入口规则检查
	if(NULL == mac)
	{
		cs_printf("error in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	if(NULL == onuslot)
	{
		cs_printf("error in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	if(NULL == onuport)
	{
		cs_printf("error in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	if(NULL == subsw)
	{
		cs_printf("error in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	if(NULL == sw_mac)
	{
		cs_printf("error in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	if(NULL == sw_port)
	{
		cs_printf("error in %s, line :%d\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	cs_status ret = CS_E_ERROR;
	ret = onu_mac_locate(mac, onuport);
	//对于非插卡式ONU，槽位号为0
	*onuslot = 0;

	//暂不支持onu 下挂交换机mac 追踪
	//为0时，没有下挂交换机，"交换机MAC地址"和"端口号"无意义，填充0
	*subsw = 0;
	memset(sw_mac, 0, CS_MACADDR_LEN);
	memset(sw_port, 0, sizeof(cs_uint32));

	return ret;
}
#endif	/*(#ifdef HAVE_USER_MAC_TRACE)*/

#ifdef HAVE_ONU_PORT_MAC_COUNT
typedef struct
{
	cs_port_id_t	port_id;
	cs_uint32		mac_num;
}port_mac_num_t;

static port_mac_num_t *g_port_mac_num = NULL;

cs_status mac_num_hw_to_sw_init(void)
{
	cs_status ret = CS_E_ERROR;
	int num;
	port_mac_num_t *p_port_mac_num = NULL;
	
	uni_port_num_get(&num);
	p_port_mac_num = malloc(num * sizeof(port_mac_num_t));
	if(NULL != p_port_mac_num)
	{
		int i;
		for(i=0; i<num; i++)
		{
			p_port_mac_num[i].port_id = i + CS_UNI_PORT_ID1;
			p_port_mac_num[i].mac_num = 0;
		}

		ret = CS_E_OK;
	}
	else
	{
		ret = CS_E_ERROR;
	}
	
	g_port_mac_num = p_port_mac_num;

	return ret;
}

cs_status mac_num_hw_to_sw_clear(void)
{
	cs_status ret = CS_E_ERROR;
	port_mac_num_t *p_port_mac_num = g_port_mac_num;

	if(NULL != p_port_mac_num)
	{
		int num = 0;
		int i = 0;
		uni_port_num_get(&num);
		for(i=0; i<num; i++)
		{
			p_port_mac_num[i].mac_num = 0;
		}

		ret = CS_E_OK;
	}
	else
	{
		ret = CS_E_ERROR;
	}

	return ret;
}

cs_status mac_num_sw_port_self_add(cs_port_id_t port_id)
{
	cs_status ret = CS_E_ERROR;
	port_mac_num_t *p_port_mac_num = g_port_mac_num;

	if(NULL != p_port_mac_num)
	{
		int num = 0;
		int i = 0;
		uni_port_num_get(&num);
		for(i=0; i<num; i++)
		{
			if(port_id == p_port_mac_num[i].port_id)
			{
				p_port_mac_num[i].mac_num++;
				ret = CS_E_OK;
				break;
			}
		}
	}
	else
	{
		ret = CS_E_ERROR;
	}

	return ret;
}

void mac_num_update_hw_to_sw(void)
{
	cs_callback_context_t context;
	cs_uint16 idx = 0;
	cs_uint16 next = 0;
	cs_sdl_fdb_entry_t entry;
	
	mac_num_hw_to_sw_clear();
	memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));
	while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
	{
		idx = next;
		mac_num_sw_port_self_add(entry.port);
	}
}

cs_status mac_num_sw_port_get(cs_port_id_t port_id, cs_uint32 *mac_num)
{
	if(NULL == mac_num)
	{
		return CS_E_ERROR;
	}

	cs_status ret = CS_E_ERROR;
	
	port_mac_num_t *p_port_mac_num = g_port_mac_num;

	if(NULL != p_port_mac_num)
	{
		int num = 0;
		int i = 0;
		uni_port_num_get(&num);
		for(i=0; i<num; i++)
		{
			if(port_id == p_port_mac_num[i].port_id)
			{
				*mac_num = p_port_mac_num[i].mac_num;
				ret = CS_E_OK;
				break;
			}
		}
	}
	else
	{
		ret = CS_E_ERROR;
	}
	
	return ret;
}
cs_status port_mac_count_get(cs_port_id_t port_id, cs_uint32 *mac_count)
{
	return mac_num_sw_port_get(port_id, mac_count);
}
#endif	/*(#ifdef HAVE_ONU_PORT_MAC_COUNT)*/

//REVIEW: get full-behavior from startup-cfg for all customer.
//default value: enable learning; shared-vlan mode; limit 64; aging 300sec.
cs_status sdl_fdb_init(
    CS_IN const sdl_init_cfg_t *cfg
)
{
    cs_callback_context_t context;
    cs_port_id_t          portid;
    cs_aal_fdb_port_cfg_t port_cfg;
    cs_aal_fdb_port_msk_t mask;
    
    memset(__fdb_static_entry_table, 0, UNI_PORT_MAX*sizeof(__fdb_static_entry_t));

    for(portid=CS_UNI_PORT_ID1; portid<=UNI_PORT_MAX; portid++){
    
#if 0
        g_fdb_port_cfg[portid-1].lrn_en = SDL_FDB_MAC_LEARN_DISABLE;
        rt = epon_request_onu_mac_learn_set(context, 0, 0, portid, SDL_FDB_MAC_LEARN_ENABLE);
        if(rt){
            SDL_MIN_LOG("epon_request_onu_mac_learn_set return %d. FILE: %s, LINE: %d\n", rt, __FILE__, __LINE__);
            goto END;
        }

   
        g_fdb_port_cfg[portid-1].lrn_mod = SDL_FDB_INDEPENDENT_VLAN_LEARNING;
        rt = epon_request_onu_fdb_learn_mode_set(context, 0, 0, portid, SDL_FDB_SHARED_VLAN_LEARNING);
        if(rt){
            SDL_MIN_LOG("epon_request_onu_fdb_learn_mode_set return %d. FILE: %s, LINE: %d\n", rt, __FILE__, __LINE__);
            goto END;
        }
#endif
                
        g_fdb_port_cfg[portid-1].mac_limit = 0;
        epon_request_onu_fdb_mac_limit_set(context, 0, 0, portid, __FDB_NO_MAC_LIMIT);
        
        g_fdb_port_cfg[portid-1].lrn_en = SDL_FDB_MAC_LEARN_DISABLE;
        g_fdb_port_cfg[portid-1].full_mod = 0;
        g_fdb_port_cfg[portid-1].lrn_mod = SDL_FDB_INDEPENDENT_VLAN_LEARNING;
        g_fdb_port_cfg[portid-1].static_mac_num = 0;
                
    }
    
    epon_request_onu_fdb_age_set(context, 0, 0, __FDB_AGING_TIME_DEFAULT);

    
    /*learn over action is drop at init. if want to forward, set limit num > max entry num*/
    rtk_l2_limitLearningCntAction_set(RTK_WHOLE_SYSTEM, LIMIT_LEARN_CNT_ACTION_DROP);

    
    /* workaround for bug 24604.
     * When set “l2_learn_lru_en” of reg “FE_TOP_MII0_PORT_CTRL_0” 
     * (even when MII0l2 learn is disabled), 
     * LRU works fine when mac limit is set to 64.
     */
    mask.u32 = 0;
    mask.s.learn_dis = 1;
    mask.s.learn_lmt_ovrwrt = 1;
    port_cfg.learn_dis = 1;
    port_cfg.learn_lmt_ovrwrt = 1;
    
    aal_fdb_port_cfg_set(AAL_PORT_ID_MII0, mask, &port_cfg);

    
    aal_fdb_port_cfg_set(AAL_PORT_ID_GE, mask, &port_cfg);

    
    /* workaround for BUG 29184,make 802.1x packet to be known MC packet*/
    __fdb_8021x_static_mac();
 
    
    return CS_E_OK;
}


