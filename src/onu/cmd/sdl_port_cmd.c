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

Copyright (c) 2010 by Cortina Systems Incorporated
****************************************************************************/

#include "cs_cmd.h"


#ifdef HAVE_SDL_CMD

#include "sdl_port.h"
#include "sdl.h"

extern cs_status uni_min_pkt_size_set (CS_IN cs_uint16 min);
extern cs_status uni_min_pkt_size_get (CS_IN cs_uint16 *min);

#ifdef HAVE_SDL_CMD_HELP
static const char *uni_cmd_usage_mode[] = 
{
    "uni mode <port num> <mode>",
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
    "mode:",
    "   1: Auto(10/100Mbps)",
    "   2: Auto(10/100/1000Mbps)",
    "   3: 10Mbps(full) ",
    "   4: 10Mbps(half) ",
    "   5: 100Mbps(full)",
    "   6: 100Mbps(half)",
    "   7: 1000Mbps(full)"
};

static const char *uni_cmd_usage_mirror[] = 
{
    "mirror <port> <rx_port_msk> <rx_port_msk> <isolation>",
    "port num:",
    "   0x30: uplink port",
    "   1: UNI port 0",
    "   2: UNI port 1",
    "   3: UNI port 2",
    "   4: UNI port 3",
    "rx_port_mask:",
    "   bit 1: UNI port 0",
    "   bit 2: UNI port 1",
    "   bit 3: UNI port 2",
    "   bit 4: UNI port 3",
    "   bit 5: uplink port",
    "tx_port_mask:",
    "   bit 1: UNI port 0",
    "   bit 2: UNI port 1",
    "   bit 3: UNI port 2",
    "   bit 4: UNI port 3",
    "   bit 5: uplink port",
    "isolation:",
    "   1: enable mirror isolation",
    "   0: disable mirror isolationt",
};


static const char *uni_cmd_usage_mtu[] = 
{
    "uni mtu <64-1900>"
};

static const char *uni_cmd_usage_min[] = 
{
    "uni min <64-127>"
};

static const char *uni_cmd_usage_mdi[] = 
{
    "uni mdi <0 - 2>",
    "   0: Auto MDI/MDIX",
    "   1: Fixed MDI",
    "   2: Fixed MDIX"
};

static const char *uni_cmd_usage_admin[] = 
{
    "uni admin <port num> <admin>",
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
    "admin:",
    "   0: admin down",
    "   1: admin up"
};

#ifdef HAVE_SDL_CMD_NTT
static const char *uni_cmd_usage_eee[] = 
{
    "uni eee <0, 1>",
    "   0: disable",
    "   1: enable"
};
#endif

static const char *uni_cmd_usage_lpbk[] = 
{
    "uni lpbk <port num> <lpbk>",
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
    "lpbk:",
    "   0: disable loopback",
    "   1: tx2rx loopback"
    "   2: rx2tx loopback"
};

static const char *uni_cmd_usage_iso[] = 
{
    "uni iso <enable>",

    "enable:",
    "   0: disable",
    "   1: enable"
};


static const char *uni_cmd_usage_fc[] = 
{
    "uni fc <port num> <enable>",
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
    "enable:",
    "   0: disable",
    "   1: enable"
};
static const char *uni_cmd_usage_show[] = 
{
    "uni show <cr>"
};

static const char *port_cmd_usage_drp[] = 
{
    "uni drp <port num> <enable>",
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
    "enable:",
    "   0: disable",
    "   1: enable"
};
static const char *uni_cmd_usage_shp[] = 
{
    "uni shaper <port num> <enable> <rate> <cbs>",
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
    "enable:",
    "   0: disable",
    "   1: enable",
    "rate: 0~1000000kBps",
    "cbs: 0~1000kB "
};

static const char *uni_cmd_usage_policer[] = 
{
    "uni policer <port num> <enable> <rate> <cbs>",
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
    "enable:",
    "   0: disable",
    "   1: enable",
    "rate: 0~1000000kBps",
    "cbs: 0~1000kB "
};

static const char *uni_cmd_usage_storm[] = 
{
    "uni storm <port num> <mode> <type> <enable><rate> ",
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
    "mode:",
    "   0: bps",
    "   1: pps",
    "type:",
    "   0: BC",
    "   1: BC/UMC",
    "   2: BC/UMC/UUC",
    "enable:",
    "   0: disable",
    "   1: enable",
    "rate: 0~1000000kBps",
};


