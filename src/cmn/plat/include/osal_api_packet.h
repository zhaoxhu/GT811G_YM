/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002-2010 by Cortina Systems Incorporated.            */
/***********************************************************************/


#ifndef _osal_api_packet_
#define _osal_api_packet_

#include "cs_types.h"
#include "cs_config.h"

#define CS_ETHER_MTU                    1536
#define CS_ETHER_MIN_MTU                60
#define CS_LAYER2_HEADER_LEN            14
#define CS_LAYER3_HEADER_LEN            14
#define CS_FRAME_HEADER_LEN             (CS_LAYER2_HEADER_LEN + CS_LAYER3_HEADER_LEN)
#define CS_FRAME_PAYLOAD_LEN            (CS_ETHER_MTU - CS_LAYER3_HEADER_LEN)

#define CS_IF_NAME_SIZE                 16
#define CS_MACADDR_LEN                  6
#define CS_ETHERTYPE_IROS               0xFFFF


#define SC_PKT_RX_THREAD_NAME           CS_PKT_RX_THREAD_NAME
#define SC_PKT_RX_THREAD_STACK_SIZE     CS_PKT_RX_THREAD_STACK_SIZE
#define SC_PKT_RX_THREAD_PRIORITY       CS_PKT_RX_THREAD_PRIORITY
#define SC_PKT_RX_THREAD_OPTIONS        0

#define SC_PKT_TX_THREAD_NAME           CS_PKT_TX_THREAD_NAME
#define SC_PKT_TX_THREAD_STACK_SIZE     CS_PKT_TX_THREAD_STACK_SIZE
#define SC_PKT_TX_THREAD_PRIORITY       CS_PKT_TX_THREAD_PRIORITY
#define SC_PKT_TX_THREAD_OPTIONS        0

#define SC_CORE_THREAD_NAME             CS_CORE_THREAD_NAME
#define SC_CORE_THREAD_STACK_SIZE       CS_CORE_THREAD_STACK_SIZE
#define SC_CORE_THREAD_PRIORITY         CS_CORE_THREAD_PRIORITY
#define SC_CORE_THREAD_OPTIONS          0

#define SC_PKT_MSG_Q_MAX_MSG_LEN        4
#define SC_PKT_MSG_Q_MAX_MSGS           128
#define SC_PKT_MSG_Q_OPTIONS            0

#define SC_MGMT_PORT_NUM                CS_MGMT_PORT_NUM

typedef enum
{
    SC_PKT_PRIORITY_HIGH    = 0,
    SC_PKT_PRIORITY_MEDIUM  = 1,
    SC_PKT_PRIORITY_LOW     = 2,
    SC_PKT_PRIORITY_MAX
} SC_PKT_PRI_E;


typedef struct {
    /* port handler,
        for linux   - raw ethernet socket
        for vxworks - MUX cookie or END object */
    int port_handler;
    /* MAC for the port */
    cs_uint8 mgmt_port_mac[CS_MACADDR_LEN];
} cs_mport_info_t;


/* ethernet frame header */
typedef struct {
    cs_mac_t    dst;
    cs_mac_t    src;
    cs_uint16   ethertype;
} __attribute__((packed)) cs_ether_header_t;

typedef struct {
    cs_uint16 tpid;
    cs_uint16 tag;
} __attribute__((packed)) cs_vlan_hdr_t;

/* layer three header */
typedef struct {
    cs_mac_t          dst;
    cs_mac_t          src;
    cs_uint16         layer3type;
} __attribute__((packed)) cs_layer3_header_t;

/* ethernet frame encapsulation */
typedef struct {
    /* ether header */
    cs_ether_header_t       ether_header;
    /* layer three header */
    cs_layer3_header_t      layer3_header;
    /* the payload */
    cs_uint8                payload[CS_FRAME_PAYLOAD_LEN];
} __attribute__((packed)) cs_ether_frame_t;


typedef struct {
    /* the frame size */
    cs_uint32               framesz;
    /* mgmt port id of the msg: source port for incoming, dest port for outgoing */
    cs_uint16               mgmt_port;
    /* priority of the msg, SC_PKT_PRIORITY_XXX */
    cs_uint16               priority;
} __attribute__((packed)) cs_iros_msg_header_t;


/* iros message encapsulation */
typedef struct {
    cs_iros_msg_header_t    header;
    /* the received ethernet frame */
    cs_ether_frame_t        frame;
} __attribute__((packed)) cs_iros_msg_t;



/* Ethernet frame handler in generic form */
typedef void (* cs_packet_frame_handler_t)(cs_ether_frame_t* frame);

/* Register function for ether frame handler */
void cs_pkt_frame_hdl_reg(cs_packet_frame_handler_t func_ptr);

/* iROS sends a frame on the management ethernet port.
   the ethernet frame is pre-formatted with DA, SA, ethertype.
   And there MUST be a iros msg header (cs_iros_msg_header_t) ahead of the frame.
   If the return code is 0, the frame memory should NOT be freed by user.
   If the return code is NOT 0, the frame memory should be freed by user.   */
cs_int32 cs_pkt_ether_send(cs_int8 *frame, cs_int32 len);

/* packet thread init */
cs_int32 cs_pkt_init(cs_int8 *if1_name, cs_int8 *if2_name);

extern cs_mport_info_t mport[SC_MGMT_PORT_NUM];

#endif
