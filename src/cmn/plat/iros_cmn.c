/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#include "iros_config.h"
#include "plat_common.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <cyg/infra/cyg_type.h>
#include <stddef.h>
#ifdef HAVE_ZTE_SIJIE
extern cs_status epon_request_onu_reset_set(void);
#endif /*END_HAVE_ZTE_SIJIE*/

int cmd_cmp(char *str, const char *mode)
{
    if (strlen(str) != strlen(mode))
        return 0;

    while (*mode) {
        if ((*str == '\0') || (*mode != *str))
            return 0;

        mode++;
        str++;
    }

    return 1;
}

void dumpPkt(char *comment, int port, unsigned char *buffer, int len)
{
    int i = 0, j = 0;

    if (!buffer || !len)
        return;

    if (comment)
        diag_printf("#%s port 0x%x len 0x%x\n", comment, port, len);

    diag_printf("%06x ", j);
    for (i = 0; i < len; i++) {
        if (i && (i % 16) == 0) {
            j += 0x10;
            diag_printf("  .\n");
            diag_printf("%06x ", j);
        }
        diag_printf("%02x ", buffer[i]);
    }
    diag_printf(" .\n");
}


#define SWITCH_GPIO		4
#define UP_LEVEL		1
#define DOWN_LEVEL		0
#define HOLD_TIME		100		//单位:毫秒

extern void sdl_switch_hw_reset(void)
{
	gpio_mode_t mode_original;
	cs_uint8 level_original;
	
	//获取原来的配置
	cs_gpio_mode_get(SWITCH_GPIO, &mode_original);
	cs_gpio_read(SWITCH_GPIO, &level_original);
	
	//交换芯片掉电
	cs_gpio_mode_set(SWITCH_GPIO, GPIO_OUTPUT);
	cs_gpio_write(SWITCH_GPIO, DOWN_LEVEL);

	//延时
	cs_thread_delay(HOLD_TIME);
	
	//交换芯片上电
	cs_gpio_write(SWITCH_GPIO, UP_LEVEL);
	
	//恢复原来的配置
	cs_gpio_write(SWITCH_GPIO, level_original);
	cs_gpio_mode_set(SWITCH_GPIO, mode_original);
	
}

void iros_system_reset(RESET_TYPE_E reset_type)
{
    if (reset_type >= RESET_TYPE_MAX)
        return;
    switch (reset_type) {
    case RESET_TYPE_FORCE:
		#ifdef HAVE_SWITCH_HW_RESET   
		sdl_switch_hw_reset();
		#endif
#ifdef HAVE_ZTE_SIJIE
        epon_request_onu_reset_set();
#endif /*END_HAVE_ZTE_SIJIE*/
        HAL_PLATFORM_RESET();
        break;

    default:
        break;
    }
}


static int cs_char2int(char c)
{
	int digit = 0;
	
	if(c <= '9' && c >= '0')
		digit = c - '0';
	else if(c <= 'f' && c >= 'a')
		digit = c - 'a' + 10;
	else
		digit = c - 'A' + 10;

	return digit;
}

static char cs_int2char(int num)
{
	char c = 0;

	if(num >= 0 && num <= 9)
		c = '0' + num;
	else
		c = 'a' + num - 10;

	return c;
}

int cs_str2mac(char *mac_str , char *mac_addr)
{
	char *c = NULL;
	int index = 0;
	int parse_state = 0;
	int char_type = 0; /*0: decimal number   1: Hex Number,lower case 2: hex number, upper case  3:colon type 4:error type*/
	int high_bits = 0, low_bits = 0;
	
	if(mac_str == NULL || mac_addr == NULL)
		return -1;

	c = mac_str;
	while(*c != '\0')
	{
		if(*c <= '9' && *c >= '0')
			char_type = 0;
		else if(*c <= 'f' && *c >= 'a')
			char_type = 1;
		else if(*c <= 'F' && *c >= 'A')
			char_type = 2;
		else if(*c == ':'||*c == '.')
			char_type = 3;
		else
			char_type = 4;

		switch(char_type)
		{
		case 0:
		case 1:
		case 2:
			if(parse_state == 0 || parse_state == 3)
			{
				high_bits = cs_char2int(*c);
				parse_state = 1;
			}
			else if(parse_state == 1)
			{
				low_bits = cs_char2int(*c);
				parse_state = 2;
			}
			else
				return -1;

			if(index >= 6)
				return -1;
			
			c++;
			if(*c == '\0')
			{
				if(parse_state == 1)
				{
					mac_addr[index++] = high_bits;
				}

				if(parse_state == 2)
				{
					mac_addr[index++] = (high_bits<<4)+low_bits;
				}
			}
			break;
		case 3:
			if(parse_state != 1 && parse_state != 2)
				return -1;
			
			if(parse_state == 1)
			{
				mac_addr[index++] = high_bits;
			}

			if(parse_state == 2)
			{
				mac_addr[index++] = (high_bits<<4)+low_bits;
			}

			parse_state = 3;
			c++;
			break;
		default:
			return -1;
		}
	}

	if(index != 6)
		return -1;
	
	return 0;
}

