/*                   Copyright(c) 2008-2012 GWTT, Inc.                    */
/*  
**  gwd_utils.c -  C file for gwd utilities 
**
**  This file was written by wujianming, 12/04/2016
**  
**  Changes:
**
**  Version       |  Date          |    Change        |    Author	  
**  ----------|-----------|-------------|------------
**	1.00          | 12/04/2016 |	creation	      |   wujianming
*/

/*============================= Include Files ===============================*/

#include "gwd_utils.h"
#include "stdlib.h"
#include "string.h"
#include "sdl.h"
/*================================ Macros ===================================*/


/*=============================== Constants =================================*/


/*============================== Data Types =================================*/


/*========================== External Variables =============================*/


/*========================== External Functions =============================*/


/*=============================== Variables =================================*/


/*======================= Internal Functions Prototype ======================*/


/*================================ Functions ================================*/

gw_int32 gwd_ascii_to_num(char c)
{
	if(c >= '0' && c <= '9')
	{
		return c - '0';
	}
	
	c = TOUPPER(c);
	
	if(c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}

	return -1;
}

gw_boolean gwd_is_hex_num(char c)
{
	if(-1 != gwd_ascii_to_num(c))
	{
		return gwd_true;
	}

	return gwd_false;
}

cs_status gwd_parse_number_range(char *str,cs_int32 *arr,cs_int32 *num)
{
	char *p_find = NULL;
	
	if(NULL == str || NULL == arr || NULL == num)
	{
		return CS_E_PARAM;
	}

	if( NULL != (p_find = strstr(str,"-")))
	{
		cs_int32 start = atoi(str);
		cs_int32 end = atoi(p_find+1);
		cs_int32 max_num = *num;
		cs_int32 actual_count = 0;
		if(start >= end || start < 0 || end < 0)
		{
			return CS_E_PARAM;
		}
		else
		{
			while(actual_count < max_num && start <= end)
			{
				arr[actual_count] = start;
				actual_count++;
				start++;
			}
		}
		
		*num = actual_count;
		return CS_E_OK;
	}
	else
	{
		return CS_E_NOT_FOUND;
	}
}

cs_status gwd_parse_number_list(char *str,cs_int32 *arr,cs_int32 *num)
{
	cs_int32 max_num = 0;
	cs_int32 actual_count = 0;
	char *p = NULL;

	if(NULL == str || NULL == arr || NULL == num)
	{
		return CS_E_PARAM;
	}
	
	max_num = *num;
	
	p=strtok(str,",");
	while(p)
	{
		cs_int32 range_num = max_num - actual_count;

		if(range_num > 0)
		{
			if(CS_E_NOT_FOUND == gwd_parse_number_range(p,&arr[actual_count],&range_num))
			{
				arr[actual_count++] = atoi(p);
			}
			else
			{
				actual_count += range_num;		
			}
			p=strtok(NULL,",");
		}
		else
		{
			break;
		}
	}

	*num = actual_count;
	
	return CS_E_OK;
}

cs_status gwd_num_in_array(cs_uint32 num,cs_uint32 array[],cs_uint32 array_size)
{
	int i = 0;
	
	if(NULL == array)
	{
		return CS_E_PARAM;
	}

	for(i = 0;i < array_size;i++)
	{
		if(num == array[i])
		{
			return CS_E_OK;
		}
	}

	return CS_E_NOT_FOUND;
}

