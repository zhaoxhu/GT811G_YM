#ifdef HAVE_GWD_OAM
#include "app_oam_gwd.h"
#include "oam_supp.h"
#include "oam_core.h"
#include "oam_client.h"
#include "sdl_mpcp_cmn.h"
#include "oam_mux_par.h"
#include "sdl.h"
#include "registers.h"
#include "sdl_pon.h"

#ifdef HAVE_GWD_OAM_PTY
#include "oam_pty.h"
#endif
#ifdef HAVE_GWD_FLASH_OPER
#include "gwd_flash_oper.h"
#endif
#ifdef HAVE_GWD_MC_TVM
#include "gwd_mc_process.h"
#endif
#ifdef HAVE_GWD_DHCP_RELAY
#include "gwd_dhcp.h"
#endif
#ifdef HAVE_GWD_BC_LIMIT
#include "gwd_broadstorm.h"
#endif
#ifdef HAVE_GWD_OAM_FAST_STATISTIC
#include "app_stats_ctc.h"
#endif
#ifdef HAVE_USER_MAC_TRACE
#include "sdl_fdb.h"
#endif
#ifdef HAVE_OPTICAL_MODULE_PARAMETERS_QUERY
#include "sdl_i2c.h"
#endif
#ifdef HAVE_ONU_PORT_MAC_COUNT
#include "sdl_fdb.h"
#endif
#ifdef HAVE_EVENT_SHOW_CMD
#include "gwd_log.h"
#endif


#ifdef HAVE_GWD_LOOP_DETECT
#include "gwd_eth_loop_detect.h"
#include "gwd_eth_loop_detect_realize.h"
#endif

#ifdef HAVE_DHCPV4_SUPPORT
#include "app_ip_types.h"
#include "app_ip_util.h"
#endif

#ifdef HAVE_DHCPV6_SUPPORT
#include "app_ip_util.h"
#include "sdl_l2.h"
#endif

#if (PRODUCT_CLASS == PRODUCTS_GT811D)
	const unsigned char SYS_SOFTWARE_MAJOR_VERSION_NO = 1;
	const unsigned char SYS_SOFTWARE_RELEASE_VERSION_NO = 2;
	const unsigned char SYS_SOFTWARE_BRANCH_VERSION_NO = 6;
	const unsigned char SYS_SOFTWARE_DEBUG_VERSION_NO = 1;

	const unsigned char SYS_HARDWARE_MAJOR_VERSION_NO = 2;
	const unsigned char SYS_HARDWARE_RELEASE_VERSION_NO = 1;
	const unsigned char SYS_HARDWARE_BRANCH_VERSION_NO = 1;
	const unsigned char SYS_HARDWARE_DEBUG_VERSION_NO = 1;
#endif

#if (PRODUCT_CLASS == PRODUCTS_GT811G)
	const unsigned char SYS_SOFTWARE_MAJOR_VERSION_NO = 1;
	const unsigned char SYS_SOFTWARE_RELEASE_VERSION_NO = 2;
	const unsigned char SYS_SOFTWARE_BRANCH_VERSION_NO = 005;
	const unsigned char SYS_SOFTWARE_DEBUG_VERSION_NO = 1;

	const unsigned char SYS_HARDWARE_MAJOR_VERSION_NO = 2;
	const unsigned char SYS_HARDWARE_RELEASE_VERSION_NO = 1;
	const unsigned char SYS_HARDWARE_BRANCH_VERSION_NO = 1;
	const unsigned char SYS_HARDWARE_DEBUG_VERSION_NO = 1;
#endif


#define GWD_OAM_PKT_LENGTH	128

unsigned char gwdOamTrace = 0;
#define GWDOAMTRC               if(gwdOamTrace) diag_printf

/*for type define*/
enum GW_OAM_RESULT
{
	OAM_MESSAGE_RECEV_OK,
	OAM_MESSAGE_RECEV_ERR,
	OAM_MESSAGE_RECEV_NOT_COMPLETE,
	OAM_MESSAGE_RECEV_TOO_LONG,
	OAM_MESSAGE_RECEV_NO_MEM,
	OAM_MESSAGE_RECEV_OPCODE_OK,
	OAM_MESSAGE_RECEV_OPCODE_ERR,
	OAM_MESSAGE_SEND_OPCODE_OK,
	OAM_MESSAGE_SEND_OPCODE_ERR,
	OAM_MESSAGE_RECEV_TIMER_ERR,
	OAM_MESSAGE_SEND_ERROR,
	OAM_MESSAGE_SEND_OK
};


/*for global variables*/
oam_oui_t  oam_oui_gwd = {0x00, 0x0f, 0xe9};

cs_llid_t active_pon_port = CS_PON_PORT_ID;

char g_onu_boot_version[] = "V1.0";
GWTT_OAM_MESSAGE_NODE GwOamMessageListHead={0};

cyg_sem_t	OamListSem;

ONU_SYS_INFO_TOTAL onu_system_info_total;

unsigned char *irosbootver = "iROSBoot ONU 02.08.01 1286761672 Oct 11 2010";


unsigned long   gulDebugOamRx = 0;
unsigned long   gulDebugOamTx = 0;
unsigned long   gulDebugOamRxCount = 0;
unsigned long   gulDebugOamTxCount = 0;
unsigned long   gulDebugOamFileOp = 0;


int GwOamMessageListInit(void);

int Onu_Sysinfo_Get_From_Flash(void);

int Debug_Print_Tx_OAM(GWTT_OAM_HEADER *avender, unsigned char *pSentData);

int Debug_Print_Rx_OAM(GWTT_OAM_MESSAGE_NODE *pMessage);

int GwOamSendOpcodeCheck(unsigned char GwOpcode);

static int GwCommOamHeadBuild(GWTT_OAM_HEADER *pHead,  unsigned char GwOpcode,unsigned int SendSerNo,const unsigned short SendDataSize,unsigned char  *pSessionIdfield);

int Gwd_OAM_get_length_negotiation(unsigned short *pusOAMFrameLen);

void OamMessageRecevTimeOut(epon_timer_t *timer);

void GwOamMessageListNodeAdd(GWTT_OAM_MESSAGE_NODE *pNode);

GWTT_OAM_MESSAGE_NODE *GwOamMessageListNodeNew(unsigned short MessLen);

GWTT_OAM_MESSAGE_NODE *GwOamMessageListGetNode(unsigned int SerNo);

int GwOamMessageListNodeRem(GWTT_OAM_MESSAGE_NODE *pNode);

void GwOamMessageListNodeFree(GWTT_OAM_MESSAGE_NODE *pNode);

int CommOnuMsgSend(unsigned char GwOpcode, unsigned int SendSerNo, unsigned char *pSentData,const unsigned short SendDataSize, unsigned char  *pSessionIdfield);

int GwOamRecevOpcodeCheck(unsigned char GwOpcode );

int GwOamAlarmResponse(GWTT_OAM_MESSAGE_NODE *pRequest );

int GwOamInformationRequest(GWTT_OAM_MESSAGE_NODE *pRequest );

int CommOnuMsgReveive(GWTT_OAM_MESSAGE_NODE **ppMessage,unsigned char *MessagData);
void handle_gwd_oams(
        cs_port_id_t port,
        cs_uint8 *frame,
        cs_uint32 length);

extern cs_status local_dhcp_access_identifier_set(cs_uint8 *data);

extern void pkt_print(char *buf, cs_uint16 len)
{
    int i;
	cs_printf("pkt len :%d\n", len);
    cs_printf("---------------------------------------------------------");
    for(i = 0; i<len; ++i)
    {
        if(0 == (i&0xf)) cs_printf("\n%04x: ", i);
        if(0 == (i&0x7)) cs_printf(" ");
        cs_printf("%02x ", buf[i]);
    }
    cs_printf(".\n---------------------------------------------------------\n");

}

int GwOamMessageListInit(void)
{
	cyg_semaphore_init(&OamListSem, 1);
	return 0;
}

int Onu_Sysinfo_Save(void)
{
    unsigned char *buff=NULL;
    int size=0;
    int ret=0;

    buff=(unsigned char  *)&onu_system_info_total;
    size =sizeof (onu_system_info_total);

	ret = save_userdata_to_flash(buff, GWD_PRODUCT_CFG_OFFSET, size);

    return ret;
}