int cs_str2hex(char *oui_str , char *oui_value , int maxbytes)
{
	char *c = NULL;
	int index = 0;
	int parse_state = 0;
	int char_type = 0; /*0: decimal number   1: Hex Number,lower case 2: hex number, upper case  3:colon type 4:error type*/
	int high_bits = 0, low_bits = 0;
	
	if(oui_str == NULL || oui_value == NULL)
		return -1;

	c = oui_str;
	while(*c != '\0')
	{
		if(*c <= '9' && *c >= '0')
			char_type = 0;
		else if(*c <= 'f' && *c >= 'a')
			char_type = 1;
		else if(*c <= 'F' && *c >= 'A')
			char_type = 2;
		else if(*c == ':'||*c == '.')
			char_type = 3;
		else
			char_type = 4;

		switch(char_type)
		{
		case 0:
		case 1:
		case 2:
			if(parse_state == 0 || parse_state == 3)
			{
				high_bits = cs_char2int(*c);
				parse_state = 1;
			}
			else if(parse_state == 1)
			{
				low_bits = cs_char2int(*c);
				parse_state = 2;
			}
			else
				return -1;

			if(index >= maxbytes)
				return -1;
			
			c++;
			if(*c == '\0')
			{
				if(parse_state == 1)
				{
					oui_value[index++] = high_bits;
				}

				if(parse_state == 2)
				{
					oui_value[index++] = (high_bits<<4)+low_bits;
				}
			}
			break;
		case 3:
			if(parse_state != 1 && parse_state != 2)
				return -1;
			
			if(parse_state == 1)
			{
				oui_value[index++] = high_bits;
			}

			if(parse_state == 2)
			{
				oui_value[index++] = (high_bits<<4)+low_bits;
			}

			parse_state = 3;
			c++;
			break;
		default:
			return -1;
		}
	}

	if(index != maxbytes)
		return -1;
	
	return 0;
}


int cs_mac2str(char *mac_str , char *mac_addr)
{
	char digit = 0;
	int i;
	int index = 0;
	
	if(mac_str == NULL || mac_addr == NULL)
		return -1;

	for(i = 0 ; i < 6 ; i++)
	{
		digit = mac_addr[i];
		mac_str[index++] = cs_int2char((digit>>4)&0x0F);
		mac_str[index++] = cs_int2char(digit&0x0F);
		if(i < 5)
			mac_str[index++] = ':';
	}
    mac_str[index] = '\0';

	return 0;
}

int cs_str2ip(char *ip_str , unsigned int *ipaddr)
{
	char *c = NULL;
	int parse_state = 0;
	int index = 0;
	int ip[4];
	
	if(ip_str == NULL || ipaddr == NULL)
		return -1;

	c = ip_str;
	while(*c != '\0')
	{
		switch(*c)
		{
		case '0':
			if(parse_state == 0)
				return -1;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(parse_state == 0 || parse_state == 2)
			{
				ip[index++] = atoi(c);
				if(ip[index-1] > 255 || ip[index-1] < 0)
					return -1;
			}
			c++;
			parse_state = 1;
			break;
		case '.':
			if(parse_state != 1)
				return -1;
			
			c++;
			parse_state = 2;
			break;
		default:
			return -1;
		}
	}

	if(parse_state != 1)
		return -1;
	*ipaddr = (unsigned int)(((ip[0]<<24)&0xFF000000)|((ip[1]<<16)&0xFF0000)|((ip[2]<<8)&0xFF00)|((ip[3]<<0)&0xFF));
	
	return 0;
}

