/****************************************************************************
            Software License for Customer Use of Cortina Software
                          Grant Terms and Conditions

IMPORTANT NOTICE - READ CAREFULLY: This Software License for Customer Use
of Cortina Software ("LICENSE") is the agreement which governs use of
software of Cortina Systems, Inc. and its subsidiaries ("CORTINA"),
including computer software (source code and object code) and associated
printed materials ("SOFTWARE").  The SOFTWARE is protected by copyright laws
and international copyright treaties, as well as other intellectual property
laws and treaties.  The SOFTWARE is not sold, and instead is only licensed
for use, strictly in accordance with this document.  Any hardware sold by
CORTINA is protected by various patents, and is sold but this LICENSE does
not cover that sale, since it may not necessarily be sold as a package with
the SOFTWARE.  This LICENSE sets forth the terms and conditions of the
SOFTWARE LICENSE only.  By downloading, installing, copying, or otherwise
using the SOFTWARE, you agree to be bound by the terms of this LICENSE.
If you do not agree to the terms of this LICENSE, then do not download the
SOFTWARE.

DEFINITIONS:  "DEVICE" means the Cortina Systems?LynxD SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems?SDK software.

GRANT OF LICENSE:  Subject to the restrictions below, CORTINA hereby grants
CUSTOMER a non-exclusive, non-assignable, non-transferable, royalty-free,
perpetual copyright license to (1) install and use the SOFTWARE for
reference only with the DEVICE; and (2) copy the SOFTWARE for your internal
use only for use with the DEVICE.

RESTRICTIONS:  The SOFTWARE must be used solely in conjunction with the
DEVICE and solely with Your own products that incorporate the DEVICE.  You
may not distribute the SOFTWARE to any third party.  You may not modify
the SOFTWARE or make derivatives of the SOFTWARE without assigning any and
all rights in such modifications and derivatives to CORTINA.  You shall not
through incorporation, modification or distribution of the SOFTWARE cause
it to become subject to any open source licenses.  You may not
reverse-assemble, reverse-compile, or otherwise reverse-engineer any
SOFTWARE provided in binary or machine readable form.  You may not
distribute the SOFTWARE to your customers without written permission
from CORTINA.

OWNERSHIP OF SOFTWARE AND COPYRIGHTS. All title and copyrights in and the
SOFTWARE and any accompanying printed materials, and copies of the SOFTWARE,
are owned by CORTINA. The SOFTWARE protected by the copyright laws of the
United States and other countries, and international treaty provisions.
You may not remove any copyright notices from the SOFTWARE.  Except as
otherwise expressly provided, CORTINA grants no express or implied right
under CORTINA patents, copyrights, trademarks, or other intellectual
property rights.

DISCLAIMER OF WARRANTIES. THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY
EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING ANY IMPLIED WARRANTIES
OF MERCHANTABILITY, NONINFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE,
TITLE, AND NON-INFRINGEMENT.  CORTINA does not warrant or assume
responsibility for the accuracy or completeness of any information, text,
graphics, links or other items contained within the SOFTWARE.  Without
limiting the foregoing, you are solely responsible for determining and
verifying that the SOFTWARE that you obtain and install is the appropriate
version for your purpose.

LIMITATION OF LIABILITY. IN NO EVENT SHALL CORTINA OR ITS SUPPLIERS BE
LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, LOST
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING OUT
OF THE USE OF OR INABILITY TO USE OF OR INABILITY TO USE THE SOFTWARE, EVEN
IF CORTINA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
TERMINATION OF THIS LICENSE. This LICENSE will automatically terminate if
You fail to comply with any of the terms and conditions hereof. Upon
termination, You will immediately cease use of the SOFTWARE and destroy all
copies of the SOFTWARE or return all copies of the SOFTWARE in your control
to CORTINA.  IF you commence or participate in any legal proceeding against
CORTINA, then CORTINA may, in its sole discretion, suspend or terminate all
license grants and any other rights provided under this LICENSE during the
pendency of such legal proceedings.
APPLICABLE LAWS. Claims arising under this LICENSE shall be governed by the
laws of the State of California, excluding its principles of conflict of
laws.  The United Nations Convention on Contracts for the International Sale
of Goods is specifically disclaimed.  You shall not export the SOFTWARE
without first obtaining any required export license or other approval from
the applicable governmental entity, if required.  This is the entire
agreement and understanding between You and CORTINA relating to this subject
matter.
GOVERNMENT RESTRICTED RIGHTS. The SOFTWARE is provided with "RESTRICTED
RIGHTS." Use, duplication, or disclosure by the Government is subject to
restrictions as set forth in FAR52.227-14 and DFAR252.227-7013 et seq. or
its successor. Use of the SOFTWARE by the Government constitutes
acknowledgment of CORTINA's proprietary rights therein. Contractor or
Manufacturer is CORTINA.

Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/
#define SYS_SCFG_PARAM
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "cs_types.h"
#include "osal_common.h"
#include "iros_flash_oper.h"
#include "tlv_parser.h"
#include "startup_cfg.h"
#include "cs_cmd.h"
#include "cs_utils.h"
#include "sys_cfg.h"
#include "xyzModem.h"

unsigned int gStartupInst = TLV_INVALID_INST_ID;
unsigned int gStartupInst_bak = TLV_INVALID_INST_ID;

#ifdef HAVE_SCFG_PROTECTION
#define SCFG_PARTION  IROS_FLASH_PARTITION_STARTUP_CONFIG_ID
#define SCFG_PARTION1 IROS_FLASH_PARTITION_STARTUP_CONFIG1_ID
#else
#define SCFG_PARTION IROS_FLASH_PARTITION_STARTUP_CONFIG_ID
#endif

cs_int32 scfg_warning_flag = 0;
cs_uint32 active_part_index = IROS_FLASH_PARTITION_INDEX_ANY;
cs_int32 scfg_enc_flag = 1;
cs_int32 scfg_autosave_flag = 1;
cs_uint8 scfg_mif_update=0;

void startup_cfg_enc_enable(int flag)
{
    if(!flag)
        scfg_enc_flag = 0;
    else
        scfg_enc_flag = 1;

    return;
}

void startup_cfg_autosave_enable(int flag)
{
    if(!flag)
        scfg_autosave_flag = 0;
    else
        scfg_autosave_flag = 1;

    return;
}

void startup_cfg_encrypt(char *pbuf , int len)
{
	char c = 0;
	int i;

    if(!scfg_enc_flag)
        return;

	for(i = 0 ; i < len ; i++)
	{
		c = 0;
		c |= ((pbuf[i]>>7)&0x01)|
			((pbuf[i]>>3)&0x08)|
			((pbuf[i]>>4)&0x02)|
			((pbuf[i]>>2)&0x04)|
			((pbuf[i]<<1)&0x10)|
			((pbuf[i]<<5)&0x80)|
			((pbuf[i]<<4)&0x20)|
			((pbuf[i]<<6)&0x40);
		pbuf[i] = (char)c;
	}

	return;
}

void startup_cfg_decrypt(char *pbuf , int len)
{
	char c = 0;
	int i;

    if(!scfg_enc_flag)
        return;

	for(i = 0 ; i < len ; i++)
	{
		c = 0;
		c |= ((pbuf[i]<<7)&0x80)|
			((pbuf[i]<<3)&0x40)|
			((pbuf[i]<<4)&0x20)|
			((pbuf[i]<<2)&0x10)|
			((pbuf[i]>>1)&0x08)|
			((pbuf[i]>>5)&0x04)|
			((pbuf[i]>>4)&0x02)|
			((pbuf[i]>>6)&0x01);
		pbuf[i] = (char)c;
	}

	return;
}

static int startup_cfg_default_cfg_len()
{
    int i;
    int total_field = 0;
    unsigned int total_cfg_len = 0;

    total_field = sizeof(glb_field)/sizeof(glb_field[0]);
    total_cfg_len = sizeof(tlv_header_t);
    for(i = 0 ; i < total_field ; i++)
    {
        total_cfg_len += TLV_VALUE_OFFSET;
        total_cfg_len += glb_field[i].value_max_len;
    }

    return total_cfg_len;
}

static void startup_config_restore(unsigned int inst_id)
{
    int i;
    int total_field = 0;

    total_field = sizeof(glb_field)/sizeof(glb_field[0]);
    for(i = 0 ; i < total_field ; i++)
    {
        switch(glb_field[i].value_type)
        {
        case FIELD_TYPE_CHAR:
            tlv_write_char(inst_id, glb_field[i].type, (unsigned char)glb_field[i].dflt_val);
            break;
        case FIELD_TYPE_SHORT:
            tlv_write_short(inst_id, glb_field[i].type, (unsigned short)glb_field[i].dflt_val);
            break;
        case FIELD_TYPE_INT:
            tlv_write_int(inst_id, glb_field[i].type, (unsigned int)glb_field[i].dflt_val);
            break;
        case FIELD_TYPE_BUFF:
        case FIELD_TYPE_STRING:
            tlv_write_buffer(inst_id, glb_field[i].type, glb_field[i].value_max_len ,glb_field[i].value_max_len,glb_field[i].dflt_buf);
            break;
        default:
            break;
        }
    }

    return;
}

static int startup_config_write_to_flash(void *addr , int len)
{
#ifdef HAVE_SCFG_PROTECTION
    unsigned int part_index = IROS_FLASH_PARTITION_INDEX_ANY;
#endif
    char *startup_cfg_addr = NULL;
    int written_len = 0;

    if(!scfg_autosave_flag)
        return 0;

#ifdef HAVE_SCFG_PROTECTION
    part_index = flash_part_id_to_part_index(SCFG_PARTION);
    if(active_part_index == part_index)
        active_part_index = flash_part_id_to_part_index(SCFG_PARTION1);
    else
        active_part_index = part_index;
#endif
    if(active_part_index == IROS_FLASH_PARTITION_INDEX_ANY)
    {
        if(scfg_warning_flag == 0)
            cs_printf("write startup-cfg error!\n");
        return -1;
    }
    startup_cfg_addr = flash_dev.info.super_block.part_tab[active_part_index].part_loc;
    if (flash_part_write_init(active_part_index, startup_cfg_addr)) { /* prepare to write flash */
        startup_cfg_encrypt((char *)addr+sizeof(tlv_header_t), len-sizeof(tlv_header_t));
        written_len = flash_write(startup_cfg_addr, (char *)addr, len);
        flash_part_write_done(active_part_index);
        flash_write_verify(startup_cfg_addr, (char *)addr, len);
        if(written_len != len)
        {
            startup_cfg_decrypt((char *)addr+sizeof(tlv_header_t), len-sizeof(tlv_header_t));
            if(scfg_warning_flag == 0)
                cs_printf("write startup-config error\n");
            return -1;
        }
        startup_cfg_decrypt((char *)addr+sizeof(tlv_header_t), len-sizeof(tlv_header_t));
        if(scfg_warning_flag == 0)
            cs_printf("write startup-cfg successfully\n");
        return 0;
    }
    else
    {
        if(scfg_warning_flag == 0)
            cs_printf("write startup-cfg error\n");
    }

    return -1;
}

