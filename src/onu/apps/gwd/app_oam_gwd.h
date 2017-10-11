#ifndef __APP_OAM_GWD_H__
#define __APP_OAM_GWD_H__

#ifdef HAVE_GWD_OAM
#include "plat_common.h"
#include "timer.h"
#include "oam_sdl_if.h"
#if 1
//gwd code
#define GWD_OAM_CAP_CTC_STATISTIC 		(0x80>>0)
#ifdef HAVE_GWD_OAM_FAST_STATISTIC
#define GWD_OAM_CAP_CTC_FAST_STATISTIC	(0x80>>2)
#define GWD_OAM_CAP_PON_STATISTIC		(0x80>>5)
#endif
#ifdef HAVE_ONU_PORT_MAC_COUNT
#define GWD_OAM_CAP_MAC_COUNT_STATISTIC	(0x80>>4)
#endif

/*for define*/
#define OK			(0)
#define ERROR		(-1)

/* This struct should be moved to a header file */
typedef struct standard_oam_header
{
	cs_uint8 dmac[6];				/* Destination MAC: 01-80-c2-00-00-02 */
	cs_uint8 smac[6];				/* Source MAC */
	cs_uint16 type;					/* 88 09 */
	cs_uint8 subtype;				/* 0x03 - OAM */
	cs_uint16 flags;
	cs_uint8 code;					/* 0xFE Vender Externsion */
} __attribute__ ((packed)) OAM_HEADER;

typedef struct gwtt_oam_header
{
	struct standard_oam_header std_hdr;
	cs_uint8 oui[3];				/* GWTT's OUI is 00-0f-e9 */
	cs_uint8 opCode;				/* Operation Code */
	cs_ulong32 senderSerNo;		/* Sender's serial no */
	cs_uint16 wholePktLen;		/* The whole packet length, including the fragments */
	cs_uint16 payloadOffset;		/* Offset in the entire packet */
	cs_uint16 payLoadLength;		/* Payload length in this packet */
	cs_uint8 sessionId[8];		/* sesion id used by EMS */
} __attribute__ ((packed)) GWTT_OAM_HEADER;


/*Add bye QJF 2006.12.26 */
typedef struct gwtt_oam_message_list_node
{
	struct gwtt_oam_message_list_node *next;
	cs_uint8 GwOpcode;
	cs_uint32 SendSerNo;
	cs_uint16 WholePktLen;
	cs_uint16 RevPktLen;
	cs_uint8 SessionID[8];
	struct epon_timer_t TimerID;
	cs_uint8 *pPayLoad;
} GWTT_OAM_MESSAGE_NODE;

typedef struct gwtt_oam_session_info
{
	cs_uint32 SendSerNo;
	cs_uint8 SessionID[8];
} GWTT_OAM_SESSION_INFO;


#define GWD_RETURN_OK 0
#define GWD_RETURN_ERR -1

extern cs_ulong32   gulDebugOamRx;
extern cs_ulong32   gulDebugOamTx;
extern cs_ulong32   gulDebugOamRxCount;
extern cs_ulong32   gulDebugOamTxCount;

#define OAM_RX_PACKET_DEBUG(str) if( gulDebugOamRx ){ Debug_Print_Rx_OAM str ;}
#define OAM_TX_PACKET_DEBUG(str) if( gulDebugOamTx ){ Debug_Print_Tx_OAM str ;}
#define OAM_FILE_OP_DEBUG(str) if( gulDebugOamFileOp ){ cl_vty_all_out str ;}

#define OAM_DATA_LEN				65535
#define OAM_OVERHEAD_LEN_STD		22	/* DA/SA/Len/Sub/Flag/Code/FCS */
#define OAM_OVERHEAD_LEN_GW			22	/* OUI/Op/Ser/WLen/POff/PLen/SnID/ */
#define OAM_MAX_FRAM_SIZE 			(106-22)	/*GW私有帧净荷的最大长度 */
#define OAM_MIN_FRAM_SIZE			20	/*GW私有帧净荷的最小长度 */

/* OAM opCode definations */
#define EQU_DEVICE_INFO_REQ			0x01	/* Device information REQUEST */
#define EQU_DEVICE_INFO_RESP		0x02	/* Device information RESPONSE */
/*#define SET_REQ                       0x03
 * #define SET_RESP                 0x04 */
#define ALARM_REQ					0x03
#define ALARM_RESP					0x04
#define FILE_READ_WRITE_REQ			0x05
#define FILE_RESERVER				0x06
#define FILE_TRANSFER_DATA			0x07
#define FILE_TRANSFER_ACK			0x08
#define CHURNING					0x09
#define DBA							0x0A

#define SNMP_TRAN_REQ 				0xB
#define SNMP_TRAN_RESP				0xC
#define SNMP_TRAN_TRAP				0xD

#define CLI_REQ_TRANSMIT			0x10
#define CLI_RESP_TRANSMIT			0x11

#define IGMP_AUTH_TRAN_REQ			0x12
#define IGMP_AUTH_TRAN_RESP 		0x13

#define CLI_PTY_TRANSMIT            0x14

#define  IGMP_TVM_REQ				0x16
#define  IGMP_TVM_RESP				0x17