int gwd_bitmap2str(char* dstStr, unsigned long *strLen, unsigned char *byteMap, unsigned long mapSize, unsigned long bitBegin, unsigned long bitEnd)
{
    int i, j, m, n, x;
    int pos1, pos2, pos, len1, len2;
    int byte_begin, byte_end;
    int bit_begin, bit_end;
    char szStr[32];

    if ( bitEnd > 0 )
    {
        byte_end = bitEnd >> 3;
        bit_end  = bitEnd & 7;

        if ( byte_end > mapSize )
        {
            return CS_E_ERROR;
        }
    }
    else
    {
        byte_end = mapSize;
        bit_end  = 7;
    }

    if ( bitBegin > 0 )
    {
        byte_begin = bitBegin >> 3;
        bit_begin  = bitBegin & 7;

        if ( byte_begin > byte_end )
        {
            return CS_E_ERROR;
        }
    }
    else
    {
        byte_begin = 0;
        bit_begin  = 0;
    }

    pos1 = -2;
    pos2 = -2;
    len1 = 0;

    m = bit_begin;
    n = 7;
    for ( i = byte_begin; i <= byte_end; )
    {
        if ( byte_end == i )
        {
            n = bit_end;
        }

        if ( 0 != byteMap[i] )
        {
            /* 閻庢稒顨夋俊顓㈠箳閵忥紕�?闁告牕缍婇崢�?*/
            for ( j = m; j <= n; j++ )
            {
                if ( byteMap[i] & (1 << j) )
                {
                    pos = (i << 3) + j;

                    if ( pos - 1 > pos2 )
                    {
                        if ( pos2 >= 0 )
                        {
                            if ( pos2 - 1 > pos1 )
                            {
                                len2 = sprintf(szStr, "-%d,%d", pos2, pos);
                            }
                            else
                            {
                                if ( pos2 > pos1 )
                                {
                                    len2 = sprintf(szStr, ",%d,%d", pos2, pos);
                                }
                                else
                                {
                                    len2 = sprintf(szStr, ",%d", pos);
                                }
                            }
                        }
                        else
                        {
                            len2 = sprintf(szStr, "%d", pos);
                        }

                        if ( (len1 + len2) < *strLen )
                        {
                            memcpy(&dstStr[len1], szStr, len2);
                            len1 += len2;
                        }
                        else
                        {
                            return CS_E_ERROR;
                        }

                        pos1 = pos;
                    }

                    pos2 = pos;
                }
            }

            i++;
        }
        else
        {
            /* 閻忓繑绻嗛惁顖炲嫉閵囧洤顫㈤妷鈺傛瘣闁汇劑宕犺ぐ鎺戝�?*/
            x = byte_end - (i++);

            if ( x >= sizeof(unsigned long) )
            {
                if ( 0 == *(unsigned long*)(byteMap + i) )
                {
                    i += sizeof(unsigned long);
                }
                else
                {
                    if ( 0 == *(unsigned short*)(byteMap + i) )
                    {
                        i += sizeof(unsigned short);
                    }

                    if ( 0 == byteMap[i] )
                    {
                        i++;
                    }
                }
            }
            else if ( x >= sizeof(unsigned short) )
            {
                if ( 0 == *(unsigned short*)(byteMap + i) )
                {
                    i += sizeof(unsigned short);
                }
                else if ( 0 == byteMap[i] )
                {
                    i++;
                }
                else{}
            }
            else{}
        }

        if ( m != 0 )
        {
            m = 0;
        }
    }

    if ( pos2 > pos1 )
    {
        if ( pos2 - 1 > pos1 )
        {
            len2 = sprintf(szStr, "-%d", pos2);
        }
        else
        {
            len2 = sprintf(szStr, ",%d", pos2);
        }

        if ( (len1 + len2) < *strLen )
        {
           memcpy(&dstStr[len1], szStr, len2);
            len1 += len2;
        }
        else
        {
            return CS_E_ERROR;
        }
    }

    dstStr[len1] = '\0';
    *strLen = len1;

    return len1;
}


int gwd_port_list_long_to_string(unsigned long portlist, char *str, unsigned long str_len)
{
    unsigned char auc4Bytes[sizeof(unsigned long)];

    GW_UINT32_TOCHAR(portlist, auc4Bytes);
    
    return gwd_bitmap2str(str, &str_len, auc4Bytes, sizeof(auc4Bytes), CS_UNI_PORT_ID1, UNI_PORT_MAX);
}

cs_status gwd_parse_num_list_from_str(char* str,cs_int32 *array,cs_int32 *max_size)
{
	if(NULL == str || NULL == max_size || NULL == array)
	{
		return CS_E_PARAM;
	}
	
	if(CS_E_OK != gwd_parse_number_list(str,array,max_size))
	{
		return CS_E_ERROR;
	}

	#ifdef TEST
	{
		int i = 0;
		for(i=0;i < *max_size;i++)
		{
			cs_printf("[%d]",array[i]);
		}
		cs_printf("\n");
	}
	#endif
	
	return CS_E_OK;
}