char *startup_cfg_get_verstr(int version)
{
    switch(version)
    {
    case SCFG_KT_VERSION:
        return "KT";
    case SCFG_NTT_VERSION:
        return "NTT";
    case SCFG_CTC_VERSION:
        return "CTC";
    case SCFG_MDU_VERSION:
        return "MDU";
    default:
        return "NONE";
    }
}

#ifdef HAVE_SCFG_PROTECTION
static cs_uint8 scfg_active_cnt_compare(cs_uint8 cnt, cs_uint8 cnt1)
{
    if(cnt == cnt1)
        return cnt;
    
    if(cnt == SCFG_MAX_CNT)
    {
        if(cnt1 == SCFG_MAX_CNT-1)
            return cnt;
        else
            return cnt1;
    }

    if(cnt1 == SCFG_MAX_CNT)
    {
        if(cnt == SCFG_MAX_CNT-1)
            return cnt1;
        else
            return cnt;
    }

    if(cnt == 0xFF && cnt1 == 0xFF)
        return cnt;

    if(cnt == 0xFF)
        return cnt1;

    if(cnt1 == 0xFF)
        return cnt;

    if(cnt > cnt1)
        return cnt;
    else
        return cnt1;
}
#endif

void startup_cfg_rebuild(unsigned int *inst_id)
{
    unsigned int total_len = 0;
    char *pAllCfg = NULL;
    unsigned int new_instid = 0;
    int i;
    unsigned char value[64];
    int total_field = 0;
    unsigned char *pChar = (unsigned char *)value;
    unsigned short *pShort = (unsigned short *)value;
    unsigned int *pInt = (unsigned int *)value;
    cs_int32 read_len = 0;
    unsigned char rsvd_flag = 0;
    tlv_instance_t *pInst = (tlv_instance_t *)(*inst_id);
    tlv_header_t *pHeader = (tlv_header_t *)pInst->base_address;
    unsigned char active_cnt = 0;
    
    active_cnt = pHeader->active;
    
    total_len = startup_cfg_default_cfg_len();
    pAllCfg = (char *)malloc(total_len);
    if(pAllCfg == NULL)
        return;
    memset(pAllCfg, 0, total_len);
    new_instid = tlv_inst_init((void *)pAllCfg, total_len, NULL);
    if(new_instid == TLV_INVALID_INST_ID)
    {
        free(pAllCfg);
        return;
    }
    if(TLV_PARSER_ERROR == tlv_load(new_instid))
    {
        free((void *)new_instid);
        free(pAllCfg);
        return;
    }
    startup_config_restore(new_instid);
    rsvd_flag = rsvd_flag|TLV_CFG_ENC_FLAG;
    tlv_write_flag(new_instid, rsvd_flag);
    tlv_write_version(new_instid, gScfgVer);
#ifdef HAVE_SCFG_PROTECTION
    if(active_cnt == SCFG_MAX_CNT)
        active_cnt = 0;
    else
        active_cnt++;
#else
    active_cnt = SCFG_MAX_CNT;
#endif
    tlv_write_active(new_instid, active_cnt);
    
    total_field = sizeof(glb_field)/sizeof(glb_field[0]);
    for(i = 0 ; i < total_field ; i++)
    {
        switch(glb_field[i].value_type)
        {
        case FIELD_TYPE_CHAR:
            if(TLV_PARSER_OK == tlv_read_char(*inst_id, glb_field[i].type, pChar))
            {
                tlv_write_char(new_instid, glb_field[i].type, *pChar);
            }
            break;
        case FIELD_TYPE_SHORT:
            if(TLV_PARSER_OK == tlv_read_short(*inst_id, glb_field[i].type, pShort))
            {
                tlv_write_short(new_instid, glb_field[i].type, *pShort);
            }
            break;
        case FIELD_TYPE_INT:
            if(TLV_PARSER_OK == tlv_read_int(*inst_id, glb_field[i].type, pInt))
            {
                tlv_write_int(new_instid, glb_field[i].type, *pInt);
            }
            break;
        case FIELD_TYPE_BUFF:
        case FIELD_TYPE_STRING:
            read_len = tlv_read_buffer(*inst_id, glb_field[i].type, sizeof(value),value);
            if(0 != read_len)
            {
                tlv_write_buffer(new_instid, glb_field[i].type, glb_field[i].value_max_len,read_len,value);
            }
            break;
        default:
            break;
        }
    }

    free(pInst->base_address);
    free(pInst);
    *inst_id = new_instid;
    return;
}