#endif

sal_cmd_result_t uni_cmd_mirror(int argc, char **argv)
{
#ifdef HAVE_RTK_CMD

    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  mirror_port;
    cs_uint32  rx_port_msk;
    cs_uint32  tx_port_msk;
    //cs_boolean isolation;
    
    if(argc != 5) return SAL_CMD_INVALID_PARAM;
    
    mirror_port = (cs_port_id_t)iros_strtol(argv[2]);
    rx_port_msk = (cs_uint32)iros_strtol(argv[3]);
    tx_port_msk = (cs_uint32)iros_strtol(argv[4]);

    status = epon_request_onu_port_mirror_set(context, 0, 0, 
                                              mirror_port, 
                                              rx_port_msk, 
                                              tx_port_msk);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_mirror_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }
    
    mirror_port = 0;
    rx_port_msk = 0;
    tx_port_msk = 0;
    //isolation = 0;
    
    status = epon_request_onu_port_mirror_get(context, 0, 0, 
                                              &mirror_port, 
                                              &rx_port_msk, 
                                              &tx_port_msk);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_mirror_get failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }
    
    cs_printf("mirror port          : %d\n", mirror_port);
    cs_printf("rx_port_msk          : 0x%04x\n", rx_port_msk);
    cs_printf("tx_port_msk          : 0x%04x\n", tx_port_msk);
    //cs_printf("mirror port isolation: %d\n", isolation);
    
#endif
    

    return 0;
}

sal_cmd_result_t uni_cmd_mode(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_uint8 mode = 0;
    cs_status  status = 0;
    cs_port_id_t  portid;

    if(argc != 4) return SAL_CMD_INVALID_PARAM;
    
    portid = iros_strtol(argv[2]);
    mode = iros_strtol(argv[3]);
    if(mode > 8 || mode < 1) return SAL_CMD_INVALID_PARAM;

    status = epon_request_onu_port_status_set(context, 0, 0, portid, mode);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_status_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }

    return 0;
}


sal_cmd_result_t uni_cmd_mtu(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_uint16 mtu = 0;
    cs_status  status = 0;

    cs_port_id_t  portid = CS_UNI_PORT_ID1;

    if(argc != 4) return SAL_CMD_INVALID_PARAM;
    
    portid = (cs_port_id_t)iros_strtol(argv[2]);
    mtu = (cs_uint16)iros_strtol(argv[3]);
    
    status = epon_request_onu_port_mtu_set(context, 0, 0, portid, mtu);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_mtu_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }

    return cs_status_2_cmd_rc_map(status); ;
}

