#ifdef HAVE_GWD_FLASH_OPER
#include "gwd_flash_oper.h"
#include "plat_common.h"
#include "iros_flash_oper.h"

/*add by zhaoxh 2016-8-11*/
#ifdef HAVE_STATIC_FDB_ADDR_RESTORE 
extern int static_fdb_tlv_infor_get(int *len, char **value, int *free_need);
extern int static_fdb_tlv_infor_handle(int len, char *data, int opcode);
extern int static_fdb_running_config_show(void);
#endif

/*add by zhaoxh 2016-8-11*/
#ifdef HAVE_AGING_FDB_ADDR_RESTORE 
extern int aging_fdb_tlv_infor_get(int * len,char * * value,int * free_need);
extern int aging_fdb_tlv_infor_handle(int len,char * data,int opcode);
extern int aging_fdb_running_config_show(void);
#endif

/*add by zhaoxh 2016-8-17*/
#ifdef HAVE_LEARNING_FDB_ADDR_RESTORE 
extern int learning_fdb_tlv_infor_get(int * len,char * * value,int * free_need);
extern int learning_fdb_tlv_infor_handle(int len,char * data,int opcode);
extern int learning_fdb_running_config_show(void);
#endif

#ifdef HAVE_IGMP_MODE_RESTORE
extern int igmp_mode_tlv_infor_get(int *len, char **value, int *free_need);
extern int igmp_mode_tlv_infor_handle(int len, char *data, int opcode);
extern int igmp_mode_running_config_show(void);
#endif

#ifdef HAVE_QINQ_CFG_RESTORE
extern int vlan_qinq_table_tlv_infor_get(int *len, char **value, int *free_need);
extern int vlan_qinq_table_tlv_infor_handle(int len, char *data, int opcode);
extern int vlan_qinq_running_config_show(void);
#endif

#ifdef HAVE_PORT_ISOLATION_RESTORE
extern int port_isolate_mode_tlv_infor_get(int *len, char **value, int *free_need);
extern int port_isolate_mode_tlv_infor_handle(int len, char *data, int opcode);
extern int port_isolate_mode_running_config_show(void);
#endif

#ifdef HAVE_GWD_BC_THRESHOLD_RESTORE
extern int broadcast_storm_threshold_tlv_infor_get(int *len, char **value, int *free_need);
extern int broadcast_storm_threshold_tlv_infor_handle(int len, char *data, int opcode);
extern int broadcast_storm_threshold_running_config_show(void);
#endif

#ifdef HAVE_GWD_PORT_MODE_RESTORE
extern int gwd_port_mode_tlv_info_get(int *len, char **value, int *free_need);
extern int gwd_port_mode_tlv_info_handle(int length,char *value,int opcode);
extern int gwd_port_mode_running_config_show(void);
#endif

#if (HAVE_GE_RATE_LIMIIT_RESTORE == MODULE_YES)
extern int uni_egress_rate_tlv_infor_get(int *len, char **value, int *free_need);
extern int uni_egress_rate_tlv_infor_handle(int len, char *data, int opcode);
extern int uni_egress_rate_running_config_show(void);
extern int uni_ingress_rate_tlv_infor_get(int *len, char **value, int *free_need);
extern int uni_ingress_rate_tlv_infor_handle(int len, char *data, int opcode);
extern int uni_ingress_rate_running_config_show(void);
#endif

#ifdef HAVE_ETH0_IP_ADDR_RESTORE
extern int eth0_ip_tlv_infor_get(int *len, char **value, int *free_need);
extern int eth0_ip_tlv_infor_handle(int len, char *data, int opcode);
extern int eth0_ip_running_config_show(void);
#endif

#ifdef HAVE_ETH1_IP_ADDR_RESTORE
extern int eth1_ip_tlv_infor_get(int *len, char **value, int *free_need);
extern int eth1_ip_tlv_infor_handle(int len, char *data, int opcode);
extern int eth1_ip_running_config_show(void);
#endif

#define TLV_BUF_LEN		(432)