//以下设备类型只有在GwOamInformationRequest()和环路检测中用到，已经与其他ONU
//类型冲突，以后禁止使用
#define DEVICE_TYPE_GT821			0x0005	/* GT821 */
#define DEVICE_TYPE_GT831			0x0006	/* GT831 */
#define DEVICE_TYPE_GT813			0x0008	/* GT813 */
#define DEVICE_TYPE_GT812			0x0007/*7*/	/* GT812 */
#define DEVICE_TYPE_GT811			0x0004/*11 - a*/ /*4-*/	/* GT811 */
#define DEVICE_TYPE_GT810			0x000c	/* GT810 */
#define DEVICE_TYPE_GT816			0x0010	/* GT816 */
#define DEVICE_TYPE_GT821_A			0x0013	/* GT821A */
#define DEVICE_TYPE_GT831_A			0x0014	/* GT831A */
#define DEVICE_TYPE_GT812_A			0x0012/*7*/	/* GT812 */
#define DEVICE_TYPE_GT811_A			0x0011/*11 - a*/ /*4-*/	/* GT811 */
#define DEVICE_TYPE_GT865			0x000F	/* GT865 */
#define DEVICE_TYPE_GT861			0x000A	/* GT861 */
#define DEVICE_TYPE_GT815			0x0015	/* GT815 */
#define DEVICE_TYPE_UNKNOWN			0x0001	/* unknown */
#define DEVICE_TYPE_GT812PB			0x0016	/* GT812PB */
#define DEVICE_TYPE_GT831_B			0x0017	/* GT831B */
#define DEVICE_TYPE_GT866			0x0018	/* GT866 */
#define DEVICE_TYPE_GT811_B			0x0019	/* GT811_B */
#define DEVICE_TYPE_GT851			0x001A	/* GT851 */
#define DEVICE_TYPE_GT813_B			0x001B	/* GT813_B */
#define DEVICE_TYPE_GT862			0x001C	/* GT862 */
#define DEVICE_TYPE_GT892			0x001D	/* GT892 */
#define DEVICE_TYPE_GT835			0x001E	/* GT835 */
#define DEVICE_TYPE_GT831_B_CATV	0x001F	/* GT831_B_CATV */
#define DEVICE_TYPE_GT815_B			0x0020	/* GT815_B */

#define DEVICE_TYPE_GT863			0x000D	/* GT863 */ //added by wangxiaoyu 2009-05-25
#define DEVICE_TYPE_GT871B			0x0021	/* GT871 */ //added by dushaobo 2009-07-13
#define DEVICE_TYPE_GT871R                    0x0022
#define DEVICE_TYPE_GT872                       0x0025
#define DEVICE_TYPE_GT873                       0x0028
#define DEVICE_TYPE_GT870                       0x002C
#define DEVICE_TYPE_GT811D						0x002D
#define DEVICE_TYPE_GT811G                      0x002E
#define DEVICE_TYPE_GT812C						0x002F
#define DEVICE_TYPE_GT873_M_4F4S				0x0030
#define DEVICE_TYPE_VALID_MAX		DEVICE_TYPE_GT873_M_4F4S
#define DEVICE_TYPE_VALID_MIN		DEVICE_TYPE_GT811

#if (PRODUCT_CLASS == PRODUCTS_GT811D)
#define ONU_DEVICE_TYPE		DEVICE_TYPE_GT811D

#elif (PRODUCT_CLASS == PRODUCTS_GT811G)
#define ONU_DEVICE_TYPE		DEVICE_TYPE_GT811G

#elif (PRODUCT_CLASS == PRODUCTS_GT873_M_4F4S)
#define ONU_DEVICE_TYPE		DEVICE_TYPE_GT873_M_4F4S

#else
#define ONU_DEVICE_TYPE		0x0000
#endif


#define DeviceTypeIsValid( _device_type ) \
    ((_device_type)>=DEVICE_TYPE_VALID_MIN && (_device_type)<=DEVICE_TYPE_VALID_MAX)

#define MODULE_TYPE_GT861_NULL		0	/* empty */
#define MODULE_TYPE_GT861_EPON_B	1	/* EPON_B */
#define MODULE_TYPE_GT861_8POTS_A	2	/* RJ11 */
#define MODULE_TYPE_GT861_6FE		3	
#define MODULE_TYPE_GT861_8FE		4	
#define MODULE_TYPE_GT861_16FE		5	
#define MODULE_TYPE_GT861_8FXS_A	6	/* RJ11 */	
#define MODULE_TYPE_GT861_8POTS_B	7	/* RJ21 */
#define MODULE_TYPE_GT861_8FXS_B	8	/* RJ21 */
#define MODULE_TYPE_GT861_8POTSO_A	9	/* RJ11 */
#define MODULE_TYPE_GT861_8POTSO_B	10	/* RJ21 */
#define MODULE_TYPE_GT861_4E1_120	11	/* RJ21 */
#define MODULE_TYPE_GT861_4E1_75	12	/* RJ21 */


#define DEVICE_CHIP_6301			0x6301
#define DEVICE_CHIP_6201			0x6201

