/*
 * $Id: cs_types.h,v 1.1.2.3.36.1 2010/12/23 05:56:38 ddong Exp $
 */
/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
/***********************************************************************/
/*
 * cs_types.h
 *
 * Include file containing the basic data types
 *
 */

#ifndef __CS_TYPES_H__
#define __CS_TYPES_H__


typedef unsigned long long      cs_uint64;
typedef long long               cs_int64;
typedef unsigned int            cs_uint32;
typedef int                     cs_int32;
typedef unsigned short          cs_uint16;
typedef short                   cs_int16;
typedef unsigned char           cs_uint8;
typedef char                    cs_int8;
typedef unsigned char           cs_boolean;

typedef enum {
    CS_E_OK = 0,
    CS_E_RESOURCE = 1,
    CS_E_PARAM = 2,
    CS_E_NOT_FOUND = 3,
    CS_E_CONFLICT = 4,
    CS_E_TIMEOUT = 5,
    CS_E_ERROR = 0xffffffff
} cs_status;

typedef int (*cs_funcptr)(void *arg);

typedef cs_uint32                cs_ipv6_t[4];
typedef cs_uint16                cs_dev_id_t;
typedef cs_uint16                cs_port_id_t;
typedef cs_uint16                cs_llid_t;
typedef struct {
    cs_uint32    bmp[4];
}                                cs_llid_bmp_t;

typedef cs_uint16                cs_sub_id_t;
typedef struct {
#   ifdef __BIG_ENDIAN__
    cs_uint8      byte5 ;
    cs_uint8      byte4 ;
    cs_uint8      byte3 ;
    cs_uint8      byte2 ;
    cs_uint8      byte1 ;
    cs_uint8      byte0 ;
#elif defined(__LITTLE_ENDIAN__)
    cs_uint8      byte0 ;
    cs_uint8      byte1 ;
    cs_uint8      byte2 ;
    cs_uint8      byte3 ;
    cs_uint8      byte4 ;
    cs_uint8      byte5 ;

#else
#error Endianness not defined
#endif
}                                   cs_mac;

typedef union {
    cs_uint8       addr[6];
#if 0    
    struct {
        cs_uint32 hi32;
        cs_uint16 lo16;
    } __attribute__((packed)) u;    /* most callers will prefer this format; */
    struct {
        cs_uint16 hi16;
        cs_uint32 lo32;
    } __attribute__((packed)) u2;   /* while some callers will prefer this format. */
    struct {
        cs_uint16 val16[3];
    } __attribute__((packed)) u3;   /* very few callers will prefer this format */
#endif    
}__attribute__((packed)) cs_mac_t;


#define CS_OK               CS_E_OK
#define CS_ERROR            CS_E_ERROR

#ifndef TRUE
#define TRUE                       1
#endif

#ifndef FALSE
#define FALSE                  0
#endif

#ifndef NULL
#define NULL                   ((void *) 0)
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef CS_IN
#define CS_IN
#endif

#ifndef CS_OUT
#define CS_OUT
#endif

#ifndef CS_IN_OUT
#define CS_IN_OUT
#endif

#define _CS_INLINE_ __inline__ static

#ifdef __BIG_ENDIAN__

#ifndef ntohl
#define ntohl(x)        (x)
#endif

#ifndef ntohs
#define ntohs(x)        (x)
#endif

#ifndef htonl
#define htonl(x)        (x)
#endif

#ifndef htons
#define htons(x)        (x)
#endif

#elif defined(__LITTLE_ENDIAN__)

#ifndef ntohl
#define ntohl(x)        ((((x) & 0x000000ff) << 24) | \
                            (((x) & 0x0000ff00) <<  8) | \
                            (((x) & 0x00ff0000) >>  8) | \
                            (((x) & 0xff000000) >> 24))
#endif

#ifndef htonl
#define htonl(x)        ((((unsigned long)(x) & 0x000000ff) << 24) | \
                            (((unsigned long)(x) & 0x0000ff00) <<  8) | \
                            (((unsigned long)(x) & 0x00ff0000) >>  8) | \
                            (((unsigned long)(x) & 0xff000000) >> 24))
#endif


#ifndef ntohs
#define ntohs(x)        ((((x) & 0x00ff) << 8) | \
                            (((x) & 0xff00) >> 8))
#endif

#ifndef htons
#define htons(x)        ((((x) & 0x00ff) << 8) | \
                            (((x) & 0xff00) >> 8))

#endif

#else
#error Endianness not defined
#endif




#endif /* __CS_TYPES_H__ */
