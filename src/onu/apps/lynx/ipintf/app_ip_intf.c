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
#ifdef HAVE_IP_STACK
#include <pkgconf/system.h>
#ifndef CYGPKG_LWIP_IPV4_SUPPORT
#include <sys/param.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/mbuf.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <net/netisr.h>
#include <net/if_var.h>
#include <net/if_arp.h>
#ifdef INET
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#endif

#ifndef NBPFILTER
#define NBPFILTER 0
#endif

#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

#include <cyg/infra/cyg_ass.h>
#include <cyg/hal/drv_api.h>
#include <pkgconf/hal.h>
#include <cyg/hal/hal_if.h>
#include <pkgconf/io_eth_drivers.h> // module configury; SIMULATED_FAILURES
#include <pkgconf/net.h>            // CYGPKG_NET_FAST_THREAD_TICKLE_DEVS?

#include <cyg/io/eth/eth_drv.h>
#include <cyg/io/eth/netdev.h>
#else
#include <network.h>
#endif

#include <cyg/hal/drv_api.h>
#include <cyg/io/eth/netdev.h>
#include <cyg/io/eth/eth_drv.h>

#include "plat_common.h"
#include "app_ip_types.h"
#include "app_ip_api.h"
#include "app_ip_init.h"
#ifdef CYGPKG_LWIP_IPV4_SUPPORT
#include "app_ip_arp.h"
#endif
#include "app_ip_util.h"

#ifdef HAVE_TCP_UDP_NETWORK_CARD
//added by zhuxh
#define ONU_ETH_NUM                 2
#else
#define ONU_ETH_NUM                 1
#endif
#define PACKET_LIST_MAX             32
#define MIN_ETH_FRAME_SIZE          60
#define MAX_ETH_FRAME_SIZE          (1536 - 4)
#define ONU_ETH_FLAG_STARTED        0x00000001

typedef struct cortina_if_eth_private_t {
    cs_uint8*  packet_list[PACKET_LIST_MAX];
    cs_uint16  packet_list_packet_len[PACKET_LIST_MAX];
    cs_uint8   packet_list_head;
    cs_uint8   packet_list_tail;
    cs_uint8*  packet_buf;
    cs_uint16  packet_len;
    cs_uint32  mac_addr_ok;
    cs_uint8   mac_address[ETHER_ADDR_LEN];
    cs_uint32  flags;
    cs_uint32  packets_received;
} cortina_if_eth_private_t;

cs_uint32 g_eth_ip_mutex = 0; 

cortina_if_eth_private_t cortian_if_eth_tab[ONU_ETH_NUM];

ETH_DRV_SC(eth_drv_sc_eth0,
           &cortian_if_eth_tab[0],
           IPINTF_NAME_ETH0,
           if_eth_start,
           if_eth_stop,
           if_eth_control,
           if_eth_can_send,
           if_eth_send,
           if_eth_recv,
           if_eth_deliver,
           if_eth_poll,
           if_eth_int_vector
           );
 
NETDEVTAB_ENTRY(netdev0,
                "netdev0",
                netdev_init_eth0,
                &eth_drv_sc_eth0);

#ifdef HAVE_TCP_UDP_NETWORK_CARD
//added by zhuxh
cs_uint32 g_eth1_ip_mutex = 0;

ETH_DRV_SC(eth_drv_sc_eth1,
           &cortian_if_eth_tab[1],
           IPINTF_NAME_ETH1,
           if_eth1_start,
           if_eth1_stop,
           if_eth1_control,
           if_eth1_can_send,
           if_eth1_send,
           if_eth1_recv,
           if_eth1_deliver,
           if_eth1_poll,
           if_eth1_int_vector
           );
 
NETDEVTAB_ENTRY(netdev1,
                "netdev1",
                netdev_init_eth1,
                &eth_drv_sc_eth1);
#endif


#ifdef CYGPKG_LWIP_IPV4_SUPPORT

struct netif * ipintf_net_if_get()
{
    return &(eth_drv_sc_eth0.sc_arpcom.ac_if);
}

#ifdef HAVE_TCP_UDP_NETWORK_CARD
//added by zhuxh
struct netif * ipintf_net_if_eth1_get()
{
    return &(eth_drv_sc_eth1.sc_arpcom.ac_if);
}
#endif