#define ONU_TEMPRATURE_ALARM			2	/*ONU温度告警 */
#define ONU_ETH_PORT_STATE				10	/*ONU以太网端口状态告警 */
#define ONU_ETH_PORT_ABILITY			20	/*ONU以太网端口性能告警 */
#define ONU_ETH_WORK_STOP				21	/*ONU以太网端口业务中断告警 */
#define ONU_STP_EVENT					30	/*STP事件 */
#define ONU_DEVICE_INFO_CHANGE			100	/*ONU设备信息修改事件 */
#define ONU_FILE_DOWNLOAD				50	/*ONU数据加载事件 */
#define ONU_DATAFILE_CHG				60	/*ONU数据文件修改事件 */
#define ONU_PORT_LOOP_ALARM      		11  /*ONU or Switch port loop alarm*/
#define ONU_PORT_BROADCAST_STORM		16	/*ONU broadcast storm*/
#define ONU_PORT_BROADCAST_STORM_ALARM	22	/*ONU broadcast storm add by zhangjj 2014-8-14*/
#define ONU_SWITCH_STATUS_CHANGE_ALARM  80  /*ONU下挂交换机的注册离线告警*/
#define ONU_SWITCH_STATUS_CHANGE_ALARM_LEN  14  /*ONU下挂交换机的注册离线告警消息长度*/

#define ONU_INFOR_GET				1	/*ONU设备信息查询 */
#define ONU_INFOR_SET				2	/*ONU设备信息设置 */
#define ONU_REALTIME_SYNC			3	/*ONU系统时间同步 */
#ifdef	HAVE_GWD_DHCP_RELAY
#define ACCESS_IDENTIFIER     		8	/*ONU用户接入线路标识*/
#endif


#ifdef HAVE_GWD_OAM_FAST_STATISTIC
#define ONU_GWD_FAST_STATISTIC		(201)
#define FAST_STATISTIC_OCTETS		((cs_uint64)1<<63)
#define FAST_STATISTIC_GET_SUCCESS	(1)
#define FAST_STATISTIC_GET_FAILED	(2)

typedef struct
{
	cs_uint8 type;
	cs_uint64 data_type_bit_map;
	cs_uint32 port_bit_map;
}__attribute__((packed))onu_gwd_dact_request_pdu_t;

typedef struct
{
	cs_uint64 in_octets;
	cs_uint64 out_octets;
}__attribute__((packed))port_octets_t;
typedef struct
{
	cs_uint8 type;
	cs_uint8 result;
	cs_uint64 data_type_bit_map;
	cs_uint32 port_bit_map;
	port_octets_t first_port_octets;
}__attribute__((packed))onu_gwd_dact_response_t;
#endif	/*(#ifdef HAVE_GWD_OAM_FAST_STATISTIC)*/


#ifdef HAVE_USER_MAC_TRACE
#define ONU_LOCATE_USER			200	/*用户mac 地址追踪*/
#define USR_MAC_ADDRES_CHEAK	1
typedef struct
{
	cs_uint8 swmac[CS_MACADDR_LEN];
}__attribute__((packed))userMacRequest_t;
typedef struct
{
	cs_uint8 type;						/*类型*/
	cs_uint8 reserve;					/*保留*/
	cs_uint8 mode;						/*查询模式*/
	cs_uint8 macNum;					/*查询MAC数*/
	userMacRequest_t info[64];					/*信息*/
}__attribute__((packed))userMacRequest_pdu_t;

typedef struct
{
	cs_uint8 type;						/*类型*/
	cs_uint8 result;					/*查询结果*/
	cs_uint8 mode;						/*查询模式*/
	cs_uint8 macNum;					/*查询MAC数*/
}__attribute__((packed))userMacResponse_pdu_t;

typedef struct
{
	cs_uint8 usermac[CS_MACADDR_LEN];	/*用户MAC地址*/
	cs_uint16 reserved;					/*保留*/
	cs_uint8 onuslot;					/*ONU槽位号*/
	cs_uint8 onuport;					/*ONU端口号*/
	cs_uint8 subsw;						/*MAC地址对应eth口是否下挂交换机*/
	cs_uint8 swmac[CS_MACADDR_LEN];		/*交换机MAC地址*/
	cs_uint32 swport;					/*交换机端口号*/	
}__attribute__((packed))userMacResponse_t;
#endif	/*(#ifdef HAVE_USER_MAC_TRACE)*/


#ifdef HAVE_OPTICAL_MODULE_PARAMETERS_QUERY
#define ONU_EPON_PARAMETERS_GET			7
typedef struct
{
	cs_uint8	type;				/*类型*/
	cs_uint32	reserve;			/*保留*/
	cs_uint8	result;				/*查询结果*/
	cs_uint16	tx_power;			/*上行光功率*/
	cs_uint16	rx_power;			/*下行光功率*/
	cs_uint16	temperature;		/*温度*/
	cs_uint16	voltage;			/*供电电压*/
	cs_uint16	tx_bias;			/*偏置电流*/
}__attribute__((packed))epon_parameters_response_t;
#endif	/*(#ifdef HAVE_OPTICAL_MODULE_PARAMETERS_QUERY)*/


