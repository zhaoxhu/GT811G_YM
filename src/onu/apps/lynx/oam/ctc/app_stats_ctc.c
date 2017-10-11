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

DEFINITIONS:  "DEVICE" means the Cortina Systems? Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems? SDK software.

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
#ifdef HAVE_CTC_OAM
#include "plat_common.h"
#include "port_stats.h"
#include "oam_ctc_client.h"
#include "app_stats_ctc.h"
#include "app_alarm_ctc.h"
#include "sdl_port.h"

#define STATS_MONITOR_ALARM_CHECK(name, id)          value = ctrl->last_duration.name;  \
                                                                                    if(CS_E_OK == ctc_oam_get_alarm_threshold(type, port,  \
                                                                                                              id, &raise_value, &clear_value)) \
                                                                                    {     \
                                                                                        if(value >= raise_value)  \
                                                                                        {     \
                                                                                            ctc_oam_build_alarm_event(type, port, id,  \
                                                                                                                                 CTC_ALARM_RAISE, value);  \
                                                                                        }  \
                                                                                        else if(value < clear_value) \
                                                                                        { \
                                                                                            ctc_oam_build_alarm_event(type, port, id,   \
                                                                                                                                   CTC_ALARM_CLEAR, value); \
                                                                                        }  \
                                                                                    }  \

/*pon port(index 0) and uni port(index 1...) monitor*/
ctc_stats_monitor_ctrl_t *g_moitor_stats = NULL;


void ctc_onu_stats_monitor_init()
{
    cs_uint8 i = 0;
    cs_uint32 len = 0;

    len = sizeof(ctc_stats_monitor_ctrl_t)*(CTC_MAX_ONU_UNI_PORTS + 1);
    
    g_moitor_stats = (ctc_stats_monitor_ctrl_t*)iros_malloc(IROS_MID_OAM, len);
    if(g_moitor_stats == NULL)
        return;

    memset(g_moitor_stats, 0, len);
    for(i = 0; i <= CTC_MAX_ONU_UNI_PORTS; i++)
    {
        g_moitor_stats[i].port = i;
        g_moitor_stats[i].msg_info.msg_type = IROS_MSG_TIME_OUT;
        g_moitor_stats[i].msg_info.timer_type = APP_TIMER_PERFORM_MONITOR;
    }
}

cs_status ctc_onu_stats_monitor_stop(cs_port_id_t port)
{
    ctc_stats_monitor_ctrl_t *ctrl = NULL;
    
    if(port > CTC_MAX_ONU_UNI_PORTS)
        return CS_E_PARAM;

    ctrl = &g_moitor_stats[port];
    if(ctrl->timer != 0)
    {
        cs_timer_del(ctrl->timer);
    }
    memset(ctrl, 0, sizeof(ctc_stats_monitor_ctrl_t));
    ctrl->port = port;
    ctrl->msg_info.msg_type = IROS_MSG_TIME_OUT;
    ctrl->msg_info.timer_type = APP_TIMER_PERFORM_MONITOR;

    return CS_E_OK;
}