int Onu_Sysinfo_Get_From_Flash(void)
{
	int ret=GWD_RETURN_OK;
	int iLastChar;

	memset(&onu_system_info_total, 0, sizeof(onu_system_info_total));
	
	if (GWD_RETURN_OK != (ret = get_userdata_from_flash((unsigned char *)&onu_system_info_total, GWD_PRODUCT_CFG_OFFSET,  sizeof(onu_system_info_total))))
	{
		memset(&onu_system_info_total, 0, sizeof(onu_system_info_total));
		IROS_LOG_MAJ(IROS_MID_OAM, "Read system info from flash failed.(%d)\r\n", ret);
		ret = GWD_RETURN_ERR;
	}
	
	iLastChar = sizeof(onu_system_info_total.device_name) - 1;
	onu_system_info_total.device_name[iLastChar] = '\0';
	iLastChar = sizeof(onu_system_info_total.serial_no) - 1;
	onu_system_info_total.serial_no[iLastChar] = '\0';
	iLastChar = sizeof(onu_system_info_total.hw_manufature_date) - 1;
	onu_system_info_total.hw_manufature_date[iLastChar] = '\0';

	onu_system_info_total.product_type = DEVICE_TYPE_GT870;
	sprintf(onu_system_info_total.sw_version, "V%dR%02dB%03d", 
		SYS_SOFTWARE_MAJOR_VERSION_NO,
		SYS_SOFTWARE_RELEASE_VERSION_NO,
		SYS_SOFTWARE_BRANCH_VERSION_NO);
	
	return ret;
}

int Onu_Sysinfo_Get(void)
{
    return Onu_Sysinfo_Get_From_Flash();
}

#ifdef HAVE_EVENT_SHOW_CMD
static time_t g_timer = 0;
static cs_uint64 g_offset_time_s = 0;
void gw_time_set(const time_t *timer)
{
	g_timer = *timer;
	#if 0
	g_offset_time_s = cs_current_time()/100;
	#else
	// modified by wujm@2016-04-12,timer is too fast
	g_offset_time_s = cs_current_time()/1000;
	#endif
	
	return;
}

time_t gw_time_get(time_t *timer)
{
	cs_uint64 current_time_s = 0;
	#if 0
	current_time_s = cs_current_time()/100;
	#else
	// modified by wujm@2016-04-12,timer is too fast
	current_time_s = cs_current_time()/1000;
	#endif
	*timer = g_timer + current_time_s - g_offset_time_s;

	return *timer;
}
#endif


int Debug_Print_Tx_OAM(GWTT_OAM_HEADER *avender, unsigned char *pSentData)
{
    int i;
	unsigned short wholePktLen;			/* The whole packet length, including the fragments */
	unsigned short payloadOffset;		/* Offset in the entire packet */
	unsigned short payLoadLength;		/* Payload length in this packet */

	wholePktLen = /*VOS_NTOHS*/(avender->wholePktLen); 		/* LD modified*/
	payloadOffset = /*VOS_NTOHS*/(avender->payloadOffset);	/* LD modified*/
	payLoadLength = /*VOS_NTOHS*/(avender->payLoadLength);	/* LD modified*/
	
    diag_printf("\r\nTx OAM packet as following:\r\n");
    diag_printf("    oam_oui_gwd:        %02X-%02X-%02X\r\n", avender->oui[0], avender->oui[1], avender->oui[2]);
    diag_printf("    GwOpCode:     %02X", avender->opCode);
    switch (avender->opCode)
    {
        case EQU_DEVICE_INFO_REQ:
            diag_printf("(EQU_DEVICE_INFO_REQ)\r\n");
            break;
        case EQU_DEVICE_INFO_RESP:
            diag_printf("(EQU_DEVICE_INFO_RESP)\r\n");
            break;
        case ALARM_REQ:
            diag_printf("(ALARM_REQ)\r\n");
            break;
        case ALARM_RESP:
            diag_printf("(ALARM_RESP)\r\n");
            break;
        case FILE_READ_WRITE_REQ:
            diag_printf("(FILE_READ_WRITE_REQ)\r\n");
            break;
        case FILE_RESERVER:
            diag_printf("(FILE_RESERVER)\r\n");
            break;
        case FILE_TRANSFER_DATA:
            diag_printf("(FILE_TRANSFER_DATA)\r\n");
            break;
        case FILE_TRANSFER_ACK:
            diag_printf("(FILE_TRANSFER_ACK)\r\n");
            break;
        case CHURNING:
            diag_printf("(CHURNING)\r\n");
            break;
        case DBA:
            diag_printf("(DBA)\r\n");
            break;
        case SNMP_TRAN_REQ:
            diag_printf("(SNMP_TRAN_REQ)\r\n");
            break;
        case SNMP_TRAN_RESP:
            diag_printf("(SNMP_TRAN_RESP)\r\n");
            break;
        case SNMP_TRAN_TRAP:
            diag_printf("(SNMP_TRAN_TRAP)\r\n");
            break;
        case CLI_REQ_TRANSMIT:
            diag_printf("(CLI_REQ_TRANSMIT)\r\n");
            break;
        case CLI_RESP_TRANSMIT:
            diag_printf("(CLI_RESP_TRANSMIT)\r\n");
            break;
        case IGMP_AUTH_TRAN_REQ:
            diag_printf("(IGMP_AUTH_TRAN_REQ)\r\n");
            break;
        case IGMP_AUTH_TRAN_RESP:
            diag_printf("(IGMP_AUTH_TRAN_RESP)\r\n");
            break;
        case CLI_PTY_TRANSMIT:
            diag_printf("(CLI_PTY_TRANSMIT)\r\n");
            break;
        default:
            diag_printf("(unknown)\r\n");
            break;
    }
    diag_printf("    SendSerNo:    %u\r\n", (unsigned int)(avender->senderSerNo));
    diag_printf("    WholePktLen:  %u\r\n", wholePktLen);
    diag_printf("    PayloadOffSet:%u\r\n", payloadOffset);
    diag_printf("    payLoadLength:%u\r\n", payLoadLength);
    diag_printf("    SessionID:    ");
    for (i=0; i<8; i++)
        diag_printf("%02X", avender->sessionId[i]);
    diag_printf("\r\n    Payload: \r\n");
    for (i=0; i<payLoadLength; i++)
    {
        if ((i % 16) == 0)
            diag_printf("        ");
        diag_printf("%02X ", pSentData[i]);
        if ((i % 16) == 15)
            diag_printf("\r\n");
    }
    diag_printf("\r\n");
    diag_printf("Total Tx OAM frames'number this time: %u\r\n", (unsigned int)gulDebugOamTxCount);
    return GWD_RETURN_OK;
}

/* Functions for debug */
int Debug_Print_Rx_OAM(GWTT_OAM_MESSAGE_NODE *pMessage)
{
    int i;
    diag_printf("\r\nRx OAM packet as following:\r\n");
    diag_printf("    GwOpCode:    %02X", pMessage->GwOpcode);
    switch (pMessage->GwOpcode)
    {
        case EQU_DEVICE_INFO_REQ:
            diag_printf("(EQU_DEVICE_INFO_REQ)\r\n");
            break;
        case EQU_DEVICE_INFO_RESP:
            diag_printf("(EQU_DEVICE_INFO_RESP)\r\n");
            break;
        case ALARM_REQ:
            diag_printf("(ALARM_REQ)\r\n");
            break;
        case ALARM_RESP:
            diag_printf("(ALARM_RESP)\r\n");
            break;
        case FILE_READ_WRITE_REQ:
            diag_printf("(FILE_READ_WRITE_REQ)\r\n");
            break;
        case FILE_RESERVER:
            diag_printf("(FILE_RESERVER)\r\n");
            break;
        case FILE_TRANSFER_DATA:
            diag_printf("(FILE_TRANSFER_DATA)\r\n");
            break;
        case FILE_TRANSFER_ACK:
            diag_printf("(FILE_TRANSFER_ACK)\r\n");
            break;
        case CHURNING:
            diag_printf("(CHURNING)\r\n");
            break;
        case DBA:
            diag_printf("(DBA)\r\n");
            break;
        case SNMP_TRAN_REQ:
            diag_printf("(SNMP_TRAN_REQ)\r\n");
            break;
        case SNMP_TRAN_RESP:
            diag_printf("(SNMP_TRAN_RESP)\r\n");
            break;
        case SNMP_TRAN_TRAP:
            diag_printf("(SNMP_TRAN_TRAP)\r\n");
            break;
        case CLI_REQ_TRANSMIT:
            diag_printf("(CLI_REQ_TRANSMIT)\r\n");
            break;
        case CLI_RESP_TRANSMIT:
            diag_printf("(CLI_RESP_TRANSMIT)\r\n");
            break;
        case IGMP_AUTH_TRAN_REQ:
            diag_printf("(IGMP_AUTH_TRAN_REQ)\r\n");
            break;
        case IGMP_AUTH_TRAN_RESP:
            diag_printf("(IGMP_AUTH_TRAN_RESP)\r\n");
            break;
        case CLI_PTY_TRANSMIT:
            diag_printf("(CLI_PTY_TRANSMIT)\r\n");
            break;
        default:
            diag_printf("(unknown)\r\n");
            break;
    }
    diag_printf("    SendSerNo:   %u\r\n", pMessage->SendSerNo);
    diag_printf("    WholePktLen: %u\r\n", pMessage->WholePktLen);
    diag_printf("    RevPktLen:   %u\r\n", pMessage->RevPktLen);
    diag_printf("    SessionID:   ");
    for (i=0; i<8; i++)
        diag_printf("%02X", pMessage->SessionID[i]);
    diag_printf("\r\n    TimerID:     %u\r\n", (unsigned int)((pMessage->TimerID).opaque));
    diag_printf("    Payload: \r\n");
    for (i=0; i<pMessage->RevPktLen; i++)
    {
        if ((i % 16) == 0)
            diag_printf("        ");
        diag_printf("%02X ", pMessage->pPayLoad[i]);
        if ((i % 16) == 15)
            diag_printf("\r\n");
    }
    diag_printf("\r\n");
    diag_printf("Total Rx OAM frames'number: %u\r\n", (unsigned int)gulDebugOamRxCount);
    return GWD_RETURN_OK;
}

