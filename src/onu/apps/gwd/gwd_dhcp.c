#ifdef HAVE_GWD_DHCP_RELAY
/*
 * gw_access_identifier.c
 *
 *  Created on: Jan 5, 2015
 *      Author: kevin
 */
#include "iros_config.h"
#include "app_oam_gwd.h"
#include "gwd_dhcp.h"
#include <stdio.h>
#include "cs_cmn.h"
#include "packet.h"
#include "osal_api_core.h"
#include "startup_cfg.h"
#include "mempool.h"
#include "cs_module.h"
#include "sdl_vlan_util.h"
#include "app_vlan.h"
#include "sdl_vlan.h"
#include "app_oam_gwd.h"
#include "sdl.h"
#include "sdl_l2.h"
#ifdef HAVE_QINQ
#include "gwd_qinq.h"
#endif

enum{
	GW_LOG_LEVEL_DEBUG,
	GW_LOG_LEVEL_INFO,
	GW_LOG_LEVEL_MINOR,
	GW_LOG_LEVEL_MAJOR,
	GW_LOG_LEVEL_CRI,
};

static unsigned int DhcpProxyMode = 0;
unsigned int  DhcpRelaySem;
dhcp_option82_data_info_t option82info;

#define DHCP_PROXY_MODE_CHACK(mode) if(mode > DHCP_RELAY_GWD_MODE)
#define FUNC_RETURN_VALUE_CHECK(ret) if(ret != OK)
static unsigned int DhcpRelayRcpFlag = 0;

#ifndef HAVE_EVENT_SHOW_CMD
static unsigned int log_level = GW_LOG_LEVEL_CRI;
#endif

const unsigned int crc_table[256] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};

#define DO1(buf) crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);

#ifdef HAVE_EVENT_SHOW_CMD
extern cs_int32 getGwlogLevel();
#endif

#if 1
extern void pkt_print(char *buf, cs_uint16 len);
#endif
unsigned int gwd_crc32(unsigned int crc,  const unsigned char *buf, unsigned int len)
{
    crc = crc ^ 0xffffffffL;
    while (len >= 8)
    {
      DO8(buf);
      len -= 8;
    }
    if (len) do {
      DO1(buf);
    } while (--len);
    return crc ^ 0xffffffffL;
}

static unsigned int gw_log(unsigned int level, const unsigned char *String, ...)
{
	return 0;
}

#ifndef HAVE_EVENT_SHOW_CMD
unsigned int getGwlogLevel()
{
	return log_level;
}
#endif

unsigned int Gwd_func_switch_info_get(unsigned int ulport,unsigned char *swmac)
{
#if 0
	unsigned int ret =GW_ERROR;
	unsigned long slot,mgtPort;
	if(swmac == NULL)
	{
		gw_printf("%s %d is null error\r\n",__func__,__LINE__);
		return ret;
	}
	if(RCP_Dev_Is_Valid(ulport))
	{
		if(1 == RCP_Dev_Is_Exist(ulport))
		{
			rcpDevList[ulport]->frcpPort2LPort(rcpDevList[ulport], &slot, &mgtPort, 0, rcpDevList[ulport]->upLinkPort);
			gw_printf("%02x%02x.%02x%02x.%02x%02x",
				rcpDevList[ulport]->switchMac[0], rcpDevList[ulport]->switchMac[1], rcpDevList[ulport]->switchMac[2],
				rcpDevList[ulport]->switchMac[3], rcpDevList[ulport]->switchMac[4], rcpDevList[ulport]->switchMac[5]);
			memcpy(swmac,rcpDevList[ulport]->switchMac,RCP_MAC_SIZE);
		}
		gw_printf("rcpDevList[ulport]->upLinkPort[%d] mgtPort:[%lu]\r\n",rcpDevList[ulport]->upLinkPort,mgtPort);
	}
#endif
	return OK;
}