cs_status ctc_onu_performance_stats_get(
        cs_port_id_t port,
        oam_ctc_onu_port_stats_t * stats)
{
    cs_status ret = CS_E_OK;

    if(port > CTC_MAX_ONU_UNI_PORTS || !stats)
        return CS_E_PARAM;
    
    if(port == CS_PON_PORT_ID)
    {
        oam_pon_stats_t pon_stas;
        memset(&pon_stas, 0, sizeof(pon_stas));
        ret = app_onu_pon_stats_get(&pon_stas);
        if(ret != CS_E_OK)
        {
            return ret;
        }
        stats->dn_drop_events            = 0;  
        stats->up_drop_events            = 0;      
        stats->dn_octets                 = pon_stas.pon_byte_cnt;      
        stats->up_octets                 = pon_stas.pon_tx_byte_cnt;      
        stats->dn_frames                 = pon_stas.pon_frame_cnt + pon_stas.pon_mcframe_cnt + 
                                                     pon_stas.pon_bcframe_cnt;       
        stats->up_frames                 = pon_stas.pon_txframe_cnt + pon_stas.pon_txmcframe_cnt +
                                                    pon_stas.pon_txbcframe_cnt;       
        stats->dn_bc_frames              = pon_stas.pon_bcframe_cnt;         
        stats->up_bc_frames              = pon_stas.pon_txbcframe_cnt;         
        stats->dn_mc_frames              = pon_stas.pon_mcframe_cnt;          
        stats->up_mc_frames              = pon_stas.pon_txmcframe_cnt;          
        stats->dn_crc_err_frames         = pon_stas.pon_crc8_err;   
        stats->up_crc_err_frames         = pon_stas.pon_tx_crc_err;   
        
        stats->dn_undersize_frames       = pon_stas.pon_undersizecnt;           
        stats->up_undersize_frames       = 0;           
        stats->dn_oversize_frames        = pon_stas.pon_oversize_cnt;          
        stats->up_oversize_frames        = pon_stas.pon_txoversizecnt;          
        stats->dn_fragments                = 0;
        stats->up_fragments                = 0;
        stats->dn_jabbers                    = pon_stas.pon_jabbercnt; 
        stats->up_jabbers                    = 0;
        /*stats->dn_collisions                  = 0; */            
        stats->dn_64_octets_frames       = pon_stas.pon_rmon64_cnt;            
        stats->dn_65_127_octets_frames   = pon_stas.pon_rmon65_127_cnt;                
        stats->dn_128_255_octets_frames  = pon_stas.pon_rmon128_255_cnt;                
        stats->dn_256_511_octets_frames  = pon_stas.pon_rmon256_511_cnt;                 
        stats->dn_512_1023_octets_frames = pon_stas.pon_rmon512_1023_cnt;                 
        stats->dn_1024_1518_octets_frames= pon_stas.pon_rmon1024_1518_cnt;                  
        stats->up_64_octets_frames          = pon_stas.pon_txrmon64_cnt;                
        stats->up_65_127_octets_frames   = pon_stas.pon_txrmon65_127_cnt;                
        stats->up_128_255_octets_frames  = pon_stas.pon_txrmon128_255_cnt;                  
        stats->up_256_511_octets_frames  = pon_stas.pon_txrmon256_511_cnt;                 
        stats->up_512_1023_octets_frames = pon_stas.pon_txrmon512_1023_cnt;                  
        stats->up_1024_1518_octets_frames = pon_stas.pon_txrmon1024_1518_cnt;                   
        stats->dn_discard_frames         = 0;                 
        stats->up_discard_frames         = 0;              
        stats->dn_err_frames             = pon_stas.pon_crc8_err + pon_stas.pon_undersizecnt + 
                                                      pon_stas.pon_oversize_cnt + pon_stas.pon_jabbercnt;  

        stats->up_err_frames=pon_stas.pon_txoversizecnt ;
        stats->status_change_times       = 0;  
    }
    else
    {
        oam_port_uni_stats_t uni_stas;
        cs_uint32 change_count = 0;
        memset(&uni_stas, 0, sizeof(uni_stas));
        ret = app_onu_port_stats_get(port, &uni_stas);
        if(ret != CS_E_OK)
        {
            return ret;
        }
        app_onu_port_status_change_get(port, &change_count);
        
        stats->dn_drop_events            = 0;  
        stats->up_drop_events            = 0;      
        stats->dn_octets                 = uni_stas.txbyte_cnt;      
        stats->up_octets                 = uni_stas.rxbyte_cnt;      
        stats->dn_frames                 = uni_stas.txucfrm_cnt + uni_stas.txmcfrm_cnt + uni_stas.txbcfrm_cnt;       
        stats->up_frames                 = uni_stas.rxucfrm_cnt + uni_stas.rxmcfrm_cnt + uni_stas.rxbcfrm_cnt;       
        stats->dn_bc_frames              = uni_stas.txbcfrm_cnt;         
        stats->up_bc_frames              = uni_stas.rxbcfrm_cnt ; 

        
        stats->dn_mc_frames              = uni_stas.txmcfrm_cnt;          
        stats->up_mc_frames              = uni_stas.rxmcfrm_cnt;          
        stats->dn_crc_err_frames         = uni_stas.txcrcerrfrm_cnt;  
        stats->up_crc_err_frames         = uni_stas.rxcrcerrfrm_cnt; 
        
        stats->dn_undersize_frames       = 0;           
        stats->up_undersize_frames       = uni_stas.rxundersizefrm_cnt;           
        stats->dn_oversize_frames        = uni_stas.txoversizefrm_cnt;          
        stats->up_oversize_frames        = uni_stas.rxoversizefrm_cnt;          
        stats->dn_fragments              = 0;
        stats->up_fragments              = 0; 
        stats->dn_jabbers                = 0;
        stats->up_jabbers                =uni_stas.rxjabberfrm_cnt;  
        /*stats->dn_collisions             = uni_stas.single_collision + uni_stas.multiple_collision +
                                                   uni_stas.late_collisions + uni_stas.excessive_collisions; */            
        stats->dn_64_octets_frames       = uni_stas.txstatsfrm64_cnt;            
        stats->dn_65_127_octets_frames   = uni_stas.txstatsfrm65_127_cnt;                
        stats->dn_128_255_octets_frames  = uni_stas.txstatsfrm128_255_cnt;                
        stats->dn_256_511_octets_frames  = uni_stas.txstatsfrm256_511_cnt;                 
        stats->dn_512_1023_octets_frames = uni_stas.txstatsfrm512_1023_cnt;                 
        stats->dn_1024_1518_octets_frames= uni_stas.txstatsfrm1024_1518_cnt;                  
        stats->up_64_octets_frames       = uni_stas.rxstatsfrm64_cnt;                
        stats->up_65_127_octets_frames   = uni_stas.rxstatsfrm65_127_cnt;                
        stats->up_128_255_octets_frames  = uni_stas.rxstatsfrm128_255_cnt;                  
        stats->up_256_511_octets_frames  = uni_stas.rxstatsfrm256_511_cnt;                 
        stats->up_512_1023_octets_frames = uni_stas.rxstatsfrm512_1023_cnt;                  
        stats->up_1024_1518_octets_frames= uni_stas.rxstatsfrm1024_1518_cnt;                   
        stats->dn_discard_frames         = 0;                 
        stats->up_discard_frames         = 0;              
        stats->dn_err_frames             =uni_stas.txcrcerrfrm_cnt+uni_stas.txoversizefrm_cnt ; 
         
        stats->up_err_frames            = uni_stas.rxcrcerrfrm_cnt + uni_stas.rxundersizefrm_cnt + 
                                                        uni_stas.rxoversizefrm_cnt ;
        stats->status_change_times    = change_count;  
    }
    return CS_E_OK;
}