#endif
static bool netdev_init_eth0(struct cyg_netdevtab_entry *ndp)
{
    static cs_int32 initialized = 0;
    cs_int32 good = 0;
    struct eth_drv_sc *sc;
    cs_uint8 *mac_address = NULL;
    cortina_if_eth_private_t *p_cortina_eth;

    sc = (struct eth_drv_sc *)(ndp->device_instance);
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "netdev_init_eth0: %s\n", sc->dev_name);

    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if(0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "Bad dev private ptr %x\n", (cs_uint32)sc->driver_private);
    }
    mac_address = p_cortina_eth->mac_address;

    if ( 0 != initialized++ ) {
        /* then this is not the first time ever: */
        return 1;
    }

    /* 
    * get mac address.
    * we need do this here because startup config is not yet handled at this time 
    */
    startup_config_init();
    startup_config_read(CFG_ID_MAC_ADDRESS , ETHER_ADDR_LEN,mac_address);
    if(mac_address == NULL) {
        return 1;
    }

    if ( 0x00 != (mac_address[0] | mac_address[1] | mac_address[2] |
                  mac_address[3] | mac_address[4] | mac_address[5]) && // some nonzero bits
         0xff != (mac_address[0] & mac_address[1] & mac_address[2] &
                  mac_address[3] & mac_address[4] & mac_address[5]) && // not all ones
         0x00 == (mac_address[0] & 0x01)) {           // not multicast, bit40 not 1
        good = 1;
    }
    
    if ( !good ) {
        /* no valid MAC in cfg, return to default one */
        mac_address[0] = 0x00;
        mac_address[1] = 0x13;
        mac_address[2] = 0x25;
        mac_address[3] = 0x00;
        mac_address[4] = 0x00;
        mac_address[5] = 0x01;
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "Invalid MAC in startup cfg, set to default\n");
    }


/*    cs_printf( "MAC Address %s, ESA = %02X %02X %02X %02X %02X %02X\n",
                (1 == good) ? "OK" : "**BAD**",
                mac_address[0],
                mac_address[1],
                mac_address[2],
                mac_address[3],
                mac_address[4],
                mac_address[5]);
*/
#ifndef CYGPKG_LWIP_IPV4_SUPPORT
    /* TODO : no need to set hw address here */
    /*  Set up hardware address */
    memcpy(&sc->sc_arpcom.ac_enaddr, mac_address, ETHER_ADDR_LEN);
#endif

/*    cs_printf("%s MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", IPINTF_NAME_ETH0,
                mac_address[0],mac_address[1],mac_address[2],mac_address[3],mac_address[4],mac_address[5]);
*/
    p_cortina_eth->mac_addr_ok = good;
    p_cortina_eth->flags = 0;

    /* initialize a circular buffer list table to deliver packets recv on netdev */
    cs_uint8 **packet_list = p_cortina_eth->packet_list;
    memset(packet_list, 0, sizeof(cs_uint8 *) * PACKET_LIST_MAX);
    p_cortina_eth->packet_list_head = 0;
    p_cortina_eth->packet_list_tail = 0;
    
    cs_mutex_init(&g_eth_ip_mutex,"eth_ip_mutex",0);

    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s input packet list max: %d\n",sc->dev_name, PACKET_LIST_MAX);

    if (1 == p_cortina_eth->mac_addr_ok)
    {
        (sc->funs->eth_drv->init)(sc, &(p_cortina_eth->mac_address[0]) );
    }
    else
    {
        (sc->funs->eth_drv->init)(sc, NULL );
    }

    return (1);
}

