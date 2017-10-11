/*  
 * Copyright 2001-2010 Georges Menie (www.menie.org)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <xyzModem.h>
#include <cyg/crc/crc.h>
#include <string.h>
#include <pkgconf/hal.h>
#include <cyg/hal/hal_if.h>
#include "iros_config.h"
#include <pkgconf/system.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_immenstar.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/infra/diag.h>
#include <pkgconf/isoinfra.h>
#include <cyg/kernel/kapi.h>
#include <osal_api_core.h>

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define MAX_REV_NUM                         100 /*Max Packet to Receive*/

#define XModem_MAX_RETRIES             20


extern channel_data_t immenstar_ser_channels[IMST_NUM_OF_UARTS];

extern  bool cyg_hal_plf_serial_getc_timeout(void* __ch_data, cyg_uint8* ch);

extern  bool cyg_hal_plf_serial_getc_nonblock(void* __ch_data, cyg_uint8* ch);

extern void cyg_serial_putc(void *__ch_data, char c);

void serial_put(void * ch_data ,cs_uint8 c)
{
    cyg_serial_putc(ch_data,c);
    return ;
}
    

bool serial_get(void * ch_data ,cs_uint8* ch)
{
    return cyg_hal_plf_serial_getc_timeout(ch_data,ch);
}

unsigned short crc16_ccitt( const void *buf, int len )
{
    unsigned short crc = 0;
    while( len-- ) {
        int i;
        crc ^= *(char *)buf++ << 8;
        for( i = 0; i < 8; ++i ) {
            if( crc & 0x8000 )
                crc = (crc << 1) ^ 0x1021;
            else
                crc = crc << 1;
        }
    }
    return crc;
}

static int check(int crc, const unsigned char *buf, int sz)
{
    if (crc) {
        unsigned short crc = crc16_ccitt(buf, sz);
        unsigned short tcrc = (buf[sz]<<8)+buf[sz+1];
        if (crc == tcrc)
            return 1;
    }
    else {
        int i;
        unsigned char cks = 0;
        for (i = 0; i < sz; ++i) {
            cks += buf[i];
        }
        if (cks == buf[sz])
        return 1;
    }

    return 0;
}

static void
xyzModem_flush(void)
{
    char c;
    while (true) {
        if(!cyg_hal_plf_serial_getc_timeout((void *) &immenstar_ser_channels[0], &c))
            return;
    }
}

int Serial_step=0; /*Use to trace xmodem error*/
int XModem_stream_get(char *buf, int size, int *err)
{
    unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
    unsigned char *p;
    int bufsz, crc = 0;
    unsigned char trychar = 'C';
    unsigned char packetno = 1;
    int i, len = 0;
    cs_uint8 c=0;
    int cnt;
    int retry, retrans = XModem_MAX_RETRIES;
    xyzModem_flush();
    for(cnt=0;cnt<MAX_REV_NUM;cnt++) {
        for(retry = 0; retry < MAX_REV_NUM; ++retry) {
            Serial_step=0;
            if (trychar) {
                HAL_DELAY_US(1000000);
                serial_put((void *) &immenstar_ser_channels[0], trychar);
            }
            if (serial_get((void *) &immenstar_ser_channels[0], &c)) {
                Serial_step=Serial_step|1;
                switch (c) {
                case SOH:
                    bufsz = 128;
                    goto start_recv;
                case STX:
                    bufsz = 1024;
                    goto start_recv;
                case EOT:
                    xyzModem_flush();
                    serial_put((void *) &immenstar_ser_channels[0], ACK);
                    return len; /* normal end */
                case CAN:
                    serial_get((void *) &immenstar_ser_channels[0], &c);
                    if (c == CAN) {
                        xyzModem_flush();
                        serial_put((void *) &immenstar_ser_channels[0], ACK);
                        *err = -1; /* canceled by remote */
                        return len;
                    }
                    break;
                default:
                    break;
                }
            }
        }

        if (trychar == 'C') { 
            trychar = NAK; 
            continue; 
        }
        if (trychar == NAK) { 
            trychar = 'C'; 
            continue; 
        }

        Serial_step=Serial_step|2;
        xyzModem_flush();
        serial_put((void *) &immenstar_ser_channels[0], CAN);
        serial_put((void *) &immenstar_ser_channels[0], CAN);
        serial_put((void *) &immenstar_ser_channels[0], CAN);
        *err = -2; /* canceled by remote */
        return len;

    start_recv:
        if (trychar == 'C') crc = 1;
        trychar = 0;
        p = xbuff;
        *p++ = c;

        for (i = 0;  i < (bufsz+(crc?1:0)+3); ++i) {
            if (!serial_get((void *) &immenstar_ser_channels[0], &c)) {
                Serial_step=(Serial_step&0xFFFF)|((i+1)<<16);
                /*For Tera term Bug fix*/
                if((i+1==bufsz+4)&&(crc==1)){
                    unsigned short temp_crc=crc16_ccitt(&xbuff[3], bufsz);
                    xbuff[bufsz+3]=(char)(temp_crc>>8);
                    xbuff[bufsz+4]=(char)(temp_crc);
                    break;
                }
                goto reject;  
            }
            *p++ = c;
        }
        Serial_step=Serial_step|4;
        if ((xbuff[1] == (unsigned char)(~xbuff[2])) && 
            (xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno-1) &&
            check(crc, &xbuff[3], bufsz)) {
            Serial_step=Serial_step|8;
            if (xbuff[1] == packetno)   {
                register int count = size - len;
                if (count > bufsz) count = bufsz;
                if (count > 0) {
                    memcpy (&buf[len], &xbuff[3], count);
                    len += count;
                }
                ++packetno;
                retrans = XModem_MAX_RETRIES+1;
            }
            if ((--retrans <= 0)||(size<=len)) {
                Serial_step=Serial_step|0x10;
                xyzModem_flush();
                serial_put((void *) &immenstar_ser_channels[0], CAN);
                serial_put((void *) &immenstar_ser_channels[0], CAN);
                serial_put((void *) &immenstar_ser_channels[0], CAN);
                *err = -3;/* too many retry error */
                return len; 
            }
            serial_put((void *) &immenstar_ser_channels[0], ACK);
            Serial_step=Serial_step|0x20;
            continue;
        }
    reject:
        Serial_step=Serial_step|0x40;
        xyzModem_flush();
        serial_put((void *) &immenstar_ser_channels[0], NAK);
    }
    return len;
}