#ifdef HAVE_ONU_PORT_MAC_COUNT
#define ONU_PORT_MAC_COUNT	(210)		/*查询ONU端口Mac地址数目*/
typedef struct
{
	cs_uint8 type;						/*类型：301－查询ONU以太端口Mac地址统计*/	
	cs_uint8 port_index;				/*端口索引：以太端口号，从1开始编号，如果是0xFF代表获取所有端口的Mac地址*/
}__attribute__((packed))PortMacCountRequest_pdu_t;

typedef struct
{
	cs_uint8	port_index;			/*端口索引： 以太端口号*/
	cs_uint32	mac_count;			/*Mac地址数目： 指定端口号的Mac地址数目值*/
}__attribute__((packed))port_mac_count_t;

typedef struct
{
	cs_uint8	type;				/*类型：301－查询ONU以太端口Mac地址统计*/	
	cs_uint8	result;				/*查询结果：1－成功；2－失败*/
	cs_uint8	port_count;			/*实际获取的端口数量，针对request是0xFF的情况，此处应该返回实际的ONU端口数目*/
	port_mac_count_t first_port_mac_count;
}__attribute__((packed))PortMacCountResponse_t;
#endif	/*(#ifdef HAVE_ONU_PORT_MAC_COUNT)*/

#ifdef HAVE_GWD_LOOP_DETECT
#define ONU_LPB_DETECT					4
#endif

#define ONU_BOARD_GET_RESP_SUCCESS		1
#define ONU_BOARD_GET_RESP_FAIL			2

#define ONU_BOARD_GET_STATUS_NULL		1
#define ONU_BOARD_GET_STATUS_INITIALIZING	2
#define ONU_BOARD_GET_STATUS_UPGRADING	3
#define ONU_BOARD_GET_STATUS_RUNNING	4
#define ONU_BOARD_GET_STATUS_ALARM		5

#define ONU_SLOT_RST_SET				6	/*Reset ONU Slot*/

#define ONU_PON_PARAMETERS_GET			7	/*ONU PON transceiver info get*/
#define ONU_GET_RESP_SUCCESS			1
#define ONU_GET_RESP_FAIL				2
/*begin:
added by wangxiaoyu 2008-05-21
modified by wangxiaoyu 2008-12-25 IP_RESOURCE_ALLOC value 10-->12
*/
#define IP_RESOURCE_ALLOC				12
#define IP_RESOURCE_FREE				11
//#endif

#define ONU_IGMP_REGISTER			1	/*ONU注册报文 */
#define ONU_IGMP_UNREGISTER			2	/*ONU注销报文 */
#define ONU_IGMP_LEAVE_REQ			3	/*ONU强制离开报文 */
#define ONU_IGMP_REGISTER_ACK		11	/*ONU注册应答报文 */
#define ONU_IGMP_UNREGISTER_ACK		12	/*ONU注销应答报文 */
#define ONU_IGMP_LEAVE_ACK			13	/*ONU强制离开应答报文 */

#if(RPU_MODULE_VOICE == RPU_YES)		/* VM = Voice Module */
#define ONU_VM_BASIC_SET			100	/*ONU Voice module chip-enable, DA, SA set*/
#define ONU_VM_BASIC_GET			101	/*ONU Voice module chip-enable, DA, SA get*/
#define ONU_VM_VLAN_SET				102	/*ONU Voice module vlan-enable, Tag set*/
#define ONU_VM_VLAN_GET				103	/*ONU Voice module vlan-enable, Tag get*/
#define ONU_VM_PORT_EN_SET			104	/*ONU Voice module port-enable set*/
#define ONU_VM_PORT_EN_GET			105	/*ONU Voice module port-enable get*/
#define ONU_VM_PORT_STATUS_GET		106	/*ONU Voice module port status get*/
#define ONU_VM_PORT_LOOP_SET		107	/*ONU Voice module port loop set*/
#define ONU_VM_PORT_LOOP_GET		108	/*ONU Voice module port loop get*/
#define ONU_INFOR_EXT_GET			109	/*ONU extended info set*/
#define ONU_VM_TOTAL_SET			110	/*ONU Voice module all feature set*/
#define ONU_VM_TOTAL_GET			111	/*ONU Voice module all feature get*/
#endif

#define ONU_E1_LINK_SET				120	/*ONU E1 module port, port_en, DA, SA, Vlan_en, Vlan_Tag set*/
#define ONU_E1_VLAN_SET				121	/*ONU E1 module port, Vlan_en, Vlan_Tag set*/
#define ONU_E1_LOOP_SET				122	/*ONU E1 module port, loop-mode*/
#define ONU_E1_CLK_SET				123	/*ONU E1 module port, clk source*/
#define ONU_E1_ALM_MSK_SET			124	/*ONU E1 module port, alarm mask*/
#define ONU_E1_SERVICE_SET			125	/*ONU E1 module port, all para*/
#define ONU_E1_SERVICE_GET			126	/*ONU E1 module port, all para*/

#define ONU_E1_LINK_SET_LEN			20	/*Length of each link para set*/
#define ONU_E1_SERVICE_GET_LEN		26	/*Length of each port para set*/
#define ONU_MAX_E1_PORTS			16