extern int save_user_tlv_data_to_flash(void)
{
	int ret = 0;
	const uint32 tlv_area_volume = GWD_TLV_LEN;
	const uint32 user_data_tlv_offset = GWD_TLV_OFFSET;
	tlv_area_head_t tlv_area_head;
	uint32 tlv_area_saved_len = 0;
	char *buf = NULL;
	uint32 buf_len = 0;
	uint32 buf_valid_len = 0;
	uint32 type = 0;
	uint32 len = 0;
	char *value = NULL;
	int free_need = 0;
	char *p = NULL;
	uint32 single_tlv_len = 0;
	
	buf_len = TLV_BUF_LEN;
	buf_valid_len = 0;
	buf = (char *)iros_malloc(IROS_MID_SYS, buf_len);
	memset(buf, 0, buf_len);

	tlv_area_head.head_len = sizeof(tlv_area_head);
	for(type=TYPE_START;type<TYPE_NUM;type++)		//遍历每个tlv
	{
		switch(type)
		{
			#ifdef HAVE_GWD_BC_THRESHOLD_RESTORE
			case BROADCAST_STORM_THRESHOLD:
				broadcast_storm_threshold_tlv_infor_get(&len, &value, &free_need);
				break;
			#endif
			
			#ifdef HAVE_IGMP_MODE_RESTORE
			case IGMP_MODE:
				igmp_mode_tlv_infor_get(&len, &value, &free_need);
				break;
			#endif

			#if (HAVE_GE_RATE_LIMIIT_RESTORE == MODULE_YES)
			case UNI_EGRESS:
				uni_egress_rate_tlv_infor_get(&len, &value, &free_need);
				break;
			case UNI_INGRESS:
				uni_ingress_rate_tlv_infor_get(&len, &value, &free_need);
				break;
			#endif
			
			#ifdef HAVE_PORT_ISOLATION_RESTORE
			case PORT_ISOLATE_MODE:
				port_isolate_mode_tlv_infor_get(&len, &value, &free_need);
				break;
			#endif
			
			#ifdef HAVE_QINQ_CFG_RESTORE
			case VLAN_QINQ:
				vlan_qinq_table_tlv_infor_get(&len, &value, &free_need);
				break;
			#endif
			
			#ifdef HAVE_GWD_PORT_MODE_RESTORE
			case PORT_MODE:
				gwd_port_mode_tlv_info_get(&len, &value, &free_need);
				break;
			#endif

			#ifdef HAVE_ETH0_IP_ADDR_RESTORE
			case ETH0_IP:
				eth0_ip_tlv_infor_get(&len, &value, &free_need);
				break;
			#endif

			#ifdef HAVE_ETH1_IP_ADDR_RESTORE
			case ETH1_IP:
				eth1_ip_tlv_infor_get(&len, &value, &free_need);
				break;
			#endif
			/*add by zhaoxh@2016-8-11*/
			#ifdef HAVE_STATIC_FDB_ADDR_RESTORE
			case STATIC_FDB: 
                static_fdb_tlv_infor_get(&len,&value,&free_need);
				break;
			#endif
			/*add by zhaoxh@2016-8-11*/
			#ifdef HAVE_AGING_FDB_ADDR_RESTORE
				case AGING_FDB: 
                aging_fdb_tlv_infor_get(&len,&value,&free_need);
				break;
			#endif
			/* add by zhaoxh@2016-8-17*/
			#ifdef HAVE_LEARNING_FDB_ADDR_RESTORE
				case LEARNING_FDB: 
                learning_fdb_tlv_infor_get(&len,&value,&free_need);
				break;
			#endif
			default:
				len = 0;
				value = NULL;
				free_need =0;
				break;
		}
		
		if(0 != len)
		{
			//do nothing
		}
		else
		{
			continue;
		}
		single_tlv_len = sizeof(type) + sizeof(len) + len;
		if(single_tlv_len > buf_len)
		{
			cs_printf("error: in %s, line :%d, single_tlv_len > buf_len\n", __func__, __LINE__);
		}
		if(tlv_area_volume >= tlv_area_head.head_len + tlv_area_saved_len + single_tlv_len)
		{
			//do nothing
		}
		else
		{
			//tishi
			cs_printf("type :%d and the rest type is not saved to flash, tlv area is full!",type);
			break;
		}
		
		if(buf_len - buf_valid_len >= single_tlv_len)
		{
			//do nothing
		}
		else
		{
			ret = save_userdata_to_flash(buf, user_data_tlv_offset + tlv_area_head.head_len + tlv_area_saved_len, buf_valid_len);	//保存tlv 数据区 
			if(0 == ret)
			{
				//do nothing
			}
			else
			{
				iros_free(buf);
				buf = NULL;
				iros_free(value);
				value = NULL;
				goto end;
			}
			tlv_area_saved_len = tlv_area_saved_len + buf_valid_len;
			memset(buf, 0, buf_len);
			buf_valid_len = 0;
		}
		
		if(buf_len - buf_valid_len >= single_tlv_len)
		{
			p = buf + buf_valid_len;
			memcpy(p, (char *)&type, sizeof(type));
			p = p + sizeof(type);
			memcpy(p, (char *)&len, sizeof(len));
			p = p + sizeof(len);
			memcpy(p, (char *)value, len);
			p = p + len;
			buf_valid_len = buf_valid_len + single_tlv_len;
			
			if(free_need != 0)
			{
				iros_free(value);
				value = NULL;
			}
			else
			{
				//do nothing
			}
		}
		else
		{
			if(free_need != 0)
			{
				iros_free(value);
				value = NULL;
			}
			else
			{
				//do nothing
			}
			//tishi
			break;
		}
		
		
	}

	ret = save_userdata_to_flash(buf, user_data_tlv_offset + tlv_area_head.head_len + tlv_area_saved_len, buf_valid_len);		//保存tlv 数据区
	if(0 == ret)
	{
		//do nothing
	}
	else
	{
		iros_free(buf);
		buf = NULL;
		goto end;
	}
	tlv_area_saved_len = tlv_area_saved_len + buf_valid_len;
	memset(buf, 0, buf_len);
	buf_valid_len = 0;

	tlv_area_head.tlv_area_len = tlv_area_saved_len;
	memcpy(buf, &tlv_area_head, sizeof(tlv_area_head_t));
	buf_valid_len = sizeof(tlv_area_head_t);
	ret = save_userdata_to_flash(buf, user_data_tlv_offset, buf_valid_len);	//保存tlv 区 头	
	if(0 == ret)
	{
		//do nothing
	}
	else
	{
		iros_free(buf);
		buf = NULL;
		goto end;
	}
	iros_free(buf);
	buf = NULL;
	
end:	
	return ret;
}