void ctc_onu_stats_monitor_alarm_check(ctc_stats_monitor_ctrl_t *ctrl)
{
    cs_uint64 value = 0;
    cs_uint32 raise_value = 0, clear_value  = 0;
    ctc_oam_alarm_class_e type = CTC_PON_ALARM;
    cs_port_id_t port = CS_PON_PORT_ID;
    
    if(ctrl->port == 0)
    {
        type = CTC_PON_ALARM;
        port = CS_PON_PORT_ID;
        
        /*check for CTC_ALARM_PON_RX_DROP*/
        STATS_MONITOR_ALARM_CHECK(dn_drop_events, CTC_ALARM_PON_RX_DROP) 
        STATS_MONITOR_ALARM_CHECK(dn_drop_events, CTC_WARN_PON_RX_DROP)
        /*check for CTC_ALARM_PON_TX_DROP*/
        STATS_MONITOR_ALARM_CHECK(up_drop_events, CTC_ALARM_PON_TX_DROP)  
        STATS_MONITOR_ALARM_CHECK(up_drop_events, CTC_WARN_PON_TX_DROP)
        /*check for CTC_ALARM_PON_RX_CRC*/
        STATS_MONITOR_ALARM_CHECK(dn_crc_err_frames, CTC_ALARM_PON_RX_CRC) 
        STATS_MONITOR_ALARM_CHECK(dn_crc_err_frames, CTC_WARN_PON_RX_CRC) 

        /*check for CTC_ALARM_PON_TX_CRC*/
        STATS_MONITOR_ALARM_CHECK(up_crc_err_frames, CTC_ALARM_PON_TX_CRC) 
        STATS_MONITOR_ALARM_CHECK(up_crc_err_frames, CTC_WARN_PON_TX_CRC) 
        
        /*check for CTC_ALARM_PON_RX_UNDER*/
        STATS_MONITOR_ALARM_CHECK(dn_undersize_frames, CTC_ALARM_PON_RX_UNDER)  
        STATS_MONITOR_ALARM_CHECK(dn_undersize_frames, CTC_WARN_PON_RX_UNDER)  
        /*check for CTC_ALARM_PON_TX_UNDER*/
        STATS_MONITOR_ALARM_CHECK(up_undersize_frames, CTC_ALARM_PON_TX_UNDER)  
        STATS_MONITOR_ALARM_CHECK(up_undersize_frames, CTC_WARN_PON_TX_UNDER)
        /*check for CTC_ALARM_PON_RX_OVER*/
        STATS_MONITOR_ALARM_CHECK(dn_oversize_frames, CTC_ALARM_PON_RX_OVER) 
        STATS_MONITOR_ALARM_CHECK(dn_oversize_frames, CTC_WARN_PON_RX_OVER) 
        /*check for CTC_ALARM_PON_TX_OVER*/
        STATS_MONITOR_ALARM_CHECK(up_oversize_frames, CTC_ALARM_PON_TX_OVER)
        STATS_MONITOR_ALARM_CHECK(up_oversize_frames, CTC_WARN_PON_TX_OVER)
        /*check for CTC_ALARM_PON_RX_FRAG*/
        STATS_MONITOR_ALARM_CHECK(dn_fragments, CTC_ALARM_PON_RX_FRAG) 
        STATS_MONITOR_ALARM_CHECK(dn_fragments, CTC_WARN_PON_RX_FRAG) 

        /*check for CTC_ALARM_PON_TX_FRAG*/
        STATS_MONITOR_ALARM_CHECK(up_fragments, CTC_ALARM_PON_TX_FRAG) 
        STATS_MONITOR_ALARM_CHECK(up_fragments, CTC_WARN_PON_TX_FRAG) 
        
        /*check for CTC_ALARM_PON_RX_JABBER*/
        STATS_MONITOR_ALARM_CHECK(dn_jabbers, CTC_ALARM_PON_RX_JABBER) 
        STATS_MONITOR_ALARM_CHECK(dn_jabbers, CTC_WARN_PON_RX_JABBER)

        /*check for CTC_ALARM_PON_TX_JABBER*/
        STATS_MONITOR_ALARM_CHECK(up_jabbers, CTC_ALARM_PON_TX_JABBER) 
        STATS_MONITOR_ALARM_CHECK(up_jabbers, CTC_WARN_PON_TX_JABBER)
        

#if 0       
        /*check for CTC_ALARM_PON_RX_COLLIS*/
        STATS_MONITOR_ALARM_CHECK(dn_collisions, CTC_ALARM_PON_RX_COLLIS) 
        STATS_MONITOR_ALARM_CHECK(dn_collisions, CTC_WARN_PON_RX_COLLIS) 
 #endif
        
        /*check for CTC_ALARM_PON_RX_DISCA*/
        STATS_MONITOR_ALARM_CHECK(dn_discard_frames, CTC_ALARM_PON_RX_DISCA) 
        STATS_MONITOR_ALARM_CHECK(dn_discard_frames, CTC_WARN_PON_RX_DISCA)
        /*check for CTC_ALARM_PON_TX_DISCA*/
        STATS_MONITOR_ALARM_CHECK(up_discard_frames, CTC_ALARM_PON_TX_DISCA)
        STATS_MONITOR_ALARM_CHECK(up_discard_frames, CTC_WARN_PON_TX_DISCA)
        /*check for CTC_ALARM_PON_RX_ERROR*/
        STATS_MONITOR_ALARM_CHECK(dn_err_frames, CTC_ALARM_PON_RX_ERROR)
        STATS_MONITOR_ALARM_CHECK(dn_err_frames, CTC_WARN_PON_RX_ERROR)

        /*check for CTC_ALARM_PON_TX_ERROR*/
        STATS_MONITOR_ALARM_CHECK(up_err_frames, CTC_ALARM_PON_TX_ERROR)
        STATS_MONITOR_ALARM_CHECK(up_err_frames, CTC_WARN_PON_TX_ERROR)
 
         
    }
    else
    {
        type = CTC_PORT_ALARM;
        port = ctrl->port;

        /*check for CTC_ALARM_PORT_RX_DROP*/
        STATS_MONITOR_ALARM_CHECK(dn_drop_events, CTC_ALARM_PORT_RX_DROP) 
        STATS_MONITOR_ALARM_CHECK(dn_drop_events, CTC_WARN_PORT_RX_DROP)
        /*check for CTC_ALARM_PORT_TX_DROP*/
        STATS_MONITOR_ALARM_CHECK(up_drop_events, CTC_ALARM_PORT_TX_DROP)  
        STATS_MONITOR_ALARM_CHECK(up_drop_events, CTC_WARN_PORT_TX_DROP)
        /*check for CTC_ALARM_PORT_RX_CRC*/
        STATS_MONITOR_ALARM_CHECK(dn_crc_err_frames, CTC_ALARM_PORT_RX_CRC) 
        STATS_MONITOR_ALARM_CHECK(dn_crc_err_frames, CTC_WARN_PORT_RX_CRC) 

        /*check for CTC_ALARM_PORT_TX_CRC*/
        STATS_MONITOR_ALARM_CHECK(up_crc_err_frames, CTC_ALARM_PORT_TX_CRC) 
        STATS_MONITOR_ALARM_CHECK(up_crc_err_frames, CTC_WARN_PORT_TX_CRC) 
        
        /*check for CTC_ALARM_PORT_RX_UNDER*/
        STATS_MONITOR_ALARM_CHECK(dn_undersize_frames, CTC_ALARM_PORT_RX_UNDER)  
        STATS_MONITOR_ALARM_CHECK(dn_undersize_frames, CTC_WARN_PORT_RX_UNDER)  
        /*check for CTC_ALARM_PORT_TX_UNDER*/
        STATS_MONITOR_ALARM_CHECK(up_undersize_frames, CTC_ALARM_PORT_TX_UNDER)  
        STATS_MONITOR_ALARM_CHECK(up_undersize_frames, CTC_WARN_PORT_TX_UNDER)
        /*check for CTC_ALARM_PORT_RX_OVER*/
        STATS_MONITOR_ALARM_CHECK(dn_oversize_frames, CTC_ALARM_PORT_RX_OVER) 
        STATS_MONITOR_ALARM_CHECK(dn_oversize_frames, CTC_WARN_PORT_RX_OVER) 
        /*check for CTC_ALARM_PORT_TX_OVER*/
        STATS_MONITOR_ALARM_CHECK(up_oversize_frames, CTC_ALARM_PORT_TX_OVER)
        STATS_MONITOR_ALARM_CHECK(up_oversize_frames, CTC_WARN_PORT_TX_OVER)
        /*check for CTC_ALARM_PORT_RX_FRAG*/
        STATS_MONITOR_ALARM_CHECK(dn_fragments, CTC_ALARM_PORT_RX_FRAG) 
        STATS_MONITOR_ALARM_CHECK(dn_fragments, CTC_WARN_PORT_RX_FRAG) 
        /*check for CTC_ALARM_PORT_TX_FRAG*/
        STATS_MONITOR_ALARM_CHECK(up_fragments, CTC_ALARM_PORT_TX_FRAG) 
        STATS_MONITOR_ALARM_CHECK(up_fragments, CTC_WARN_PORT_TX_FRAG) 
        
        /*check for CTC_ALARM_PORT_RX_JABBER*/
        STATS_MONITOR_ALARM_CHECK(dn_jabbers, CTC_ALARM_PORT_RX_JABBER) 
        STATS_MONITOR_ALARM_CHECK(dn_jabbers, CTC_WARN_PORT_RX_JABBER)

        /*check for CTC_ALARM_PORT_TX_JABBER*/
        STATS_MONITOR_ALARM_CHECK(up_jabbers, CTC_ALARM_PORT_TX_JABBER) 
        STATS_MONITOR_ALARM_CHECK(up_jabbers, CTC_WARN_PORT_TX_JABBER)
        

        #if 0
        /*check for CTC_ALARM_PORT_RX_COLLIS*/
        STATS_MONITOR_ALARM_CHECK(dn_collisions, CTC_ALARM_PORT_RX_COLLIS) 
        STATS_MONITOR_ALARM_CHECK(dn_collisions, CTC_WARN_PORT_RX_COLLIS) 
        #endif
        
        /*check for CTC_ALARM_PORT_RX_DISCA*/
        STATS_MONITOR_ALARM_CHECK(dn_discard_frames, CTC_ALARM_PORT_RX_DISCA) 
        STATS_MONITOR_ALARM_CHECK(dn_discard_frames, CTC_WARN_PORT_RX_DISCA)
        /*check for CTC_ALARM_PORT_TX_DISCA*/
        STATS_MONITOR_ALARM_CHECK(up_discard_frames, CTC_ALARM_PORT_TX_DISCA)
        STATS_MONITOR_ALARM_CHECK(up_discard_frames, CTC_WARN_PORT_TX_DISCA)
        /*check for CTC_ALARM_PORT_RX_ERROR*/
        STATS_MONITOR_ALARM_CHECK(dn_err_frames, CTC_ALARM_PORT_RX_ERROR)
        STATS_MONITOR_ALARM_CHECK(dn_err_frames, CTC_WARN_PORT_RX_ERROR)

        /*check for CTC_ALARM_PORT_TX_ERROR*/
        STATS_MONITOR_ALARM_CHECK(up_err_frames, CTC_ALARM_PORT_TX_ERROR)
        STATS_MONITOR_ALARM_CHECK(up_err_frames, CTC_WARN_PORT_TX_ERROR)
        
        /*check for CTC_ALARM_PORT_STATUS*/
        STATS_MONITOR_ALARM_CHECK(status_change_times, CTC_ALARM_PORT_STATUS)
        STATS_MONITOR_ALARM_CHECK(status_change_times, CTC_WARN_PORT_STATUS)
    }
}