#define E1_PORTS_PER_SLOT 4			
#define E1_SLOT_MIN 2
#define E1_SLOT_MAX 5

#define IGMP_AUTH_REQ			0x1
#define IGMP_LEAVE_REQ			0x2
#define IGMP_FRC_LEAVE			0x3
#define IGMP_AUTH_RESP			11
#define IGMP_LEAVE_RESP			12
#define IGMP_FRC_LEAVE_RESP		13

#define IGMP_AUTH_SESSION_TIMEOUT_TIME	1000	/* 1 s */

#define SOFTWARE_UPGRADE		0x01
#define FIRMWARE_UPGRADE		0x02
#define BOOT_UPGRADE			0x03
#define CONFIG_UPGRADE			0x04
#define VOICE_UPGRADE			0x05
#define FPGA_UPGRADE			0x06	/* Combination file include Voice and E1 fpga */

#define ONU_SWITCH_STATUS_CHANGE_ALM_REG    	0x01  /*type of switch status change alarm : new switch register */
#define ONU_SWITCH_STATUS_CHANGE_ALM_REREG  	0x02  /*type of switch status change alarm : switch register again */
#define ONU_SWITCH_STATUS_CHANGE_ALM_OFFLINE  	0x03  /*type of switch status change alarm : switch offline */

#define UPGRADE_RESULT_OK		0x01
#define UPGRADE_RESULT_ERROR	0x02

#define APP_IMAGE_NAME			"appimage.bin"
#define BOOT_IAMGE_NAME			"boot.bin"
#define CONFIG_FILE_NAME		"config.txt"
#define SYSLOG_FILE_NAME		"log.txt"
#define FIRMWARE_FILE_NAME		"firmware.bin"
#define VOICEAPP_IMAGE_NAME		"appvoice.bin"
#define FPGA_IMAGE_NAME			"appfpga.bin"

#define IROS_TICK_IN_MILLISECOND	10
#define IROS_TICK_PER_SECOND		100
#define WAIT_TIME_FOR_OAM_MESSAGE  	(2*IROS_TICK_PER_SECOND)	/* Ticks */

/*
 * File operation state machine
 *  Manage the file operation
 */
/* States */
#define FSM(state, event) (event + (state<<8))
#define FMST_IDLE			0			/* idle */
#define FMST_WAITDATA		1			/* process receive */
#define FMST_WAITACK		2			/* process transmit */
#define FMST_TERMINATE		3			/* terminated state */
#define FMST_FILE_TRAN_COMPLETE 4

/* Events */
#define FCMD_WRITE			0			/* receive an WRITE CMD */
#define FCMD_READ			1			/* receive an READ CMD */
#define FCMD_TO0			2			/* process 2S TIMERS' TimeOut */
#define FCMD_TO1			3			/* process 15S TIMERS' TimeOut */
#define FCMD_ACKOK			4			/* receive a SUCCESS ACK */
#define FCMD_ACKERROR		5			/* receive a ERROR INDIACT ACK */
#define FCMD_ACK303		6				/* receive a FINISH ACK */
#define FCMD_TRANSFER		7			/* receive a DATA-TRANSFER PACKET */
#define FCMD_TERMINATE		8			/* receive a TERMINATE CMD, clear the machine */
#define FCMD_UST_ERROR		9			/* retransmit software update error */
#define FCMD_UST_OK		10			/* retransmit software update ok */
#define FCMD_UVST_ERROR	11			/* retransmit voip software update error */
#define FCMD_UVST_OK		12			/* retransmit voip software update ok */
#define FCMD_RECVALAMRESP	13			/* receive alarm response */
#define FCMD_UFPGA_ERROR	14			/* retransmit FPGA update error */
#define FCMD_UFPGA_OK		15			/* retransmit FPGA update ok */

#define E1_FPGA_UPDATE_RESULT_SUCCESS 	0
#define E1_FPGA_UPDATE_RESULT_NOMEM 	1
#define E1_FPGA_UPDATE_RESULT_SEM_FAIL 	2
#define E1_FPGA_UPDATE_RESULT_NOE1 		3
#define E1_FPGA_UPDATE_RESULT_SLOT_ERR	4
#define E1_FPGA_UPDATE_RESULT_MAN_MAC	5
#define E1_FPGA_UPDATE_RESULT_NOQUEUE	6
#define E1_FPGA_UPDATE_RESULT_HW_FAIL	7
#define E1_FPGA_UPDATE_RESULT_MSG_FAIL	8
#define E1_FPGA_UPDATE_RESULT_TIMEOUT	9
#define E1_FPGA_UPDATE_RESULT_QUEUE_FAIL	10
#define E1_FPGA_UPDATE_RESULT_STATE_ERR	11
#define E1_FPGA_UPDATE_RESULT_MSG_ERR	12
#define E1_FPGA_UPDATE_RESULT_NOTINIT 	13
#define E1_FPGA_UPDATE_RESULT_READ_FILE	14
#define E1_FPGA_UPDATE_RESULT_TASK		15
#define E1_FPGA_UPDATE_RESULT_FILE_ERR	16
#define E1_FPGA_UPDATE_RESULT_PORT_DOWN	17
#define E1_FPGA_UPDATE_RESULT_PORT_ERR	18


