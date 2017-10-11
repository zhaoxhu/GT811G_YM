#ifndef __IROS_CONFIG_H
#define __IROS_CONFIG_H
#define HAVE_MPORTS 1
#define HAVE_SYS_MEMPOOL 1
#define HAVE_MEM_TRACE 1
#define HAVE_MINI_CLI 1
#define HAVE_LOG_THREAD 1
#define HAVE_EXCEPTION_HANDLER 1
#define HAVE_CPULOAD_PER_THREAD 1
#define HAVE_IP_STACK 1
#define HAVE_WEB_SERVER 1
#define HAVE_FLASH_FS 1
#define HAVE_CTC_OAM 1
#define HAVE_POSIX 1
#define HAVE_SERIAL_FLASH 1
#define HAVE_SDL_CMD 1
#define HAVE_SDL_CMD_CTC 1
#define HAVE_RTK_CMD 1
#define HAVE_SDL_CTC 1
#define HAVE_DB_MANAGMENT 1
#define HAVE_MC_CTRL 1
#define HAVE_LOG_FILE 1
#define HAVE_ONU_RSTP 1
//#define HAVE_LOOP_DETECT 1
//#define HAVE_PPPOE 1
//#define HAVE_PPP 1
#define HAVE_TELNET_CLI 1
#define HAVE_MIF 1


/*--------------------------gwd start-------------------------------*/
#define HAVE_STANDARD_CLI
/*sdk design:when onu start,if HAVE_MINI_CLI defined start mini cli,else start standard cli. 
after modified :when onu start, if HAVE_STANDARD_CLI defined start standard cli and there is the command that can entry mini cli,
else if HAVE_MINI_CLI defined start mini cli*/

#define HAVE_GWD_OAM

#define MODULE_YES	1
#define MODULE_NO	0

#define PRODUCTS_GT811D			1
#define PRODUCTS_GT811G			2
#define PRODUCTS_GT873_M_4F4S	3

//load product type
#include "products_config.h"

#if (PRODUCT_CLASS == PRODUCTS_GT811D)
#define HAVE_SWITCH_SPEED_100	MODULE_YES
#define HAVE_SWITCH_SPEED_1000	MODULE_NO


#elif (PRODUCT_CLASS == PRODUCTS_GT811G)
#define HAVE_SWITCH_SPEED_100	MODULE_NO
#define HAVE_SWITCH_SPEED_1000	MODULE_YES


#elif (PRODUCT_CLASS == PRODUCTS_GT873_M_4F4S)
#define HAVE_SWITCH_SPEED_100	MODULE_YES
#define HAVE_SWITCH_SPEED_1000	MODULE_NO
#define HAVE_TERMINAL_SERVER	1


#else
#warning "PRODUCT_CLASS define is unknnow!!"
#endif

/*local GE rate limit*/
#ifdef	HAVE_SWITCH_SPEED_1000
#define HAVE_GE_RATE_LIMIIT_CMD		MODULE_YES
#define HAVE_GE_RATE_LIMIIT_RESTORE	MODULE_YES
#endif


#define HAVE_GWD_OAM_PTY
//#define HAVE_GWD_VLAN_MODE
#define HAVE_MC_REPORT_V3_TO_V2				/*olt don't support mc report v3 to v2, so onu do it*/

/*ctc mc mode got only have 2 value,0 is for snooping, 1 is for authentication, so transparent cann't be got.
we consult with gwd olt,when onu return not support,it means transparent*/
#define HAVE_MC_TRANSPARENT_GET_GWD	

/*gwd cli*/
#define HAVE_GWD_CLI

/*onu add another network card, it is only used for tcp and udp service*/
#define HAVE_TCP_UDP_NETWORK_CARD

/*gwd flash oper funtion*/
#define HAVE_GWD_FLASH_OPER

/*Whether to allow restore  user name and password from hardware*/
//#define HAVE_USER_PASSWORD_RESTORE

/*onu register to olt at the end of main init*/
#define HAVE_ONU_REGISTER_LAST

/*gwd igmp command*/
#define HAVE_GWD_IGMP_CMD

/*base on HAVE_GWD_FLASH_OPER, save and restore igmp mode*/
#define HAVE_IGMP_MODE_RESTORE

/*qinq function, support vlan exchange, vlan attach*/
#define HAVE_QINQ

/*base on HAVE_GWD_FLASH_OPER, save and restore qinq config*/
#define HAVE_QINQ_CFG_RESTORE

/*base on HAVE_GWD_FLASH_OPER, save and restore port isolation config*/
#define HAVE_PORT_ISOLATION_RESTORE

/*gwd translate vlan mc*/
//#define HAVE_GWD_MC_TVM

