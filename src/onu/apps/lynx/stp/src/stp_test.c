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
#ifdef HAVE_ONU_RSTP
#include "base.h"
#include "cs_utils.h"
#include "stpm.h"
#include "stp_in.h"
#include "stp_to.h"
#include "sdl_mpcp_cmn.h"
#include "stp_sdl_adapt.h"
#include "stp_api.h"

extern cs_uint8 max_port;

void stp_set_port_debug(cs_uint8 type, cs_uint8  value )
{
        cs_port_id_t   port_index=0;
        PORT_T *   port =NULL;
        STPM_T * this=NULL;

        this = stpapi_stpm_find (STP_DEF_VLAN);
        if ( this == NULL) {
                return;
        }

        for (port_index = CS_UNI_PORT_ID1; port_index <= max_port; port_index++){
                port = _stpapi_port_find (this, port_index);
                if (! port) {
                        continue;
                }

	         switch(type)
	         	{
	         	case 0:
		               port->info->debug = value;
				break;
	         	case 1:
                		port->roletrns->debug = value;
				break;
	         	case 2:
		                port->sttrans->debug = value;
				break;
	         	case 3:
		               port->topoch->debug = value;
				break;
	         	case 4:
		               port->migrate->debug = value;
				break;
	         	case 5:
		                port->transmit->debug = value;
				break;
	         	case 6:
		                port->p2p->debug = value;
				break;
	         	case 7:
		                port->edge->debug = value;
				break;
	         	case 8:
		                port->pcost->debug = value;
				break;
	         	case 255:
		                port->info->debug=value;
		                port->roletrns->debug=value;
		                port->sttrans->debug=value;
		                port->topoch->debug=value;
		                port->migrate->debug=value;
		                port->transmit->debug=value;
		                port->p2p->debug=value;
		                port->edge->debug=value;
		                port->pcost->debug=value;
				break;
			default:
				break;
	         	}
       }

        return;
}
 
 void stp_bridge_state_show()
 {
     cs_status   retCode = CS_E_OK;
     cs_onu_stp_state_config_t bridge_state;

     memset(&bridge_state, 0, sizeof(cs_onu_stp_state_config_t));
     retCode = app_stp_bridge_state_get(&bridge_state );
  
     cs_printf("\r\n mask=%x, stp_enable=%d, bridge_priority=%d, max_age=%d, hello_time=%d, forward_delay=%d, root_port =%d\r\n"
                                      ,bridge_state.mask
                                      ,bridge_state.stp_enable
                                      ,bridge_state.bridge_prio
                                      ,bridge_state.max_age
                                      ,bridge_state.hello_time
                                      ,bridge_state.forward_delay
                                      ,bridge_state.root_port);
     cs_printf("\r\n retCode =%d\r\n", retCode);
 }

void stp_port_state_show(cs_port_id_t port)
{
    cs_status   retCode = CS_E_OK;
    cs_onu_port_stp_state_config_t port_state;
    char role = 0, state = 0;
    
    if(port == CS_PON_PORT_ID)
        return;
    
    retCode = app_stp_port_state_get(port, &port_state );

    //cs_printf("port %d, state %d\n", port, port_state.state);

    switch (port_state.state) {
      case 0:   state = 'D'; break;
      case 1:  state = 'B'; break;
      case 2:     state = 'L'; break;
      case 3:       state = 'F'; break;
      case 4: state = '-'; break;
      default:             state = '?'; break;
    }

    switch (port_state.role) {
      case DisabledPort:   role = ' '; break;
      case AlternatePort:  role = 'A'; break;
      case BackupPort:     role = 'B'; break;
      case RootPort:       role = 'R'; break;
      case DesignatedPort: role = 'D'; break;
      case NonStpPort:     role = '-'; break;
      default:             role = '?'; break;
    }

    cs_printf("\r\n state=%c, stp_enable=%d, role=%c, pcost=%x, port_pri=%d, oper_p2p=%d, oper_edge =%d\r\n"
                                     ,state
                                     ,port_state.stp_enable
                                     ,role
                                     ,port_state.path_cost
                                     ,port_state.port_prio
                                     ,port_state.oper_point2point
                                     ,port_state.oper_edge);
    
    cs_printf("\r\n d_brg_prio=%d, d_brg_mac =%x:%x:%x:%x:%x:%x \r\n"
                                     ,port_state.designated_bridge_prio
                                     ,port_state.designated_bridge_addr[0]
                                     ,port_state.designated_bridge_addr[1]
                                     ,port_state.designated_bridge_addr[2]
                                     ,port_state.designated_bridge_addr[3]
                                     ,port_state.designated_bridge_addr[4]
                                     ,port_state.designated_bridge_addr[5]);

    cs_printf("\r\n rx_cfg_cnt=%x, rx_rstp_cnt=%x, rx_tcn_cnt=%x, tx_cfg_cnt=%x, tx_rstp_cnt=%x, tx_tcn_cnt=%x\r\n"
                                     ,port_state.rx_cfg_bpdu_cnt
                                     ,port_state.rx_rstp_bpdu_cnt
                                     ,port_state.rx_tcn_bpdu_cnt
                                     ,port_state.tx_cfg_bpdu_cnt
                                     ,port_state.tx_rstp_bpdu_cnt
                                     ,port_state.tx_tcn_bpdu_cnt);
    
    cs_printf("\r\n retCode =%d\r\n", retCode);
}

cs_int8 * stp_strdup(cs_int8  *string) 
{
        cs_int8  *newbit = iros_malloc(IROS_MID_APP, strlen(string)+1);
        if (newbit) {
                strcpy(newbit,string);
        }
        return newbit;
}
#endif