int cs_ip2str(char *ip_str , unsigned int ipaddr)
{
	if(ip_str == NULL)
		return -1;

	sprintf(ip_str , "%d.%d.%d.%d",(ipaddr>>24)&0xFF,
								(ipaddr>>16)&0xFF,
								(ipaddr>>8)&0xFF,
								(ipaddr>>0)&0xFF);
	return 0;
}

static int cs_str2dec(char *str , unsigned int *num)
{
	char *c = str;
	int len = 0;
	int parse_status = 0;
	unsigned int temp_num = 0;

	/******************************************************************
		PARSE_STATUS DESC
		------------------------------------------------------
				   0 init status
				   1 string length is 1, it must be a dec num.
				   2 a hex-string, header is '0x'.
				   3 a decimal-string,start with a digit '1-9'
		------------------------------------------------------
	******************************************************************/

	len = strlen(str);
	if(len == 0)
	{
		*num = 0;
		return -1;
	}
	
	if(len == 1)
		parse_status = 1;
	
	while(*c != '\0')
	{
		switch(*c)
		{
		case '0':
			if(parse_status == 0)	
			{
				c++;
				if(*c != 'x')	/*str = 01/02/0a/0b/0h/00, but not '0x'*/
				{
					*num = 0;
					return -1;
				}
				else
				{
					parse_status = 2;
					if(len > 10)/*the max len of a hex-string must less than 10. Eg. '0x12345678'*/
					{
						*num = 0;
						return -1;
					}
				}
			}
			else if(parse_status == 1)	/*just one byte,and it is '0'.*/
			{
				*num = 0;
				return 0;
			}
			else if(parse_status == 2)
			{
				temp_num = temp_num<<4;
			}
			else if(parse_status == 3)
			{
				temp_num = 10*temp_num;
			}
			else
			{
				*num = 0;
				return -1;
			}
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(parse_status == 0)
			{
				temp_num = *c-'0';
				parse_status = 3;
			}
			else if(parse_status == 1)
			{
				*num = atoi(str);
				return 0;
			}
			else if(parse_status == 2)
			{
				temp_num = (temp_num<<4) + *c - '0';
			}
			else if(parse_status == 3)
			{
				temp_num = (temp_num*10) + *c - '0';
			}
			else
			{
				*num = 0;
				return -1;
			}
			break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			if(parse_status == 0 || parse_status == 1 || parse_status == 3)
			{
				*num = 0;
				return -1;
			}

			if(parse_status == 2)
			{
				temp_num = (temp_num<<4) + *c - 'a' + 10;
			}
			else
			{
				*num = 0;
				return -1;
			}
			break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			if(parse_status == 0 || parse_status == 1 || parse_status == 3)
			{
				*num = 0;
				return -1;
			}

			if(parse_status == 2)
			{
				temp_num = (temp_num<<4) + *c - 'A' + 10;
			}
			else
			{
				*num = 0;
				return -1;
			}
			break;
		default:
			*num = 0;
			return -1;
		}
		c++;
	}

	*num = temp_num;
	return 0;
}

int cs_isdigit(char c)
{
    if(('0' <= c) && (c <= '9'))
        return 1;
    
    return 0;
}

int cs_isdigit_str(char *str)
{
    char *c = str;

    while(*c != '\0')
    {
        if(!cs_isdigit(*c))
            return 0;
    }

    return 1;
}

int cs_isalpha(char c)
{
    if((c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z'))
        return 1;

    return 0;
}

int cs_atoi(char *str)
{
    unsigned int num=0;
    if(cs_str2dec(str, &num) != 0)
        return 0;

    return num;
}

void cs_str2lower(char *str)
{
    int len = strlen(str);
    int i;

    for(i = 0 ; i < len ; i++)
    {
        if(!cs_isalpha(str[i]))
            continue;

        if(str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i]-'A'+'a';
    }

    return;
}

void cs_str2upper(char *str)
{
    int len = strlen(str);
    int i;

    for(i = 0 ; i < len ; i++)
    {
        if(!cs_isalpha(str[i]))
            continue;

        if(str[i] >= 'a' && str[i] <= 'z')
            str[i] = str[i]-'a'+'A';
    }

    return;
}

int cs_strxcmp(char *src , char *dst)
{
    if(strlen(src) != strlen(dst))
        return 1;

    return strcmp(src, dst);
}

int cs_strcmp(char *src , char *dst)
{
    return strcmp(src, dst);
}