void netdev_pkt_recv(cs_pkt_t * event_buf)
{
    struct eth_drv_sc *sc;
    cortina_if_eth_private_t *p_cortina_eth;
    cs_uint8* frame_buf;
    cs_uint16 frame_len;

    if(event_buf == NULL)
    {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "netdev_pkt_recv,null pointer.\n");
        return;
    }
    frame_buf = ETH_GET_FRAME_BUFFER(event_buf);
    frame_len = ETH_GET_FRAME_LENGTH(event_buf);
    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "netdev recv 0x%08x %d\n", (cs_uint32)frame_buf, frame_len);

    cs_mutex_lock(g_eth_ip_mutex);
    /* dispatch to eth0 packet buffer list */
    sc = &eth_drv_sc_eth0;
    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if (0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "sc corrupted\n");
        goto free_buf;
    }

    if (0 == (p_cortina_eth->flags & ONU_ETH_FLAG_STARTED)) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "device not started\n");
        goto free_buf;
    }

    if ( frame_len <= sizeof(struct ether_header) ) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "drop packet with invalid len %d\n", frame_len);
        goto free_buf;
    }

    cs_uint8 **packet_list = p_cortina_eth->packet_list;
    cs_uint32 head = p_cortina_eth->packet_list_head;

    if (0 != packet_list[head]) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "packet_list full. packet dropped\n");
        /* signal IP Stack thread for cached packets */
        eth_drv_dsr(0, 0, (cyg_addrword_t)sc);
        goto free_buf;
    }

    /* record event_puf here for release mem, record real frame length */
    packet_list[head] = (cs_uint8 *)event_buf;
    p_cortina_eth->packet_list_packet_len[head] = frame_len;

    p_cortina_eth->packet_list_head++;
    if (p_cortina_eth->packet_list_head >= PACKET_LIST_MAX ) {
        p_cortina_eth->packet_list_head = 0;
    }

    p_cortina_eth->packets_received++;

    /* signal IP Stack thread for coming packets */
    eth_drv_dsr(0, 0, (cyg_addrword_t)sc);

    cs_mutex_unlock(g_eth_ip_mutex);
    return;

free_buf:

    iros_free(event_buf);
    cs_mutex_unlock(g_eth_ip_mutex);
}


static void if_eth_start(struct eth_drv_sc *sc, unsigned char *enaddr, cs_int32 flags)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_start\n");

    cortina_if_eth_private_t *p_cortina_eth;
    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if (0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"sc corrupted\n");
        return;
    }

    p_cortina_eth->flags |= ONU_ETH_FLAG_STARTED;
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s started\n", sc->dev_name);
    #ifdef CYGPKG_LWIP_IPV4_SUPPORT
    sc->sc_arpcom.ac_if.flags |= (NETIF_FLAG_BROADCAST | NETIF_FLAG_LINK_UP | NETIF_FLAG_UP);
    #endif
    
    return;
    
}

static void if_eth_stop(struct eth_drv_sc *sc)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "if_eth_stop\n");

    cortina_if_eth_private_t *p_cortina_eth;
    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);

    if (0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "sc corrupted\n");
        return;
    }

    p_cortina_eth->flags &= ~ONU_ETH_FLAG_STARTED;
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s stoped\n", sc->dev_name);
    return;
}

static cs_int32 if_eth_control(struct eth_drv_sc *sc, unsigned long key,
                          void *data, cs_int32 data_length)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_control\n");

    cortina_if_eth_private_t *p_cortina_eth;
    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if (0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "sc corrupted\n");
        return -1;
    }

    switch(key)
    {
#if defined(CYGPKG_NET)
        case ETH_DRV_GET_IF_STATS_UD:
        case ETH_DRV_GET_IF_STATS:
        {
            struct ether_drv_stats *p = (struct ether_drv_stats*)data;
            p = p;
            /* get stat here */
            return 0;
        }
#endif
        case ETH_DRV_SET_MAC_ADDRESS:
        {
            cs_int32 act;
            act = p_cortina_eth->flags;

            if (ETHER_ADDR_LEN != data_length)
                return -1;

            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_control: ETH_DRV_SET_MAC_ADDRESS.\n");

            if_eth_stop(sc);
            if_eth_start(sc, data, 0);
            p_cortina_eth->flags = act;
            return 0;
        }
#ifdef  TH_DRV_GET_MAC_ADDRESS
        case ETH_DRV_GET_MAC_ADDRESS:
        {
            if (data_length < ETHER_ADDR_LEN)
                return -1;
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_control: ETH_DRV_GET_MAC_ADDRESS.\n");
            return 0;
        }
#endif
        case ETH_DRV_SET_MC_LIST:
        case ETH_DRV_SET_MC_ALL:
        {
            /* set/get multicast table here */
            return 0;
        }
        default:
            return -1;
    }
}

