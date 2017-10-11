#ifndef __GWD_FLASH_OPER_H__
#define __GWD_FLASH_OPER_H__

typedef unsigned int uint32;

enum data_type
{
	TYPE_START = 1,
#ifdef HAVE_GWD_BC_THRESHOLD_RESTORE
	BROADCAST_STORM_THRESHOLD = 2,
#endif
#ifdef HAVE_IGMP_MODE_RESTORE
	IGMP_MODE = 3,
#endif
#if (HAVE_GE_RATE_LIMIIT_RESTORE == MODULE_YES)
	UNI_EGRESS = 4,
	UNI_INGRESS = 5,
#endif
#ifdef HAVE_PORT_ISOLATION_RESTORE
	PORT_ISOLATE_MODE = 6,
#endif
#ifdef HAVE_QINQ_CFG_RESTORE
	VLAN_QINQ = 7,
#endif
#ifdef HAVE_GWD_PORT_MODE_RESTORE
	PORT_MODE = 10,
#endif
#ifdef HAVE_ETH0_IP_ADDR_RESTORE
		ETH0_IP = 11,
#endif
#ifdef HAVE_ETH1_IP_ADDR_RESTORE
		ETH1_IP = 12,
#endif
#ifdef HAVE_STATIC_FDB_ADDR_RESTORE
			STATIC_FDB= 13,
#endif
	
#ifdef HAVE_AGING_FDB_ADDR_RESTORE
			AGING_FDB= 14,
#endif

#ifdef HAVE_AGING_FDB_ADDR_RESTORE
			LEARNING_FDB= 15,
#endif
	TYPE_NUM
};

typedef struct
{
	uint32 head_len;
	uint32 tlv_area_len;
}tlv_area_head_t;

typedef enum
{
	DATA_RECOVER = 0,
	DATA_SHOW = 1
}gwd_flash_oper_t;
extern int save_user_tlv_data_to_flash(void);
extern int get_user_tlv_data_from_flash(gwd_flash_oper_t opcode);
extern int start_up_config_erase(void);
extern int running_config_show(void);
extern int start_up_config_syn_to_running_config(void);

#endif	/*end of the file*/