int GwOamSendOpcodeCheck(unsigned char GwOpcode)
{
	switch(GwOpcode)
	{
		case EQU_DEVICE_INFO_RESP:
		case ALARM_REQ:
		case FILE_READ_WRITE_REQ:
		case FILE_TRANSFER_DATA:
		case FILE_TRANSFER_ACK:
		case SNMP_TRAN_RESP:
		case SNMP_TRAN_TRAP:		
		case CLI_RESP_TRANSMIT:
		case IGMP_AUTH_TRAN_REQ:
		case IGMP_AUTH_TRAN_RESP:
		case CLI_PTY_TRANSMIT:
		case IGMP_TVM_REQ:
		case IGMP_TVM_RESP:
			break;
		default:
			return OAM_MESSAGE_RECEV_OPCODE_ERR;
	}
	return OAM_MESSAGE_RECEV_OPCODE_OK;

}

static int GwCommOamHeadBuild(GWTT_OAM_HEADER *pHead,  unsigned char GwOpcode,unsigned int SendSerNo,const unsigned short SendDataSize,unsigned char  *pSessionIdfield)
{
	oam_if_t *oamif = oam_intf_find(oam_sdl_get_llid());

    if(!oamif)
            return GWD_RETURN_ERR;

	oam_build_pdu_hdr(oamif, (oam_pdu_hdr_t *)pHead, OAM_PDU_CODE_ORG_SPEC);

	if((NULL == pHead)||(NULL == pSessionIdfield))
		return GWD_RETURN_ERR;
	if(OAM_MESSAGE_RECEV_OPCODE_ERR == GwOamSendOpcodeCheck(GwOpcode))
		return GWD_RETURN_ERR;

	pHead->oui[0] = 0x00;
	pHead->oui[1] = 0x0F;
	pHead->oui[2] = 0xE9;
	pHead->opCode = GwOpcode;
	pHead->senderSerNo = SendSerNo;
	pHead->wholePktLen = SendDataSize;
	if(NULL != pSessionIdfield)
		memcpy(pHead->sessionId,pSessionIdfield,8);
	return GWD_RETURN_OK;
}

int Gwd_OAM_get_length_negotiation(unsigned short *pusOAMFrameLen)
{
	*pusOAMFrameLen = GWD_OAM_PKT_LENGTH;

	return GWD_RETURN_OK;
}

void OamMessageRecevTimeOut(epon_timer_t *timer)
{
	GWTT_OAM_MESSAGE_NODE *pNode;
	unsigned int SerNo = (unsigned int)(timer->opaque);

	pNode = GwOamMessageListGetNode(SerNo);
	if(NULL != pNode)
		GwOamMessageListNodeFree(pNode);
}

void GwOamMessageListNodeAdd(GWTT_OAM_MESSAGE_NODE *pNode)
{
	if(NULL == pNode)
		return;
	if(NULL != GwOamMessageListGetNode(pNode->SendSerNo))
		IROS_LOG_CRI(IROS_MID_OAM, "GwOamMessageListNodeAdd::GwOamMessageListGetNode failed\n");
	cyg_semaphore_wait(&OamListSem);
	pNode->next = GwOamMessageListHead.next;
	GwOamMessageListHead.next = pNode;
	cyg_semaphore_post(&OamListSem);
}

GWTT_OAM_MESSAGE_NODE *GwOamMessageListNodeNew(unsigned short MessLen)
{
	GWTT_OAM_MESSAGE_NODE *pNode=NULL;

	pNode = iros_malloc(IROS_MID_OAM, sizeof(GWTT_OAM_MESSAGE_NODE));
	if(NULL!=pNode)
	{
		memset(pNode, '\0', sizeof(GWTT_OAM_MESSAGE_NODE));
	}
	else
	{
		return NULL;
	}

	if(0 !=MessLen )
	{
		pNode->pPayLoad = iros_malloc(IROS_MID_OAM, MessLen+1);
		if(NULL != pNode->pPayLoad )
		{
			memset(pNode->pPayLoad, '\0', MessLen+1);
		}
		else
		{
			iros_free(pNode);
			return NULL;
		}
	}
	return pNode;
}

GWTT_OAM_MESSAGE_NODE *GwOamMessageListGetNode(unsigned int SerNo)
{
	GWTT_OAM_MESSAGE_NODE *pNode;

	GWDOAMTRC("  GWD OAM handler - GwOamMessageListGetNode - SerNo: 0x%x.\n", SerNo);
	cyg_semaphore_wait(&OamListSem);
	GWDOAMTRC("  GWD OAM handler - GwOamMessageListGetNode - semWait: ok.\n");
	pNode = GwOamMessageListHead.next;
	while(NULL!=pNode)
	{
	
		if(SerNo==pNode->SendSerNo)
		{
			break;
		}
		else
		{
			pNode=pNode->next;
		}
	}
	cyg_semaphore_post(&OamListSem);
	return pNode;

}

int GwOamMessageListNodeRem(GWTT_OAM_MESSAGE_NODE *pNode)
{
	GWTT_OAM_MESSAGE_NODE *pPrNode=NULL;

	if(NULL == pNode)
		return GWD_RETURN_ERR;
	cyg_semaphore_wait(&OamListSem);
	pPrNode = &GwOamMessageListHead;
	while(NULL!=pPrNode->next)
	{
		if(pNode == pPrNode->next)
		{
			pPrNode->next=pNode->next;
			pNode->next = NULL;
			cyg_semaphore_post(&OamListSem);
			return GWD_RETURN_OK;
		}
		else
		{
			pPrNode=pPrNode->next;
		}
	}
	cyg_semaphore_post(&OamListSem);
	return ERROR;
}


void GwOamMessageListNodeFree(GWTT_OAM_MESSAGE_NODE *pNode)
{
	if(NULL == pNode)
		return;
	if(OK!=GwOamMessageListNodeRem(pNode))
		IROS_LOG_CRI(IROS_MID_OAM, "GwOamMessageListNodeFree::GwOamMessageListNodeRem failed\n");
	if(NULL != pNode->pPayLoad)
		iros_free(pNode->pPayLoad);
	iros_free(pNode);

	return;
}