/*gwd dhcp realy*/
#define HAVE_GWD_DHCP_RELAY

/*gwd port statistics cmd*/
#define HAVE_GWD_STATISTICS_CMD

/*gwd broadcast storm limit*/
#define HAVE_GWD_BC_LIMIT

/*base on HAVE_GWD_FLASH_OPER, gwd broadcast storm threshold restore*/
#define HAVE_GWD_BC_THRESHOLD_RESTORE

/*gwd oam fast statistic,dact*/
#define HAVE_GWD_OAM_FAST_STATISTIC

/*gwd user mac trace*/
#define HAVE_USER_MAC_TRACE

/*gwd optical module parameter query*/
#define HAVE_OPTICAL_MODULE_PARAMETERS_QUERY

/*gwd onu port mac count*/
#define HAVE_ONU_PORT_MAC_COUNT

/*gwd port mode command*/
#define HAVE_GWD_PORT_MODE_CMD

/*base on HAVE_GWD_FLASH_OPER,gwd port mode restore*/
#define HAVE_GWD_PORT_MODE_RESTORE

/*to prevent the downstream arp attack*/
#define HAVE_ARP_DOWN_INTO_CPU_LIMIT

/*when onu mpcp deregister, reset immediately*/
#define HAVE_MPCP_DEREGISTER_REST

/*add reset control interface*/
#define HAVE_RESET_CONTROL

/*when the onu MPCP registered timeout(10s), reset immediately*/
#define HAVE_MPCP_REG_TIMEOUT_RESET

/*onu port mirror function*/
#define HAVE_PORT_MIRROR_CMD

/*onu pon laser control*/
#define HAVE_GWD_PON_LASER_CONTROL

/*gpio debug commad*/
#define HAVE_GPIO_DEBUG_CMD

/*statistics nni*/
#define HAVE_STATISTICS_NNI

/* new platform unify cmd support */
#define HAVE_PLATFORM_UNIFY_CMD

/* inactive reduplicate cmd */
#define HAVE_INACTIVE_REDUPLICATE_CMD

/* event show cmd */
#define HAVE_EVENT_SHOW_CMD

/* device name cmd */
#define HAVE_DEVICE_NAME_CMD

/* vlan cmd */
#define HAVE_VLAN_CMD

/*base on HAVE_GWD_FLASH_OPER,eth0 ip address restore*/
#define HAVE_ETH0_IP_ADDR_RESTORE

#ifdef HAVE_TCP_UDP_NETWORK_CARD
/*base on HAVE_GWD_FLASH_OPER,eth1 ip address restore*/
#define HAVE_ETH1_IP_ADDR_RESTORE
#endif

/* enable cli filter */
#define HAVE_CLI_FILTER

/*gwd loop detect*/
#define	HAVE_GWD_LOOP_DETECT

/*olt inform the onu the onu's location*/
#define	HAVE_LOOP_DETECT_ALARM_BASE_OLT

/* when reset ONU,reset switch hw */ 
#define HAVE_SWITCH_HW_RESET

/* support dhcpv4 option 82 add */
#define HAVE_DHCPV4_SUPPORT

/* support DHCPV6 pkt upstream dest set to cpu */
#define HAVE_DHCPV6_PKT_DEST_PORT_SUPPORT

#ifdef HAVE_DHCPV6_PKT_DEST_PORT_SUPPORT
/* support dhcpv6 option 18 add */
#define HAVE_DHCPV6_SUPPORT
#endif

/* when onu deregistered,NOT reset immediately,delay for a period of time */
#define HAVE_ONU_DELAY_OFFLINE_RESET

/* ctc loop detect default config,enable ctc loop detect */
/* #define HAVE_CTC_LOOP_DETECT_DEFAULT_CONFIG_ENABLE */

/* ctc loop control default config,enable ctc loop control  */
/* #define HAVE_CTC_LOOP_CONTROL_DEFAULT_CONFIG_ENABLE */

/* set mac learning enable,when startup for cmd atu learning get wrong config */
#define HAVE_ATU_INIT

/*add by zhaoxh 2016-8-11*/
/*base on HAVE_GWD_FLASH_OPER,add static fdb to flash */
#define HAVE_STATIC_FDB_ADDR_RESTORE  

/*base on HAVE_GWD_FLASH_OPER,add fdb aging to flash*/
#define HAVE_AGING_FDB_ADDR_RESTORE

/*base on HAVE_GWD_FLASH_OPER,add fdb learning to flash*/
#define HAVE_LEARNING_FDB_ADDR_RESTORE

/* for HangZhou nscrtv test */
#define HAVE_VLAN_MAPPING
/*--------------------------gwd end--------------------------------*/
#endif	/*end of file*/