extern int get_user_tlv_data_from_flash(gwd_flash_oper_t opcode)
{
	int ret = 0;
	const uint32 user_data_tlv_offset = GWD_TLV_OFFSET;
	tlv_area_head_t tlv_area_head;
	uint32 tlv_area_len = 0;
	char *buf = NULL;
	uint32 buf_len = 0;
	uint32 buf_valid_len = 0;
	uint32 buf_parsed_len = 0;
	uint32 type = 0;
	uint32 len = 0;
	char *value = NULL;
	uint32 tlv_parsed_len = 0;
	uint32 single_tlv_len = 0;
	tlv_parsed_len = 0;
	
	ret = get_userdata_from_flash((char *)&tlv_area_head, user_data_tlv_offset, sizeof(tlv_area_head_t));		//获得tlv 头
	if(0 == ret)
	{
		//do nothing
	}
	else
	{
		goto end;
	}
	tlv_area_len = tlv_area_head.tlv_area_len;
	buf_len = TLV_BUF_LEN;
	if(0 == buf_len)
	{
		goto end;
	}
	else
	{
		
	}
	buf = (char *)iros_malloc(IROS_MID_SYS,buf_len);
	if(NULL != buf)
	{
		//do nothing
	}
	else
	{
		cs_printf("in %s, malloc error!\n", __func__);
		goto end;
	}
	buf_parsed_len = 0;
	memset(buf, 0, buf_len);
	if(buf_len <= tlv_area_len)
	{
		buf_valid_len = buf_len;
	}
	else
	{
		buf_valid_len = tlv_area_len;
	}
	ret = get_userdata_from_flash(buf, user_data_tlv_offset + tlv_area_head.head_len + tlv_parsed_len, buf_valid_len);		//获得tlv 数据区
	if(0 == ret)
	{
		//do nothing
	}
	else
	{
		iros_free(buf);
		goto end;
	}
	char *p = NULL;
	while(tlv_parsed_len < tlv_area_len)
	{
		p = buf + buf_parsed_len;
		if(buf_valid_len - buf_parsed_len >= sizeof(type) + sizeof(len))
		{
			memcpy((char *)&type, p, sizeof(type));
			p = p + sizeof(type);
			memcpy((char *)&len, p, sizeof(len));
			p = p + sizeof(len);
			if(buf_valid_len - buf_parsed_len >= sizeof(type) + sizeof(len) + len)
			{
				value = p;
				switch(type)
				{
					#ifdef HAVE_GWD_BC_THRESHOLD_RESTORE
					case BROADCAST_STORM_THRESHOLD:
						broadcast_storm_threshold_tlv_infor_handle(len, value, opcode);
						break;
					#endif
					
					#ifdef HAVE_IGMP_MODE_RESTORE
					case IGMP_MODE:
						igmp_mode_tlv_infor_handle(len, value, opcode);
						break;
					#endif

					#if (HAVE_GE_RATE_LIMIIT_RESTORE == MODULE_YES)
					case UNI_EGRESS:
						uni_egress_rate_tlv_infor_handle(len, value, opcode);
						break;
					case UNI_INGRESS:
						uni_ingress_rate_tlv_infor_handle(len, value, opcode);
						break;
					#endif
					
					#ifdef HAVE_PORT_ISOLATION_RESTORE
					case PORT_ISOLATE_MODE:
						port_isolate_mode_tlv_infor_handle(len, value, opcode);
						break;
					#endif
					
					#ifdef HAVE_QINQ_CFG_RESTORE
					case VLAN_QINQ:
						vlan_qinq_table_tlv_infor_handle(len, value, opcode);
						break;
					#endif
					
					#ifdef HAVE_GWD_PORT_MODE_RESTORE
					case PORT_MODE:
						gwd_port_mode_tlv_info_handle(len, value, opcode);
						break;
					#endif

					#ifdef HAVE_ETH0_IP_ADDR_RESTORE
					case ETH0_IP:
						eth0_ip_tlv_infor_handle(len, value, opcode);
						break;
					#endif

					#ifdef HAVE_ETH1_IP_ADDR_RESTORE
					case ETH1_IP:
						eth1_ip_tlv_infor_handle(len, value, opcode);
						break;
					#endif
					/*add by zhaoxh 2016-8-11*/
					#ifdef HAVE_STATIC_FDB_ADDR_RESTORE
					case STATIC_FDB:
						static_fdb_tlv_infor_handle(len,value,opcode);
						break;
					#endif
					/*add by zhaoxh 2016-8-11*/
					#ifdef HAVE_AGING_FDB_ADDR_RESTORE
				    case AGING_FDB: 
                        aging_fdb_tlv_infor_handle(len,value,opcode);
				        break;
			        #endif
					/* add by zhaoxh@2016-8-17*/
        			#ifdef HAVE_LEARNING_FDB_ADDR_RESTORE
        				case LEARNING_FDB: 
                        learning_fdb_tlv_infor_handle(len,value,opcode);
        				break;
        			#endif
					default:
						cs_printf("in %s, line :%d, unknown type :%d\n", __func__, __LINE__,type);
						break;
				}
				p = p + len;
				single_tlv_len = sizeof(type) + sizeof(len) + len;
				buf_parsed_len = buf_parsed_len + single_tlv_len;
				tlv_parsed_len = tlv_parsed_len + single_tlv_len;
			}
			else
			{
				if(tlv_area_len - tlv_parsed_len >= sizeof(type) + sizeof(len) + len)
				{
					if(buf_len >= sizeof(type) + sizeof(len) + len)
					{
						buf_parsed_len = 0;
						memset(buf, 0, buf_len);
						ret = get_userdata_from_flash(buf, user_data_tlv_offset + tlv_area_head.head_len + tlv_parsed_len, buf_len);		//获得tlv 数据区
						if(0 == ret)
						{
							//do nothing
						}
						else
						{
							iros_free(buf);
							goto end;
						}
					}
					else
					{
						break;
					}
					
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			buf_parsed_len = 0;
			memset(buf, 0, buf_len);
			ret = get_userdata_from_flash(buf, user_data_tlv_offset + tlv_area_head.head_len + tlv_parsed_len, buf_len);		//获得tlv 数据区
			if(0 == ret)
			{
				//do nothing
			}
			else
			{
				iros_free(buf);
				goto end;
			}
		
		}
	}
	
end:
	return ret;
}

extern int start_up_config_erase(void)
{
	int ret = 0;
	const uint32 user_data_tlv_offset = GWD_TLV_OFFSET;
	tlv_area_head_t tlv_area_head;
	tlv_area_head.head_len = sizeof(tlv_area_head_t);
	tlv_area_head.tlv_area_len = 0;

	ret = save_userdata_to_flash((char *)&tlv_area_head, user_data_tlv_offset, sizeof(tlv_area_head));	//保存tlv 区 头
	
	return ret;
}

extern int running_config_show(void)
{
	int type = 0;
	for(type=TYPE_START; type<TYPE_NUM;type++)
	{
		switch(type)
		{
			#ifdef HAVE_GWD_BC_THRESHOLD_RESTORE
			case BROADCAST_STORM_THRESHOLD:
				broadcast_storm_threshold_running_config_show();
				break;
			#endif
			
			#ifdef HAVE_IGMP_MODE_RESTORE
			case IGMP_MODE:
				igmp_mode_running_config_show();
				break;
			#endif

			#if (HAVE_GE_RATE_LIMIIT_RESTORE == MODULE_YES)
			case UNI_EGRESS:
				uni_egress_rate_running_config_show();
				break;
			case UNI_INGRESS:
				uni_ingress_rate_running_config_show();
				break;
			#endif
			
			#ifdef HAVE_PORT_ISOLATION_RESTORE
			case PORT_ISOLATE_MODE:
				port_isolate_mode_running_config_show();
				break;
			#endif
			
			#ifdef HAVE_QINQ_CFG_RESTORE
			case VLAN_QINQ:
				vlan_qinq_running_config_show();
				break;
			#endif
			
			#ifdef HAVE_GWD_PORT_MODE_RESTORE
			case PORT_MODE:
				gwd_port_mode_running_config_show();
				break;
			#endif

			#ifdef HAVE_ETH0_IP_ADDR_RESTORE
			case ETH0_IP:
				eth0_ip_running_config_show();
				break;
			#endif

			#ifdef HAVE_ETH1_IP_ADDR_RESTORE
			case ETH1_IP:
				eth1_ip_running_config_show();
				break;
			#endif
			/*add by zhaoxh 2016-8-11*/
			#ifdef HAVE_STATIC_FDB_ADDR_RESTORE
			case STATIC_FDB: 
                 static_fdb_running_config_show();
				break;
			   #endif
			/*add by zhaoxh 2016-8-11*/
			#ifdef HAVE_AGING_FDB_ADDR_RESTORE
			case AGING_FDB: 
                aging_fdb_running_config_show();
				break;
			    #endif
			/*add by zhaoxh 2016-8-17*/
			#ifdef HAVE_LEARNING_FDB_ADDR_RESTORE
			case LEARNING_FDB: 
                learning_fdb_running_config_show();
				break;
			    #endif
			default:
				break;
		}
	}
	
	return 0;
}

extern int start_up_config_syn_to_running_config(void)
{
	int ret = 0;

	get_user_tlv_data_from_flash(DATA_RECOVER);
	cs_printf("recover start-up config...\n");

	return ret;
}

#endif /*end of the file*/
