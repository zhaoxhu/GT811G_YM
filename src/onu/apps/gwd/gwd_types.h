#ifndef	__GWD_TYPES_H__
#define	__GWD_TYPES_H__
#include "cs_types.h"

typedef cs_int8		gw_int8;
typedef cs_uint8	gw_uint8;
typedef cs_int16	gw_int16;
typedef cs_uint16	gw_uint16;
typedef cs_int32 	gw_int32;
typedef cs_uint32	gw_uint32;
typedef cs_int64 	gw_int64;
typedef cs_uint64	gw_uint64;
typedef float		gw_float;
typedef cs_boolean	gw_boolean;

#define gwd_true	1;
#define gwd_false	0;

#define IPV4_MASK_LEN 32

#define GWD_DHCP_OPTION_PAYLOAD_MAX 128

#ifdef __LITTLE_ENDIAN__
#define GW_UINT32_TOCHAR(u32Val, aucVals)   do{ \
                                                *((unsigned long*)(aucVals)) = (u32Val);    \
                                            }while(0) 

#define GW_UINT16_TOCHAR(u16Val, aucVals)   do{ \
                                                *((unsigned short*)(aucVals)) = (u16Val);    \
                                            }while(0) 

#define GW_CHAR_TOUINT32(u32Val, aucVals)   do{ \
                                                (u32Val) = *((unsigned long*)(aucVals));    \
                                            }while(0) 

#define GW_CHAR_TOUINT16(u16Val, aucVals)   do{ \
                                                (u16Val) = *((unsigned short*)(aucVals));    \
                                            }while(0) 

#else
#define GW_UINT32_TOCHAR(u32Val, aucVals)   do{ \
                                                (aucVals)[3] = ( ((u32Val) & 0xFF000000) >> 24 ); \
                                                (aucVals)[2] = ( ((u32Val) & 0x00FF0000) >> 16 ); \
                                                (aucVals)[1] = ( ((u32Val) & 0x0000FF00) >> 8 ); \
                                                (aucVals)[0] = ( (u32Val) & 0x000000FF ); \
                                            }while(0) 

#define GW_UINT16_TOCHAR(u16Val, aucVals)   do{ \
                                                (aucVals)[1] = ( ((u16Val) & 0xFF00) >> 8 ); \
                                                (aucVals)[0] = ( (u16Val) & 0x00FF ); \
                                            }while(0) 

#define GW_CHAR_TOUINT32(u32Val, aucVals)   do{ \
                                                (u32Val) = ((aucVals)[3]<<24) | ((aucVals)[2]<<16) | ((aucVals)[1]<<8) | ((aucVals)[0]); \
                                            }while(0) 

#define GW_CHAR_TOUINT16(u16Val, aucVals)   do{ \
                                                (u16Val) = ((aucVals)[1]<<8) | ((aucVals)[0]);    \
                                            }while(0) 
           
#endif /* #ifdef __LITTLE_ENDIAN__ */

#ifndef TOUPPER
#define TOUPPER(c)	(c >= 'a' && c <= 'z' ? c - ('a' - 'A') : c)
#endif

typedef enum {
	GW_E_OK				= 0,
	GW_E_ERROR			= -1,
	GW_E_TIMEOUT		= -2,
	GW_E_NOT_SUPPORT	= -3,
	GW_E_PARAM			= -4,
	GW_E_NOT_FOUND		= -5,
	GW_E_HAVE_EXIST		= -6,
	GW_E_RESOURCE		= -7,
	GW_E_NOT_ENOUGH		= -8,
	GW_E_MALLOC			= -9,
	GW_E_CONFLICT		= -10,
	GW_E_PORT			= -11,
	GW_E_MODE_ERROR		= -12,
	GW_E_MAX			= -13
} gw_status;

#endif	/*end of the file*/
