/*                   Copyright(c) 2008-2012 GWTT, Inc.                    */
/*  
**  gwd_utils.h -  Header file for gwd utilities 
**
**  This file was written by wujianming, 12/04/2016
**  
**  Changes:
**
**  Version       |  Date          |    Change        |    Author	  
**  ----------|-----------|-------------|------------
**	1.00          | 12/04/2016 |	creation	      | wujianming
*/

#if !defined(__GWD_UTILS_H__)
#define __GWD_UTILS_H__

#if defined(__cplusplus)
extern "C" {
#endif

/*============================= Include Files ===============================*/
#include "gwd_types.h"
/*================================ Macros ===================================*/

/*=============================== Constants =================================*/

/*========================== External Variables =============================*/

/*============================== Data Types =================================*/



gw_int32 gwd_ascii_to_num(char c);

gw_boolean gwd_is_hex_num(char c);

cs_status gwd_parse_number_range(char *str,cs_int32 *arr,cs_int32 *num);

cs_status gwd_parse_number_list(char *str,cs_int32 *arr,cs_int32 *num);

cs_status gwd_num_in_array(cs_uint32 num,cs_uint32 array[],cs_uint32 array_size);

int gwd_bitmap2str(char* dstStr, unsigned long *strLen, unsigned char *byteMap, unsigned long mapSize, unsigned long bitBegin, unsigned long bitEnd);

int gwd_port_list_long_to_string(unsigned long portlist, char *str, unsigned long str_len);

cs_status gwd_parse_num_list_from_str(char* str,cs_int32 *array,cs_int32 *max_size);


#if defined(__cplusplus)
}
#endif

#endif /* __GWD_UTILS_H__ */