static void startup_config_dump_hex();
int init_flag = 0;
void startup_config_init()
{
    tlv_header_t tlv_header;
    char *pAllCfg = NULL;
    unsigned int part_index = IROS_FLASH_PARTITION_INDEX_ANY;
    int total_len = 0;
    tlv_instance_t *pInst = NULL;
    unsigned char rsvd_flag = 0;
    int ver_invalid = 0;
    cs_int32 load_backup_flag = 0;
    cs_uint8 active_cnt = 0;
#ifdef HAVE_SCFG_PROTECTION
    unsigned int part_index1 = IROS_FLASH_PARTITION_INDEX_ANY;
    tlv_header_t tlv_header1;
    cs_uint8 active_cnt1 = 0;
#endif

    if(init_flag)
        return ;

    cs_printf("enter startup_config_init\n");
    part_index = flash_part_id_to_part_index(SCFG_PARTION);
    memset(&tlv_header , 0 , sizeof(tlv_header_t));
    memcpy(&tlv_header , flash_dev.info.super_block.part_tab[part_index].part_loc , sizeof(tlv_header_t));
    total_len = sizeof(tlv_header_t) + ntohl(tlv_header.cfg_len);
    active_cnt = tlv_header.active;
    active_part_index = part_index;
#ifdef HAVE_SCFG_PROTECTION
    part_index1 = flash_part_id_to_part_index(SCFG_PARTION1);
    memset(&tlv_header1 , 0 , sizeof(tlv_header_t));
    memcpy(&tlv_header1 , flash_dev.info.super_block.part_tab[part_index1].part_loc , sizeof(tlv_header_t));
    active_cnt1 = tlv_header1.active;
#endif

#ifdef HAVE_SCFG_PROTECTION
    if(active_cnt == scfg_active_cnt_compare(active_cnt, active_cnt1))
        active_part_index = part_index;
    else
        active_part_index = part_index1;
#endif

    if(active_part_index == IROS_FLASH_PARTITION_INDEX_ANY)
    {
        cs_printf("Invalid partition\n");
        return;
    }
    else
    {
        cs_printf("Init active startup cfg %d instance\n",active_part_index);
        goto LOAD_ACTIVE_SCFG;
LOAD_BACKUP_SCFG:
#ifdef HAVE_SCFG_PROTECTION
        if(active_part_index == part_index1)
            active_part_index = part_index;
        else
            active_part_index = part_index1;

        load_backup_flag = 1;
        cs_printf("Init backup startup cfg %d instance\n",active_part_index);
#else
        goto CFG_INIT_ERROR;
#endif
LOAD_ACTIVE_SCFG:
        memset(&tlv_header , 0 , sizeof(tlv_header_t));
        memcpy(&tlv_header , flash_dev.info.super_block.part_tab[active_part_index].part_loc , sizeof(tlv_header_t));
        total_len = sizeof(tlv_header_t) + ntohl(tlv_header.cfg_len);
        
        if(ntohl(tlv_header.magic) != TLV_HEADER_MAGIC)
        {
            if(load_backup_flag == 0)
                goto LOAD_BACKUP_SCFG;
            else
                goto CFG_INIT_ERROR;
        }

        if(ntohl(tlv_header.version) == SCFG_INVALID_VERSION)
        {
            ver_invalid = 1;
        }
        else
        {
            ver_invalid = 0;
            if(ntohl(tlv_header.version) != gScfgVer)
            {
                cs_printf("Scfg version is not match(SCFG is for %s, but FM is for %s)\n",
                    startup_cfg_get_verstr(ntohl(tlv_header.version)),
                    startup_cfg_get_verstr(gScfgVer));
                if(load_backup_flag == 0)
                    goto LOAD_BACKUP_SCFG;
                else
                    goto CFG_INIT_ERROR;
            }
        }
        
        rsvd_flag = tlv_header.reserved;
        if(rsvd_flag&TLV_CFG_ENC_FLAG)
            startup_cfg_enc_enable(1);
        else
            startup_cfg_enc_enable(0);

        pAllCfg = (char *)malloc(total_len);
        if(pAllCfg == NULL)
        {
            if(load_backup_flag == 0)
                goto LOAD_BACKUP_SCFG;
            else
                goto CFG_INIT_ERROR;
        }

        memcpy(pAllCfg , flash_dev.info.super_block.part_tab[active_part_index].part_loc , total_len);
        startup_cfg_decrypt((char *)pAllCfg+sizeof(tlv_header_t), total_len-sizeof(tlv_header_t));
        gStartupInst = tlv_inst_init((void *)pAllCfg, total_len, NULL);
        if(gStartupInst == TLV_INVALID_INST_ID)
        {
            free(pAllCfg);
            if(load_backup_flag == 0)
                goto LOAD_BACKUP_SCFG;
            else
                goto CFG_INIT_ERROR;
        }

        if(TLV_PARSER_ERROR == tlv_load(gStartupInst))
        {
            free((void *)gStartupInst);
            free(pAllCfg);
            if(load_backup_flag == 0)
                goto LOAD_BACKUP_SCFG;
            else
                goto CFG_INIT_ERROR;
        }

        if(ver_invalid)
        {
            tlv_write_version(gStartupInst, gScfgVer);
        }

        if(total_len != startup_cfg_default_cfg_len())
        {
            startup_cfg_rebuild(&gStartupInst);
            cs_printf("Startup-cfg rebuild\n");
        }
        
        pInst = (tlv_instance_t *)gStartupInst;
        pInst->store = startup_config_write_to_flash;
        gStartupInst_bak = tlv_inst_duplicate(pInst);
        init_flag = 1;
        return;
    }

CFG_INIT_ERROR:
    cs_halt("Startup-cfg file is damaged!\n");
    cs_printf("Restore startup cfg\n");
    total_len = startup_cfg_default_cfg_len();
    pAllCfg = (char *)malloc(total_len);
    if(pAllCfg == NULL)
        return;
    memset(pAllCfg , 0 , total_len);
    gStartupInst = tlv_inst_init((void *)pAllCfg, total_len, NULL);
    if(gStartupInst == TLV_INVALID_INST_ID)
    {
        free(pAllCfg);
        return ;
    }
    if(TLV_PARSER_ERROR == tlv_load(gStartupInst))
    {
        free(pAllCfg);
        free((void *)gStartupInst);
        cs_printf("Restore failed\n");
        return;
    }
    startup_config_restore(gStartupInst);
    rsvd_flag = rsvd_flag|TLV_CFG_ENC_FLAG;
    tlv_write_flag(gStartupInst, rsvd_flag);
    tlv_write_version(gStartupInst, gScfgVer);
    pInst = (tlv_instance_t *)gStartupInst;
    pInst->store = startup_config_write_to_flash;
    gStartupInst_bak = tlv_inst_duplicate(pInst);
    init_flag = 1;
    cs_printf("Restore successfully\n");
    return;
}