static cs_int32 if_eth_can_send(struct eth_drv_sc *sc)
{
    // HW to provide, if the interface is ready to send packet
    return 1;
}


static void if_eth_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, cs_int32 sg_len)
{
    cortina_if_eth_private_t *p_cortina_eth;
    cs_uint8* from_p;
    cs_int16 total_len;
    struct eth_drv_sg *last_sg;

    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_recv\n");

    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if (NULL == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"sc corrupted\n");
        return;
    }

    if(0 == p_cortina_eth->packet_len
        || NULL == p_cortina_eth->packet_buf)
    {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "packet already freed\n");
        return;
    }

    total_len = p_cortina_eth->packet_len;
    from_p = ETH_GET_FRAME_BUFFER(p_cortina_eth->packet_buf);

    // check we have memory to copy into; we would be called even if
    // caller was out of memory in order to maintain our state.
    if ( 0 == sg_len || 0 == sg_list ) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"caller was out of mbufs\n");
        goto free_buf;
    }

    for ( last_sg = &sg_list[sg_len]; sg_list < last_sg; sg_list++ ) {
        cs_uint8* to_p;
        cs_int32 l;

        to_p = (cs_uint8 *)(sg_list->buf);
        l = sg_list->len;

        CYG_ASSERT( 0 < l, "sg length -ve" );

        if ( 0 >= l || (CYG_ADDRESS)0 == to_p ) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "caller was out of mbufs\n");
            goto free_buf;
        }

        if ( l > total_len )
            l = total_len;

        memcpy( to_p, (cs_uint8*)from_p, l );
        from_p += l;
        total_len -= l;
        if(total_len <=0){
            break;
        }
    }

free_buf:
    iros_free(p_cortina_eth->packet_buf);
    p_cortina_eth->packet_buf = NULL;
    p_cortina_eth->packet_len = 0;

    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_recv done\n");

    return;
}

static void if_eth_send(struct eth_drv_sc *sc,
            struct eth_drv_sg *sg_list, cs_int32 sg_len, cs_int32 total_len,
            unsigned long key)
{
    cs_status status = 0;
    cs_uint8 * from_p;
    struct eth_drv_sg *last_sg;
    cs_uint32 len = 0;;
    cs_uint32 pkt_alloc_len = 0;
    cs_pkt_t *pkt = NULL;

    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_send\n");

    if (total_len > MAX_ETH_FRAME_SIZE) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "invalid len\n");
        (sc->funs->eth_drv->tx_done)(sc, key, -EINVAL);
        return;
    }

    if (total_len < MIN_ETH_FRAME_SIZE) {
        len = MIN_ETH_FRAME_SIZE;
    }
    else {
        len = total_len;
    }
    pkt_alloc_len = CS_PKT_OFFSET+(sizeof(cs_pkt_t)-1)+len;
    pkt = (cs_pkt_t *)iros_malloc(IROS_MID_PACKET, pkt_alloc_len);
    if (NULL == pkt) {
        (sc->funs->eth_drv->tx_done)(sc, key, -EIO);
        return;
    }
	
    app_ipintf_init_pkt(pkt_alloc_len,pkt);
    
    cs_uint8 *to_p;
    to_p = pkt->data+pkt->offset;

    for ( last_sg = &sg_list[sg_len]; sg_list < last_sg; sg_list++ ) {
        cs_int32 l;

        from_p = (cs_uint8 *)(sg_list->buf); // normal cached address
        l = sg_list->len;

        if ( l > total_len )
            l = total_len;

        memcpy(to_p, from_p, l);
        to_p += l;

        total_len -= l;
        if ( 0 > total_len )
            break; // Should exit via sg_last normally
    }

    // tell upper layer that we're done with this sglist
    (sc->funs->eth_drv->tx_done)(sc, key, 0);
    status = app_ipintf_packet_tx(pkt);
    if(status != CS_OK)
    {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "ethdrv_pkt_send failed!\n");
    }
    iros_free(pkt);

    return;
}

extern int eth_drv_recv_error;