int CommOnuMsgSend(unsigned char GwOpcode, unsigned int SendSerNo, unsigned char *pSentData,const unsigned short SendDataSize, unsigned char  *pSessionIdfield)
{
	unsigned char OamFrame[GWD_OAM_PKT_LENGTH] = {0};
	GWTT_OAM_HEADER *avender;
	unsigned short DataLenSended=0;
	unsigned short usOAMFrameLen;
	unsigned short usOAMPayloadLenGW;
	int	bSlowProtocol = FALSE;
	int	iSendPacketNumber = 0;
	cs_llid_t llid;
	cs_callback_context_t context;
	if(epon_request_onu_mpcp_llid_get(context, 0, 0, &llid) != CS_OK)
		return GWD_RETURN_ERR;
	if( GWD_RETURN_OK != Gwd_OAM_get_length_negotiation(&usOAMFrameLen) )
	{
		return GWD_RETURN_ERR;
	}
	else
	{
		usOAMPayloadLenGW = usOAMFrameLen - OAM_OVERHEAD_LEN_STD - OAM_OVERHEAD_LEN_GW;
	}

	if (CLI_RESP_TRANSMIT == GwOpcode)
	{
		bSlowProtocol = TRUE;
	}
	
	gulDebugOamTxCount = 0;
	if(SendDataSize > (OAM_DATA_LEN-sizeof(GWTT_OAM_HEADER)))
		return GWD_RETURN_ERR;
	avender = (GWTT_OAM_HEADER *)OamFrame;
	if(GWD_RETURN_OK != GwCommOamHeadBuild(avender,GwOpcode,SendSerNo,SendDataSize,pSessionIdfield))
		return OAM_MESSAGE_SEND_ERROR;
	
	if(usOAMPayloadLenGW < SendDataSize)
	{
		while((usOAMPayloadLenGW+DataLenSended) < SendDataSize)
		{
			avender->payLoadLength = usOAMPayloadLenGW;
			avender->payloadOffset = DataLenSended;
			memset(OamFrame+sizeof(GWTT_OAM_HEADER), '\0',GWD_OAM_PKT_LENGTH-sizeof(GWTT_OAM_HEADER));
			memcpy(OamFrame+sizeof(GWTT_OAM_HEADER),pSentData+DataLenSended,usOAMPayloadLenGW);
			oam_send(llid,active_pon_port, (unsigned char *)avender,(int)(usOAMPayloadLenGW + sizeof(GWTT_OAM_HEADER)));
			gulDebugOamTxCount++;
            OAM_TX_PACKET_DEBUG((avender, pSentData+DataLenSended));

			DataLenSended+=usOAMPayloadLenGW;

			iSendPacketNumber ++;
			if ((0 == (iSendPacketNumber % 10)) && (TRUE == bSlowProtocol))
			{
				cyg_thread_delay(1); /* 1 tick 10ms */
			}
		}

		avender->payLoadLength = SendDataSize-DataLenSended;
		avender->payloadOffset = DataLenSended;
		memset(OamFrame+sizeof(GWTT_OAM_HEADER), '\0',GWD_OAM_PKT_LENGTH-sizeof(GWTT_OAM_HEADER));
		memcpy(OamFrame+sizeof(GWTT_OAM_HEADER),pSentData+DataLenSended,SendDataSize-DataLenSended);
		oam_send(llid,active_pon_port, (unsigned char *)avender,(int)(sizeof(GWTT_OAM_HEADER) + SendDataSize - DataLenSended));
		gulDebugOamTxCount++;
        OAM_TX_PACKET_DEBUG((avender, pSentData+DataLenSended));
		return GWD_RETURN_OK;
	}
	else
	{
		avender->payLoadLength = SendDataSize;
		avender->payloadOffset = 0;
		memcpy(OamFrame+sizeof(GWTT_OAM_HEADER),pSentData,SendDataSize);

		oam_send(llid,active_pon_port, (unsigned char *)avender,(int)(sizeof(GWTT_OAM_HEADER)+SendDataSize));
		gulDebugOamTxCount++;
        OAM_TX_PACKET_DEBUG((avender, pSentData));
		return GWD_RETURN_OK;
	}
}

int GwOamRecevOpcodeCheck(unsigned char GwOpcode )
{
	switch(GwOpcode)
	{
		case EQU_DEVICE_INFO_REQ:
		case ALARM_RESP:
		case FILE_READ_WRITE_REQ:
		case FILE_TRANSFER_DATA:
		case FILE_TRANSFER_ACK:
		case SNMP_TRAN_REQ:
		case CLI_REQ_TRANSMIT:
		case IGMP_AUTH_TRAN_REQ:
		case IGMP_AUTH_TRAN_RESP:
		case CLI_PTY_TRANSMIT:
		case IGMP_TVM_REQ:
		case IGMP_TVM_RESP:
			break;
		default:
			return OAM_MESSAGE_RECEV_OPCODE_ERR;
	}
	return OAM_MESSAGE_RECEV_OPCODE_OK;
}


int GwOamAlarmResponse(GWTT_OAM_MESSAGE_NODE *pRequest )
{
	if(NULL == pRequest)
		return GWD_RETURN_ERR;
	if(NULL == pRequest->pPayLoad)
		return GWD_RETURN_ERR;
	switch((*pRequest->pPayLoad))
	{
		case ONU_TEMPRATURE_ALARM:
		{	
			break;
		}
		case ONU_PORT_LOOP_ALARM:
		{
			break;
		}
		case ONU_PORT_BROADCAST_STORM:
		{
			break;
		}
		case ONU_PORT_BROADCAST_STORM_ALARM:
		{
			break;
		}
		case ONU_ETH_PORT_STATE:
		{	
			break;
		}
		case ONU_ETH_PORT_ABILITY:
		{	
			break;
		}
		case ONU_ETH_WORK_STOP:
		{	
			break;
		}
		case ONU_STP_EVENT:
		{	
			break;
		}
		case ONU_DEVICE_INFO_CHANGE:
		{	
			break;
		}
		case ONU_SWITCH_STATUS_CHANGE_ALARM:
		{	
	        break;
		}
		case ONU_FILE_DOWNLOAD:
		{
			break;
		}
		default:
			return GWD_RETURN_ERR;
	}
	return GWD_RETURN_OK;
}