/* ACK Codes */
#define READ_DENY           0x100		/*  读拒绝 */
#define READ_ACCEPT	    	0x101		/*  读允许 */
#define WRITE_DENY         0x200		/*  写拒绝 */
#define WRITE_ACCEPT     	0x201		/*  写允许 */
#define TRANS_ERROR       	0x300		/*  传送错误 */
#define TRANS_START      	0x301		/*  传送开始（传送过程控制应答，也表示传送正常） */
#define TRANS_DOING      	0x302		/*  传送中（传送过程控制应答，也表示传送正常） */
#define TRANS_DONE        	0x303		/*  传送结束（传送过程控制应答，也表示传送正常） */

/* ACK ERROR Codes */
#define SYS_NOERROR			0x00		/*  无错误 */
#define SYS_BUSY			0x01		/*  系统忙 */
#define SYS_NORESOURCE 	0x02			/*  系统资源不足 */
#define SYS_PROCESSERR		0x03		/*  系统处理错误 */
#define SYS_PROTOERR		0x04		/*  流程错误 */
#define SYS_NOSUCHFILE		0x05		/*  文件不存在 */
#define SYS_FILETOOLONG	0x06			/*  文件太长 */
#define SYS_FILETOOSHORT	0x07		/*  文件太短 */
#define SYS_FILEOPERR		0x08		/*  长度或偏移匹配错误 */
#define SYS_FILECKERR		0x09		/*  数据校验错误 */
#define SYS_FILESAVEERR		0x0A		/*  文件保存错误 */

#define FILE_OP_PACKET		0x01
#define FILE_OP_COMMAND		0x02
#define CLI_PACKET			0x03
#define SNMP_PACKET			0x04
#define FILE_OP_TIMERCHECK	0x05


#define PTY_PACKET       	0x06
#define PTY_NOTI_MSG        0x07
#define PTY_TIMER_MSG       0x08
#define PTY_ONU_LOSE        0x09

#define GET_SHORT( _BUF )    ((((cs_uint16)(((cs_uint8 *)(_BUF))[0])) << 8) | (((cs_uint16)(((cs_uint8 *)(_BUF))[1])) << 0))
#define GET_LONG( _BUF )    ((((cs_ulong32)(((cs_uint8 *)(_BUF))[0])) << 24) | (((cs_ulong32)(((cs_uint8 *)(_BUF))[1])) << 16) | (((cs_ulong32)(((cs_uint8 *)(_BUF))[2])) << 8) | (((cs_ulong32)(((cs_uint8 *)(_BUF))[3])) << 0))
#define SET_LONG( _BUF, value)	\
{ \
    cs_ulong32 _ulValue;\
    _ulValue = (cs_ulong32)(htonl(value));\
    memcpy(_BUF,&_ulValue,4);\
}
#define GET_LONG_OP( _BUF )    ((((cs_ulong32)(((cs_uint8 *)(_BUF))[3])) << 24) | (((cs_ulong32)(((cs_uint8 *)(_BUF))[2])) << 16) | (((cs_ulong32)(((cs_uint8 *)(_BUF))[1])) << 8) | (((cs_ulong32)(((cs_uint8 *)(_BUF))[0])) << 0))

#define SET_SHORT( _BUF, value)	\
{ \
    cs_uint16 _ulValue;\
    _ulValue = (cs_uint16)(htons(value));\
    memcpy(_BUF,&_ulValue,2);\
}

/* Op-Code for CTC extend OAM */
#define Extended_Variable_Request		0x1	/* 用于OLT 向ONU 查询扩展属性 */
#define Extended_Variable_Response		0x2	/* 用于ONU向OLT 返回扩展属性 */
#define Extended_Variable_Set_Request	0x3	/* 用于OLT 向ONU 配置扩展属性/操作 */
#define Extended_Variable_Set_Response	0x4	/* 用于ONU向OLT返回对扩展属性/操作配置的确认 */
#define Extended_Variable_Churning		0x5	/* 与Triply-Churning 相关的密钥交互 */
#define Extended_Variable_DBA			0x6	/* DBA 参数配置与查询 */

/* Branch ID for CTC */
#define Branch_Standard_Attribute1		0x07	/* IEEE 802.3 Clause 30规定的标准属性 */
#define Branch_Standard_Attribute2		0x09	/* IEEE 802.3 Clause 30规定的操作功能 */
#define Branch_Ctc_Extended_Attribute1	0xc7	/* CTC扩展的属性，可以执行Get和(或)Set命令 */
#define Branch_Ctc_Extended_Attribute2	0xc9	/* CTC扩展的操作 */
#define Branch_Instance_Index			0x36	/* 实例索引，其后为实例属性 */

/* Leaf */
#define Leaf_Index_Port			0x0001	/* 端口实例索引的leaf值 */
#define Leaf_ONU_SN				0x0001	/* ONU的标识符 */
#define Leaf_FirmwareVer		0x0002	/* ONU的固件版本 */
#define Leaf_ChipsetID			0x0003	/* ONU的PON芯片厂商和版本 */
#define Leaf_ONU_Capabilities		0x0004	/* ONU的端口、功能 */