void ctc_onu_stats_monitor_update(ctc_stats_monitor_ctrl_t* ctrl)
{
    oam_ctc_onu_port_stats_t port_stats;
    ctc_onu_performance_stats_get(ctrl->port, &port_stats);

    ctrl->last_duration.dn_drop_events            = port_stats.dn_drop_events - ctrl->last_save.dn_drop_events;  
    ctrl->last_duration.up_drop_events            = port_stats.up_drop_events - ctrl->last_save.up_drop_events;     
    ctrl->last_duration.dn_octets                 = port_stats.dn_octets - ctrl->last_save.dn_octets;        
    ctrl->last_duration.up_octets                 = port_stats.up_octets - ctrl->last_save.up_octets;        
    ctrl->last_duration.dn_frames                 = port_stats.dn_frames - ctrl->last_save.dn_frames;         
    ctrl->last_duration.up_frames                 = port_stats.up_frames - ctrl->last_save.up_frames;     
    ctrl->last_duration.dn_bc_frames              = port_stats.dn_bc_frames - ctrl->last_save.dn_bc_frames;           
    ctrl->last_duration.up_bc_frames              = port_stats.up_bc_frames - ctrl->last_save.up_bc_frames;           
    ctrl->last_duration.dn_mc_frames              = port_stats.dn_mc_frames - ctrl->last_save.dn_mc_frames;            
    ctrl->last_duration.up_mc_frames              = port_stats.up_mc_frames - ctrl->last_save.up_mc_frames;          
    ctrl->last_duration.dn_crc_err_frames         = port_stats.dn_crc_err_frames - ctrl->last_save.dn_crc_err_frames;     
    ctrl->last_duration.up_crc_err_frames         = port_stats.up_crc_err_frames - ctrl->last_save.up_crc_err_frames; 

    
    ctrl->last_duration.dn_undersize_frames       = port_stats.dn_undersize_frames - ctrl->last_save.dn_undersize_frames;          
    ctrl->last_duration.up_undersize_frames       = port_stats.up_undersize_frames - ctrl->last_save.up_undersize_frames;            
    ctrl->last_duration.dn_oversize_frames        = port_stats.dn_oversize_frames - ctrl->last_save.dn_oversize_frames;          
    ctrl->last_duration.up_oversize_frames        = port_stats.up_oversize_frames - ctrl->last_save.up_oversize_frames;         
    ctrl->last_duration.dn_fragments              = port_stats.dn_fragments - ctrl->last_save.dn_fragments;
    ctrl->last_duration.up_fragments              = port_stats.up_fragments - ctrl->last_save.up_fragments;
    ctrl->last_duration.dn_jabbers                = port_stats.dn_jabbers - ctrl->last_save.dn_jabbers;
    ctrl->last_duration.up_jabbers                = port_stats.up_jabbers - ctrl->last_save.up_jabbers; 
    /* ctrl->last_duration.dn_collisions             = port_stats.dn_collisions - ctrl->last_save.dn_collisions;*/           
    ctrl->last_duration.dn_64_octets_frames       = port_stats.dn_64_octets_frames - ctrl->last_save.dn_64_octets_frames;          
    ctrl->last_duration.dn_65_127_octets_frames   = port_stats.dn_65_127_octets_frames - ctrl->last_save.dn_65_127_octets_frames;               
    ctrl->last_duration.dn_128_255_octets_frames  = port_stats.dn_128_255_octets_frames - ctrl->last_save.dn_128_255_octets_frames;          
    ctrl->last_duration.dn_256_511_octets_frames  = port_stats.dn_256_511_octets_frames - ctrl->last_save.dn_256_511_octets_frames;                 
    ctrl->last_duration.dn_512_1023_octets_frames = port_stats.dn_512_1023_octets_frames - ctrl->last_save.dn_512_1023_octets_frames;                 
    ctrl->last_duration.dn_1024_1518_octets_frames= port_stats.dn_1024_1518_octets_frames - ctrl->last_save.dn_1024_1518_octets_frames;                 
    ctrl->last_duration.up_64_octets_frames       = port_stats.up_64_octets_frames - ctrl->last_save.up_64_octets_frames;              
    ctrl->last_duration.up_65_127_octets_frames   = port_stats.up_65_127_octets_frames - ctrl->last_save.up_65_127_octets_frames;                
    ctrl->last_duration.up_128_255_octets_frames  = port_stats.up_128_255_octets_frames - ctrl->last_save.up_128_255_octets_frames;                    
    ctrl->last_duration.up_256_511_octets_frames  = port_stats.up_256_511_octets_frames - ctrl->last_save.up_256_511_octets_frames;               
    ctrl->last_duration.up_512_1023_octets_frames = port_stats.up_512_1023_octets_frames - ctrl->last_save.up_512_1023_octets_frames;                 
    ctrl->last_duration.up_1024_1518_octets_frames= port_stats.up_1024_1518_octets_frames - ctrl->last_save.up_1024_1518_octets_frames;                  
    ctrl->last_duration.dn_discard_frames         = port_stats.dn_discard_frames - ctrl->last_save.dn_discard_frames;                  
    ctrl->last_duration.up_discard_frames         = port_stats.up_discard_frames - ctrl->last_save.up_discard_frames;                
    ctrl->last_duration.dn_err_frames             = port_stats.dn_err_frames - ctrl->last_save.dn_err_frames;  
    ctrl->last_duration.up_err_frames             = port_stats.up_err_frames - ctrl->last_save.up_err_frames; 
    ctrl->last_duration.status_change_times       = port_stats.status_change_times - ctrl->last_save.status_change_times;  

    memcpy(&(ctrl->last_save), &port_stats, sizeof(port_stats));
}

