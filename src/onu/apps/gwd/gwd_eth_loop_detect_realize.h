#ifndef __GWD_ETH_LOOP_DETECT_REALIZE_H__
#define	__GWD_ETH_LOOP_DETECT_REALIZE_H__
#include "gwd_types.h"
#include "gwd_eth_loop_detect.h"

#ifdef	HAVE_LOOP_DETECT_ALARM_BASE_OLT
#define ONU_LOCAL_ID (1)

typedef struct
{
    gw_uint8 ucOltType;		/*0x61, 0x67, 0x69, 0x80*/
    gw_uint8 ucOltSlot;
    gw_uint8 ucOltPort;
    gw_uint8 ucOnuId;
}onu_local_info_t;

typedef struct
{
	gw_uint8	cell_id;
	gw_uint16	cell_len;
	gw_uint8	cell_data[1];
}__attribute__((packed))gw_cell_tlv_t;

typedef struct
{
	gw_uint8		type;
	gw_uint16		cell_flag;
	gw_cell_tlv_t	cell_tlv;
}__attribute__((packed))onu_infor_request_t;

gw_cell_tlv_t *gw_tlv_list_get_elem(gw_uint8 *tlv_list, gw_uint16 len, gw_uint8 id);
void olt_inform_onu_local_infor_enable_set(bool enable);
bool olt_inform_onu_local_infor_enable_get(void);
gw_status olt_inform_onu_local_infor_set(onu_local_info_t *onu_local_info);
gw_status olt_inform_onu_local_infor_get(onu_local_info_t *onu_local_info);
#endif


#endif	/*end of file*/