int Gwd_func_dhcp_relay_rcp_status_get(unsigned int *status)
{
	int ret = ERROR;
	if(status == NULL)
	{
		return ret;
	}
	*status = DhcpRelayRcpFlag;
	return OK;
}
int Gwd_func_dhcp_relay_rcp_status_set(unsigned int status)
{
	int ret =ERROR;
	if((status != 0) && (status !=1))
	{
		return ret;
	}
	DhcpRelayRcpFlag = status;
	return OK;
}
unsigned int Gwd_func_dhcp_local_option82_data_info_init()
{
	memset(&option82info,0,sizeof(dhcp_option82_data_info_t));
	return OK;
}
unsigned int Gwd_func_dhcp_local_option82_data_info_get(dhcp_option82_data_info_t *local_option82_data)
{
	unsigned int ret = ERROR;
    if(local_option82_data==NULL)
    {
    	cs_printf("%s %d is NULL \r\n",__func__,__LINE__);
    	return ret;
    }
//    gw_semaphore_wait(DhcpRelaySem, GW_OSAL_WAIT_FOREVER);
    memcpy(local_option82_data,&option82info,sizeof(dhcp_option82_data_info_t));
//    gw_semaphore_post(DhcpRelaySem);
	return OK;
}
unsigned int Gwd_func_dhcp_local_option82_data_info_set(dhcp_option82_data_info_t *local_option82_data)
{
	unsigned int ret = ERROR;
    if(local_option82_data==NULL)
    {
    	cs_printf("%s %d is NULL \r\n",__func__,__LINE__);
    	return ret;
    }
//    gw_semaphore_wait(DhcpRelaySem, GW_OSAL_WAIT_FOREVER);
    memcpy(&option82info,local_option82_data,sizeof(dhcp_option82_data_info_t));
//    gw_semaphore_post(DhcpRelaySem);
	return OK;
}
unsigned int Gwd_Func_Dhcp_Proxy_Mode_set(unsigned int mode)
{
	unsigned int ret = ERROR;
	DHCP_PROXY_MODE_CHACK(mode)
	{
		cs_printf("%s %d is error\r\n",__func__,__LINE__);
		return ret;
	}
//	gw_semaphore_wait(DhcpRelaySem, GW_OSAL_WAIT_FOREVER);
	DhcpProxyMode = mode;
//	gw_semaphore_post(DhcpRelaySem);
	return OK;
}
unsigned int Gwd_Func_Dhcp_Proxy_Mode_get(unsigned int *mode)
{
	unsigned int ret = ERROR;
	if(mode == NULL)
	{
		cs_printf("%s %d is NULL \r\n",__func__,__LINE__);
		return ret;
	}
//	gw_semaphore_wait(DhcpRelaySem, GW_OSAL_WAIT_FOREVER);
	*mode = DhcpProxyMode;
//	gw_semaphore_post(DhcpRelaySem);
	return OK;
}
unsigned int Gwd_Func_Dhcp_Proxy_Mode_Process(unsigned char*option82_data,unsigned int proxy_mode,unsigned int *option82len,int ulport,unsigned int vlan)
{
	unsigned int ret =ERROR;
	unsigned int i =0;
	unsigned int len = 0;
	unsigned int switchdhcpstatus = 0;
	unsigned char *ptr_front=NULL;
	unsigned char clv_code=0;
	unsigned char clv_len = 0;
	unsigned char  re_len = 0;
	unsigned char cumulative_len = 0;
	unsigned char dhcp_discover_pkt_flag = 0;
	unsigned char dhcpmessagetype = 0;
	unsigned int g_uni_port_num = 0;
	gw_macaddr_t g_sys_mac;
	unsigned char swichmac[RCP_MAC_SIZE] = {0};
	dhcp_option82_data_info_t local_option82_data;
	dhcp_option82_t *clv = NULL;
	dhcpOption82_ctc_str_t *option82_str = NULL;

	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d 0x%02x\r\n",__func__,__LINE__,*option82_data);
	if((option82_data == NULL) || (proxy_mode >=DHCP_RELAY_GWD_MAX))
	{
		cs_printf("%s %d is NULL\r\n",__func__,__LINE__);
		return ret;
	}
	do{
		ptr_front = (unsigned char*)option82_data;
		clv = (dhcp_option82_t*)option82_data;
		gw_log(GW_LOG_LEVEL_DEBUG,"%s %d clv->option_cod:0x%02x clv->option_len:0x%02x\r\n",__func__,__LINE__,clv->option_code,clv->option_len);
		clv_code = clv->option_code;
		clv_len = clv->option_len;
		cumulative_len +=clv->option_len;
		cumulative_len +=sizeof(dhcp_option82_t);
		option82_data +=sizeof(dhcp_option82_t);
		gw_log(GW_LOG_LEVEL_DEBUG,"%s %d clv_code:0x%02x clv_len:0x%02x\r\n",__func__,__LINE__,clv_code,clv_len);
		if(clv_code == DHCP_MESSAGE_TYPE)
		{
			dhcpmessagetype = *option82_data;
			if((DHCPDISCOVER == dhcpmessagetype)||(DHCPREQUEST == dhcpmessagetype)||
				(DHCPDECLINE == dhcpmessagetype)||(DHCPRELEASE == dhcpmessagetype)||
				(DHCPINFORM == dhcpmessagetype))
			{
				dhcp_discover_pkt_flag = DHCP_PKT_FLAG;
			}

			gw_log(GW_LOG_LEVEL_DEBUG,"%s %d dhcp_discover_pkt_flag:%d clv_len:%d\r\n",__func__,__LINE__,dhcp_discover_pkt_flag,clv_len);
		}
		else
		{
			if(clv_code == DHCP_OPTION82_TYPE)
			{
				dhcp_discover_pkt_flag = 0;
				gw_log(GW_LOG_LEVEL_DEBUG,"%s %d rcv dhcp pkt is have DHCP_OPTION82_TYPE\r\n",__func__,__LINE__);
			}
		}
		option82_data += clv_len;
	}while(clv_code != DHCP_OPTION_END);

	if(dhcp_discover_pkt_flag != DHCP_PKT_FLAG)
	{
		cs_printf("%s , line :%d is not dhcp pkt\r\n",__func__,__LINE__);
		goto FUNC_END;
	}

	memset(&local_option82_data,0,sizeof(dhcp_option82_data_info_t));
	ret = Gwd_func_dhcp_local_option82_data_info_get(&local_option82_data);
	FUNC_RETURN_VALUE_CHECK(ret)
	{
		cs_printf("%s %d return error\r\n",__func__,__LINE__);
		return ret;
	}
	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d ",__func__,__LINE__);
	option82_str = (dhcpOption82_ctc_str_t*)ptr_front;
	/*
	 * 0 0/0/0:0.0 OLT_MAC_ADDRESS/0/0/1/0/1/ONU_MAC_ADRESS 1/0/PORT_ON_ONU/SWITCH MAC/SWITCH_PORT:VLAN_ID EP
	 * */
	option82_str->option82.option_code=DHCP_OPTION82_TYPE;
	option82_str->cir_id.subOp = DHCP_SUB_OPTION82_1;

	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d re_len:%d",__func__,__LINE__,re_len);
	memcpy(&option82_str->cir_id.str_info[0],&local_option82_data.dhcprelayidentifierinfo[0],local_option82_data.datalen);
	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d dhcp_discover_pkt_flag:0x%02x datalen:%d\r\n",__func__,__LINE__,dhcp_discover_pkt_flag,local_option82_data.datalen);

	re_len += (local_option82_data.datalen);
	if(proxy_mode == DHCP_RELAY_GWD_MODE)
	{

	}
	else
	{
#if 0
		unsigned char mac[6]={0x00,0x0f,0xe9,0x01,0x02,0x03};
		len = sprintf(&option82_str->cir_id.str_info[re_len],"%02X%02X%02X%02X%02X%02X 1/0/1/O00000000000:1 EP",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],24);
		re_len +=len;
#else
//		ret = call_gwdonu_if_api(LIB_IF_SYSINFO_GET, 2, g_sys_mac, &g_uni_port_num);

	    ret = startup_config_read(CFG_ID_MAC_ADDRESS, CS_MACADDR_LEN, g_sys_mac);

//	    if (CS_E_OK != ret)
//	    {
//			return 1;
//	    }

	    g_uni_port_num = UNI_PORT_MAX;

		FUNC_RETURN_VALUE_CHECK(ret)
		{
			cs_printf("%s %d return error\r\n",__func__,__LINE__);
			return ret;
		}
		len = sprintf(&option82_str->cir_id.str_info[re_len],"%02X%02X%02X%02X%02X%02X ",g_sys_mac[0],g_sys_mac[1],g_sys_mac[2],g_sys_mac[3],g_sys_mac[4],g_sys_mac[5]);
		re_len +=len;
		len = sprintf(&option82_str->cir_id.str_info[re_len],"%d/%d/",ONU_SLOT_NUM,ONU_SUBSLOT_NUM);
		re_len +=len;
		len = sprintf(&option82_str->cir_id.str_info[re_len],"%d",ulport);
		re_len +=len;
		ret = Gwd_func_dhcp_relay_rcp_status_get(&switchdhcpstatus);
		FUNC_RETURN_VALUE_CHECK(ret)
		{
			cs_printf("%s %d return error\r\n",__func__,__LINE__);
			return ret;
		}
#if 0
		if(switchdhcpstatus)
#else
		if(0)
#endif
		{
			ret = Gwd_func_switch_info_get(ulport,swichmac);

			FUNC_RETURN_VALUE_CHECK(ret)
			{
				cs_printf("%s %d return error\r\n",__func__,__LINE__);
				return ret;
			}
//			printf("%02x%02x%02x%02x%02x%02x\r\n",swichmac[0],swichmac[1],swichmac[2],swichmac[3],swichmac[4],swichmac[5]);
			len = sprintf(&option82_str->cir_id.str_info[re_len],"/%02x%02x%02x%02x%02x%02x",swichmac[0],swichmac[1],swichmac[2],swichmac[3],swichmac[4],swichmac[5]);
			re_len +=len;
		}
//		len = sprintf(&option82_str->cir_id.str_info[re_len],"1:");
//		re_len +=len;
#if 0
		len = sprintf(&option82_str->cir_id.str_info[re_len],":%d EP",DEFAULT_SWITCH_VLAN);
#else
		len = sprintf(&option82_str->cir_id.str_info[re_len],":%d EP",vlan);
#endif
		re_len +=len;
#endif
	}
	option82_str->cir_id.len=re_len;
	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d option82_str->cir_id.len:%d",__func__,__LINE__,option82_str->cir_id.len);

    option82_str->option82.option_len=(option82_str->cir_id.len+SubOptionHeadLen);
    ptr_front += (option82_str->cir_id.len+Option82HeadLen+SubOptionHeadLen);
    cumulative_len +=(option82_str->cir_id.len+Option82HeadLen+SubOptionHeadLen);
    *ptr_front =EndOpion;
    ptr_front++;
    *ptr_front =Padding;
    ptr_front++;
    *ptr_front =Padding;
    ptr_front++;
	FUNC_END:
	*option82len =(cumulative_len+1);
	if(GW_LOG_LEVEL_DEBUG >= getGwlogLevel())
	{
		cs_printf("local dhcp info:\r\n");
		cs_printf("----------------------------------------------------------------------------------------------\r\n");
		for(i=0;i<re_len ;i++)
		{
			if(i%16 == 0)
				cs_printf("\r\n");
			cs_printf("0x%02x ",option82_data[i]);
		}
		cs_printf("\r\n");

		for(i=0;i< local_option82_data.datalen;i++)
		{
			if(i%16 == 0)
				cs_printf("\r\n");
			cs_printf("0x%02x ",local_option82_data.dhcprelayidentifierinfo[i]);
		}
		cs_printf("\r\n");
		cs_printf("----------------------------------------------------------------------------------------------\r\n");
	}
	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d sendlen:%d\r\n",__func__,__LINE__,*option82len);
	return OK;
}