int GwOamInformationRequest(GWTT_OAM_MESSAGE_NODE *pRequest )
{
	unsigned char ver[4] = {1, 1, 1, 1};
	unsigned char Response[1024]={'\0'},*ptr, *pReq;
	unsigned char temp[128];
	int ResLen=0;
//	int i;
	unsigned short device_type;
	int tmpRet;

	if(NULL == pRequest)
		return GWD_RETURN_ERR;
	if(EQU_DEVICE_INFO_REQ != pRequest->GwOpcode)
		return GWD_RETURN_ERR;
	if(NULL == pRequest->pPayLoad)
		return GWD_RETURN_ERR;
	switch(*pRequest->pPayLoad)
	{
		case ONU_INFOR_GET:
		{
			GWDOAMTRC("EQU_DEVICE_INFO_REQ - ONU_INFOR_GET received.\n");
			ptr = Response;
			/* Payload */
			*ptr++  = ONU_INFOR_GET;	/* type : 1 for opCode 1's reply */

			/* Device Type */
			device_type = ONU_DEVICE_TYPE;
			SET_SHORT(ptr, device_type);
			ptr += sizeof(short);
			/* OUI */
			*ptr ++ = oam_oui_gwd[0];
			*ptr ++ = oam_oui_gwd[1];
			*ptr ++ = oam_oui_gwd[2];

			/* Contents */
			memset(temp, '\0', sizeof(temp));
			//ResLen = sprintf(temp, "V%d.%dB%d",SYS_HARDWARE_MAJOR_VERSION_NO, SYS_HARDWARE_RELEASE_VERSION_NO, SYS_HARDWARE_BRANCH_VERSION_NO);
			ResLen = sprintf(temp, "%s",onu_system_info_total.hw_version);
			*ptr++ = ResLen;
			sprintf(ptr,"%s",temp);
			ptr += ResLen;

			/*Boot Version*/
			if (ver[0] != 0)
			{
				ResLen = sprintf(temp, "%s",irosbootver);
				*ptr++ = ResLen;
				sprintf(ptr,"%s",temp);
				ptr += ResLen;
			}
			else
			{
				*ptr ++= (strlen(g_onu_boot_version));
				sprintf(ptr,"%s",g_onu_boot_version);
				ptr += (strlen(g_onu_boot_version));
			}
			
			/*Software version*/
			memset(temp, '\0', sizeof(temp));
			//ResLen = sprintf(temp, "%s",onu_system_info_total.sw_version);
			#if 1
			sprintf(temp,"V%dR%02dB%03d",
							SYS_SOFTWARE_MAJOR_VERSION_NO,
							SYS_SOFTWARE_RELEASE_VERSION_NO,
							SYS_SOFTWARE_BRANCH_VERSION_NO);
			#endif
			*ptr++ = strlen(temp);
			sprintf(ptr, "%s",temp);
			ptr += strlen(temp);

			/*Firmware version*/
			memset(temp, '\0', sizeof(temp));
			ResLen = sprintf(temp, "V%s.%s.%s.%s",IROS_ONU_APP_VER_MAJOR,IROS_ONU_APP_VER_MINOR,IROS_ONU_APP_VER_REVISION,IROS_ONU_APP_VER_BUILD);
			*ptr++ = ResLen;
			sprintf(ptr,"%s",temp);
			ptr += ResLen;

			/*Onu name*/
			ResLen = strlen(onu_system_info_total.device_name);
			if (ResLen > 128)
				ResLen = 128;
			*ptr++ = ResLen;
			memcpy(ptr, onu_system_info_total.device_name, ResLen);
			ptr += ResLen;

			/*Description*/
			ResLen = strlen("GT811-CTC-Ready");
			if (ResLen > 128)
				ResLen = 128;
			*ptr++ = ResLen;
			memcpy(ptr, "GT811-CTC-Ready", ResLen);
			ptr += ResLen;

			/*Location*/
			ResLen = strlen("Beijing China");
			if (ResLen > 128)
				ResLen = 128;
			*ptr ++= ResLen;
			memcpy(ptr,"Beijing China", ResLen);
			ptr += ResLen;

			/*Vendor*/
			ResLen = strlen("GW Delight");
			if (ResLen > 128)
				ResLen = 128;
			*ptr ++= ResLen;
			memcpy(ptr, "GW Delight", ResLen);
			ptr += ResLen;

			/*Serial Number*/
			ResLen = strlen(onu_system_info_total.serial_no);
			*ptr ++ = ResLen;
			memcpy(ptr, onu_system_info_total.serial_no, ResLen);
			ptr += ResLen;
			
			/*Manufacture Date*/
			ResLen = strlen(onu_system_info_total.hw_manufature_date);
			*ptr ++ = ResLen;
			memcpy(ptr, onu_system_info_total.hw_manufature_date, ResLen);
			ptr += ResLen;

			/*auto request*/

			*ptr ++ =0;

			/*slot num*/
			*ptr ++=0;

			/*ablity*/
			*ptr ++= 0xfe;
			
			*ptr ++= 3;

			cs_uint8 ablity_bits = 0;
			ablity_bits |= GWD_OAM_CAP_CTC_STATISTIC;	//是否支持ctc 性能查询功能, 1表示支持, 0表示不支持
			#ifdef HAVE_GWD_OAM_FAST_STATISTIC
			ablity_bits |= (GWD_OAM_CAP_CTC_FAST_STATISTIC);	//支持获取所有端口入口和出口方向的字节数统计数据,1表示支持, 0表示不支持
			ablity_bits |= (GWD_OAM_CAP_PON_STATISTIC);	//是否支持获取PON口的CTC统计数据，1表示支持, 0表示不支持
			#endif
			#ifdef HAVE_ONU_PORT_MAC_COUNT
			ablity_bits |= (GWD_OAM_CAP_MAC_COUNT_STATISTIC);	//是否支持ONU端口Mac地址统计，1表示支持, 0表示不支持
			#endif
			*ptr ++ = ablity_bits;

			ResLen = ((unsigned long)ptr-(unsigned long)Response);	
			#ifdef	HAVE_LOOP_DETECT_ALARM_BASE_OLT
			{
				gw_uint16	cell_flag;
				onu_infor_request_t *onu_infor_request = NULL;

				onu_infor_request = (onu_infor_request_t *)pRequest->pPayLoad;
				cell_flag = ntohs(onu_infor_request->cell_flag);

				if(cell_flag & (0x1<<0))
				{
					gw_cell_tlv_t *cell_tlv = NULL;

					if(NULL != (cell_tlv = gw_tlv_list_get_elem((gw_uint8 *)&onu_infor_request->cell_tlv, pRequest->WholePktLen, ONU_LOCAL_ID)))
					{
						onu_local_info_t *local = NULL;

						local = (onu_local_info_t *)(cell_tlv->cell_data);
						olt_inform_onu_local_infor_enable_set(true);
						olt_inform_onu_local_infor_set(local);
					}
				}
			}
			#endif
			break;
		}
		
		case ONU_INFOR_SET:
		{
			unsigned char nameLen, descrLen, locationLen;
			
			tmpRet = ONU_INFOR_SET<<8;
			pReq = pRequest->pPayLoad+1;

			ptr = Response;
			/* Payload */
			*ptr++  = ONU_INFOR_SET;	

			/* Name */
			nameLen = *pReq;
			pReq ++;
			if (nameLen)
			{
				unsigned char tmpLen;
				if (nameLen > sizeof(onu_system_info_total.device_name))
					tmpLen = sizeof(onu_system_info_total.device_name);
				else
					tmpLen = nameLen;
				
				Onu_Sysinfo_Get();
				memset(onu_system_info_total.device_name, 0, sizeof(onu_system_info_total.device_name));
				memcpy(onu_system_info_total.device_name, pReq, tmpLen);
				Onu_Sysinfo_Save();
				/* Success */
				*ptr ++ = 1;
				pReq += nameLen;
			}

			/* Description */
			descrLen = *pReq;
			pReq ++;
			if (descrLen)
			{
				/* Success */
				*ptr ++ = 1;
			
				pReq += descrLen;
			}

			/* Location */
			locationLen = *pReq;
			pReq ++;
			if (locationLen)
			{
				/* Success */
				*ptr ++ = 1;

				pReq += locationLen;
			}
			ResLen = ((unsigned int)ptr-(unsigned int)Response);
			break;
		}
		
		case ONU_REALTIME_SYNC:
		{
			
			#ifdef HAVE_EVENT_SHOW_CMD
			time_t time_sys;
			struct tm time_set;
			gw_realtime_sync_t *gw_realtime_sync_net = NULL;

			gw_realtime_sync_net = (gw_realtime_sync_t *)pRequest->pPayLoad;
			
			time_set.tm_year = ntohs(gw_realtime_sync_net->year)-1900;
			time_set.tm_mon = gw_realtime_sync_net->month-1;
			time_set.tm_mday = gw_realtime_sync_net->day;
			time_set.tm_hour = gw_realtime_sync_net->hour;
			time_set.tm_min = gw_realtime_sync_net->minute;
			time_set.tm_sec = gw_realtime_sync_net->second;
			
			time_sys = mktime(&time_set);
			
			gw_time_set(&time_sys);
			#endif
			break;
		}

		#ifdef	HAVE_GWD_DHCP_RELAY
		case ACCESS_IDENTIFIER:
		{
			unsigned short pReqlen = 0;
			unsigned int ret = ERROR;
			access_identifier_admin_t req_AdminInfoHead;
			access_identifier_admin_t *response_AdminInfoHead;
	
			ptr = Response;
			pReq= pRequest->pPayLoad;
			pReqlen = pRequest->RevPktLen;
			ResLen = pReqlen;
			response_AdminInfoHead = (access_identifier_admin_t*)Response;
			memcpy(Response,pRequest->pPayLoad,pReqlen);
			memset(&req_AdminInfoHead,0,sizeof(access_identifier_admin_t));
			memcpy(&req_AdminInfoHead,pReq,sizeof(access_identifier_admin_t));
			
			switch(req_AdminInfoHead.identifiermode)
			{
				case PPPOE_IDENTIFIER_MODE:
					response_AdminInfoHead->returnstatus=ACCESS_IDENT_SET_FAIL;
					break;
					
				case DHCP_IDENTIFIER_MODE:
					ret = Gwd_Func_Dhcp_relay_Oam_Admin_Process(pReq,pRequest->RevPktLen);
					if(ret != OK)
					{
						response_AdminInfoHead->returnstatus=ACCESS_IDENT_SET_FAIL;
					}
					else
					{
						response_AdminInfoHead->returnstatus=ACCESS_IDENT_SET_SUCCUSS;
					}
					response_AdminInfoHead->opcode=ACCESS_IDENTIFIER;
					break;
					
				default:
					response_AdminInfoHead->returnstatus=ACCESS_IDENT_SET_FAIL;
					break;
			}

			// added by wujm@2016-04-12,for SDK dhcpv4 option 82(v6 option 18) add 
			local_dhcp_access_identifier_set(pReq);
		}
		break;
		#endif
		
		#ifdef HAVE_GWD_OAM_FAST_STATISTIC
		case ONU_GWD_FAST_STATISTIC:
		{
			onu_gwd_dact_request_pdu_t *requestPdu = (onu_gwd_dact_request_pdu_t *)pRequest->pPayLoad;
			onu_gwd_dact_response_t *responsePdu = NULL;

			responsePdu = (onu_gwd_dact_response_t *)Response;
			memset(responsePdu, 0, sizeof(onu_gwd_dact_response_t));
			ResLen = (int)&(((onu_gwd_dact_response_t *)0)->first_port_octets);
			responsePdu->type = ONU_GWD_FAST_STATISTIC;
			responsePdu->data_type_bit_map = requestPdu->data_type_bit_map;
			if(FAST_STATISTIC_OCTETS == (ntohll(requestPdu->data_type_bit_map) | FAST_STATISTIC_OCTETS))
			{
				int i = 0;
				int port_num = 0;

				uni_port_num_get(&port_num);
				port_octets_t *Pport_octets = NULL;
				Pport_octets = (port_octets_t *)&(responsePdu->first_port_octets);
				memset(Pport_octets, 0, sizeof(port_octets_t)*port_num);

				for(i=0; i<=port_num; i++)
				{
					int port_id = i;
					cs_uint32 port_bit = ((cs_uint32)1<<(sizeof(cs_uint32)*8-1-i));
					cs_uint64 in_octets = 0;
					cs_uint64 out_octets = 0;

					if((cs_uint32)(-1) == requestPdu->port_bit_map)
					{
						//go on
					}
					else if(0 != (requestPdu->port_bit_map | port_bit))
					{
						//go on
					}
					else
					{
						//skip
						continue;
					}

					if(CS_E_OK == onu_port_stats_octets_get(port_id, &in_octets, &out_octets))
					{
						responsePdu->port_bit_map = htonl(ntohl(responsePdu->port_bit_map)|port_bit);
						Pport_octets->in_octets = htonll(in_octets);
						Pport_octets->out_octets = htonll(out_octets);
						ResLen += sizeof(port_octets_t);
						Pport_octets++;
					}
					else
					{
						responsePdu->port_bit_map = htonl(ntohl(responsePdu->port_bit_map)&(~port_bit));
					}

				}

				responsePdu->result = FAST_STATISTIC_GET_SUCCESS;
			}
			else
			{
				responsePdu->result = FAST_STATISTIC_GET_FAILED;
			}

		}
		break;
		#endif	/*(#ifdef HAVE_GWD_OAM_FAST_STATISTIC)*/
		
		#ifdef HAVE_USER_MAC_TRACE
		case ONU_LOCATE_USER:
		{
			cs_uint8 swmac[6] = {0};
			cs_uint8 subsw = 0;
			cs_uint8 onuslot =0;
			cs_uint8 onuport =0;
			cs_uint32 swport;
			userMacRequest_pdu_t *requestPdu = (userMacRequest_pdu_t *)pRequest->pPayLoad;
			userMacResponse_pdu_t *responsePdu = NULL;
			userMacResponse_t responseInfo[32];
			int requestNum = 0;
			int responseNum = 0;
			cs_uint8 *tempP = Response;
	
			memset(responseInfo, 0, sizeof(responseInfo));
			responsePdu = iros_malloc(IROS_MID_OAM, sizeof(userMacResponse_pdu_t));
			if (NULL == responsePdu)
			{
				break;
			}
	
			ResLen = sizeof(userMacResponse_pdu_t);

			for (requestNum = 0; requestNum < requestPdu->macNum; requestNum++)
			{
				/*generating response pdu only for found the mac because of OLT broadcast oam request*/
				if(CS_E_OK == locateUserMac( requestPdu->info[requestNum].swmac, &onuslot, &onuport, &subsw, swmac, &swport ))
				{
					memcpy(responseInfo[responseNum].usermac, requestPdu->info[requestNum].swmac, 6);/*USR MAC*/
					responseInfo[responseNum].reserved = 0;
					responseInfo[responseNum].onuslot = onuslot;/*SLOT 单槽位为0*/
					responseInfo[responseNum].onuport = onuport;
					responseInfo[responseNum].subsw = subsw;
					memcpy(responseInfo[responseNum].swmac, swmac, 6);
					responseInfo[responseNum].swport = swport;
	
					responseNum++;
					ResLen += sizeof(userMacResponse_t);
				}
	
				if (responseNum >= 32)
				{
					break;
				}
			}
	
			ResLen += 1;/*reserved bit.*/

			if (0 != responseNum)
			{
				responsePdu->type = ONU_LOCATE_USER;/*cheak type*/
				responsePdu->result = 1;/*查询结果*/
				responsePdu->mode	= USR_MAC_ADDRES_CHEAK;/*查询模式*/ 			
				responsePdu->macNum = responseNum;/*查到的MAC地址数*/
				
				memcpy(tempP, responsePdu, sizeof(userMacResponse_pdu_t));
				tempP += sizeof(userMacResponse_pdu_t);
	
				memcpy(tempP, responseInfo, (responseNum*sizeof(userMacResponse_t)));
				iros_free(responsePdu);
				responsePdu = NULL;
			}
			else
			{
				ResLen = 0;
				iros_free(responsePdu);
				responsePdu = NULL;
				return CS_E_ERROR;/*如果查询的MAC 都没有找到不回复，直接丢弃*/
			}
		}
		break;
		#endif	/*(#ifdef HAVE_USER_MAC_TRACE)*/

		#ifdef HAVE_OPTICAL_MODULE_PARAMETERS_QUERY
		case ONU_EPON_PARAMETERS_GET:
		{
			cs_callback_context_t	context;
			cs_int16 	temperature;
			cs_uint16	voltage;
			cs_uint16	tx_bias;
			cs_uint16	tx_power;
			cs_uint16	rx_power;
			epon_parameters_response_t epon_parameters_response;
			cs_uint8 *tempP = Response;

			cs_plat_opm_status_get(context, 0, 0, &temperature, &voltage, &tx_bias, &tx_power, &rx_power);

			memset(&epon_parameters_response, 0, sizeof(epon_parameters_response));
			epon_parameters_response.type = ONU_EPON_PARAMETERS_GET;
			epon_parameters_response.result = 0;
			epon_parameters_response.tx_power = htons(tx_power);
			epon_parameters_response.rx_power = htons(rx_power);
			epon_parameters_response.temperature = htons(temperature);
			epon_parameters_response.voltage = htons(voltage);
			epon_parameters_response.tx_bias = htons(tx_bias);

			memcpy(tempP, &epon_parameters_response, sizeof(epon_parameters_response));
			ResLen = sizeof(epon_parameters_response);
		}
		break;
		#endif	/*(#ifdef HAVE_OPTICAL_MODULE_PARAMETERS_QUERY)*/
		
		#ifdef HAVE_ONU_PORT_MAC_COUNT
		case ONU_PORT_MAC_COUNT:
		{
			PortMacCountRequest_pdu_t *requestPdu = (PortMacCountRequest_pdu_t *)pRequest->pPayLoad;
			PortMacCountResponse_t *responsePdu = NULL;
			int port_count = 1;
			int i = 0;
			cs_port_id_t port_id;
			cs_uint32 mac_count = 0;
			port_mac_count_t *Pport_mac_count = NULL;

			if(0xff == requestPdu->port_index)	//获取所有端口
			{
				uni_port_num_get(&port_count);
				port_id = CS_UNI_PORT_ID1;
			}
			else	//获取特定的一个端口
			{
				port_count = 1;
				port_id = requestPdu->port_index;
			}
			
			responsePdu = (PortMacCountResponse_t *)Response;
			ResLen = sizeof(PortMacCountResponse_t) + sizeof(port_mac_count_t) * (port_count-1);
			memset(responsePdu, 0, ResLen);
			
			responsePdu->type = ONU_PORT_MAC_COUNT;
			responsePdu->result = 1;
			responsePdu->port_count = port_count;
			Pport_mac_count = &(responsePdu->first_port_mac_count);

			mac_num_update_hw_to_sw();
			for(i=0; i<port_count; i++, port_id++, Pport_mac_count++)
			{
				mac_count = 0;
				if(CS_E_OK == port_mac_count_get(port_id, &mac_count))
				{
					Pport_mac_count->port_index = port_id;
					Pport_mac_count->mac_count = htonl(mac_count);
				}
			}
		}
		break;
		#endif	/*(#ifdef HAVE_ONU_PORT_MAC_COUNT)*/
		
		#ifdef HAVE_GWD_LOOP_DETECT
		case ONU_LPB_DETECT:
		{
			loop_back_control_pkt_proc(pRequest->pPayLoad, pRequest->WholePktLen);
		}
		break;
		#endif
		
		default:
		{
//			IROS_LOG_MAJ(IROS_MID_OAM, "OAM INFO request (%d) no suportted!", *pRequest->pPayLoad);
			GWDOAMTRC("EQU_DEVICE_INFO_REQ - unknown received.(%d)\n", *pRequest->pPayLoad);
			return GWD_RETURN_ERR;
		}
	}
	return (CommOnuMsgSend(EQU_DEVICE_INFO_RESP, pRequest->SendSerNo, Response, ResLen, pRequest->SessionID));
}