#define Leaf_EthLinkState			0x0011	/* 以太网用户端口的链路状态 */
#define Leaf_EthPortPause		0x0012	/* 以太网端口的流控功能及参数 */
#define Leaf_EthPortPolicing		0x0013	/* 以太网端口的限速功能（上行） */
#define Leaf_VoIP_Port			0x0014	/* VoIP端口管理 */

#define Leaf_VLAN				0x0021	/* ONU的VLAN功能 */

#define Leaf_ClassMarking		0x0031	/* 业务流分类与标记 */

#define Leaf_Add_Del_Multicast_VLAN	0x0041	/* ONU的以太网端口的组播VLAN配置 */
#define Leaf_MulticastTagStripe	0x0042	/* ONU对下行Multicast数据报文的VLAN TAG处理 */
#define Leaf_MulticastSwitch		0x0043	/* 组播协议开关 */
#define Leaf_MulticastControl		0x0044	/* 基于频道的组播业务控制 */
#define Leaf_Group_Num_Max		0x0045	/* ONU或端口同时支持的组播组数量 */

/* Leaf,  branch 0x07/0x09 */
#define Leaf_aPhyAdminState		0x0025	/* 查询以太网端口的状态, 0x07 */
#define Leaf_acPhyAdminControl	0x0005	/* 设置或更改以太网物理端口的状态, 0x09 */
#define Leaf_aAutoNegAdminState	0x004f	/* 以太网端口的状态（自协商）, 0x07 */
#define Leaf_aAutoNegLocalTechnologyAbility	0x0052	/* actual port capabilities, 0x07 */
#define Leaf_aAutoNegAdvertisedTechnologyAbility	0x0053	/* 端口自协商能力通告, 0x07 */
#define Leaf_acAutoNegRestartAutoConfig	0x000b	/* 强制链路重新协商, 0x09 */
#define Leaf_acAutoNegAdminControl	0x000c	/* 打开或者关闭PHY端口的自协商功能, 0x09 */
#define Leaf_aFECAbility			0x0139	/* FEC能力查询（IEEE 802.3-2005 Clause 30.5.1.1.13）, 0x07 */
#define Leaf_aFECmode			0x013a	/* 双向FEC功能的打开/关闭（IEEE 802.3-2005 Clause30.5.1.1.14）, 0x07 */

#define TLV_SET_OK				0x80	/* 对set variable request或操作（Action）的确认 */
#define TLV_SET_PARAM_ERR		0x86	/* 设置请求（Set Request）或操作（Action）的参数无效 */
#define TLV_SET_ERR				0x87	/* 设置请求（Set Request）或操作（Action）的参数有效，但ONU的当前状态使该操作无法完成 */

#define CLASS_MARK_DEL			0x00	/* 删除下述的Classification、Queuing&Marking控制规则（用于Set Variable Request消息）*/
#define CLASS_MARK_ADD			0x01	/* 增加下述的Classification、Queuing&Marking控制规则（用于Set Variable Request消息）*/
#define CLASS_MARK_CLR			0x02	/* 清除ONU的Classification、Queuing&Marking控制表（即删除该ONU所有
										    的分类、排队和标记规则）；该操作类型仅用于Set Variable
										    Request消息。当本container为此操作类型时，本字节后面没有其他数据*/
#define CLASS_MARK_GET			0x03	/* 列出该ONU所有的Classification、Queuing&Marking控制条目（用
										     于Get Variable Request/Response消息)；当本container用于Get Variable Request时，
										     本字节后面没有其他数据；当本container用于Get Variable Response时，
										     本字节后面为该端口的所有分类、排队和标记规则*/


#define OAM_ALM_SLOT_ID							3 
#define OAM_ALM_LNKCHG_ID						10 
#define OAM_ALM_PON_ID							25 
#define OAM_ALM_E1_ID							40 
#define OAM_ALM_SAMPLE_PKT_ID					4 

#define OAM_ALM_PON_TRANSMISSION_POWER_HIGH_ID	1 
#define OAM_ALM_PON_TRANSMISSION_POWER_LOW_ID	2 
#define OAM_ALM_PON_RECEIVER_POWER_HIGH_ID		3 
#define OAM_ALM_PON_RECEIVER_POWER_LOW_ID		4 
#define OAM_ALM_PON_APPLIED_VOLTAGE_HIGH_ID		5 
#define OAM_ALM_PON_APPLIED_VOLTAGE_LOW_ID		6 
#define OAM_ALM_PON_BIAS_CURRENT_HIGH_ID		7 
#define OAM_ALM_PON_BIAS_CURRENT_LOW_ID			8 
#define OAM_ALM_PON_MODULE_TEMPERATURE_HIGH_ID	9
#define OAM_ALM_PON_MODULE_TEMPERATURE_LOW_ID	10 
#define OAM_ALM_PON_LASER_ALWAYS_ON_ID			11 