void ctc_onu_stats_monitor_timer_handle(void* data)
{
    ctc_stats_monitor_ctrl_t *ctrl = (ctc_stats_monitor_ctrl_t*)data;
    if(!ctrl)
        return;

    ctc_onu_stats_monitor_update(ctrl);  

    /*check alarm*/
    ctc_onu_stats_monitor_alarm_check(ctrl);
}

void ctc_onu_stats_monitor_start_timer(ctc_stats_monitor_ctrl_t *ctrl)
{
    ctrl->timer = cs_msg_circle_timer_add(app_msg_q, (ctrl->interval)*1000, (void*)ctrl);
}

cs_status ctc_onu_stats_monitor_start(
        cs_port_id_t port,
        cs_uint32 interval)
{
    ctc_stats_monitor_ctrl_t *ctrl = NULL;
    
    if(port > CTC_MAX_ONU_UNI_PORTS)
        return CS_E_PARAM;

    ctc_onu_stats_monitor_stop(port);

    ctrl = &g_moitor_stats[port];
    ctrl->interval = interval;

    /* init data*/
    ctc_onu_stats_monitor_update(ctrl);   
    memset(&ctrl->last_duration, 0, sizeof(oam_ctc_onu_port_stats_t));

    ctc_onu_stats_monitor_start_timer(ctrl);

    return CS_E_OK;
    
}