static void if_eth_deliver(struct eth_drv_sc *sc)
{
    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "if_eth_deliver\n");

    cortina_if_eth_private_t* p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if (0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "sc corrupted\n");
        return;
    }

    cs_mutex_lock(g_eth_ip_mutex);
    cs_uint8 **packet_list = p_cortina_eth->packet_list;

    while (1) {
        if (p_cortina_eth->packet_buf != NULL) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"wait due recv still busy\n");
            break;
        }

        cs_uint32 tail = p_cortina_eth->packet_list_tail;
        if (NULL == packet_list[tail]) {
            //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "no more packet\n");
            cs_mutex_unlock(g_eth_ip_mutex);
            return;
        }

        cs_uint16 length = p_cortina_eth->packet_list_packet_len[tail];
        p_cortina_eth->packet_buf = packet_list[tail];
        p_cortina_eth->packet_len = length;

        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "fwd packet to stack, len %d\n", length);
        // offer the data to the network stack
        (sc->funs->eth_drv->recv)(sc, length);

        packet_list[tail] = NULL;
        p_cortina_eth->packet_list_packet_len[tail] = 0;

        p_cortina_eth->packet_list_tail++;
        if (p_cortina_eth->packet_list_tail >= PACKET_LIST_MAX ) {
            p_cortina_eth->packet_list_tail = 0;
        }

        
        if(eth_drv_recv_error != 0){
            iros_free(p_cortina_eth->packet_buf);
            p_cortina_eth->packet_buf = NULL;
            p_cortina_eth->packet_len = 0;
            break;
        }
    }

    cs_mutex_unlock(g_eth_ip_mutex);
    return;
}

static void if_eth_poll(struct eth_drv_sc *sc)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "if_eth_poll: NOT SUPPORTED\n");
    return;
}

static cs_int32 if_eth_int_vector(struct eth_drv_sc *sc)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "if_eth_int_vector: NOT SUPPORTED\n");
    return 0;
}

#ifdef HAVE_TCP_UDP_NETWORK_CARD
//added by zhuxh
static bool netdev_init_eth1(struct cyg_netdevtab_entry *ndp)
{
    static cs_int32 initialized = 0;
    cs_int32 good = 0;
    struct eth_drv_sc *sc;
    cs_uint8 *mac_address = NULL;
    cortina_if_eth_private_t *p_cortina_eth;

    sc = (struct eth_drv_sc *)(ndp->device_instance);
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "netdev_init_eth0: %s\n", sc->dev_name);

    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if(0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "Bad dev private ptr %x\n", (cs_uint32)sc->driver_private);
    }
    mac_address = p_cortina_eth->mac_address;

    if ( 0 != initialized++ ) {
        /* then this is not the first time ever: */
        return 1;
    }

    /* 
    * get mac address.
    * we need do this here because startup config is not yet handled at this time 
    */
    startup_config_init();
    startup_config_read(CFG_ID_MAC_ADDRESS , ETHER_ADDR_LEN,mac_address);
    if(mac_address == NULL) {
        return 1;
    }

    if ( 0x00 != (mac_address[0] | mac_address[1] | mac_address[2] |
                  mac_address[3] | mac_address[4] | mac_address[5]) && // some nonzero bits
         0xff != (mac_address[0] & mac_address[1] & mac_address[2] &
                  mac_address[3] & mac_address[4] & mac_address[5]) && // not all ones
         0x00 == (mac_address[0] & 0x01)) {           // not multicast, bit40 not 1
        good = 1;
    }
    
    if ( !good ) {
        /* no valid MAC in cfg, return to default one */
        mac_address[0] = 0x00;
        mac_address[1] = 0x13;
        mac_address[2] = 0x25;
        mac_address[3] = 0x00;
        mac_address[4] = 0x00;
        mac_address[5] = 0x01;
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "Invalid MAC in startup cfg, set to default\n");
    }


/*    cs_printf( "MAC Address %s, ESA = %02X %02X %02X %02X %02X %02X\n",
                (1 == good) ? "OK" : "**BAD**",
                mac_address[0],
                mac_address[1],
                mac_address[2],
                mac_address[3],
                mac_address[4],
                mac_address[5]);
*/
#ifndef CYGPKG_LWIP_IPV4_SUPPORT
    /* TODO : no need to set hw address here */
    /*  Set up hardware address */
    memcpy(&sc->sc_arpcom.ac_enaddr, mac_address, ETHER_ADDR_LEN);