int XModem_stream_put(char *buf, int size, int *err)
{
    unsigned char xbuff[1030]={0}; /* 256 for XModem 1k + 3 head chars + 2 crc + nul */
    int bufsz;
    unsigned char packetno = 1;
    int i, len = 0;
    unsigned char c=0;
    int retry;
    char crc=-1;
    int cnt;
    unsigned char start_trans=0;
    xyzModem_flush();
    for( retry = 0; retry < MAX_REV_NUM; ++retry) {
        HAL_DELAY_US(1000);
        if (serial_get((void *) &immenstar_ser_channels[0], &c)) {
            switch (c) {
                case 'C':
                     crc=1;
                     start_trans=1;
                     Serial_step=Serial_step|1;
                    break;
                case NAK:
                    crc=0;
                    start_trans=1;
                    break;
                case CAN:
                    if(serial_get((void *) &immenstar_ser_channels[0], &c)){
                        if (c == CAN) {
                            serial_put((void *) &immenstar_ser_channels[0], ACK);
                            xyzModem_flush();
                            *err = -1; /* canceled by remote */
                            return len;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        if(start_trans)
            break;
    }
    
    if(!start_trans){
        Serial_step=Serial_step|2;
        serial_put((void *) &immenstar_ser_channels[0], CAN);
        serial_put((void *) &immenstar_ser_channels[0], CAN);
        serial_put((void *) &immenstar_ser_channels[0], CAN);
        xyzModem_flush();
        *err=-2;
        return len; /* no sync */
    }

  /*Sync finish, start to transport the data.*/
    while(size>len) {
        xbuff[0] = STX; 
        xbuff[1] = packetno;
        xbuff[2] = ~packetno;
        cnt = size - len;
        bufsz = 1024;
        if (cnt > bufsz) cnt = bufsz;       
        memset (&xbuff[3], CTRLZ, bufsz);
        memcpy (&xbuff[3], &buf[len], cnt);
        Serial_step=Serial_step+0x10000;;
        if(crc){
            unsigned short ccrc=0;
            ccrc = crc16_ccitt(&xbuff[3], bufsz);
            xbuff[bufsz+3] = (ccrc>>8) & 0xFF;
            xbuff[bufsz+4] = ccrc & 0xFF;  
        }
        else{
            unsigned char ccks = 0;
            for (i = 3; i < bufsz+3; ++i) {
                ccks += xbuff[i];
            }
            xbuff[bufsz+3] = ccks;
        }
        
        start_trans=0;
        xyzModem_flush();
        for (retry = 0; retry < 2*XModem_MAX_RETRIES; ++retry) {
            for (i = 0; i < bufsz+4+(crc?1:0); ++i) {/*2 Byte for CRC, 3 for header */
                serial_put((void *) &immenstar_ser_channels[0],xbuff[i]);
            }
            if(serial_get((void *) &immenstar_ser_channels[0], &c)){
                switch (c) {
                case ACK:
                    ++packetno;
                    len += bufsz;
                    start_trans=1;
                    Serial_step=Serial_step|0x8;
                    break;
                case CAN:
                    Serial_step=Serial_step|0x10;
                    if(serial_get((void *) &immenstar_ser_channels[0], &c)){
                        if (c == CAN) {
                            serial_put((void *) &immenstar_ser_channels[0],ACK);
                            xyzModem_flush();
                            *err = -1; /* canceled by remote */
                            return len;
                        }
                    }
                    break;
                case NAK:
                    Serial_step=Serial_step|0x20;
                default:
                    Serial_step=Serial_step|0x40;
                    break;
                }
            }
            if(start_trans){
                break;
            }else{
                //HAL_DELAY_US(100);/*Clear all the char if send fail.*/
                xyzModem_flush();
            }         
        }
        if(!start_trans){
            Serial_step=Serial_step|0x80;
            serial_put((void *) &immenstar_ser_channels[0],CAN);
            serial_put((void *) &immenstar_ser_channels[0],CAN);
            serial_put((void *) &immenstar_ser_channels[0],CAN);
            xyzModem_flush();
            *err=-2;
            return len; /* no sync */
        }
    }
/*Finish the transport and wait the ACK from receiver.*/
    for (retry = 0; retry < XModem_MAX_RETRIES; ++retry) {
        serial_put((void *) &immenstar_ser_channels[0],EOT);
        if(serial_get((void *) &immenstar_ser_channels[0], &c)){
            Serial_step=Serial_step|0x100;
            if(c == ACK) {        
                Serial_step=Serial_step|0x200;
                xyzModem_flush();
                return len;
            }
        }
    }
    xyzModem_flush();
    *err=-5;
    return len;   
}