cs_status ctc_onu_stats_monitor_restart(cs_port_id_t port)
{
    ctc_stats_monitor_ctrl_t *ctrl = NULL;
    cs_uint32 interval = 0;
    oam_ctc_onu_port_stats_t last_duration;
    
    if(port > CTC_MAX_ONU_UNI_PORTS)
        return CS_E_PARAM;

    
    ctrl = &g_moitor_stats[port];
    if(ctrl->timer == 0)
        return CS_E_ERROR;

    interval = ctrl->interval;
    last_duration = ctrl->last_duration;

    /*restart current stats monitor, history stats should keep unchange?*/
    ctc_onu_stats_monitor_start(port, interval);
    ctrl->last_duration = last_duration;
    return CS_E_OK;
}

cs_status ctc_onu_stats_monitor_status_get(
        cs_port_id_t port,
        cs_uint16 *monitor_status,
        cs_uint32 * monitor_period)
{
    if(port > CTC_MAX_ONU_UNI_PORTS)
        return CS_E_PARAM;

    if(g_moitor_stats[port].timer != 0)
    {
        *monitor_status = MONITOR_STATUS_ENABLE;
    }
    else
        *monitor_status = MONITOR_STATUS_DISABLE;
    
    *monitor_period = g_moitor_stats[port].interval;
    return CS_E_OK;
}

cs_status ctc_onu_stats_monitor_status_set(
        cs_port_id_t port,
        cs_uint16 monitor_status,
        cs_uint32  monitor_period)
{
    if(monitor_status == MONITOR_STATUS_DISABLE)
    {
        /*stop monitor*/
        return ctc_onu_stats_monitor_stop(port);
    }
    else if(monitor_status == MONITOR_STATUS_ENABLE)
    {
        /*enable monitor*/
        return ctc_onu_stats_monitor_start(port, monitor_period);
    }
    else
    {
        return CS_E_PARAM;
    }
}

cs_status ctc_onu_monitor_stats_current_get(
        cs_port_id_t port,
        oam_ctc_onu_port_stats_t * stats)
{
    oam_ctc_onu_port_stats_t port_stats;
    ctc_stats_monitor_ctrl_t *ctrl = NULL;

    if(port > CTC_MAX_ONU_UNI_PORTS || !stats)
        return CS_E_PARAM;
    
    ctrl = &g_moitor_stats[port];
    ctc_onu_performance_stats_get(port, &port_stats);
    
    stats->dn_drop_events            = port_stats.dn_drop_events - ctrl->last_save.dn_drop_events;  
    stats->up_drop_events            = port_stats.up_drop_events - ctrl->last_save.up_drop_events;        
    stats->dn_octets                 = port_stats.dn_octets - ctrl->last_save.dn_octets;      
    stats->up_octets                 = port_stats.up_octets - ctrl->last_save.up_octets;      
    stats->dn_frames                 = port_stats.dn_frames - ctrl->last_save.dn_frames;       
    stats->up_frames                 = port_stats.up_frames - ctrl->last_save.up_frames;       
    stats->dn_bc_frames              = port_stats.dn_bc_frames - ctrl->last_save.dn_bc_frames;         
    stats->up_bc_frames              = port_stats.up_bc_frames - ctrl->last_save.up_bc_frames;         
    stats->dn_mc_frames              = port_stats.dn_mc_frames - ctrl->last_save.dn_mc_frames;          
    stats->up_mc_frames              = port_stats.up_mc_frames - ctrl->last_save.up_mc_frames;          
    stats->dn_crc_err_frames         = port_stats.dn_crc_err_frames - ctrl->last_save.dn_crc_err_frames;
    stats->up_crc_err_frames         = port_stats.up_crc_err_frames - ctrl->last_save.up_crc_err_frames; 
    
    stats->dn_undersize_frames       = port_stats.dn_undersize_frames - ctrl->last_save.dn_undersize_frames;           
    stats->up_undersize_frames       = port_stats.up_undersize_frames - ctrl->last_save.up_undersize_frames;           
    stats->dn_oversize_frames        = port_stats.dn_oversize_frames - ctrl->last_save.dn_oversize_frames;          
    stats->up_oversize_frames        = port_stats.up_oversize_frames - ctrl->last_save.up_oversize_frames;          
    stats->dn_fragments              = port_stats.dn_fragments - ctrl->last_save.dn_fragments; 
    stats->up_fragments              = port_stats.up_fragments - ctrl->last_save.up_fragments;
    stats->dn_jabbers                = port_stats.dn_jabbers - ctrl->last_save.dn_jabbers; 
    stats->up_jabbers                = port_stats.up_jabbers - ctrl->last_save.up_jabbers; 
   /* stats->dn_collisions             = port_stats.dn_collisions - ctrl->last_save.dn_collisions;*/             
    stats->dn_64_octets_frames       = port_stats.dn_64_octets_frames - ctrl->last_save.dn_64_octets_frames;            
    stats->dn_65_127_octets_frames   = port_stats.dn_65_127_octets_frames - ctrl->last_save.dn_65_127_octets_frames;                
    stats->dn_128_255_octets_frames  = port_stats.dn_128_255_octets_frames - ctrl->last_save.dn_128_255_octets_frames;                
    stats->dn_256_511_octets_frames  = port_stats.dn_256_511_octets_frames - ctrl->last_save.dn_256_511_octets_frames;                 
    stats->dn_512_1023_octets_frames = port_stats.dn_512_1023_octets_frames - ctrl->last_save.dn_512_1023_octets_frames;                 
    stats->dn_1024_1518_octets_frames= port_stats.dn_1024_1518_octets_frames - ctrl->last_save.dn_1024_1518_octets_frames;                  
    stats->up_64_octets_frames       = port_stats.up_64_octets_frames - ctrl->last_save.up_64_octets_frames;                
    stats->up_65_127_octets_frames   = port_stats.up_65_127_octets_frames - ctrl->last_save.up_65_127_octets_frames;                
    stats->up_128_255_octets_frames  = port_stats.up_128_255_octets_frames - ctrl->last_save.up_128_255_octets_frames;                  
    stats->up_256_511_octets_frames  = port_stats.up_256_511_octets_frames - ctrl->last_save.up_256_511_octets_frames;                 
    stats->up_512_1023_octets_frames = port_stats.up_512_1023_octets_frames - ctrl->last_save.up_512_1023_octets_frames;                  
    stats->up_1024_1518_octets_frames= port_stats.up_1024_1518_octets_frames - ctrl->last_save.up_1024_1518_octets_frames;                   
    stats->dn_discard_frames         = port_stats.dn_discard_frames - ctrl->last_save.dn_discard_frames;                 
    stats->up_discard_frames         = port_stats.up_discard_frames - ctrl->last_save.up_discard_frames;              
    stats->dn_err_frames             = port_stats.dn_err_frames - ctrl->last_save.dn_err_frames;
    stats->up_err_frames             = port_stats.up_err_frames - ctrl->last_save.up_err_frames; 
    stats->status_change_times       = port_stats.status_change_times - ctrl->last_save.status_change_times;  
    return CS_E_OK;
}