#endif

/*    cs_printf("%s MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", IPINTF_NAME_ETH0,
                mac_address[0],mac_address[1],mac_address[2],mac_address[3],mac_address[4],mac_address[5]);
*/
    p_cortina_eth->mac_addr_ok = good;
    p_cortina_eth->flags = 0;

    /* initialize a circular buffer list table to deliver packets recv on netdev */
    cs_uint8 **packet_list = p_cortina_eth->packet_list;
    memset(packet_list, 0, sizeof(cs_uint8 *) * PACKET_LIST_MAX);
    p_cortina_eth->packet_list_head = 0;
    p_cortina_eth->packet_list_tail = 0;
    
    cs_mutex_init(&g_eth1_ip_mutex,"eth_ip_mutex",0);

    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s input packet list max: %d\n",sc->dev_name, PACKET_LIST_MAX);

    if (1 == p_cortina_eth->mac_addr_ok)
    {
        (sc->funs->eth_drv->init)(sc, &(p_cortina_eth->mac_address[0]) );
    }
    else
    {
        (sc->funs->eth_drv->init)(sc, NULL );
    }

    return (1);
}

void netdev_eth1_pkt_recv(cs_pkt_t * event_buf)
{
    struct eth_drv_sc *sc;
    cortina_if_eth_private_t *p_cortina_eth;
    cs_uint8* frame_buf;
    cs_uint16 frame_len;

    if(event_buf == NULL)
    {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "netdev_eth1_pkt_recv,null pointer.\n");
        return;
    }
    frame_buf = ETH_GET_FRAME_BUFFER(event_buf);
    frame_len = ETH_GET_FRAME_LENGTH(event_buf);
    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "netdev recv 0x%08x %d\n", (cs_uint32)frame_buf, frame_len);

    cs_mutex_lock(g_eth1_ip_mutex);
    /* dispatch to eth0 packet buffer list */
    sc = &eth_drv_sc_eth1;
    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if (0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "sc corrupted\n");
        goto free_buf;
    }

    if (0 == (p_cortina_eth->flags & ONU_ETH_FLAG_STARTED)) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "device not started\n");
        goto free_buf;
    }

    if ( frame_len <= sizeof(struct ether_header) ) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "drop packet with invalid len %d\n", frame_len);
        goto free_buf;
    }

    cs_uint8 **packet_list = p_cortina_eth->packet_list;
    cs_uint32 head = p_cortina_eth->packet_list_head;

    if (0 != packet_list[head]) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "packet_list full. packet dropped\n");
        /* signal IP Stack thread for cached packets */
        eth_drv_dsr(0, 0, (cyg_addrword_t)sc);
        goto free_buf;
    }

    /* record event_puf here for release mem, record real frame length */
    packet_list[head] = (cs_uint8 *)event_buf;
    p_cortina_eth->packet_list_packet_len[head] = frame_len;

    p_cortina_eth->packet_list_head++;
    if (p_cortina_eth->packet_list_head >= PACKET_LIST_MAX ) {
        p_cortina_eth->packet_list_head = 0;
    }

    p_cortina_eth->packets_received++;

    /* signal IP Stack thread for coming packets */
    eth_drv_dsr(0, 0, (cyg_addrword_t)sc);

    cs_mutex_unlock(g_eth1_ip_mutex);
    return;

free_buf:

    iros_free(event_buf);
    cs_mutex_unlock(g_eth1_ip_mutex);
}


static void if_eth1_start(struct eth_drv_sc *sc, unsigned char *enaddr, cs_int32 flags)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_start\n");

    cortina_if_eth_private_t *p_cortina_eth;
    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if (0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"sc corrupted\n");
        return;
    }

    p_cortina_eth->flags |= ONU_ETH_FLAG_STARTED;
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s started\n", sc->dev_name);
    #ifdef CYGPKG_LWIP_IPV4_SUPPORT
    sc->sc_arpcom.ac_if.flags |= (NETIF_FLAG_BROADCAST | NETIF_FLAG_LINK_UP | NETIF_FLAG_UP);
    #endif
    
    return;
    
}

