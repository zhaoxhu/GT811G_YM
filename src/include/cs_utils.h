#ifndef __CS_UTILS_H__
#define __CS_UTILS_H__

#define iros_strtol cs_atoi

int cs_str2mac(char *mac_str , char *mac_addr);
int cs_mac2str(char *mac_str , char *mac_addr);
int cs_str2ip(char *ip_str , unsigned int *ipaddr);
int cs_ip2str(char *ip_str , unsigned int ipaddr);
int cs_isdigit(char c);
int cs_isdigit_str(char *str);
int cs_isalpha(char c);
int cs_atoi(char *str);
void cs_str2lower(char *str);
void cs_str2upper(char *str);
int cs_strxcmp(char *src , char *dst);
int cs_strcmp(char *src , char *dst);
int cs_str2hex(char *src , char *dst , int maxbytes);

#endif