cs_status ctc_onu_monitor_stats_histroy_get(
        cs_port_id_t port,
        oam_ctc_onu_port_stats_t * stats)
{
    if(port > CTC_MAX_ONU_UNI_PORTS || !stats)
        return CS_E_PARAM;
    
    memcpy(stats, &g_moitor_stats[port].last_duration, sizeof(oam_ctc_onu_port_stats_t)); 
    return CS_E_OK;
}

void ctc_onu_stats_dump(cs_port_id_t port)
{
    if(port > CTC_MAX_ONU_UNI_PORTS)
        return;

    oam_ctc_onu_port_stats_t * stats = &g_moitor_stats[port].last_duration;
    cs_printf("last period:\n");
    
    cs_printf("dn_drop_events                       = %x \n",stats->dn_drop_events            );  
    cs_printf("up_drop_events            = %x \n",stats->up_drop_events            );      
    cs_printf("dn_octets                 = %x \n",stats->dn_octets                 );      
    cs_printf("up_octets                 = %x \n",stats->up_octets                 );      
    cs_printf("dn_frames                 = %x \n",stats->dn_frames                 );       
    cs_printf("up_frames                 = %x \n",stats->up_frames                 );       
    cs_printf("dn_bc_frames              = %x \n",stats->dn_bc_frames              );         
    cs_printf("up_bc_frames              = %x \n",stats->up_bc_frames              );         
    cs_printf("dn_mc_frames              = %x \n",stats->dn_mc_frames              );          
    cs_printf("up_mc_frames              = %x \n",stats->up_mc_frames              );          
    cs_printf("dn_crc_err_frames         = %x \n",stats->dn_crc_err_frames         );
    cs_printf("up_crc_err_frames         = %x \n",stats->up_crc_err_frames         );
    
    cs_printf("dn_undersize_frames       = %x \n",stats->dn_undersize_frames       );           
    cs_printf("up_undersize_frames       = %x \n",stats->up_undersize_frames       );           
    cs_printf("dn_oversize_frames        = %x \n",stats->dn_oversize_frames        );          
    cs_printf("up_oversize_frames        = %x \n",stats->up_oversize_frames        );          
    cs_printf("dn_fragments              = %x \n",stats->dn_fragments              );  
    cs_printf("up_fragments              = %x \n",stats->up_fragments              );
    cs_printf("dn_jabbers                = %x \n",stats->dn_jabbers                );
    cs_printf("up_jabbers                = %x \n",stats->up_jabbers                ); 
    /*    cs_printf("dn_collisions             = %x \n",stats->dn_collisions             );    */ 
    cs_printf("dn_64_octets_frames       = %x \n",stats->dn_64_octets_frames       );            
    cs_printf("dn_65_127_octets_frames   = %x \n",stats->dn_65_127_octets_frames   );                
    cs_printf("dn_128_255_octets_frames  = %x \n",stats->dn_128_255_octets_frames  );                
    cs_printf("dn_256_511_octets_frames  = %x \n",stats->dn_256_511_octets_frames  );                 
    cs_printf("dn_512_1023_octets_frames = %x \n",stats->dn_512_1023_octets_frames );                 
    cs_printf("dn_1024_1518_octets_frames= %x \n",stats->dn_1024_1518_octets_frames);                  
    cs_printf("up_64_octets_frames       = %x \n",stats->up_64_octets_frames       );                
    cs_printf("up_65_127_octets_frames   = %x \n",stats->up_65_127_octets_frames   );                
    cs_printf("up_128_255_octets_frames  = %x \n",stats->up_128_255_octets_frames  );                  
    cs_printf("up_256_511_octets_frames  = %x \n",stats->up_256_511_octets_frames  );                 
    cs_printf("up_512_1023_octets_frames = %x \n",stats->up_512_1023_octets_frames );                  
    cs_printf("up_1024_1518_octets_frames= %x \n",stats->up_1024_1518_octets_frames);                   
    cs_printf("dn_discard_frames         = %x \n",stats->dn_discard_frames         );                 
    cs_printf("up_discard_frames         = %x \n",stats->up_discard_frames         );              
    cs_printf("dn_err_frames             = %x \n",stats->dn_err_frames             );
    cs_printf("up_err_frames             = %x \n",stats->up_err_frames             ); 
    cs_printf("status_change_times       = %x \n",stats->status_change_times       );  

    stats = &g_moitor_stats[port].last_save;
    cs_printf("last point:\n");
    
    cs_printf("dn_drop_events                       = %x \n",stats->dn_drop_events            );  
    cs_printf("up_drop_events            = %x \n",stats->up_drop_events            );      
    cs_printf("dn_octets                 = %x \n",stats->dn_octets                 );      
    cs_printf("up_octets                 = %x \n",stats->up_octets                 );      
    cs_printf("dn_frames                 = %x \n",stats->dn_frames                 );       
    cs_printf("up_frames                 = %x \n",stats->up_frames                 );       
    cs_printf("dn_bc_frames              = %x \n",stats->dn_bc_frames              );         
    cs_printf("up_bc_frames              = %x \n",stats->up_bc_frames              );         
    cs_printf("dn_mc_frames              = %x \n",stats->dn_mc_frames              );          
    cs_printf("up_mc_frames              = %x \n",stats->up_mc_frames              );          
    cs_printf("dn_crc_err_frames         = %x \n",stats->dn_crc_err_frames         );     
    cs_printf("up_crc_err_frames         = %x \n",stats->up_crc_err_frames         ); 
    
    cs_printf("dn_undersize_frames       = %x \n",stats->dn_undersize_frames       );           
    cs_printf("up_undersize_frames       = %x \n",stats->up_undersize_frames       );           
    cs_printf("dn_oversize_frames        = %x \n",stats->dn_oversize_frames        );          
    cs_printf("up_oversize_frames        = %x \n",stats->up_oversize_frames        );          
    cs_printf("dn_fragments              = %x \n",stats->dn_fragments              );
    cs_printf("up_fragments              = %x \n",stats->up_fragments              );
    cs_printf("dn_jabbers                = %x \n",stats->dn_jabbers                ); 
    cs_printf("up_jabbers                = %x \n",stats->up_jabbers                ); 
    /*cs_printf("dn_collisions             = %x \n",stats->dn_collisions             ); */            
    cs_printf("dn_64_octets_frames       = %x \n",stats->dn_64_octets_frames       );            
    cs_printf("dn_65_127_octets_frames   = %x \n",stats->dn_65_127_octets_frames   );                
    cs_printf("dn_128_255_octets_frames  = %x \n",stats->dn_128_255_octets_frames  );                
    cs_printf("dn_256_511_octets_frames  = %x \n",stats->dn_256_511_octets_frames  );                 
    cs_printf("dn_512_1023_octets_frames = %x \n",stats->dn_512_1023_octets_frames );                 
    cs_printf("dn_1024_1518_octets_frames= %x \n",stats->dn_1024_1518_octets_frames);                  
    cs_printf("up_64_octets_frames       = %x \n",stats->up_64_octets_frames       );                
    cs_printf("up_65_127_octets_frames   = %x \n",stats->up_65_127_octets_frames   );                
    cs_printf("up_128_255_octets_frames  = %x \n",stats->up_128_255_octets_frames  );                  
    cs_printf("up_256_511_octets_frames  = %x \n",stats->up_256_511_octets_frames  );                 
    cs_printf("up_512_1023_octets_frames = %x \n",stats->up_512_1023_octets_frames );                  
    cs_printf("up_1024_1518_octets_frames= %x \n",stats->up_1024_1518_octets_frames);                   
    cs_printf("dn_discard_frames         = %x \n",stats->dn_discard_frames         );                 
    cs_printf("up_discard_frames         = %x \n",stats->up_discard_frames         );              
    cs_printf("dn_err_frames             = %x \n",stats->dn_err_frames             );
    cs_printf("up_err_frames             = %x \n",stats->up_err_frames             ); 
    cs_printf("status_change_times       = %x \n",stats->status_change_times       );  
}