static void startup_config_save(cs_uint32 inst_id)
{
    tlv_instance_t *pInst = (tlv_instance_t *)inst_id;
    tlv_header_t *pHeader = NULL;

    if(pInst == NULL)
        return;

    scfg_autosave_flag = 1;
    if(pInst->store)
    {
        pHeader = (tlv_header_t *)pInst->base_address;
        pInst->store(pInst->base_address , pInst->max_tlv_len);
    }

    return;
}

static int startup_config_field_idx_get(cs_uint32 type)
{
    int i;
    int total_field = 0;

    total_field = sizeof(glb_field)/sizeof(glb_field[0]);
    for(i = 0 ; i < total_field ; i++)
    {
        if(glb_field[i].type == type)
            return i;
    }

    return -1;
}

cs_uint32 startup_config_backup_index(cs_uint8 update)
{
    cs_uint32 index =IROS_FLASH_PARTITION_INDEX_ANY;
    
#ifdef HAVE_SCFG_PROTECTION   
    index = flash_part_id_to_part_index(SCFG_PARTION);
    if(active_part_index == index)
        index = flash_part_id_to_part_index(SCFG_PARTION1); 
#else
    index = active_part_index;
#endif
    if(update)
        scfg_mif_update = 1;
        
    return index;
}

cs_status startup_config_update()  
{
    tlv_instance_t *pInst = (tlv_instance_t *)gStartupInst;
    tlv_instance_t scfg_inst;
    tlv_header_t *pHeader = NULL;
    cs_uint8 active_cnt = 0;
    cs_uint32 index;
    char *startup_cfg_addr = NULL;
    int written_len = 0;
    char addr[STARTUP_CFG_LEN]={0};
    cs_uint32 enc_flag=0;

    if(!scfg_mif_update)
        return CS_OK;
    
    if(pInst == NULL || pInst->base_address == NULL)
        return CS_ERROR;
    
    pHeader = (tlv_header_t *)pInst->base_address;
    
#ifdef HAVE_SCFG_PROTECTION   
    active_cnt = pHeader->active;
    if(active_cnt == SCFG_MAX_CNT)
        active_cnt = 0;
    else
        active_cnt++;
#else
    active_cnt = SCFG_MAX_CNT; 
#endif

    index = startup_config_backup_index(0);
    startup_cfg_addr = flash_dev.info.super_block.part_tab[index].part_loc;
    
    memcpy(addr,startup_cfg_addr,STARTUP_CFG_LEN);

    /*Don't Change current Mac address*/
    {
        int i;
        cs_uint8 mac_addr[6]={0};
        cs_uint8 c;
        startup_config_read(CFG_ID_MAC_ADDRESS, 6, mac_addr);
        /*Default scfg use enc mode.*/
        for(i = 0 ; i < sizeof(mac_addr) ; i++)
        {
            c = 0;
            c |= ((mac_addr[i]>>7)&0x01)|
                ((mac_addr[i]>>3)&0x08)|
                ((mac_addr[i]>>4)&0x02)|
                ((mac_addr[i]>>2)&0x04)|
                ((mac_addr[i]<<1)&0x10)|
                ((mac_addr[i]<<5)&0x80)|
                ((mac_addr[i]<<4)&0x20)|
                ((mac_addr[i]<<6)&0x40);
            mac_addr[i] = c;
        }
        /*Mac address offset---4 Bytes */
        memcpy(addr+sizeof(tlv_header_t)+4,mac_addr,sizeof(mac_addr));
    }
    
    enc_flag = scfg_enc_flag;
    scfg_enc_flag = 1;
    memset(&scfg_inst,0,sizeof(scfg_inst));
    scfg_inst.base_address = addr;   
    startup_cfg_decrypt((char *)addr+sizeof(tlv_header_t), STARTUP_CFG_LEN-sizeof(tlv_header_t));
    tlv_write_active((unsigned int)&scfg_inst, active_cnt);
    startup_cfg_encrypt((char *)addr+sizeof(tlv_header_t), STARTUP_CFG_LEN-sizeof(tlv_header_t));  
    scfg_enc_flag = enc_flag;

    if (flash_part_write_init(index, startup_cfg_addr)) { /* prepare to write flash */
        written_len = flash_write(startup_cfg_addr, addr, STARTUP_CFG_LEN);
        flash_part_write_done(index);
        flash_write_verify(startup_cfg_addr, addr, STARTUP_CFG_LEN);
        if(written_len != STARTUP_CFG_LEN)
        {
            cs_printf("Update startup-config error\n");
            return CS_E_ERROR;
        }
        scfg_mif_update = 0;
        cs_printf("Update startup-cfg %d successfully\n",index);  
        return CS_OK;
    }
    else
    {
        cs_printf("write startup-cfg error\n");
    }
    return CS_E_ERROR;

}