sal_cmd_result_t uni_cmd_min(int argc, char **argv)
{
    cs_uint16 min = 0;
    cs_status  status = 0;

    if(argc != 3) return SAL_CMD_INVALID_PARAM;
    min = iros_strtol(argv[2]);

    status = uni_min_pkt_size_set(min);
    if(status != CS_E_OK){
        cs_printf("uni_min_pkt_size_set failed \n");    
    }

    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t uni_cmd_mdi(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_uint8 mdi = 0;
    cs_status  status = 0;
    cs_port_id_t  portid = CS_UNI_PORT_ID1;

    if(argc != 3) return SAL_CMD_INVALID_PARAM;
    
    mdi = iros_strtol(argv[2]);
    if(mdi > 2) return SAL_CMD_INVALID_PARAM;

    status = epon_request_onu_port_mdi_set(context, 0, 0, portid, mdi);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_mdi_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }

    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t uni_cmd_admin(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  portid;
    cs_boolean admin = 0;

    if(argc != 4) return SAL_CMD_INVALID_PARAM;
    
    portid = (cs_port_id_t)iros_strtol(argv[2]);
    admin = iros_strtol(argv[3]);

    if (admin == 1) {
        status = epon_request_onu_port_admin_set(context, 0, 0, portid, 1);
    } else if (admin == 0) {
        status = epon_request_onu_port_admin_set(context, 0, 0, portid, 0);
    } else {
        return SAL_CMD_INVALID_PARAM;
    }
    
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_admin_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }
    
    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t uni_cmd_fc(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  portid;
    cs_boolean enable = 0;

    if(argc != 4) return SAL_CMD_INVALID_PARAM;
    
    portid = iros_strtol(argv[2]);
    enable = (cs_boolean)iros_strtol(argv[3]);
    
    status = epon_request_onu_port_flow_ctrl_set(context, 0, 0, portid, enable);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_flow_ctrl_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }
        
    return cs_status_2_cmd_rc_map(status);
}
sal_cmd_result_t uni_cmd_lpbk(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  portid = CS_UNI_PORT_ID1;
    cs_uint8 loopback = 0;

    if(argc != 4) return SAL_CMD_INVALID_PARAM;

    portid = (cs_port_id_t)iros_strtol(argv[2]);
    loopback = iros_strtol(argv[3]);
    if(loopback > SDL_PORT_LOOPBACK_RX2TX) 
        return SAL_CMD_INVALID_PARAM;

    status = epon_request_onu_port_lpbk_set(context, 0, 0, portid, loopback);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_lpbk_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }

    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t uni_cmd_iso(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_uint8 enable = 0;

    if(argc != 3) return SAL_CMD_INVALID_PARAM;

    enable = (cs_boolean)iros_strtol(argv[2]);
    //cs_printf("enable = %d \n", enable);    
    
    status = epon_request_onu_port_isolation_set(context, 0, 0, enable);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_isolation_set failed! \n");    
        return cs_status_2_cmd_rc_map(status);       
    }

    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t port_cmd_drp(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  portid;
    cs_boolean enable;
    
    if(argc != 4) {
        return SAL_CMD_INVALID_PARAM;
    }

    portid = (cs_port_id_t)iros_strtol(argv[2]);
    enable = iros_strtol(argv[3]);

    status = (cs_status)epon_request_onu_port_traffic_set(context, 0, 0, portid,enable);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_traffic_set failed! \n");    
        return cs_status_2_cmd_rc_map(status);       
    }
        
    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t uni_cmd_shaper(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  portid;
    cs_sdl_policy_t shaper;
    
    if(argc != 6) {
        return SAL_CMD_INVALID_PARAM;
    }

    portid = (cs_port_id_t)iros_strtol(argv[2]);
    shaper.enable = iros_strtol(argv[3]);
    shaper.rate = iros_strtol(argv[4]);
    shaper.cbs = iros_strtol(argv[5]);

    status = (cs_status)epon_request_onu_port_ds_rate_limit_set(context, 0, 0, portid,&shaper);
    if(status){
        cs_printf("epon_request_onu_port_ds_rate_limit_set failed! \n");    
        return cs_status_2_cmd_rc_map(status);       
    }
    
    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t uni_cmd_eee(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_port_id_t  portid = CS_UNI_PORT_ID1;
    cs_status  status = 0;
    cs_boolean eee = 0;

    if(argc != 3) 
        return SAL_CMD_INVALID_PARAM;
    
    if (argv[2][0] != '0' && argv[2][0] != '1')
        return SAL_CMD_INVALID_PARAM;
    
    eee = iros_strtol(argv[2]);

    if (eee == 1) {
        status = epon_request_onu_port_eee_set(context, 0, 0, portid, 1);
    } else if (eee == 0) {
        status = epon_request_onu_port_eee_set(context, 0, 0, portid, 0);
    } else {
        return SAL_CMD_INVALID_PARAM;
    }

    return cs_status_2_cmd_rc_map(status);
    
}

sal_cmd_result_t uni_cmd_policer(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  portid;
    cs_sdl_policy_t policing;
    
    if(argc != 6) {
        return SAL_CMD_INVALID_PARAM;
    }

    portid = (cs_port_id_t)iros_strtol(argv[2]);
    policing.enable = iros_strtol(argv[3]);
    policing.rate = iros_strtol(argv[4]);
    policing.cbs = iros_strtol(argv[5]);
    policing.ebs = 0;
    status = (cs_status)epon_request_onu_port_policy_set(context, 0, 0, portid, &policing);
    if(status){
       cs_printf("epon_request_onu_port_policy_set failed! \n");    
       return cs_status_2_cmd_rc_map(status);       
    }
   
    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t uni_cmd_storm_ctrl(int argc, char **argv)
{
    cs_status  sdl_rt;
    cs_sdl_policy_t rate;
    cs_port_id_t portid;
    cs_sdl_rl_mod_e mode;
    cs_sdl_storm_ctrl_e type;
    cs_callback_context_t context;
    
    if(argc != 7){
        return SAL_CMD_INVALID_PARAM;
    }
    
    portid = (cs_port_id_t)iros_strtol(argv[2]);
    mode = (cs_sdl_rl_mod_e)iros_strtol(argv[3]);
    type = (cs_sdl_storm_ctrl_e)iros_strtol(argv[4]);
    rate.enable = iros_strtol(argv[5]);
    rate.rate = iros_strtol(argv[6]);
    
    sdl_rt = epon_request_onu_port_storm_ctrl_set(context,  0, 0, portid, mode, type, &rate);
    if(sdl_rt){
        cs_printf("epon_request_onu_port_storm_ctrl_set failed! \n");    
        return cs_status_2_cmd_rc_map(sdl_rt);       
    }
    
    return cs_status_2_cmd_rc_map(sdl_rt);       
}


void __show_uni_cfg(void)
{
    cs_callback_context_t   context;
    cs_status               status = 0;
    cs_port_id_t            portid;
    cs_uint16               mtu = 0;      
    cs_sdl_port_mdi_t       mdi = 0;      
    cs_sdl_port_admin_t     admin = 0;
    cs_sdl_port_loopback_t  loopback = 0;
    cs_sdl_port_speed_cfg_t mode_cfg = 0;
    cs_uint16               min = 0;    
    cs_sdl_policy_t         policer;   
    cs_boolean              flow_control;
    cs_sdl_policy_t         shaper;
    cs_sdl_port_drop_cfg_t  is_drop;   
    cs_sdl_rl_mod_e         storm_mod;        
    cs_sdl_storm_ctrl_e     storm_type = 0;
    cs_sdl_policy_t         storm_rate;    
#ifdef HAVE_SDL_CMD_NTT
    cs_boolean              eee_cfg = 0;
#endif

char *mode2string[] = 
{
    "Reserved",
    "1: Auto(10/100Mbps)",
    "2: Auto(10/100/1000Mbps)",
    "3: 10Mbps(full)",
    "4: 10Mbps(half)",
    "5: 100Mbps(full)",
    "6: 100Mbps(half)",
    "7: 1000Mbps(full)"
};

char *mdi2string[] = 
{
    "Auto MDI/MDIX",
    "Fixed MDI",
    "Fixed MDIX"
};

char *admin2string[] = 
{
    "down",
    "up"
};

char *loopback2string[] = 
{
    "none",
    "tx2rx",
    "rx2tx"
};

char *storm2string[] = 
{
    "BC",
    "BC+UMC",
    "BC+UMC+UUC",
    "DISABLE"
};


#ifdef HAVE_RTK_CMD
    for(portid = CS_UNI_PORT_ID1; portid<= CS_DOWNLINK_PORT;portid++){
        if(portid > UNI_PORT_MAX && portid != CS_DOWNLINK_PORT){
            continue;
        }
#else
    portid = CS_UNI_PORT_ID1;
#endif

        if(portid != CS_DOWNLINK_PORT){
            status = epon_request_onu_port_mtu_get(context, 0, 0, portid, &mtu);
            if(status != CS_E_OK){
                cs_printf("epon_request_onu_port_mtu_get failed \n");    
            }
        
            status = epon_request_onu_port_status_get(context, 0, 0, portid, &mode_cfg);
            if(status != CS_E_OK){
                cs_printf("epon_request_onu_port_status_get failed \n");    
            }
        
#ifndef HAVE_RTK_CMD
            status = epon_request_onu_port_mdi_get(context, 0, 0, portid, &mdi);
            if(status != CS_E_OK){
                cs_printf("epon_request_onu_port_mdi_get failed \n");    
            }
#endif

            status = epon_request_onu_port_admin_get(context, 0, 0, portid, &admin);
            if(status != CS_E_OK){
                cs_printf("epon_request_onu_port_admin_get failed \n");    
            }
        
            status = epon_request_onu_port_lpbk_get(context, 0, 0, portid, &loopback);
            if(status != CS_E_OK){
                cs_printf("epon_request_onu_port_lpbk_get failed \n");    
            }

            status = uni_min_pkt_size_get(&min);
            if(status != CS_E_OK){
                cs_printf("uni_min_pkt_size_get failed \n");    
            }
        
            memset(&policer, 0, sizeof(cs_sdl_policy_t));
            status = epon_request_onu_port_policy_get(context, 0, 0, portid, &policer);
            if(status != CS_E_OK){
                cs_printf("epon_request_onu_port_policy_get failed \n");    
            }
        }
 
        status = epon_request_onu_port_flow_ctrl_get(context, 0, 0, portid, &flow_control);
        if(status != CS_E_OK){
            cs_printf("epon_request_onu_port_flow_ctrl_get failed \n");    
        }

        memset(&shaper, 0, sizeof(cs_sdl_policy_t));
        status = epon_request_onu_port_ds_rate_limit_get(context, 0, 0, portid, &shaper);
        if(status != CS_E_OK){
            cs_printf("epon_request_onu_port_ds_rate_limit_get failed \n");    
        }
    
        memset(&storm_rate, 0, sizeof(cs_sdl_policy_t));
        status = epon_request_onu_port_storm_ctrl_get(context, 0, 0, portid, &storm_mod,&storm_type,&storm_rate);
        if(status != CS_E_OK){
            cs_printf("epon_request_onu_port_storm_ctrl_get failed \n");    
        }
    
        if(portid == CS_DOWNLINK_PORT || portid == CS_UNI_PORT_ID1){
            status = epon_request_onu_port_traffic_get(context, 0, 0, portid, &is_drop);
            if(status != CS_E_OK){
                cs_printf("epon_request_onu_port_traffic_get failed \n");    
            }
#ifdef HAVE_SDL_CMD_NTT
            status = epon_request_onu_port_eee_get(context, 0, 0, portid, &eee_cfg);
            if(status != CS_E_OK){
                cs_printf("epon_request_onu_port_eee_get failed \n");    
            }
#endif
        }

        cs_printf("----------------------------------\n");
        cs_printf("portid         is %d\n", portid);   
        if(portid != CS_DOWNLINK_PORT){     
            cs_printf("admin          is %s\n", admin2string[admin]);
            cs_printf("Speed config   is %s\n", mode2string[mode_cfg]);
            cs_printf("mtu            is %d\n", mtu);
            cs_printf("min pkt size   is %d\n", min);
            cs_printf("loopback       is %s\n", loopback2string[loopback]);
            cs_printf("mdi            is %s\n", mdi2string[mdi]);
        
            cs_printf("policer        is %s\n", policer.enable ? "enable":"disable");
            cs_printf("policer rate   is %d\n", policer.rate);
            cs_printf("policer burst  is %d\n", policer.cbs);
    
#ifdef HAVE_SDL_CMD_NTT
            cs_printf("EEE(802.3az)   is %s\n", eee_cfg ? "enable" : "disable");
#endif    
        }
    
        cs_printf("flow_control   is %s\n", flow_control ? "enable":"disable");    
        cs_printf("shaper         is %s\n", shaper.enable ? "enable":"disable");
        cs_printf("shaper rate    is %d\n", shaper.rate);
        cs_printf("shaper burst   is %d\n", shaper.cbs); 

        cs_printf("storm          is %s\n", storm2string[storm_type]);
        cs_printf("storm rate     is %d\n", storm_rate.rate);
        cs_printf("storm burst    is %d\n", storm_rate.cbs); 

        if(portid == CS_DOWNLINK_PORT || portid == CS_UNI_PORT_ID1){
            cs_printf("is_drop        is %d\n", is_drop); 
        }

        cs_printf("----------------------------------\n");
        
#ifdef HAVE_RTK_CMD
    }
#endif

}


sal_cmd_result_t uni_cmd_show(int argc, char **argv)
{
    /* Only show CFG here. For status/stats, pls use 'show uni' */
    if(2 == argc) {
        __show_uni_cfg();
        return SAL_CMD_OK; 
    } else {
        return SAL_CMD_INVALID_PARAM;
    }

    return SAL_CMD_OK;       
}

static sal_cmd_reg_struct_t uni_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {uni_cmd_mirror, "mirror", "uni mirror configuration", uni_cmd_usage_mirror, sizeof(uni_cmd_usage_mirror)/4},
    {uni_cmd_show, "show", "show uni configuration", uni_cmd_usage_show, sizeof(uni_cmd_usage_show)/4},
    {uni_cmd_mode, "mode", "config uni speed/duplex/auto mode", uni_cmd_usage_mode, sizeof(uni_cmd_usage_mode)/4},
    {uni_cmd_mtu, "mtu", "config mtu", uni_cmd_usage_mtu, sizeof(uni_cmd_usage_mtu)/4},
    {uni_cmd_min, "min", "config min pkt length", uni_cmd_usage_min, sizeof(uni_cmd_usage_min)/4},
    {uni_cmd_mdi, "mdi", "config mdi", uni_cmd_usage_mdi, sizeof(uni_cmd_usage_mdi)/4},
    {uni_cmd_admin, "admin", "MAC admin down/up", uni_cmd_usage_admin, sizeof(uni_cmd_usage_admin)/4},
#ifdef HAVE_SDL_CMD_NTT
    {uni_cmd_eee, "eee", "EEE(802.3az) enable/disable", uni_cmd_usage_eee, sizeof(uni_cmd_usage_eee)/4},
#endif
    {uni_cmd_fc, "fc", "config flow control", uni_cmd_usage_fc , sizeof(uni_cmd_usage_fc)/4},
    {uni_cmd_iso, "iso", "config isolation", uni_cmd_usage_iso , sizeof(uni_cmd_usage_iso)/4},
    {uni_cmd_lpbk, "lpbk", "config lpbk", uni_cmd_usage_lpbk , sizeof(uni_cmd_usage_lpbk)/4},
    {port_cmd_drp, "drp", "port traffic drop", port_cmd_usage_drp, sizeof(port_cmd_usage_drp)/4},
    {uni_cmd_shaper,"shaper","uni port egress rate limit",uni_cmd_usage_shp,sizeof(uni_cmd_usage_shp)/4},
    {uni_cmd_policer,"policer","uni port us rate limit",uni_cmd_usage_policer,sizeof(uni_cmd_usage_policer)/4},
    {uni_cmd_storm_ctrl,"storm","uni port storm ctrl",uni_cmd_usage_storm,sizeof(uni_cmd_usage_storm)/4},
#else
    {uni_cmd_mirror, "mirror"}, //"uni mirror configuration", uni_cmd_usage_mirror, sizeof(uni_cmd_usage_mirror)/4},
    {uni_cmd_show, "show"},// "show uni configuration"},// uni_cmd_usage_show, sizeof(uni_cmd_usage_show)/4},
    {uni_cmd_mode, "mode"},// "config uni speed/duplex/auto mode"},// uni_cmd_usage_mode, sizeof(uni_cmd_usage_mode)/4},
    {uni_cmd_mtu, "mtu"},// "config mtu"},// uni_cmd_usage_mtu, sizeof(uni_cmd_usage_mtu)/4},
    {uni_cmd_min, "min"},// "config min pkt length"},// uni_cmd_usage_min, sizeof(uni_cmd_usage_min)/4},
    {uni_cmd_mdi, "mdi"},// "config mdi"},// uni_cmd_usage_mdi, sizeof(uni_cmd_usage_mdi)/4},
    {uni_cmd_admin, "admin"},// "MAC admin down/up"},// uni_cmd_usage_admin, sizeof(uni_cmd_usage_admin)/4},
#ifdef HAVE_SDL_CMD_NTT
    {uni_cmd_eee, "eee"}, //"EEE(802.3az) enable/disable", uni_cmd_usage_eee, sizeof(uni_cmd_usage_eee)/4},
#endif
    {uni_cmd_fc, "fc"}, //"config flow control", uni_cmd_usage_fc , sizeof(uni_cmd_usage_fc)/4},
    {uni_cmd_lpbk, "lpbk"},// "config lpbk"},// uni_cmd_usage_lpbk , sizeof(uni_cmd_usage_lpbk)/4},
    {port_cmd_drp, "drp"},// "port traffic drop"},// port_cmd_usage_drp, sizeof(port_cmd_usage_drp)/4},
    {uni_cmd_iso, "iso"}, //"config isolation", uni_cmd_usage_iso , sizeof(uni_cmd_usage_iso)/4},
    {uni_cmd_policer,"policer"},//"uni port us rate limit",uni_cmd_usage_policer,sizeof(uni_cmd_usage_policer)/4},
    {uni_cmd_storm_ctrl,"storm"},//"uni port storm ctrl",uni_cmd_usage_storm,sizeof(uni_cmd_usage_storm)/4},
#endif
};

sal_cmd_cfg_t uni_cmd_cfg = {"uni", "uni configuration", uni_cmd_list, sizeof(uni_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

#endif //#ifdef HAVE_SDL_CMD

cs_status uni_cmd_self_reg(void)
{
        cs_status ret = CS_E_OK;
#ifdef HAVE_SDL_CMD
    
        ret = sal_cmd_reg(&uni_cmd_cfg);
#endif
        return ret;
}