static void if_eth1_stop(struct eth_drv_sc *sc)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "if_eth_stop\n");

    cortina_if_eth_private_t *p_cortina_eth;
    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);

    if (0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "sc corrupted\n");
        return;
    }

    p_cortina_eth->flags &= ~ONU_ETH_FLAG_STARTED;
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s stoped\n", sc->dev_name);
    return;
}

static cs_int32 if_eth1_control(struct eth_drv_sc *sc, unsigned long key,
                          void *data, cs_int32 data_length)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_control\n");

    cortina_if_eth_private_t *p_cortina_eth;
    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if (0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "sc corrupted\n");
        return -1;
    }

    switch(key)
    {
#if defined(CYGPKG_NET)
        case ETH_DRV_GET_IF_STATS_UD:
        case ETH_DRV_GET_IF_STATS:
        {
            struct ether_drv_stats *p = (struct ether_drv_stats*)data;
            p = p;
            /* get stat here */
            return 0;
        }
#endif
        case ETH_DRV_SET_MAC_ADDRESS:
        {
            cs_int32 act;
            act = p_cortina_eth->flags;

            if (ETHER_ADDR_LEN != data_length)
                return -1;

            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_control: ETH_DRV_SET_MAC_ADDRESS.\n");

            if_eth_stop(sc);
            if_eth_start(sc, data, 0);
            p_cortina_eth->flags = act;
            return 0;
        }
#ifdef  TH_DRV_GET_MAC_ADDRESS
        case ETH_DRV_GET_MAC_ADDRESS:
        {
            if (data_length < ETHER_ADDR_LEN)
                return -1;
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_control: ETH_DRV_GET_MAC_ADDRESS.\n");
            return 0;
        }
#endif
        case ETH_DRV_SET_MC_LIST:
        case ETH_DRV_SET_MC_ALL:
        {
            /* set/get multicast table here */
            return 0;
        }
        default:
            return -1;
    }
}

static cs_int32 if_eth1_can_send(struct eth_drv_sc *sc)
{
    // HW to provide, if the interface is ready to send packet
    return 1;
}


static void if_eth1_recv(struct eth_drv_sc *sc, struct eth_drv_sg *sg_list, cs_int32 sg_len)
{
    cortina_if_eth_private_t *p_cortina_eth;
    cs_uint8* from_p;
    cs_int16 total_len;
    struct eth_drv_sg *last_sg;

    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_recv\n");

    p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if (NULL == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"sc corrupted\n");
        return;
    }

    if(0 == p_cortina_eth->packet_len
        || NULL == p_cortina_eth->packet_buf)
    {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "packet already freed\n");
        return;
    }

    total_len = p_cortina_eth->packet_len;
    from_p = ETH_GET_FRAME_BUFFER(p_cortina_eth->packet_buf);

    // check we have memory to copy into; we would be called even if
    // caller was out of memory in order to maintain our state.
    if ( 0 == sg_len || 0 == sg_list ) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"caller was out of mbufs\n");
        goto free_buf;
    }

    for ( last_sg = &sg_list[sg_len]; sg_list < last_sg; sg_list++ ) {
        cs_uint8* to_p;
        cs_int32 l;

        to_p = (cs_uint8 *)(sg_list->buf);
        l = sg_list->len;

        CYG_ASSERT( 0 < l, "sg length -ve" );

        if ( 0 >= l || (CYG_ADDRESS)0 == to_p ) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "caller was out of mbufs\n");
            goto free_buf;
        }

        if ( l > total_len )
            l = total_len;

        memcpy( to_p, (cs_uint8*)from_p, l );
        from_p += l;
        total_len -= l;
        if(total_len <=0){
            break;
        }
    }

free_buf:
    iros_free(p_cortina_eth->packet_buf);
    p_cortina_eth->packet_buf = NULL;
    p_cortina_eth->packet_len = 0;

    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_recv done\n");

    return;
}