cs_status startup_config_write(cs_uint32 type, cs_uint32 len, cs_uint8 *buf)
{
    cs_uint32 field_idx = -1;
    int ret = CS_ERROR;
    cs_int8 tempbuf[32];
    tlv_instance_t *pInst = (tlv_instance_t *)gStartupInst;
    tlv_header_t *pHeader = NULL;
    cs_uint8 active_cnt = 0;

    field_idx = startup_config_field_idx_get(type);
    if(field_idx == -1)
        return CS_ERROR;

    if(pInst == NULL || pInst->base_address == NULL)
        return CS_ERROR;
    
    pHeader = (tlv_header_t *)pInst->base_address;
#ifdef HAVE_SCFG_PROTECTION
    active_cnt = pHeader->active;
    if(active_cnt == SCFG_MAX_CNT)
        active_cnt = 0;
    else
        active_cnt++;
#else
    active_cnt = SCFG_MAX_CNT;
#endif

    ret = tlv_write_active(gStartupInst, active_cnt);
    if(ret == TLV_PARSER_ERROR)
        return CS_ERROR;

    switch(glb_field[field_idx].value_type)
    {
        case FIELD_TYPE_CHAR:
            if(len != glb_field[field_idx].value_max_len)
                return CS_ERROR;
            ret = tlv_write_char(gStartupInst, (cs_uint16)type, buf[0]);
            break;
        case FIELD_TYPE_SHORT:
            if(len != glb_field[field_idx].value_max_len)
                return CS_ERROR;
            ret = tlv_write_short(gStartupInst, (cs_uint16)type, *((unsigned short *)buf));
            break;
        case FIELD_TYPE_INT:
            if(len != glb_field[field_idx].value_max_len)
                return CS_ERROR;
            ret = tlv_write_int(gStartupInst, (cs_uint16)type, *((unsigned int *)buf));
            break;
        case FIELD_TYPE_BUFF:
        case FIELD_TYPE_STRING:
            if(len > glb_field[field_idx].value_max_len)
                return CS_ERROR;

            if(len == 0 || buf == NULL)
            {
                len = glb_field[field_idx].value_max_len;
                memset(tempbuf , 0 , sizeof(tempbuf));
                buf = tempbuf;
            }

            ret = tlv_write_buffer(gStartupInst, (cs_uint16)type, glb_field[field_idx].value_max_len , len , buf);
            break;
        default:
            return CS_ERROR;
    }

    if(ret == TLV_PARSER_ERROR)
        return CS_ERROR;

    if(/*type >= CFG_ID_MAC_ADDRESS && */type <= CFG_ID_GE_CLK_SEL)
    {
        scfg_warning_flag = 1;
        startup_config_save(gStartupInst);
        scfg_warning_flag = 0;
    }

    return CS_OK;
}

cs_status startup_config_read(cs_uint32 type, cs_uint32 len, cs_uint8 *buf)
{
    cs_uint32 field_idx = -1;
    int ret = CS_ERROR;
    cs_uint32 inst_id = TLV_INVALID_INST_ID;

    field_idx = startup_config_field_idx_get(type);
    if(field_idx == -1)
        return CS_E_NOT_FOUND;

    if(len < glb_field[field_idx].value_max_len)
        return CS_ERROR;

    if(glb_field[field_idx].mode == SCFG_W)
        inst_id = gStartupInst;
    else
        inst_id = gStartupInst_bak;
    
    switch(glb_field[field_idx].value_type)
    {
        case FIELD_TYPE_CHAR:
            ret = tlv_read_char(inst_id, (cs_uint16)type, buf);
            break;
        case FIELD_TYPE_SHORT:
            ret = tlv_read_short(inst_id, (cs_uint16)type, (unsigned short *)buf);
            break;
        case FIELD_TYPE_INT:
            ret = tlv_read_int(inst_id, (cs_uint16)type, (unsigned int *)buf);
            break;
        case FIELD_TYPE_BUFF:
        case FIELD_TYPE_STRING:
            ret = tlv_read_buffer(inst_id, (cs_uint16)type, len , buf);
            break;
        default:
            return CS_ERROR;
    }

    if(ret == TLV_PARSER_ERROR)
        return CS_ERROR;

    return CS_OK;
}