int Gwd_dhcp_relay_enable(int mode)
{
	cs_callback_context_t     context;
	unsigned int ret = CS_OK;
	cs_sdl_pkt_state_t state = (mode==1)?DST_CPU:DST_FE;

	ret = epon_request_onu_spec_pkt_state_set(context, 0, 0, S_UPSTREAM,CS_PKT_DHCP,state);
//	ret += epon_request_onu_spec_pkt_state_set(context, 0, 0, CS_DOWN_STREAM, CS_PKT_DHCP, state);

	return ret;
}

unsigned int GwdDhcpRelayAdmnSet(unsigned char *data,unsigned int len,unsigned int mode)
{
	unsigned int ret =ERROR;
	unsigned char *ptr = NULL;
	unsigned int olddhcpmode=0;
	dhcp_option82_data_info_t gw_option82info;
	
	if((data == NULL) || (mode >= DHCP_RELAY_GWD_MAX))
	{
		cs_printf("%s %d is NULL\r\n",__func__,__LINE__);
		return ret;
	}

	ret = Gwd_func_dhcp_local_option82_data_info_init();
	FUNC_RETURN_VALUE_CHECK(ret)
	{
		cs_printf("%s %d return error\r\n",__func__,__LINE__);
		return ret;
	}

	ret=Gwd_Func_Dhcp_Proxy_Mode_get(&olddhcpmode);
	FUNC_RETURN_VALUE_CHECK(ret)
	{
		cs_printf("%s %d return error\r\n",__func__,__LINE__);
		return ret;
	}
	if((olddhcpmode != DHCP_RELAY_DISABLE) && (mode==DHCP_RELAY_DISABLE))
	{
	   if(Gwd_dhcp_relay_enable(0) != CS_OK)
		{
			cs_printf("set dhcp relay rule mode fail\n");
			return ret;
		}
	}else if((olddhcpmode == DHCP_RELAY_DISABLE) && (mode != DHCP_RELAY_DISABLE))
	{
		   if(Gwd_dhcp_relay_enable(1) != CS_OK)
			{
			   cs_printf("set dhcp relay rule mode fail\n");
				return ret;
			}
	}
	else
	{

	}

	ret = Gwd_Func_Dhcp_Proxy_Mode_set(mode);
	FUNC_RETURN_VALUE_CHECK(ret)
	{
		cs_printf("%s %d return error\r\n",__func__,__LINE__);
		return ret;
	}
	if(mode !=DHCP_RELAY_DISABLE)
	{
		memset(&gw_option82info,0,sizeof(dhcp_option82_data_info_t));
		ptr = (unsigned char*)gw_option82info.dhcprelayidentifierinfo;
		memcpy(ptr,data,len);
		gw_option82info.datalen=len;
		#if 0
		{
			unsigned int i =0;
			cs_printf("local dhcp admin %d:\r\n",len);
			cs_printf("----------------------------------------------------------------------------------------------\r\n");
			for(i=0;i<len ;i++)
			{
				if(i%16 == 0)
					cs_printf("\r\n");
				cs_printf("0x%02x ",data[i]);
			}
			cs_printf("\r\n");

			for(i=0;i< gw_option82info.datalen;i++)
			{
				if(i%16 == 0)
					cs_printf("\r\n");
				cs_printf("0x%02x ",ptr[i]);
			}
			cs_printf("\r\n");
			cs_printf("----------------------------------------------------------------------------------------------\r\n");
		}
		#endif
		ret = Gwd_func_dhcp_local_option82_data_info_set(&gw_option82info);
		FUNC_RETURN_VALUE_CHECK(ret)
		{
			cs_printf("%s %d return error\r\n",__func__,__LINE__);
			return ret;
		}
	}
	else
	{

	}
	return OK;
}
unsigned int Gwd_Func_Dhcp_relay_Oam_Admin_Process(unsigned char *pReq,unsigned int preq_len)
{
	int ret = OK;
	unsigned char proxymode = 0;
	access_identifier_admin_t AdminInfoHead;

	if(NULL == pReq)
	{
		cs_printf("%s %d is NULL\r\n",__func__,__LINE__);
		return ERROR;
	}
	memset(&AdminInfoHead,0,sizeof(access_identifier_admin_t));
	memcpy(&AdminInfoHead,pReq,sizeof(access_identifier_admin_t));
	pReq += sizeof(access_identifier_admin_t);
	preq_len -= sizeof(access_identifier_admin_t);
	proxymode = AdminInfoHead.proxymode;
	 gw_log(GW_LOG_LEVEL_DEBUG,"%s %d proxymode:%d\r\n",__func__,__LINE__,proxymode);
	switch(proxymode)
	{
		case DHCP_RELAY_DISABLE:
		case DHCP_RELAY_GWD_MODE:
		case DHCP_RELAY_CTC_MODE:
			 gw_log(GW_LOG_LEVEL_DEBUG,"%s %d preq_len:%d proxymode:%d\r\n",__func__,__LINE__,preq_len,proxymode);
			ret = GwdDhcpRelayAdmnSet(pReq,preq_len,proxymode);
			FUNC_RETURN_VALUE_CHECK(ret)
			{
				cs_printf("%s %d return error\r\n",__func__,__LINE__);
				return ret;
			}
			break;
		default:
			ret = ERROR;
			break;
	}
	return ret;
}
cs_status Gwd_func_dhcp_pkt_parser(cs_pkt_t *pPkt)
{
  int ret = CS_E_NOT_SUPPORT;
  unsigned int dhcpheadlen=0;
  unsigned short ethertype = 0;
  unsigned short dhcp_server_port = 0;
  unsigned short dhcp_client_port = 0;
  unsigned char ipverlen = 0;
  unsigned char ipver = 0;
  int vlan_add_tag = 0;
#if 1
  unsigned char *dhcp_pkt = NULL;
  unsigned int dhcp_len = 0;
  dhcp_pkt = (unsigned char *)(pPkt->data + pPkt->offset);
  dhcp_len = pPkt->len;
#endif

	//报文如果不是ip 报文则返回错误
	if(1 != pkt_ip_check(dhcp_pkt))
	{
		return ret;
	}
	//报文是否有vlan tag
	if(0 == pkt_vlan_tag_check(dhcp_pkt, dhcp_len))
	{
		cs_uint16 pvid = 0;
		cs_callback_context_t context;
		cs_port_id_t port_id;

		port_id = pPkt->port;
		epon_onu_sw_get_port_pvid(context, 0, 0, port_id, &pvid);

		vlan_add_tag = 1;
		push_vlan(pvid, dhcp_pkt, dhcp_pkt, &dhcp_len);
		pPkt->len = dhcp_len;
	}

  gwd_dhcp_pkt_info_head_t* dhcphead = NULL;

  dhcpheadlen = sizeof(gwd_dhcp_pkt_info_head_t);

  if((dhcp_pkt == NULL)|| (dhcp_len < dhcpheadlen))
  {
	  gw_log(GW_LOG_LEVEL_DEBUG,"%s %d  %d %d is NULL \r\n",__func__,__LINE__,dhcp_len,dhcpheadlen);
	  goto END;
  }

  dhcphead = (gwd_dhcp_pkt_info_head_t*)dhcp_pkt;
  ipverlen = dhcphead->iphead.VerHeadLen;
  gw_log(GW_LOG_LEVEL_DEBUG,"%s %d ipverlen:(0x%02x 0x%x) \r\n",__func__,__LINE__,ipverlen,dhcphead->iphead.VerHeadLen);
  ipver = ((ipverlen >> 4)&(0x0f));
  gw_log(GW_LOG_LEVEL_DEBUG,"%s %d ipverlen:(0x%02x 0x%x) \r\n",__func__,__LINE__,ipver,ipver);
  if(ipver == IpV6Version)
  {
	  gw_log(GW_LOG_LEVEL_DEBUG,"%s %d ipverlen:(0x%02x 0x%x) is ipv6 no proc\r\n",__func__,__LINE__,ipver,ipver);
	  goto END;
  }


  ethertype = ntohs(dhcphead->ethhead.ethtype);
  dhcp_server_port = ntohs(dhcphead->udphead.destUdpPort);
  dhcp_client_port = ntohs(dhcphead->udphead.srcUdpPort);

  if((ethertype != ETH_TYPE_IP) || (dhcp_server_port != DhcpSvrPortNum) || (dhcp_client_port != DhcpCliPortNum))
  {
	  gw_log(GW_LOG_LEVEL_DEBUG,"%s %d ethertype:%0x04x dhcp_server_port:%d dhcp_client_port:%d\r\n",__func__,__LINE__,ethertype,dhcp_server_port,dhcp_client_port);
	  goto END;
  }

  pPkt->pkt_type = CS_PKT_DHCP;
  ret = CS_E_OK;
  
END:
	//将不是dhcp 的报文vlan 恢复
	if(CS_PKT_DHCP != pPkt->pkt_type)
	{
		if(1 == vlan_add_tag)
		{
			pop_vlan(dhcp_pkt, dhcp_pkt, &dhcp_len);
			pPkt->len = dhcp_len;
		}
	}
	return ret;
}
#if 0
static unsigned int crc_table[256];
static void init_crc_table(void)
{
    unsigned int c;
    unsigned int i, j;

    for (i = 0; i < 256; i++) {
        c = (unsigned int)i;
        for (j = 0; j < 8; j++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[i] = c;
    }
}

static unsigned int crc32(unsigned int crc,unsigned char *buffer, unsigned int size)
{
    unsigned int i;
    for (i = 0; i < size; i++) {
        crc = crc_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
    }
    return crc ;
}
#endif
extern unsigned int gwd_crc32(unsigned int crc,  const unsigned char *buf, unsigned int len);
unsigned short Gwd_func_checksum_get(unsigned short *buf,unsigned int len)
{
	unsigned int ret = ERROR;
	unsigned long cksum = 0;
	unsigned int nword = 0;
	unsigned short result=0;
	if((buf == NULL) || (len < 1))
	{
		gw_log(GW_LOG_LEVEL_DEBUG,"%s %d buflen:%d  is null error\r\n",__func__,__LINE__,nword);
		return ret;
	}
	nword = ((len+1)/2);
	for(cksum = 0;nword > 0;nword--)
	{
		cksum += *buf++;
	}
    cksum = (cksum >> 16) + (cksum & 0xffff);

    cksum += (cksum >>16);
    gw_log(GW_LOG_LEVEL_DEBUG,"%s %d cksum:0x%04x\r\n",__func__,__LINE__,cksum);
    result=(unsigned short)(~cksum);
    return result;
}
unsigned int Gwd_func_ip_header_checksum_process(unsigned char *response_dhcp_pkt,unsigned int responselen)
{
	unsigned int ret = ERROR;
	unsigned int nword = 0;
	unsigned short result=0;
	eth_iphead_info_t *ipheader=NULL;
	gwd_dhcp_pkt_info_head_t *response_head = NULL;

	if((response_dhcp_pkt == NULL) || (responselen < DHCP_PKT_DEF_LEN))
	{
		gw_log(GW_LOG_LEVEL_DEBUG,"%s %d responselen:%d  is null error\r\n",__func__,__LINE__,responselen);
		return ret;
	}
	response_head = (gwd_dhcp_pkt_info_head_t*)response_dhcp_pkt;
	ipheader=(eth_iphead_info_t*)&response_head->iphead;
	/*把IP头的校验和清空*/
	ipheader->Checksum = 0;
	/*IP TOT_LENTH*/
	/*responselen = ETHLEN+IPLEN+UDPLEN+PAYLOAD+FCS*/
	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d ipheader->Totlength:%d  \r\n",__func__,__LINE__,ipheader->Totlength);
#if 0
	ipheader->Totlength = (responselen-EtherHeadLen-ETHFCSLEN);
//	ipheader->Checksum = Gwd_func_checksum_get((unsigned short*)ipheader,IPHEADERLEN);
#else
	ipheader->Totlength =htons(responselen-EtherHeadLen);
#endif
	result=Gwd_func_checksum_get((unsigned short*)ipheader,IPHEADERLEN);

	#if 0
	ipheader->Checksum = htons(result);
	#else
	//change by zhuxh
	/*计算校验和的数据是强制转换类型而来，
	计算的结果也应该直接转换成报文数据
	而不是转为网络字节序*/
	ipheader->Checksum = result;
	#endif
	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d ipheader->Totlength:0x%04x ipheader->Checksum:0x%04x nword:%d\r\n",
			__func__,__LINE__,ipheader->Totlength,ipheader->Checksum,nword);
	return OK;
}
#if 1
unsigned short gwd_udp_cksum(struct tsd_header_s* pudph, char*buf, int n)
  {
    unsigned long sum = 0;
    unsigned short *tmp = NULL;
    unsigned short result;
    register int i = 0;
    unsigned char pad[2];

    tmp = (unsigned short *) pudph;
    for (i = 0; i < 6; i++) {
      sum += *tmp++;
    }

    tmp = (unsigned short *) buf;
    while (n > 1) {
      sum += *tmp++;
      n -= 2;
    }

    if ( n == 1) {      /* n % 2 == 1, have to do padding */
      pad[0] = (unsigned char)*tmp;
      pad[1] = 0;
      tmp = (unsigned short *) pad;
      sum += *tmp;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    result = (unsigned short) ~sum;
    if (result == 0)
      result = 0xffff;

    return(result);
}
unsigned int Gwd_func_udp_header_checksum_process(unsigned char *response_dhcp_pkt,unsigned int responselen)
{
	struct tsd_header_s tsdheader;
	gwd_dhcp_pkt_info_head_t *response_head = NULL;
	udp_head_info_t *udpheader=NULL;
	eth_iphead_info_t *ipheader=NULL;
	unsigned short len;
	unsigned char* buff;
	unsigned short result;
	response_head = (gwd_dhcp_pkt_info_head_t*)response_dhcp_pkt;
	ipheader = (eth_iphead_info_t*)&response_head->iphead;
	udpheader=(udp_head_info_t*)&response_head->udphead;

	udpheader->length = htons(responselen-EtherHeadLen-IPHEADERLEN);
	buff = (unsigned char*)malloc(ntohs(udpheader->length)+1);
	if (buff==NULL)
	 {
	   return 0;
	 }
	memset(buff, 0,(ntohs(udpheader->length)+1));

	memset(&tsdheader,0,sizeof(tsd_header_t));
	tsdheader.destip = ipheader->DestIP;
	tsdheader.sourceip = ipheader->SourceIP;
	tsdheader.mzero = 0;
	tsdheader.ptcl = 17;
	tsdheader.udplen = udpheader->length;
	udpheader->checkSum=0;
	memcpy((unsigned char*)buff,(response_dhcp_pkt+EtherHeadLen+IPHEADERLEN), ntohs(udpheader->length));
	  len = ntohs(udpheader->length);
	  if (len %2 ==1)
	  	len++;
    result = (gwd_udp_cksum(&tsdheader, (char *) buff, /*ntohs*/(len)));

	#if 0
    udpheader->checkSum=htons(result);
	#else
	//change by zhuxh
	/*计算校验和的数据是强制转换类型而来，
	计算的结果也应该直接转换成报文数据
	而不是转为网络字节序*/
	udpheader->checkSum=result;
	#endif
    free(buff);
    return OK;
}
#else
unsigned int Gwd_func_udp_header_checksum_process(unsigned char *response_dhcp_pkt,unsigned int responselen)
{
	unsigned int ret = ERROR;
	gwd_dhcp_pkt_info_head_t *response_head = NULL;
	udp_head_info_t *udpheader=NULL;
	eth_iphead_info_t *ipheader=NULL;
	unsigned char *udpchecksumbuf = NULL;
	unsigned char *ptr = NULL;
	unsigned char *phead=NULL;
	unsigned char *payload = NULL;
	unsigned int payloadlen = 0;
	unsigned int checknumber =0;
	unsigned int nword =0;
	tsd_header_t tsdheader;
	if((response_dhcp_pkt == NULL) || (responselen < DHCP_PKT_DEF_LEN))
	{
		gw_log(GW_LOG_LEVEL_DEBUG,"%s %d responselen:%d  is null error\r\n",__func__,__LINE__,responselen);
		return ret;
	}
	udpchecksumbuf =(unsigned char*)iros_malloc(IROS_MID_APP,responselen);
	if(udpchecksumbuf == NULL)
	{
		gw_log(GW_LOG_LEVEL_DEBUG,"%s %d  is null error\r\n",__func__,__LINE__);
		return ret;
	}
	ptr = udpchecksumbuf;
	phead = response_dhcp_pkt;
	response_head = (gwd_dhcp_pkt_info_head_t*)response_dhcp_pkt;
	ipheader = (eth_iphead_info_t*)&response_head->iphead;
	udpheader=(udp_head_info_t*)&response_head->udphead;
	payload = (phead+EtherHeadLen+IPHEADERLEN+UDPHEADERLEN);
	/*清空UDP 校验和*/
	udpheader->checkSum = 0;
	/*UDP LENGTH*/
	udpheader->length = (responselen-EtherHeadLen-IPHEADERLEN-ETHFCSLEN);
    /*UDP 校验伪头部*/
	memset(&tsdheader,0,sizeof(tsd_header_t));
	tsdheader.destip = htonl(ipheader->DestIP);
	tsdheader.sourceip = htonl(ipheader->SourceIP);
	tsdheader.mzero = 0;
	tsdheader.ptcl = ipheader->Protocol;
	tsdheader.udplen = htons(udpheader->length);
	memcpy(ptr,&tsdheader,sizeof(tsd_header_t));
	ptr +=sizeof(tsd_header_t);
	checknumber +=sizeof(tsd_header_t);
	/*UDP头长度*/
	memcpy(ptr,&udpheader,sizeof(udp_head_info_t));
	ptr +=sizeof(udp_head_info_t);
	checknumber +=sizeof(udp_head_info_t);
	/*数据长度*/
	payloadlen = (responselen-EtherHeadLen-IPHEADERLEN-UDPHEADERLEN-ETHFCSLEN);
	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d payloadlen:%d\r\n",__func__,__LINE__,payloadlen);
	memcpy(ptr,payload,payloadlen);
	ptr +=payloadlen;
	checknumber +=payloadlen;
	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d [ checknumber:%d]\r\n",__func__,__LINE__,checknumber);

	udpheader->checkSum = Gwd_func_checksum_get((unsigned short*)udpchecksumbuf,checknumber);
	iros_free(udpchecksumbuf);
	gw_log(GW_LOG_LEVEL_DEBUG,"%s %d udpheader->checkSum:0x%04x nword:%d\r\n",__func__,__LINE__,udpheader->checkSum,nword);
	return OK;
}
#endif
unsigned int Gwd_func_eth_pkt_checksum_process(unsigned char *response_dhcp_pkt,unsigned int responselen)
{
	unsigned int ret = ERROR;
//	unsigned int ethfcs = 0;
//	gwd_dhcp_pkt_info_head_t *response_head = NULL;
	if((response_dhcp_pkt == NULL) || (responselen < DHCP_PKT_DEF_LEN))
	{
		gw_log(GW_LOG_LEVEL_DEBUG,"%s %d responselen:%d  is null error\r\n",__func__,__LINE__,responselen);
		return ret;
	}
	ret = Gwd_func_ip_header_checksum_process(response_dhcp_pkt,responselen);
    FUNC_RETURN_VALUE_CHECK(ret)
    {
	  cs_printf("%s %d return error\r\n",__func__,__LINE__);
	  return ret;
    }
	ret = Gwd_func_udp_header_checksum_process(response_dhcp_pkt,responselen);
    FUNC_RETURN_VALUE_CHECK(ret)
    {
	  cs_printf("%s %d return error\r\n",__func__,__LINE__);
	  return ret;
    }
#if 0
	response_head = (gwd_dhcp_pkt_info_head_t*)response_dhcp_pkt;
    ethfcs =gwd_crc32(ethfcs,response_dhcp_pkt,(responselen-ETHFCSLEN));
    gw_log(GW_LOG_LEVEL_DEBUG,"%s %d pktlen:0x%08x  len:%d\r\n",__func__,__LINE__,ethfcs,responselen);
	memcpy(&response_dhcp_pkt[responselen-ETHFCSLEN],&ethfcs,ETHFCSLEN);
#endif
	return OK;
}

unsigned char Gwd_func_dhcp_pkt_handler(cs_pkt_t *pPkt)
{
  unsigned int ret = ERROR;
  unsigned int dhcpheadlen=0;
  unsigned char *option82 = NULL;
  unsigned char *dhcphead =NULL;
  unsigned int option82_len = 0;
  unsigned int dhcp_proxy_mode = 0;
  unsigned int response_len=0;
  unsigned char *response_dhcp_pkt = NULL;
  unsigned int vlan=0;
  unsigned char *ptr=NULL;
  gwd_dhcp_pkt_info_head_t *headstruct=NULL;

#if 1
  unsigned char *dhcp_pkt = NULL;
  unsigned int dhcp_len = 0;
  int ulport = 0;
  dhcp_pkt = (unsigned char *)(pPkt->data + pPkt->offset);
  dhcp_len = pPkt->len;
  ulport = pPkt->port;
#endif

	#ifdef HAVE_QINQ
	pkt_qinq_vlan_translate(pPkt);
	#endif
	
#if 0
//add by zhuxh
//报文如果不是ip 报文则返回错误
	if(1 != pkt_ip_check(dhcp_pkt))
	{
		return ret;
	}
	//报文是否有vlan tag
	if(0 == pkt_vlan_tag_check(dhcp_pkt, dhcp_len))
	{
		push_vlan(1, dhcp_pkt, dhcp_pkt, &dhcp_len);
		pPkt->len = dhcp_len;
	}
#endif

  dhcpheadlen = sizeof(gwd_dhcp_pkt_info_head_t);
  gw_log(GW_LOG_LEVEL_DEBUG,"%s %d dhcp rec:dhcp_len:%d ulport:%d\r\n",__func__,__LINE__,dhcp_len,ulport);
  if((dhcp_pkt == NULL)|| (dhcp_len < dhcpheadlen) || (dhcp_len > DHCP_MALLOC_RESPONSE_LEN))
  {
	  cs_printf("%s %d  %d %d is NULL \r\n",__func__,__LINE__,dhcp_len,dhcpheadlen);
	  goto end;
  }
  int response_dhcp_len = dhcp_len+sizeof(dhcpOption82_ctc_str_t);

  response_dhcp_pkt = (unsigned char*)iros_malloc(IROS_MID_APP,response_dhcp_len);

  if(response_dhcp_pkt == NULL)
  {
	  cs_printf("%s %d  %d %d is NULL \r\n",__func__,__LINE__,dhcp_len,dhcpheadlen);
	  goto end;
  }
  memset(response_dhcp_pkt,0,response_dhcp_len);
  memcpy(response_dhcp_pkt,dhcp_pkt,dhcp_len);
  dhcphead = response_dhcp_pkt;
  headstruct=(gwd_dhcp_pkt_info_head_t*)response_dhcp_pkt;
  vlan=(unsigned int)(ntohs(headstruct->ethhead.cvlan)&0xfff);
  option82 = (unsigned char*)(dhcphead+dhcpheadlen);

  ret = Gwd_Func_Dhcp_Proxy_Mode_get(&dhcp_proxy_mode);
  FUNC_RETURN_VALUE_CHECK(ret)
  {
	cs_printf("%s %d dhcp_proxy_mode:%d return error\r\n",__func__,__LINE__,dhcp_proxy_mode);
	goto end;
  }
  gw_log(GW_LOG_LEVEL_DEBUG,"%s %d dhcp_proxy_mode:%d\r\n",__func__,__LINE__,dhcp_proxy_mode);
	switch(dhcp_proxy_mode)
	{
		case DHCP_RELAY_DISABLE:
			gw_log(GW_LOG_LEVEL_DEBUG,"%s %d dhcp_proxy_mode=DHCP_RELAY_DISABLE\r\n",__func__,__LINE__);
			break;
		case DHCP_RELAY_GWD_MODE:
		case DHCP_RELAY_CTC_MODE:
			ret = Gwd_Func_Dhcp_Proxy_Mode_Process(option82,dhcp_proxy_mode,&option82_len,ulport,vlan);
			FUNC_RETURN_VALUE_CHECK(ret)
			{
				cs_printf("%s %d return error\r\n",__func__,__LINE__);
				goto end;
			}
			gw_log(GW_LOG_LEVEL_DEBUG,"%s %d sendlen:%d %d\r\n",__func__,__LINE__,option82_len,dhcp_len);
#if 0
			response_len = (dhcpheadlen+option82_len+ETHFCSLEN);
#else
			response_len = (dhcpheadlen+option82_len);
#endif
			ret = Gwd_func_eth_pkt_checksum_process(response_dhcp_pkt,response_len);
			FUNC_RETURN_VALUE_CHECK(ret)
			{
				cs_printf("%s %d return error\r\n",__func__,__LINE__);
				goto end;
			}
			if((ntohs(headstruct->ethhead.flag==0x8100)) && ((ntohs(headstruct->ethhead.cvlan)&0xfff) == 1))
			{
				//pop vlan
				ptr=response_dhcp_pkt+16;
			    memcpy((response_dhcp_pkt+12),ptr,(response_len-16));
			    response_len-=4;
			}
#if 0
			ret = call_gwdonu_if_api(LIB_IF_PORTSEND, 3,GW_PON_PORT_ID, response_dhcp_pkt,response_len);
#else
			ret = app_pkt_tx(CS_PON_PORT_ID, CS_PKT_DHCP, response_dhcp_pkt, response_len);
#endif
			FUNC_RETURN_VALUE_CHECK(ret)
			{
				cs_printf("%s %d return error\r\n",__func__,__LINE__);
				goto end;
			}
			break;
		default:
			ret = ERROR;
			break;
	}

end:
  iros_free(response_dhcp_pkt);
  response_dhcp_pkt = NULL;
  return 1;
}

extern int Gwd_func_dhcp_pkt_process_init()
{
	int ret = ERROR;
//	init_crc_table();
//	gw_semaphore_init(&DhcpRelaySem, "DhcpRelaySem", 1, 0);

	ret = app_pkt_reg_parser(CS_PKT_DHCP, Gwd_func_dhcp_pkt_parser);
	FUNC_RETURN_VALUE_CHECK(ret)
	{
		cs_printf("%s %d return error\r\n",__func__,__LINE__);
		return ret;
	}
	ret = app_pkt_reg_handler(CS_PKT_DHCP, Gwd_func_dhcp_pkt_handler);
	FUNC_RETURN_VALUE_CHECK(ret)
	{
		cs_printf("%s %d return error\r\n",__func__,__LINE__);
		return ret;
	}

	return OK;
}


#if 0
int cmd_onu_rcp_dhcp_relay_set(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int ret = ERROR;
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return gw_cli_arg_help(cli, 0,
                "<enable/disable>", "enable poe,disable poe\n",
                 NULL);
        default:
            return gw_cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(argc == 1)
    {
    	if(strcmp(argv[0],"enable") == 0)
    	{
    		ret = Gwd_func_dhcp_relay_rcp_status_set(1);
    		FUNC_RETURN_VALUE_CHECK(ret)
    		{
    			gw_cli_print(cli,"%s %d return error\r\n",__func__,__LINE__);
    			return ret;
    		}
    	}else if(strcmp(argv[0],"disable") == 0)
    	{
    		ret = Gwd_func_dhcp_relay_rcp_status_set(0);
    		FUNC_RETURN_VALUE_CHECK(ret)
    		{
    			gw_cli_print(cli,"%s %d return error\r\n",__func__,__LINE__);
    			return ret;
    		}
    	}
    	else
    	{
    		gw_cli_print(cli,"%%input error\r\n");
    		return ret;
    	}
    }
    gw_cli_print(cli,"dhcp_relay rcp status %s\r\n",DhcpRelayRcpFlag? "enable":"disable");
    return OK;
}
void cli_reg_gwd_dhcp_relay_cmd(struct cli_command **cmd_root)
{
    struct cli_command *reg;

    reg = gw_cli_register_command(cmd_root, NULL, "dhcp_relay", NULL, PRIVILEGE_UNPRIVILEGED, MODE_CONFIG, "DHCP relay switch info admin");
    gw_cli_register_command(cmd_root, reg, "rcp", cmd_onu_rcp_dhcp_relay_set, PRIVILEGE_UNPRIVILEGED, MODE_CONFIG, "set switch info status");
    return;
}
#endif

#endif	/*end of the file*/