#ifdef HAVE_GWD_OAM_FAST_STATISTIC
cs_status onu_port_stats_octets_get(cs_port_id_t port_id, cs_uint64 *in_octets, cs_uint64 *out_octets)
{
	cs_status ret = CS_E_OK;
	
	if(NULL == in_octets)
	{
		return CS_E_ERROR;
	}

	if(NULL == out_octets)
	{
		return CS_E_ERROR;
	}

	if(CS_PON_PORT_ID == port_id)
	{
		cs_callback_context_t	  context;
		cs_sdl_port_uni_stats_t uni_stats;
		
		memset(&uni_stats,0x00,sizeof(uni_stats));
		ret = epon_request_onu_port_stats_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
				CS_UPLINK_PORT,FALSE, &uni_stats);
		*in_octets = uni_stats.rxbyte_cnt;
		*out_octets = uni_stats.txbyte_cnt;
	}
	else
	{
		cs_callback_context_t	  context;
		cs_sdl_port_uni_stats_t uni_stats;
		
		memset(&uni_stats,0x00,sizeof(uni_stats));
		ret = epon_request_onu_port_stats_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
				port_id,FALSE, &uni_stats);
		*in_octets = uni_stats.rxbyte_cnt;
		*out_octets = uni_stats.txbyte_cnt;
	}
	
	return ret;
}
#endif	/*(#ifdef HAVE_GWD_OAM_FAST_STATISTIC)*/

#endif