static void startup_config_dump(unsigned int inst_id)
{
    int i , total_field = 0;
    unsigned char value[64];
    unsigned char *pChar = value;
    unsigned short *pShort = (unsigned short *)value;
    unsigned int *pInt = (unsigned int *)value;
    cfg_field_t *pField = NULL;
    int ret = TLV_PARSER_ERROR;

    total_field = sizeof(glb_field)/sizeof(glb_field[0]);
    cs_printf("%s : %d(%s)\n","Total field number",
                        total_field,scfg_enc_flag?"Encrypted":"Decrypted");
    for(i = 0 ; i < total_field ; i++)
    {
        pField = &glb_field[i];
        switch(pField->value_type)
        {
        case FIELD_TYPE_CHAR:
            *pChar = 0;
            ret = tlv_read_char(inst_id, pField->type , pChar);
            if(ret == TLV_PARSER_OK)
                cs_printf("(%3d)%32s : %d\n", pField->type,pField->desc , *pChar);
            else
                cs_printf("(%3d)%32s : %s\n", pField->type,pField->desc , "New field need to be initialized...");
            break;
        case FIELD_TYPE_SHORT:
            *pShort = 0;
            ret = tlv_read_short(inst_id, pField->type , pShort);
            if(ret == TLV_PARSER_OK)
                cs_printf("(%3d)%32s : %d\n", pField->type,pField->desc , *pShort);
            else
                cs_printf("(%3d)%32s : %s\n", pField->type,pField->desc , "New field need to be initialized...");
            break;
        case FIELD_TYPE_INT:
            *pInt = 0;
            ret = tlv_read_int(inst_id, pField->type , pInt);
            if(ret == TLV_PARSER_OK)
                cs_printf("(%3d)%32s : %d\n", pField->type,pField->desc , *pInt);
            else
                cs_printf("(%3d)%32s : %s\n", pField->type,pField->desc , "New field need to be initialized...");
            break;
        case FIELD_TYPE_BUFF:
            *pChar = 0;
            ret = 0;
            memset(pChar , 0 , sizeof(value));
            ret = tlv_read_buffer(inst_id, pField->type , pField->value_max_len , pChar);
            if(ret > 0)
            {
                int j;
                cs_printf("(%3d)%32s : ", pField->type,pField->desc);

                for(j = 0 ; j < ret ; j++)
                    cs_printf("%02x ", pChar[j]);
                cs_printf("\n");
            }
            else
                cs_printf("(%3d)%32s : %s\n", pField->type,pField->desc , "New field need to be initialized...");
            break;
        case FIELD_TYPE_STRING:
            *pChar = 0;
            ret = 0;
            memset(pChar , 0 , sizeof(value));
            ret = tlv_read_buffer(inst_id, pField->type , pField->value_max_len , pChar);
            if(ret > 0)
            {
                cs_printf("(%3d)%32s : %s\n", pField->type,pField->desc,pChar);
            }
            else
                cs_printf("(%3d)%32s : %s\n", pField->type,pField->desc , "New field need to be initialized...");
            break;
        default:
            break;
        }
    }

    return;
}

static void startup_config_dump_hex()
{
    int i;
    tlv_header_t *pHeader = NULL;
    tlv_instance_t *pInst = (tlv_instance_t *)gStartupInst;
    int total_len = 0;
    unsigned char *pbuf = NULL;

    pHeader = (tlv_header_t *)pInst->base_address;
    pbuf = (unsigned char *)pHeader;
    total_len = sizeof(tlv_header_t)+ntohl(pHeader->cfg_len);

    for(i = 0 ; i < total_len ; i++)
    {
        if(i%16 == 0)
            cs_printf("\n");
        cs_printf("%02x ",pbuf[i]);
    }
    cs_printf("\n");

    return;
}

static void startup_config_reset(cs_uint32 inst_id)
{
    tlv_instance_t *pInst = (tlv_instance_t *)inst_id;
    int (*store)(void *base_addr , int len) = NULL;
    tlv_header_t *pHeader = NULL;
    cs_uint8 flag = 0;
    int total_len = 0;
    cs_uint8 mac[6];
    cs_uint8 active_cnt = 0;

    if(pInst == NULL)
        return;

    startup_config_read(CFG_ID_MAC_ADDRESS, 6, mac);
    if(pInst->store)
        store = pInst->store;

    pHeader = (tlv_header_t *)pInst->base_address;
    active_cnt = pHeader->active;
    pInst->store = NULL;
    startup_cfg_enc_enable(1);
    pInst->tlv_num = 0;

    total_len = startup_cfg_default_cfg_len();
    if(total_len > pInst->max_tlv_len)
    {
        free(pInst->base_address);
        pInst->base_address = (void *)malloc(total_len);
        pInst->max_tlv_len = total_len;
    }

    if(pInst->base_address == NULL)
        return;
    memset(pInst->base_address , 0 , pInst->max_tlv_len);
    if(tlv_load(inst_id) == TLV_PARSER_ERROR)
    {
        return;
    }
    startup_config_restore(inst_id);
    startup_config_write(CFG_ID_MAC_ADDRESS, 6, mac);
    flag = flag|TLV_CFG_ENC_FLAG;
    tlv_write_flag(inst_id, flag);
    tlv_write_version(inst_id, gScfgVer);
#ifdef HAVE_SCFG_PROTECTION
    if(active_cnt == SCFG_MAX_CNT)
        active_cnt = 0;
    else
        active_cnt++;
#else
    active_cnt = SCFG_MAX_CNT;
#endif
    tlv_write_active(inst_id, active_cnt);
    pInst->store = store;
    if(pInst->store)
    {
        pInst->store(pInst->base_address , pInst->max_tlv_len);
    }

    return;
}

static void startup_config_clear(int type)
{
    int field_idx = -1;
    cfg_field_t *pField = NULL;
    unsigned char c_v = 0;
    unsigned short s_v = 0;
    unsigned int i_v = 0;
    unsigned char buf[64];
    tlv_header_t *pHeader = NULL;
    cs_uint8 active_cnt = 9;
    tlv_instance_t *pInst = (tlv_instance_t *)gStartupInst;

    field_idx = startup_config_field_idx_get(type);
    if(field_idx == -1)
    {
        cs_printf("Invalid type.\n");
        return;
    }

    pHeader = (tlv_header_t *)pInst->base_address;
#ifdef HAVE_SCFG_PROTECTION
    active_cnt = pHeader->active;
    if(active_cnt == SCFG_MAX_CNT)
        active_cnt = 0;
    else
        active_cnt++;
#else
    active_cnt = SCFG_MAX_CNT;
#endif
    tlv_write_active(gStartupInst , active_cnt);

    pField = &glb_field[field_idx];
    switch(pField->value_type)
    {
    case FIELD_TYPE_CHAR:
        c_v = (unsigned char)pField->dflt_val;
        tlv_write_char(gStartupInst, type , c_v);
        break;
    case FIELD_TYPE_SHORT:
        s_v = (unsigned short)pField->dflt_val;
        tlv_write_short(gStartupInst, type , s_v);
        break;
    case FIELD_TYPE_INT:
        i_v = (unsigned int)pField->dflt_val;
        tlv_write_int(gStartupInst, type , i_v);
        break;
    case FIELD_TYPE_BUFF:
        tlv_write_buffer(gStartupInst, type, pField->value_max_len, pField->value_max_len, pField->dflt_buf);
        break;
    case FIELD_TYPE_STRING:
        memset(buf , 0, sizeof(buf));
        tlv_write_buffer(gStartupInst, type, pField->value_max_len, pField->value_max_len, buf);
        break;
    default:
        break;
    }

    return;
}