int CommOnuMsgReveive(GWTT_OAM_MESSAGE_NODE **ppMessage,unsigned char *MessagData)
{
	GWTT_OAM_MESSAGE_NODE *pMessageNode=NULL;
	GWTT_OAM_HEADER *vender_header=NULL;
	unsigned char *payload=NULL;
	unsigned short PayLoadOffset=0;
	unsigned short PayloadLength=0;
	unsigned int WholePktLen=0;

	if(NULL == ppMessage)
		return OAM_MESSAGE_RECEV_ERR;	
	GWDOAMTRC("  GWD OAM handler - CommOnuMsgReveive\n");
	vender_header = (GWTT_OAM_HEADER*)MessagData;
	if (0 == memcmp(vender_header->oui, oam_oui_gwd, sizeof(oam_oui_gwd)))
	{
		/* GWTT Extend OAM */
		if(OAM_MESSAGE_RECEV_OPCODE_ERR==GwOamRecevOpcodeCheck(vender_header->opCode))
			return OAM_MESSAGE_RECEV_OPCODE_ERR;	
		GWDOAMTRC("  GWD OAM handler - CommOnuMsgReveive - opCode check OK.\n");
		WholePktLen = ntohs(vender_header->wholePktLen);
		GWDOAMTRC("  GWD OAM handler - CommOnuMsgReveive - PktLen = %d.\n", WholePktLen);
		if(OAM_DATA_LEN < WholePktLen)
			return OAM_MESSAGE_RECEV_TOO_LONG;
		payload = MessagData+sizeof(GWTT_OAM_HEADER);
		PayLoadOffset = ntohs(vender_header->payloadOffset);
		PayloadLength = ntohs(vender_header->payLoadLength);

	 	if (0 == PayLoadOffset)
	 	{
			pMessageNode = GwOamMessageListGetNode(vender_header->senderSerNo);
			if(NULL!=pMessageNode)
				return OAM_MESSAGE_RECEV_ERR;

			if(WholePktLen==PayloadLength)
			{
				pMessageNode = GwOamMessageListNodeNew(WholePktLen);
				if(NULL == pMessageNode)
					return OAM_MESSAGE_RECEV_NO_MEM;
				pMessageNode->GwOpcode=vender_header->opCode;
				pMessageNode->RevPktLen=PayloadLength;
				pMessageNode->WholePktLen=PayloadLength;
				pMessageNode->SendSerNo=vender_header->senderSerNo;
				memcpy(pMessageNode->SessionID,vender_header->sessionId,8);
				memcpy(pMessageNode->pPayLoad,payload,PayloadLength);
				GwOamMessageListNodeAdd(pMessageNode);	
				*ppMessage = pMessageNode;
				GWDOAMTRC("  GWD OAM handler - CommOnuMsgReveive - success 1.\n");
				return OAM_MESSAGE_RECEV_OK;
			}
			else
			{
				pMessageNode = GwOamMessageListNodeNew(WholePktLen);
				if(NULL == pMessageNode)
					return OAM_MESSAGE_RECEV_NO_MEM;
				pMessageNode->GwOpcode=vender_header->opCode;
				pMessageNode->RevPktLen=PayloadLength;
				pMessageNode->WholePktLen=WholePktLen;
				pMessageNode->SendSerNo=vender_header->senderSerNo;
				memcpy(pMessageNode->SessionID,vender_header->sessionId,8);
				memcpy(pMessageNode->pPayLoad,payload,PayloadLength);
				GwOamMessageListNodeAdd(pMessageNode);	
				*ppMessage = NULL;
				(pMessageNode->TimerID).opaque = (void *)(pMessageNode->SendSerNo);
				epon_timer_add(&(pMessageNode->TimerID), OamMessageRecevTimeOut, WAIT_TIME_FOR_OAM_MESSAGE);
				/*pMessageNode->TimerID=VOS_TimerCreate(MODULE_RPU_OAM, (unsigned long)NULL, WAIT_TIME_FOR_OAM_MESSAGE, OamMessageRecevTimeOut, pMessageNode->SendSerNo, VOS_TIMER_NO_LOOP);
				if(0 == pMessageNode->TimerID)
				{
					GwOamMessageListNodeFree(pMessageNode);
					return OAM_MESSAGE_RECEV_TIMER_ERR;
				}*/			
			}
		}
		else
		{
			pMessageNode = GwOamMessageListGetNode(vender_header->senderSerNo);
			if(NULL==pMessageNode)
			{
				return OAM_MESSAGE_RECEV_ERR;
			}
			if(((pMessageNode->RevPktLen+PayloadLength) > pMessageNode->WholePktLen)||
				(pMessageNode->RevPktLen!= PayLoadOffset))			
			{
				return OAM_MESSAGE_RECEV_ERR;
			}
			if((pMessageNode->RevPktLen+PayloadLength) == pMessageNode->WholePktLen)
			{
				epon_timer_del(&(pMessageNode->TimerID));
				memcpy(pMessageNode->pPayLoad+pMessageNode->RevPktLen,payload,PayloadLength);
				pMessageNode->RevPktLen = pMessageNode->WholePktLen;
				*ppMessage = pMessageNode;
				GWDOAMTRC("  GWD OAM handler - CommOnuMsgReveive - success 2.\n");
				return OAM_MESSAGE_RECEV_OK;
			}
			else
			{
				epon_timer_del(&(pMessageNode->TimerID));
				epon_timer_add(&(pMessageNode->TimerID), (pMessageNode->TimerID).tmfunc, WAIT_TIME_FOR_OAM_MESSAGE);
				memcpy(pMessageNode->pPayLoad+pMessageNode->RevPktLen,payload,PayloadLength);
				pMessageNode->RevPktLen+=PayloadLength;
				*ppMessage = NULL;
				return OAM_MESSAGE_RECEV_NOT_COMPLETE;
			}		
		}	
	}
	
	return OAM_MESSAGE_RECEV_ERR;
}