static void if_eth1_send(struct eth_drv_sc *sc,
            struct eth_drv_sg *sg_list, cs_int32 sg_len, cs_int32 total_len,
            unsigned long key)
{
    cs_status status = 0;
    cs_uint8 * from_p;
    struct eth_drv_sg *last_sg;
    cs_uint32 len = 0;;
    cs_uint32 pkt_alloc_len = 0;
    cs_pkt_t *pkt = NULL;

    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"if_eth_send\n");

    if (total_len > MAX_ETH_FRAME_SIZE) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "invalid len\n");
        (sc->funs->eth_drv->tx_done)(sc, key, -EINVAL);
        return;
    }

    if (total_len < MIN_ETH_FRAME_SIZE) {
        len = MIN_ETH_FRAME_SIZE;
    }
    else {
        len = total_len;
    }
    pkt_alloc_len = CS_PKT_OFFSET+(sizeof(cs_pkt_t)-1)+len;
    pkt = (cs_pkt_t *)iros_malloc(IROS_MID_PACKET, pkt_alloc_len);
    if (NULL == pkt) {
        (sc->funs->eth_drv->tx_done)(sc, key, -EIO);
        return;
    }
	
    app_ipintf_init_pkt(pkt_alloc_len,pkt);
    
    cs_uint8 *to_p;
    to_p = pkt->data+pkt->offset;

    for ( last_sg = &sg_list[sg_len]; sg_list < last_sg; sg_list++ ) {
        cs_int32 l;

        from_p = (cs_uint8 *)(sg_list->buf); // normal cached address
        l = sg_list->len;

        if ( l > total_len )
            l = total_len;

        memcpy(to_p, from_p, l);
        to_p += l;

        total_len -= l;
        if ( 0 > total_len )
            break; // Should exit via sg_last normally
    }

    // tell upper layer that we're done with this sglist
    (sc->funs->eth_drv->tx_done)(sc, key, 0);
    status = app_ipintf_packet_tx(pkt);
    if(status != CS_OK)
    {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "ethdrv_pkt_send failed!\n");
    }
    iros_free(pkt);

    return;
}

extern int eth_drv_recv_error;

static void if_eth1_deliver(struct eth_drv_sc *sc)
{
    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "if_eth_deliver\n");

    cortina_if_eth_private_t* p_cortina_eth = (cortina_if_eth_private_t *)(sc->driver_private);
    if (0 == p_cortina_eth) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "sc corrupted\n");
        return;
    }

    cs_mutex_lock(g_eth1_ip_mutex);
    cs_uint8 **packet_list = p_cortina_eth->packet_list;

    while (1) {
        if (p_cortina_eth->packet_buf != NULL) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"wait due recv still busy\n");
            break;
        }

        cs_uint32 tail = p_cortina_eth->packet_list_tail;
        if (NULL == packet_list[tail]) {
            //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "no more packet\n");
            cs_mutex_unlock(g_eth1_ip_mutex);
            return;
        }

        cs_uint16 length = p_cortina_eth->packet_list_packet_len[tail];
        p_cortina_eth->packet_buf = packet_list[tail];
        p_cortina_eth->packet_len = length;

        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "fwd packet to stack, len %d\n", length);
        // offer the data to the network stack
        (sc->funs->eth_drv->recv)(sc, length);

        packet_list[tail] = NULL;
        p_cortina_eth->packet_list_packet_len[tail] = 0;

        p_cortina_eth->packet_list_tail++;
        if (p_cortina_eth->packet_list_tail >= PACKET_LIST_MAX ) {
            p_cortina_eth->packet_list_tail = 0;
        }

        
        if(eth_drv_recv_error != 0){
            iros_free(p_cortina_eth->packet_buf);
            p_cortina_eth->packet_buf = NULL;
            p_cortina_eth->packet_len = 0;
            break;
        }
    }

    cs_mutex_unlock(g_eth1_ip_mutex);
    return;
}

static void if_eth1_poll(struct eth_drv_sc *sc)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "if_eth_poll: NOT SUPPORTED\n");
    return;
}

static cs_int32 if_eth1_int_vector(struct eth_drv_sc *sc)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "if_eth_int_vector: NOT SUPPORTED\n");
    return 0;
}
#endif


#endif /*#ifdef HAVE_IP_STACK*/
/*end of the file*/