sal_cmd_result_t startup_cfg_cmd_show(int argc, char **argv)
{
    if(argc == 2)
    {
        startup_config_dump(gStartupInst);
        return SAL_CMD_OK;
    }

    if(argc == 3 && !strcmp(argv[2],"h"))
    {
        startup_config_dump_hex();
        return SAL_CMD_OK;
    }

    if(argc == 3 && !strcmp(argv[2],"r"))
    {
        startup_config_dump(gStartupInst_bak);
        return SAL_CMD_OK;
    }

    return SAL_CMD_FAIL;
}

sal_cmd_result_t startup_cfg_cmd_reset(int argc, char **argv)
{
    startup_config_reset(gStartupInst);
    return SAL_CMD_OK;
}

sal_cmd_result_t startup_cfg_cmd_set(int argc, char **argv)
{
    int type = 0;
    int field_idx = -1;
    cfg_field_t *pField = NULL;
    unsigned char buff[32];
    unsigned char *pChar = buff;
    unsigned short *pShort = (unsigned short *)buff;
    unsigned int *pInt = (unsigned int *)buff;

    if(argc > 4)
        return SAL_CMD_FAIL;

    type = atoi(argv[2]);
    field_idx = startup_config_field_idx_get(type);
    if(field_idx == -1)
    {
        cs_printf("Invalid type.\n");
        return SAL_CMD_FAIL;
    }
    pField = &glb_field[field_idx];

    if(argc == 3 && !strcmp(argv[1],"set"))
    {
        cs_printf("\n%s\n",pField->help_str);
        return SAL_CMD_OK;
    }

    switch(pField->value_type)
    {
    case FIELD_TYPE_CHAR:
        *pChar = (unsigned char)atoi(argv[3]);
        if(pField->range_low != -1 && pField->range_high != -1)
        {
            if(*pChar > pField->range_high)
            {
                cs_printf("Out of range(Min:%d, Max:%d)\n",pField->range_low,pField->range_high);
                cs_printf("\n%s\n",pField->help_str);
                return SAL_CMD_OK;
            }
        }
        if(pField->type == CFG_ID_CUSTOM_MODE_SEL)
        {
            if(*pChar != 0 && *pChar != 1 && *pChar != 2 && *pChar != 0xFF)
            {
                cs_printf("This field must be one of the following value(0,1,2,0xff)\n");
                cs_printf("\n%s\n",pField->help_str);
                return SAL_CMD_OK;
            }
        }
        startup_config_write(pField->type, pField->value_max_len, (unsigned char *)pChar);
        break;
    case FIELD_TYPE_SHORT:
        *pShort = (unsigned short)atoi(argv[3]);
        if(pField->range_low != -1 && pField->range_high != -1)
        {
            if(*pShort > pField->range_high)
            {
                cs_printf("Out of range(Min:%d, Max:%d)\n",pField->range_low,pField->range_high);
                cs_printf("\n%s\n",pField->help_str);
                return SAL_CMD_OK;
            }
        }
        startup_config_write(pField->type, pField->value_max_len, (unsigned char *)pShort);
        break;
    case FIELD_TYPE_INT:
        *pInt = (unsigned int)atoi(argv[3]);
        if(pField->range_low != -1 && pField->range_high != -1)
        {
            if(*pInt > pField->range_high)
            {
                cs_printf("Out of range(Min:%d, Max:%d)\n",pField->range_low,pField->range_high);
                cs_printf("\n%s\n",pField->help_str);
                return SAL_CMD_OK;
            }
        }
        startup_config_write(pField->type, pField->value_max_len, (unsigned char *)pInt);
        break;
    case FIELD_TYPE_BUFF:
        memset(pChar , 0 , sizeof(buff));
        if(cs_str2hex(argv[3], (char *)pChar, pField->value_max_len))
            cs_printf("\n%s\n",pField->help_str);
        else
        {
            if(pField->type == CFG_ID_MAC_ADDRESS && (pChar[0]&0x01))
            {
                cs_printf("ONU MAC must not be a multicast mac\n");
                return SAL_CMD_OK;
            }
            startup_config_write(pField->type, pField->value_max_len, (unsigned char *)pChar);
        }
        break;
    case FIELD_TYPE_STRING:
        if(strlen(argv[3]) > pField->value_max_len)
        {
            cs_printf("The maximum length should be less than or equal to %d bytes.\n",pField->value_max_len);
            return SAL_CMD_OK;
        }
        startup_config_write(pField->type, strlen(argv[3]), (unsigned char *)argv[3]);
        break;
    default:
        break;
    }

    return SAL_CMD_OK;
}

sal_cmd_result_t startup_cfg_cmd_clear(int argc, char **argv)
{
    int type = -1;

    if(argc != 3)
        return SAL_CMD_OK;
    type = atoi(argv[2]);
    startup_config_clear(type);
    return SAL_CMD_OK;
}

sal_cmd_result_t startup_cfg_cmd_dump(int argc, char **argv)
{
    char *buf;
    int err=0;
    unsigned int part_index=0;
    unsigned int scfg_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_STARTUP_CONFIG_ID);
    unsigned int bk_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_STARTUP_CONFIG1_ID);
    if (argc!=3)
    {
        cs_printf("Error! scfg load [index] \n");   
        return SAL_CMD_OK;
    }
    part_index = iros_strtol(argv[2]);
    if ((part_index!=scfg_index)&&(part_index!=bk_index))
    {
        cs_printf("Para error! scfg index error");   
        return SAL_CMD_OK;
    }
    buf=flash_dev.info.super_block.part_tab[part_index].part_loc;
    XModem_stream_put(buf, STARTUP_CFG_LEN, &err);
    cs_printf("Dump Finish %x \n",err);
    return SAL_CMD_OK;
}

sal_cmd_result_t startup_cfg_cmd_enc(int argc, char **argv)
{
    unsigned char flag = 0;
    tlv_header_t *pHeader = NULL;
    cs_uint8 active_cnt = 9;
    tlv_instance_t *pInst = (tlv_instance_t *)gStartupInst;
    
    if(argc != 3)
    {
        cs_printf("scfg enc enable  - Enable startup config encryption.\n");
        cs_printf("scfg enc disable - Disable startup config encryption.\n");
        return SAL_CMD_OK;
    }

    pHeader = (tlv_header_t *)pInst->base_address;
#ifdef HAVE_SCFG_PROTECTION
    active_cnt = pHeader->active;
    if(active_cnt == SCFG_MAX_CNT)
        active_cnt = 0;
    else
        active_cnt++;
#else
    active_cnt = SCFG_MAX_CNT;
#endif
    tlv_write_active(gStartupInst , active_cnt);

    if(!strcmp(argv[2],"enable"))
    {
        startup_cfg_enc_enable(1);
        tlv_read_flag(gStartupInst, &flag);
        flag = flag|TLV_CFG_ENC_FLAG;
        tlv_write_flag(gStartupInst, flag);
    }

    if(!strcmp(argv[2],"disable"))
    {
        startup_cfg_enc_enable(0);
        tlv_read_flag(gStartupInst, &flag);
        flag = flag&(~TLV_CFG_ENC_FLAG);
        tlv_write_flag(gStartupInst, flag);
    }

    return SAL_CMD_OK;
}