#define DFT_PONRECEIVERPOWERLOW			-30
#define DFT_PONRECEIVERPOWERHIGH		-6
#define DFT_PONTRANSMISSIONPOWERLOW		1
#define DFT_PONTRANSMISSIONPOWERHIGH	50
#define DFT_PONMODULETEMPERATURELOW		-40
#define DFT_PONMODULETEMPERATUREHIGH	50
#define DFT_PONAPPLIEDVOLTAGELOW		30
#define DFT_PONAPPLIEDVOLTAGEHIGH		50
#define DFT_PONBIASCURRENTLOW     		1
#define DFT_PONBIASCURRENTHIGH			500

#define	DHCP_OPTION82_RELAY_MODE_CTC	1
#define DHCP_OPTION82_RELAY_MODE_STD	2

#define LENGTH_OF_VERSION_STRING		32
#define LENGTH_OF_VENDOR_STRING			128
#define LENGTH_OF_SN_STRING				32
#define LENGTH_OF_DATE_STRING			32

/* Onu E1 alarm flag */
#define ONU_ALM_E1_OOS		0x8000
#define ONU_ALM_E1_LOS		0x0080
#define ONU_ALM_E1_LOF		0x0040
#define ONU_ALM_E1_AIS		0x0020
#define ONU_ALM_E1_RAI		0x0010
#define ONU_ALM_E1_SMF		0x0008
#define ONU_ALM_E1_LOFSMF	0x0004
#define ONU_ALM_E1_CRC3		0x0002
#define ONU_ALM_E1_CRC6		0x0001

/*end*/

#define TASK_PRIORITY_LOWEST                  26

#define IFM_ETH_TYPE      (0x1)


/* Note: adjust reserved section , not change the TYPE size. If you change the TYPE size, not change 
   the valid_flag position for keep the data of previous version remain valid */ 
typedef struct onu_ablity_info{
	cs_uint8 type;
	cs_uint8 leng;
	cs_uint8 val;
}ONU_SYS_INFO_ABILITY;
typedef struct onu_system_information_total
{
	cs_uint16 	product_serial;					/* 2 */
	cs_uint16	product_type;					/* 2 */
	cs_uint8	device_name[127];				/* 127 */
	cs_uint8	sw_version[12];					/* 12 */
	cs_uint8	serial_no[18];					/* 18 */
	cs_uint8	hw_version[6];					/* 6 */
	cs_uint8	hw_manufature_date[12];			/* 12 */
	cs_uint8    auto_requ[2];					/*2*/    
														/* 181 */
	struct	_pon_transceiver_info_
	{
		cs_uint8 	available	: 1;
		cs_uint8	reach 		: 1;
		cs_uint8	package 	: 1;
		cs_uint8	protectEn 	: 1;
		cs_uint8	manufacturer 	: 4;
	}pon_transceiver_info;						/* 1 */
						
	cs_uint8	TDM_module_info;				/* 1 */
	cs_uint8 	VoIP_module_info;				/* 1 */
	cs_uint8	reserved_1;						/* 1 */
														/* 185 */
	cs_uint8	mac_addr_primary[6];			/* 6 */
	cs_uint8  	mac_addr_second[6];				/* 6 */
														/* 197 */
	cs_ulong32	ip_addr;						/* 4 */
	cs_ulong32	net_mask;						/* 4 */
														/* 205 */
	cs_ulong32	vos_image_size;					/* 4 */
														/* 209 */
	cs_uint8	loadstartupconfig;				/* 1 */
	cs_uint8	hardwarediagnosis;				/* 1 */
	cs_uint8	reserved_2[2];					/* 2 */
														/* 213 */
	cs_ulong32	reserved_3[64];					/* 256 */
														/* 469 */
	cs_uint8 	reserved_4[2];					/* 2 */
	cs_uint8 	reboot;							/* 1 */
	cs_uint8	valid_flag;						/* 1 */ /* Total: 473 */
}ONU_SYS_INFO_TOTAL;
#endif

typedef enum
{
	GWD_OLT_NONE,
	GWD_OLT_GFA6100,
	GWD_OLT_GFA6700,
	GWD_OLT_GFA6900,
	GWD_OLT_NOMATCH
}GWD_OLT_TYPE;

#ifndef TOUPPER
#define TOUPPER(c)	(c >= 'a' && c <= 'z' ? c - ('a' - 'A') : c)
#endif

extern ONU_SYS_INFO_TOTAL onu_system_info_total;
extern unsigned char *irosbootver;

int Onu_Sysinfo_Get(void);
int Onu_Sysinfo_Save(void);

void getOltMacAddress(epon_macaddr_t * olt_mac);
int GwGetOltType(unsigned char *mac, GWD_OLT_TYPE *type);
extern int CommOnuMsgSend(unsigned char GwOpcode, unsigned int SendSerNo, unsigned char *pSentData,
						const unsigned short SendDataSize, unsigned char  *pSessionIdfield);

#endif	/*end of #ifdef HAVE_GWD_OAM*/

#ifdef HAVE_EVENT_SHOW_CMD
typedef struct
{
	cs_uint8	type;
	cs_uint16	year;
	cs_uint8	month;
	cs_uint8	day;
	cs_uint8	hour;
	cs_uint8	minute;
	cs_uint8	second;
}__attribute__((packed))gw_realtime_sync_t;
#endif


#endif	/*end of the file*/