void handle_gwd_oams(
        cs_port_id_t port,
        cs_uint8 *frame,
        cs_uint32 length)
{
	GWTT_OAM_MESSAGE_NODE *pMessage = NULL;
	int iRet;
	
	/*extern unsigned long cl_sn_service_status ;*/
	if(!frame || !port)
		return;

	if(length < sizeof(GWTT_OAM_HEADER))
		return;

	GWDOAMTRC("Gwd_Oam_Handle - (%d, %x %x %x %x %x %x %d)\n", port, 
		frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], length);
	if(OAM_MESSAGE_RECEV_OK != (iRet = CommOnuMsgReveive(&pMessage, frame)))
	{
		IROS_LOG_MAJ(IROS_MID_OAM, "GW OAM receiving error!\r\n");
		GWDOAMTRC("  GWD OAM handler - CommOnuMsgReveive - failed(%d).\n", iRet);
		return;
	}
	
	if(NULL == pMessage)
	{
		return;
	}

	/* Then process the packet */
	gulDebugOamRxCount++;
	OAM_RX_PACKET_DEBUG((pMessage));
	switch(pMessage->GwOpcode)
	{
		case EQU_DEVICE_INFO_REQ:
			GWDOAMTRC("Gwd_Oam_Handle - EQU_DEVICE_INFO_REQ received.\n");
			if(GWD_RETURN_OK != (iRet = GwOamInformationRequest(pMessage)))
			{
//				IROS_LOG_MAJ(IROS_MID_OAM, "Generate OAM(Information Request) response Error!(%d)", iRet);
				GWDOAMTRC("Gwd_Oam_Handle - EQU_DEVICE_INFO_REQ failed.(%d)\n", iRet);
			}
			GwOamMessageListNodeFree(pMessage);
			pMessage = NULL;
			break;

		case ALARM_RESP:
			if(GWD_RETURN_OK != GwOamAlarmResponse(pMessage))
			{
				IROS_LOG_MAJ(IROS_MID_OAM, "Deal with OLT Alarm response Error!");
			}
			GwOamMessageListNodeFree(pMessage);
			pMessage = NULL;
			break;

		#ifdef HAVE_GWD_OAM_PTY
		case CLI_PTY_TRANSMIT:
		{
			if(GWD_RETURN_OK != gwd_oam_pty_trans(pMessage))
			{
				IROS_LOG_MAJ(IROS_MID_OAM, "Deal with oam pty pkt Error!");
			}
			
			GwOamMessageListNodeFree(pMessage);
			pMessage = NULL;
			break;	
		}
		#endif

		#ifdef HAVE_GWD_MC_TVM
		case IGMP_TVM_REQ:
			GwOamTvmRequestRecv((void *)pMessage);
			GwOamMessageListNodeFree(pMessage);
			pMessage = NULL;
			break;
		#endif
		
		case FILE_READ_WRITE_REQ:
		case FILE_TRANSFER_DATA:
		case FILE_TRANSFER_ACK:
		case SNMP_TRAN_REQ:
		case CLI_REQ_TRANSMIT:
		case IGMP_AUTH_TRAN_REQ:
		case IGMP_AUTH_TRAN_RESP:
		default:
			GWDOAMTRC("Gwd_Oam_Handle - unknown opcode(%d) received.\n", pMessage->GwOpcode);
			IROS_LOG_MAJ(IROS_MID_OAM, "Received an unknown packet(0x%x), drop it!\r\n", pMessage->GwOpcode);
			GwOamMessageListNodeFree(pMessage);
			break;
	}
}