int private_cfg_cmd_proc(int argc, char **argv)
{
    if(argc < 2 || !cmd_cmp(argv[0],"scfg"))
        return 0;

    if(argc == 3 && cmd_cmp(argv[1],"auto-save"))
    {
        if(cmd_cmp(argv[2],"enable"))
            scfg_autosave_flag = 1;
        else
            scfg_autosave_flag = 0;
        return 1;
    }

    if(argc == 2 && cmd_cmp(argv[1],"save"))
    {
        startup_config_save(gStartupInst);
        return 1;
    }

    return 0;
}


#if 0
static void startup_cfg_help()
{
    cs_printf("scfg show         - dump all startup configuration\n");
    cs_printf("scfg show h       - dump all startup configuration in hex-mode\n");
    cs_printf("scfg reset        - set all configuration to default value\n");
    cs_printf("scfg set <T> <V>  - set specific field value(T:type, V:value)\n");
    cs_printf("scfg clear <type> - clear specific field\n");
}

int startup_cfg_cmd_proc(int argc, char **argv)
{
    int type = 0;
    int field_idx = -1;
    cfg_field_t *pField = NULL;
    unsigned char buff[32];
    unsigned char *pChar = buff;
    unsigned short *pShort = (unsigned short *)buff;
    unsigned int *pInt = (unsigned int *)buff;

    if(strcmp(argv[0],"scfg"))
        return 0;

    if(argc == 1)
    {
        startup_cfg_help();
        return 1;
    }

    if(argc == 2)
    {
        if(!strcmp(argv[1],"show"))
            startup_config_dump();
        else if(!strcmp(argv[1],"reset"))
            startup_config_reset(gStartupInst);
        else
            startup_cfg_help();
        return 1;
    }

    if(argc == 3 && strcmp(argv[1],"set"))
    {
        if(!strcmp(argv[1],"show") && !strcmp(argv[2],"h"))
            startup_config_dump_hex();
        else if(!strcmp(argv[1],"clear"))
        {
            type = atoi(argv[2]);
            startup_config_clear(type);
        }
        else if(!strcmp(argv[1],"enc"))
        {
            if(!strcmp(argv[2],"enable"))
            {
                startup_cfg_enc_enable(1);
                cs_printf("Encrypt startup-cfg enable,Please reset startup-cfg\n");
            }
            if(!strcmp(argv[2],"disable"))
            {
                startup_cfg_enc_enable(0);
                cs_printf("Encrypt startup-cfg disable,Please reset startup-cfg\n");
            }
        }
        else
            startup_cfg_help();
        return 1;
    }

    type = atoi(argv[2]);
    field_idx = startup_config_field_idx_get(type);
    if(field_idx == -1)
    {
        cs_printf("Invalid type.\n");
        return 1;
    }
    pField = &glb_field[field_idx];

    if(argc == 3 && !strcmp(argv[1],"set"))
    {
        cs_printf("\n%s\n",pField->help_str);
        return 1;
    }

    if(argc > 4)
    {
        startup_cfg_help();
        return 1;
    }

    switch(pField->value_type)
    {
    case FIELD_TYPE_CHAR:
        *pChar = (unsigned char)atoi(argv[3]);
        if(pField->range_low != -1 && pField->range_high != -1)
        {
            if(*pChar > pField->range_high)
            {
                cs_printf("Out of range(Min:%d, Max:%d)\n",pField->range_low,pField->range_high);
                cs_printf("\n%s\n",pField->help_str);
                return 1;
            }
        }
        if(pField->type == CFG_ID_CUSTOM_MODE_SEL)
        {
            if(*pChar != 0 && *pChar != 1 && *pChar != 2 && *pChar != 0xFF)
            {
                cs_printf("This field must be one of the following value(0,1,2,0xff)\n");
                cs_printf("\n%s\n",pField->help_str);
                return 1;
            }
        }
        tlv_write_char(gStartupInst, pField->type , *pChar);
        break;
    case FIELD_TYPE_SHORT:
        *pShort = (unsigned short)atoi(argv[3]);
        if(pField->range_low != -1 && pField->range_high != -1)
        {
            if(*pShort > pField->range_high)
            {
                cs_printf("Out of range(Min:%d, Max:%d)\n",pField->range_low,pField->range_high);
                cs_printf("\n%s\n",pField->help_str);
                return 1;
            }
        }
        tlv_write_short(gStartupInst, pField->type , *pShort);
        break;
    case FIELD_TYPE_INT:
        *pInt = (unsigned int)atoi(argv[3]);
        if(pField->range_low != -1 && pField->range_high != -1)
        {
            if(*pInt > pField->range_high)
            {
                cs_printf("Out of range(Min:%d, Max:%d)\n",pField->range_low,pField->range_high);
                cs_printf("\n%s\n",pField->help_str);
                return 1;
            }
        }
        tlv_write_int(gStartupInst, pField->type , *pInt);
        break;
    case FIELD_TYPE_BUFF:
        if(pField->type == CFG_ID_8021x_USERNAME ||
            pField->type == CFG_ID_8021x_PWD)
        {
            tlv_write_buffer(gStartupInst, pField->type, pField->value_max_len, strlen(argv[3]), (unsigned char *)argv[3]);
        }
        else
        {
            memset(pChar , 0 , sizeof(buff));
            if(cs_str2hex(argv[3], (char *)pChar, pField->value_max_len))
                cs_printf("\n%s\n",pField->help_str);
            else
            {
                if(pField->type == CFG_ID_MAC_ADDRESS && (pChar[0]&0x01))
                {
                    cs_printf("ONU MAC must not be a multicast mac\n");
                    return 1;
                }
                tlv_write_buffer(gStartupInst, pField->type, pField->value_max_len, pField->value_max_len, pChar);
            }
        }
        break;
    default:
        break;
    }

    return 1;
}
#endif