void getOltMacAddress(epon_macaddr_t * olt_mac)
{
	volatile EPON_t *g_epon_base = (volatile EPON_t *)&g_lynxd_reg_ptr->epon;

	ONU_MAC_MPCP_OLTMAC1_t olt_mac1 = g_epon_base->MPCP_OLTMAC1;
	ONU_MAC_MPCP_OLTMAC0_t olt_mac0 = g_epon_base->MPCP_OLTMAC0;

	cs_uint8 *mac = (cs_uint8*)olt_mac;

	if(mac == NULL)
	return;

	mac[0] = (olt_mac1.bf.olt_mac >> 8) & 0xff;
	mac[1] = (olt_mac1.bf.olt_mac) & 0xff;
	mac[2] = (olt_mac0.bf.olt_mac >>24) & 0xff;
	mac[3] = (olt_mac0.bf.olt_mac >>16) & 0xff;
	mac[4] = (olt_mac0.bf.olt_mac >>8)  & 0xff;
	mac[5] = (olt_mac0.bf.olt_mac) & 0xff;

}

int GwGetOltType(unsigned char *mac, GWD_OLT_TYPE *type)
{
	unsigned char gwPonMac[6] = { 0x00, 0x0c, 0xd5, 0x00, 0x01, 0x00 };

	if( 0 == memcmp(mac, gwPonMac, 5)) /* old type, parsed as before*/
	{
		if((mac[5] == 0x0) ||(mac[5] == 0x10) ||
		(mac[5] == 0x20) ||(mac[5] == 0x30))
			*type = GWD_OLT_GFA6100;
		else if(mac[5] >= 0xec)
			*type = GWD_OLT_GFA6700;
		else
			*type = GWD_OLT_NOMATCH;
	}
	else if( 0 == memcmp(mac, gwPonMac, 3)) /*new type, parsed in new formula*/
	{
		if(mac[3] == 0x61)
			*type = GWD_OLT_GFA6100;
		else if(mac[3] == 0x67)
			*type = GWD_OLT_GFA6700;
		else if(mac[3] == 0x69)
			*type = GWD_OLT_GFA6900;
		else
			*type = GWD_OLT_NOMATCH;
	}
	else
		*type = GWD_OLT_NONE;

	return GWD_RETURN_OK;
}

cs_status 
onu_mac_get(
	CS_OUT	cs_mac_t *mac
	)
{
	#ifdef TEST
	cs_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	cs_callback_context_t context;
	
	return epon_request_onu_pon_mac_addr_get(context, 0, 0, mac);
}

int GwGetPonSlotPort(unsigned char *mac, GWD_OLT_TYPE type, unsigned long *slot, unsigned long *port)
{
	unsigned char ponMac;
	unsigned char gwPonMac[6] = { 0x00, 0x0c, 0xd5, 0x00, 0x01, 0x00 };
	ponMac = mac[5];
	if( 0 == memcmp(mac, gwPonMac, 5)) /* old type, parsed as before*/	
	{
		switch(type)
		{
			case GWD_OLT_GFA6100 :
				switch(ponMac)
				{
					case 0 :
						*slot = 2; *port = 1;
						break;
					case 0x10 : 
						*slot = 2; *port = 2;
						break;
					case 0x20 :
						*slot = 3; *port = 1;
						break;
					case 0x30 :
						*slot = 3; *port = 2;
						break;
					default:
						return GWD_RETURN_ERR;
				}
				break;
			case GWD_OLT_GFA6700 :
				*slot = ((ponMac & 0x1c)>>2) + 1;
				*port = ((ponMac & 0x3)) + 1;
				break;
			default :				  /*others*/
				*slot = 0xff;
				*port = 0xff;
				return GWD_RETURN_ERR;
		}
	}
	else if( 0 == memcmp(mac, gwPonMac, 3))	/*new type, parsed in new formula*/
	{
		*slot = mac[4];
		*port = mac[5];
	}
	else 
		return GWD_RETURN_ERR;

	return GWD_RETURN_OK;
}

cs_uint8 onu_product_ID_get()
{
#if (PRODUCT_CLASS == PRODUCTS_GT811D)
	return DEVICE_TYPE_GT811D;
#elif(PRODUCT_CLASS == PRODUCTS_GT811G)
	return DEVICE_TYPE_GT811G;
#elif(PRODUCT_CLASS == PRODUCTS_GT873_M_4F4S)
	return DEVICE_TYPE_GT873_M_4F4S;
#endif
}


void oam_gwd_init(void)
{
	oam_vendor_handlers_t gwd_handle;

	GwOamMessageListInit();
	oam_vendor_handlers_init(&gwd_handle);

	gwd_handle.org_handler = handle_gwd_oams;

	oam_vendor_handler_register(oam_oui_gwd, gwd_handle);

}

#ifdef HAVE_ARP_DOWN_INTO_CPU_LIMIT
void arp_down_into_cpu_monitor(void);
#endif

#ifdef HAVE_RESET_CONTROL
void onu_reset_enable_init(void);
#endif

#ifdef HAVE_MPCP_REG_TIMEOUT_RESET
void mpcp_reg_time_out_monitor();
#endif

#ifdef HAVE_DHCPV4_SUPPORT
void dhcpv4_init(void)
{
	dhcp_option_enable_t      enable;

	memset(&enable,0,sizeof(enable));
	
	// 使能 dhcpv4 报文增加 option 82
	enable.mask = DHCP_V4_OPTION82_MASK_BIT;
	enable.v4_option82 = true;
	dhcp_option_enable(&enable);
}
#endif

#ifdef HAVE_DHCPV6_SUPPORT
void dhcpv6_init(void)
{
	cs_callback_context_t     context;
	dhcp_option_enable_t      enable;

	memset(&enable,0,sizeof(enable));
	
	// 使能 dhcpv6 报文增加 option 18
	enable.mask = DHCP_V6_OPTION18_MASK_BIT;
	enable.v6_option18 = true;
	dhcp_option_enable(&enable);

	// 设置dhcpv6 报文进入CPU
	epon_request_onu_spec_pkt_state_set(context, 0, 0, S_UPSTREAM, CS_PKT_DHCPV6, DST_CPU);
}
#endif

#ifdef HAVE_CTC_LOOP_DETECT_DEFAULT_CONFIG_ENABLE
extern cs_status loop_detect_enable(cs_port_id_t port);
cs_status loop_detect_port_block_enable(cs_port_id_t port, cs_boolean enable);

void ctc_loop_detect_init(void)
{
    cs_port_id_t port = 0;
    cs_boolean  need_block = true;

    for(port = CS_UNI_PORT_ID1;port <= UNI_PORT_MAX;port++)
    {
        loop_detect_enable(port);
        
        #ifdef HAVE_CTC_LOOP_CONTROL_DEFAULT_CONFIG_ENABLE
        loop_detect_port_block_enable(port,need_block);
        #endif
    }
    
}

#endif

#ifdef HAVE_ATU_INIT
extern int fdb_learn_set(int portid, int en);

void atu_init(void)
{
    cs_port_id_t port = 0;
    int  en = true;

    for(port = CS_UNI_PORT_ID1;port <= UNI_PORT_MAX;port++)
    {
        fdb_learn_set(port,en);
    }
}
#endif

void gwd_onu_init(void)
{
	#ifdef HAVE_GWD_OAM_PTY
	init_oam_pty();
	#endif

	Onu_Sysinfo_Get();	//onu_system_info_total information need to restore from flash before onu ctc register

	#ifdef HAVE_GWD_MC_TVM
	igmp_control_table_init();
	#endif

	#ifdef HAVE_GWD_DHCP_RELAY
	Gwd_func_dhcp_pkt_process_init();
	#endif

	#ifdef HAVE_GWD_BC_LIMIT
	gw_broadcast_storm_init();
	#endif

	#ifdef HAVE_ONU_PORT_MAC_COUNT
	mac_num_hw_to_sw_init();
	#endif

	#ifdef HAVE_ARP_DOWN_INTO_CPU_LIMIT
	arp_down_into_cpu_monitor();
	#endif
	
	#ifdef HAVE_RESET_CONTROL
	onu_reset_enable_init();
	#endif
	
	#ifdef HAVE_MPCP_REG_TIMEOUT_RESET
	mpcp_reg_time_out_monitor();
	#endif

//	#ifdef HAVE_LOOP_DETECT
	loop_back_detect_init();
//	#endif
	
	#ifdef HAVE_DHCPV4_SUPPORT
	dhcpv4_init();
	#endif
	
	#ifdef HAVE_DHCPV6_SUPPORT
	dhcpv6_init();
	#endif

        #ifdef HAVE_CTC_LOOP_DETECT_DEFAULT_CONFIG_ENABLE
        ctc_loop_detect_init();
        #endif

        #ifdef HAVE_ATU_INIT
        atu_init();
        #endif
	
/*-------------------------------------------------------------*/
	/*restore config should be at the end of gwd onu init*/
	#ifdef HAVE_GWD_FLASH_OPER
	start_up_config_syn_to_running_config();
	#endif
	
}

#endif	/*end of the file*/
