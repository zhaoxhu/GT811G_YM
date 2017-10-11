/* =================================================================
 *
 *      httpd.c
 *
 *      A simple embedded HTTP server
 *
 * =================================================================
 * ####ECOSGPLCOPYRIGHTBEGIN####
 * -------------------------------------------
 * This file is part of eCos, the Embedded Configurable Operating
 * System.
 * Copyright (C) 2002 Nick Garnett.
 * Copyright (C) 2003 Andrew Lunn.
 *
 * eCos is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 or (at your option)
 * any later version.
 *
 * eCos is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with eCos; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * As a special exception, if other files instantiate templates or
 * use macros or inline functions from this file, or you compile this
 * file and link it with other works to produce a work based on this
 * file, this file does not by itself cause the resulting work to be
 * covered by the GNU General Public License. However the source code
 * for this file must still be made available in accordance with
 * section (3) of the GNU General Public License.
 *
 * This exception does not invalidate any other reasons why a work
 * based on this file might be covered by the GNU General Public
 * License.
 *
 * -------------------------------------------
 * ####ECOSGPLCOPYRIGHTEND####
 * =================================================================
 * #####DESCRIPTIONBEGIN####
 *
 *  Author(s):    nickg@calivar.com
 *  Contributors: nickg@calivar.com, Andrew.lunn@ascom.ch
 *  Date:         2002-10-14
 *  Purpose:
 *  Description:
 *
 * ####DESCRIPTIONEND####
 *
 * =================================================================
 */

#include <pkgconf/system.h>
#include <pkgconf/isoinfra.h>
#include <pkgconf/httpd.h>

#include <cyg/infra/cyg_trac.h>        /* tracing macros */
#include <cyg/infra/cyg_ass.h>         /* assertion macros */

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <network.h>
#include <arpa/inet.h>

#include <cyg/httpd/httpd.h>


/* ================================================================= */
extern void ct_get_cookie_str(char *cokieStr);
extern unsigned char ct_do_authorization(unsigned char *baseStr);
extern unsigned char ct_check_authorization(unsigned char *baseStr);
extern void ct_web_init();

/* ================================================================= */

static char *Set_cookie(void);

#if 0
#define HTTPD_DIAG diag_printf
#define HTTPD_TRACE diag_printf
#else
#define HTTPD_DIAG(...)
#define HTTPD_TRACE(...)
#endif

#define HTTPD_HDR_COOKIE "Cookie: "
#define HTTPD_AUTH_BASIC "Authorization: Basic "
#define HTTPD_DEFAULT_UNAME_PASSWD "admin:admin"
#define HTTPD_DEFAULT_COOKIE "user=admin|admin|user|0;"
#define HTTPD_DEFAULT_ETAG "ETag: \"cortina\"\r\n"
#define ADD_ETAG fprintf(client,"%s" , HTTPD_DEFAULT_ETAG)

#define HTTPD_COOKIE_LEN	256
static	unsigned char user_type;
#define REQ_METHOD_LEN		512

#define html_begin_unauthorized(__client)                            \
        cyg_http_start_unauthorized( __client, "text/html", 0 );     \
        html_tag_begin( __client, "html", "" )

unsigned char do_authorization;
unsigned char check_authorization;
unsigned char is_authorized;
unsigned char base64[120];
cyg_uint32  onu_httpd_count;


/* ================================================================= */
/* Server socket address and file descriptor.
 */

static struct sockaddr_in server_address;

static int server_socket = -1;
#ifdef CYGPKG_NET_INET6
static int server_socket6 = -1;
static struct sockaddr_in6 server_address6;
#endif

/* ================================================================= */
/* Thread stacks, etc.
 */

static cyg_uint8 httpd_stacks[CYGNUM_HTTPD_THREAD_COUNT]
                             [CYGNUM_HAL_STACK_SIZE_MINIMUM+
                              CYGNUM_HTTPD_SERVER_BUFFER_SIZE+
                              CYGNUM_HTTPD_THREAD_STACK_SIZE];

static cyg_handle_t httpd_thread[CYGNUM_HTTPD_THREAD_COUNT];

static cyg_thread httpd_thread_object[CYGNUM_HTTPD_THREAD_COUNT];

/* ================================================================= */
/* Filename lookup table
 */

CYG_HAL_TABLE_BEGIN( cyg_httpd_table, httpd_table );
CYG_HAL_TABLE_END( cyg_httpd_table_end, httpd_table );

__externC cyg_httpd_table_entry cyg_httpd_table[];
__externC cyg_httpd_table_entry cyg_httpd_table_end[];

/* ================================================================= */
/* Page not found message
 */

static char cyg_httpd_not_found[] =
"<head><title>Page Not found</title></head>\r\n"
"<body><h2>The requested URL was not found on this server.</h2></body>\r\n";

static char cyg_httpd_error_internal[] =
"<head><title>Error happened</title></head>\r\n"
"<body><h2>Internal error happened on this server.</h2></body>\r\n";

static char cyg_httpd_error_cgi[] =
"<head><title>Error happened</title></head>\r\n"
"<body><h2>CGI handler error happened on this server.</h2></body>\r\n";

static char cyg_httpd_not_authorized[] =
"<head><title>Permission Denied</title></head>\r\n"
"<body><h2>Authorization required.</h2></body>\r\n";

/* ================================================================= */
/* Simple pattern matcher for filenames
 *
 * This performs a simple pattern match between the given name and the
 * pattern. At present the only matching supported is either exact, or
 * if the pattern ends in * then that matches all remaining
 * characters. At some point we might want to implement a more
 * complete regular expression parser here.
 */

static cyg_bool match( char *name, char *pattern )
{
    while( *name != 0 && *pattern != 0 && *name == *pattern )
        name++, pattern++;

    if( *name == 0 && *pattern == 0 )
        return true;

    if( *pattern == '*' )
        return true;

    return false;
}


__externC unsigned char cyg_httpd_authorize(unsigned char *auth_val,
                                         unsigned char *uname_passwd)
{
    unsigned char auth_result;
    int len_auth_val = strlen(auth_val);
    int len_uname_passwd = strlen(uname_passwd);

    if( len_auth_val == len_uname_passwd
        && 0 == memcmp(auth_val, uname_passwd, len_auth_val) )
    {
        auth_result = 1;
    }
    else
    {
        auth_result = 0;
    }

    return auth_result;
}


/* ================================================================= */
/* Main processing function                                          */
/*                                                                   */
/* Reads the HTTP header, look it up in the table and calls the      */
/* handler.                                                          */
#define HASH_TABLE_SIZE     512

typedef struct hash_tab_stru
{
    unsigned long value_HashA;
    unsigned long value_HashB;
    bool entry_exists;
    cyg_httpd_table_entry *link_entry;
} HASH_TABLE;

HASH_TABLE hash_table[HASH_TABLE_SIZE];
unsigned long hash_element_count;
unsigned long crypt_table[0x500];

void prepare_crypt_table()
{
    unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;

    for( index1 = 0; index1 < 0x100; index1++ )
    {
        for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )
        {
            unsigned long temp1, temp2;

            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;

            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);
            crypt_table[index2] = ( temp1 | temp2 );
        }
    }
}

static unsigned long hash_string(char *input_string, unsigned long hash_type)
{
    unsigned char *key = (unsigned char *)input_string;
    unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
    int ch;

    while(*key != 0)
    {
        ch = toupper(*key++);

        seed1 = crypt_table[(hash_type << 8) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
    }
    return seed1;
}

static int hash_get_table_pos(char *input_string, HASH_TABLE *hash_tab, int tab_size)
{
    const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
    unsigned long nHash = hash_string(input_string, HASH_OFFSET);
    unsigned long nHashA = hash_string(input_string, HASH_A);
    unsigned long nHashB = hash_string(input_string, HASH_B);
    unsigned long nHashStart = nHash % tab_size;
    unsigned long nHashPos = nHashStart;

    HTTPD_DIAG("Search in hash, start pos %d\n", nHashPos);
    while (hash_tab[nHashPos].entry_exists)
    {
        if (hash_tab[nHashPos].value_HashA == nHashA && hash_tab[nHashPos].value_HashB == nHashB)
        {
            return nHashPos;
        }
        else
        {
            HTTPD_DIAG("Search again, cur pos %d, hashA: %ld - %ld, hashB: %ld - %ld\n", nHashPos,
                    hash_tab[nHashPos].value_HashA, nHashA, hash_tab[nHashPos].value_HashB, nHashB);
            nHashPos = (nHashPos + 1) % tab_size;
        }

        if (nHashPos == nHashStart)
        {
            HTTPD_DIAG("ERR: not found in hash, start pos %d\n", nHashStart);
            break;
        }
    }

    return -1;
}

static void hash_add_linkname(cyg_httpd_table_entry* link_entry, char *input_string, HASH_TABLE *hash_tab, int tab_size)
{
    const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
    unsigned long nHash = hash_string(input_string, HASH_OFFSET);
    unsigned long nHashA = hash_string(input_string, HASH_A);
    unsigned long nHashB = hash_string(input_string, HASH_B);
    unsigned long nHashStart = nHash % tab_size;
    unsigned long nHashPos = nHashStart;

    if(hash_element_count >= tab_size)
    {
        HTTPD_DIAG("Hash table full! elment=%d\n", hash_element_count);
        return;
    }

    while (hash_tab[nHashPos].entry_exists)
    {
        HTTPD_DIAG("pos occupied %d\n", nHashPos);
        nHashPos = (nHashPos + 1) % tab_size;

        if (nHashPos == nHashStart)
            break;
    }

    //diag_printf("add element to hash, total %4d, pos %4d, url: %s\n", hash_element_count, nHashPos, link_entry->pattern);

    hash_tab[nHashPos].value_HashA = nHashA;
    hash_tab[nHashPos].value_HashB = nHashB;
    hash_tab[nHashPos].entry_exists = true;
    hash_tab[nHashPos].link_entry = link_entry;
    hash_element_count++;

    return;
}

static void cyg_httpd_process( int client_socket, struct sockaddr *client_address )
{
    int calen = sizeof(*client_address);
    int nlc = 0;
    char http_inbuffer[CYGNUM_HTTPD_SERVER_BUFFER_SIZE+1];
    cyg_int32 req_buf_len;
    char *req_buf = NULL;
    FILE *client = NULL;
    cyg_httpd_table_entry *entry = cyg_httpd_table;
    int hash_pos = -1;
    char *filename;
    char *formdata = NULL;
    char *p;
    char  *condition_get;
    unsigned char handler_ret = 0; // 0 means no error
    unsigned char action = HTTP_PROCESS_OK; // 0-no action, 1-handler ret error, 2-no page, 3-internal error
    char name[64];
    char port[10];
    char req_auth[120];
    char* pos_stream = NULL;
    long header_len = 0;

    getnameinfo(client_address, calen, name, sizeof(name),
                port, sizeof(port), NI_NUMERICHOST|NI_NUMERICSERV);
    HTTPD_DIAG("Connection from %s[%s]\n", name, port);

    /* Convert the file descriptor to a C library FILE object so
     * we can use fprintf() and friends on it.
     */
    HTTPD_TRACE("client_socket :%d\n", client_socket);

    // Process request and send response to client
    client = fdopen(client_socket, "r+");
    HTTPD_TRACE("open client FILE pointer: 0x%08x\n", client);

    // By placing a terminating '\0' not only we have a safe stopper point
    //  for our parsing, but also we can detect if we have a split header.
    // Since headers always end with an extra '\r\n', if we find a '\0'
    //  before the terminator than we can safely assume that the header has
    //  not been received completely and more is following (i.e. split headers.)
    http_inbuffer[0] = '\0';
    req_buf_len = 0;

    do{
        int c = getc( client );

        http_inbuffer[req_buf_len] = c;
        req_buf_len++;
        if(req_buf_len >= CYGNUM_HTTPD_SERVER_BUFFER_SIZE)
        {
            HTTPD_TRACE("Error: header too large!\n");
            HTTPD_TRACE("fclose client FILE pointer: 0x%08x\n", client);
            fclose(client);
            return;
        }

        if( c == '\n' )
            nlc++;
        else if( c != '\r' )
            nlc = 0;
    } while(nlc < 2);

    http_inbuffer[req_buf_len] = '\0';
    HTTPD_DIAG("header: >%s<\n", http_inbuffer);

    // Parse http headers, we are interested two fiedls in the http_inbuffer.
    // first line and authentication tag
    req_buf = http_inbuffer;

    char* field_end = NULL;
    char tmp_char;

    field_end = strstr(req_buf, "\r\n");
    if(NULL == field_end)
    {
        HTTPD_TRACE("Error http header!\n");
        HTTPD_TRACE("fclose client FILE pointer: 0x%08x\n", client);
        fclose(client);
        return;
    }
    else
    {
        field_end += 2;
        tmp_char = *field_end;
        *field_end = '\0';
    }

    /*
     * Search for the HTTP Authentication header, if present
     * and save its value (120 chars max)
     */
    is_authorized = 0;
    do_authorization = 0;
    check_authorization = 0;
    memset(req_auth, 0, sizeof(req_auth));

    while(1)
    {
        req_buf = field_end;
        *req_buf = tmp_char;
        field_end = strstr(req_buf, "\r\n");

        if(field_end == req_buf || NULL == field_end)
        {
            // the last one or incomplete http header
            break;
        }
        else
        {
            field_end += 2;
            tmp_char = *field_end;
            *field_end = '\0';

            if (0 == strncasecmp(req_buf, HTTPD_AUTH_BASIC, strlen(HTTPD_AUTH_BASIC)))
            {
                do_authorization = 1;

                strncpy(req_auth, req_buf+strlen(HTTPD_AUTH_BASIC), sizeof(req_auth));
                req_auth[strlen(req_auth)-2] = '\0';
                HTTPD_DIAG("tag: %s, value: >%s<\n", HTTPD_AUTH_BASIC, req_auth);
            }
            else if (0 == strncasecmp(req_buf, HTTPD_HDR_COOKIE, strlen(HTTPD_HDR_COOKIE)))
            {
                check_authorization = 1;

                strncpy(req_auth, req_buf+strlen(HTTPD_HDR_COOKIE), sizeof(req_auth));
                req_auth[strlen(req_auth)-2] = '\0';

                HTTPD_DIAG("tag: %s, value: >%s<\n", HTTPD_HDR_COOKIE, req_auth);
            }

            if(do_authorization && check_authorization)
            {
                req_buf = field_end;
                *req_buf = tmp_char;
                break;
            }
        }
    }


    if (1 == do_authorization)
    {
        HTTPD_DIAG("Do Authentication requested w/ credentials (%s %s)\n",
                  HTTPD_AUTH_BASIC, req_auth);
        memset(base64, 0, sizeof(base64));
        _b64_decode(req_auth, base64);
        HTTPD_DIAG("Decoded credentials (%s)\n", base64);

        is_authorized = ct_do_authorization(base64);

	}
    else if (1 == check_authorization)
    {
        HTTPD_DIAG("Check Authentication requested w/ credentials (%s %s)\n",
                  HTTPD_HDR_COOKIE, req_auth);

        is_authorized = ct_check_authorization(req_auth);

    }

    if(!is_authorized)
    {
        HTTPD_DIAG("Not authorized user, send unauthorized to client\n");
        cyg_httpd_send_unauthorized( client, NULL, NULL,
                                     cyg_httpd_not_authorized );
        fclose(client);
        return;
    }

    /* Extract the filename and any form data being returned.
     * We know that the "GET " request takes 4 bytes, "POST " takes 5 bytes.
     */
    req_buf = http_inbuffer;
    if (strncasecmp("GET ", req_buf, 4) == 0)
    {
        filename = p = req_buf + 4;
    }
    else if (strncasecmp("POST ", req_buf, 5) == 0)
    {
        filename = p = req_buf + 5;
    }

    /* Now scan the filename until we hit a space or a '?'. If we
     * end on a '?' then the rest is a form request. Put NULs at
     * the end of each string.
     */
    while( *p != ' ' && *p != '?' )
        p++;
    if( *p == '?' )
        formdata = p+1;
    *p = 0;

    if( formdata != NULL )
    {
        while( *p != ' ' )
            p++;
        *p = 0;
    }

    HTTPD_DIAG("Request filename >%s< formdata >%s<\n",filename,formdata?formdata:"-NULL-");

#if 1
    //deal the cached res
    condition_get=p;
    while( *condition_get != '\r'  || *(condition_get+1) != '\n'  || *(condition_get+2) != '\r'  || *(condition_get+3) != '\n' )
    {
    	if(strncasecmp("If-None-Match", condition_get, 13) == 0)
        {
            fprintf(client,"%s%s%s\r\n" , "HTTP/1.1 304 Not Modified\r\n","Connection: Keep-Alive\r\n", HTTPD_DEFAULT_ETAG);
            fflush(client);
            fclose(client);
            return;
        }
    	else
        	condition_get++;
    }
#endif

    entry = cyg_httpd_table;
    while( entry != cyg_httpd_table_end )
    {
       if(match(filename, entry->pattern))
         {
            handler_ret = entry->handler(client, filename, formdata, entry->arg);
            if(handler_ret != HTTP_PROCESS_OK)  // 1 to false; 0 to true
            {
                diag_printf("Error: handle %s when calling %08x: %s, ret %d\n", filename,entry, entry->pattern, handler_ret);
                action = handler_ret;
            }
            else
            {
                action = HTTP_PROCESS_OK;
            }
            break;
         }
        else
        {
            entry++;
        }
    }
    if( cyg_httpd_table_end ==  entry)
    {
    	action = HTTP_PROCESS_NOT_FOUND;
    }

    // 0-no action, 1-handler ret error, 2-no page, 3-internal error
    switch(action)
    {
        case HTTP_PROCESS_OK:
            break;
        case HTTP_PROCESS_CGI_ERROR:
            cyg_httpd_send_html( client, NULL, NULL, cyg_httpd_error_cgi );
            break;
        case HTTP_PROCESS_NOT_FOUND:
            cyg_httpd_send_html( client, NULL, NULL, cyg_httpd_not_found );
            //diag_printf("filename %s not found\n",filename);
            break;
        case HTTP_PROCESS_INTERNAL_ERROR:
            cyg_httpd_send_html( client, NULL, NULL, cyg_httpd_error_internal );
            break;
        default:
            diag_printf("Error: invalid action %d\n", action);
            break;
    }


    // Reset authorization flags until next HTTP Request is received.
    do_authorization = 0;
    check_authorization = 0;
    is_authorized = 0;

    fclose(client);
    return;
}

/* ================================================================= */
/* Main HTTP server
 *
 * This just loops, collects client connections, and calls the main
 * process function on the connects*/

static void cyg_httpd_server( cyg_addrword_t arg )
{

    HTTPD_DIAG("cyg_httpd_server started\n");

    ct_web_init();

    // process HTTP request
    do
    {
        int client_socket;
        struct sockaddr client_address;
        socklen_t calen = sizeof(client_address);
        fd_set readfds;
        int n;

        /* Wait for a connection.
         */
        onu_httpd_count++;
        HTTPD_TRACE("httpd wait for a connection\n");
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);

#ifdef CYGPKG_NET_INET6
        FD_SET(server_socket6, &readfds);
        n = (server_socket > server_socket6 ? server_socket : server_socket6) + 1;
#else
        n = server_socket + 1;
#endif
        select(n,&readfds,NULL,NULL,NULL);
        HTTPD_TRACE("httpd after select\n");

        if (FD_ISSET(server_socket, &readfds))
        {
            HTTPD_DIAG("httpd to call accept\n");
            client_socket = accept( server_socket, &client_address, &calen );
            HTTPD_TRACE("accept socket=%d\n", client_socket);
            if(-1 == client_socket)
            {
                diag_printf("!!! accept failed\n");
                perror("accept");
                continue;
            }
            HTTPD_TRACE("httpd to call cyg_httpd_process\n");
            cyg_httpd_process(client_socket, &client_address);

            // client_socket already closed by fclose()
        }
        HTTPD_TRACE("httpd finished a connection\n");

#ifdef CYGPKG_NET_INET6
        if (FD_ISSET(server_socket6, &readfds)) {
          client_socket = accept( server_socket6, &client_address, &calen );
          cyg_httpd_process(client_socket, &client_address);
        }
#endif
    } while(1);
}

/* ================================================================= */
/* Initialization thread
 *
 * Optionally delay for a time before getting the network
 * running. Then create and bind the server socket and put it into
 * listen mode. Spawn any further server threads, then enter server
 * mode.
 */

static void cyg_httpd_init(cyg_addrword_t arg)
{
    int i;
    int err = 0;
    int rc;

    /* Delay for a configurable length of time to give the application
     * a chance to get going, or even complete, without interference
     * from the HTTPD.
     */
    if( CYGNUM_HTTPD_SERVER_DELAY > 0 )
    {
        cyg_thread_delay( CYGNUM_HTTPD_SERVER_DELAY );
    }

    // set tcp_msl
    extern int cyg_tcp_msl;
    //diag_printf("old tcp_msl is %d, set to 1000ms\n", cyg_tcp_msl);
    cyg_tcp_msl = 100;

    // Add table entry to hash table
    HTTPD_DIAG("table: %08x...%08x\n", cyg_httpd_table, cyg_httpd_table_end);
    /* Get the network going. This is benign if the application has
     * already done this.
     */


    memset(&server_address, 0, sizeof(struct sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_len = sizeof(server_address);
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(CYGNUM_HTTPD_SERVER_PORT);
#ifdef CYGPKG_NET_INET6
    server_address6.sin6_family = AF_INET6;
    server_address6.sin6_len = sizeof(server_address6);
    server_address6.sin6_addr = in6addr_any;
    server_address6.sin6_port = htons(CYGNUM_HTTPD_SERVER_PORT);
#endif

    /* Create and bind the server socket.
     */
    server_socket = socket( AF_INET, SOCK_STREAM, 0 );
    CYG_ASSERT( server_socket > 0, "Socket create failed");

    cyg_int32 yes = 1;
    rc = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &yes, sizeof(int));
    CYG_ASSERT( rc != -1, "setsockopt() returned error");

    err = bind( server_socket, (struct sockaddr *)&server_address,
                sizeof(server_address) );
    CYG_ASSERT( err == 0, "bind() returned error");

    err = listen( server_socket, SOMAXCONN );
    CYG_ASSERT( err == 0, "listen() returned error" );

#ifdef CYGPKG_NET_INET6
    server_socket6 = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP );
    CYG_ASSERT( server_socket6 > 0, "Socket AF_INET6 create failed");

    err = bind( server_socket6, (struct sockaddr *)&server_address6,
                sizeof(server_address6) );
    CYG_ASSERT( err == 0, "bind(AF_INET6) returned error");

    err = listen( server_socket6, SOMAXCONN );
    CYG_ASSERT( err == 0, "listen(AF_INET6) returned error" );
#endif
    /* If we are configured to have more than one server thread,
     * create them now.
     */
    for( i = 1; i < CYGNUM_HTTPD_THREAD_COUNT; i++ )
    {
        cyg_thread_create( CYGNUM_HTTPD_THREAD_PRIORITY,
                           cyg_httpd_server,
                           0,
                           "HTTPD",
                           &httpd_stacks[i][0],
                           sizeof(httpd_stacks[i]),
                           &httpd_thread[i],
                           &httpd_thread_object[i]
            );

        cyg_thread_resume( httpd_thread[i] );
    }

    /* Now go be a server ourself.
     */
    cyg_httpd_server(arg);
}

/* ================================================================= */
/* System initializer
 *
 * This is called from the static constructor in init.cxx. It spawns
 * the main server thread and makes it ready to run. It can also be
 * called explicitly by the application if the auto start option is
 * disabled.
 */

__externC void cyg_httpd_startup(void)
{
    cyg_thread_create( CYGNUM_HTTPD_THREAD_PRIORITY,
                       cyg_httpd_init,
                       0,
                       "HTTPD",
                       &httpd_stacks[0][0],
                       sizeof(httpd_stacks[0]),
                       &httpd_thread[0],
                       &httpd_thread_object[0]
        );

    cyg_thread_resume( httpd_thread[0] );
}

/* ================================================================= */
/*  HTTP protocol handling
 *
 * cyg_http_start() generates an HTTP header with the given content
 * type and, if non-zero, length.
 * cyg_http_finish() just adds a couple of newlines for luck and
 * flushes the stream.
 */

__externC void cyg_http_start( FILE *client, char *content_type,
                               int content_length )
{
    fputs( "HTTP/1.1 200 OK\r\n"
           "Server: " CYGDAT_HTTPD_SERVER_ID "\r\n",
           client );

	 ADD_ETAG ;
    if( content_type != NULL )
        fprintf( client,"Content-type: %s\r\n", content_type );

    if( content_length != 0 )
        fprintf( client, "Content-length: %d\r\n", content_length );

    fputs( "Connection: Keep-Alive\r\n"
           "\r\n",
           client );
}

__externC void cyg_http_start_with_cookie ( FILE *client, char *content_type,
                                            int content_length )
{
    unsigned char expires_on[30];
    time_t current_epoh;
    struct tm *expire_time;

    char cookie[256];

    time(&current_epoh);
    expire_time = gmtime(&current_epoh);

    /*
     * Cookie expires in 1 hour.
     *
     * TODO: Note that rollover to next day is not yet accounted for.
     */
    if (23 > expire_time->tm_hour) {
        expire_time->tm_hour++;
    }
    strftime (expires_on, sizeof(expires_on), "%a, %d-%b-%Y %H:%M:%S %Z",
              expire_time);
    fputs( "HTTP/1.1 200 OK\r\n"
           "Server: " CYGDAT_HTTPD_SERVER_ID "\r\n",
           client );
   ADD_ETAG ;
    if( content_type != NULL )
        fprintf( client,"Content-type: %s\r\n", content_type );

    if( content_length != 0 )
        fprintf( client, "Content-length: %d\r\n", content_length );

    ct_get_cookie_str(cookie);

    fprintf( client, "Set-Cookie: %s; \r\n",
             cookie);
    HTTPD_DIAG("cokie  %s\n", cookie);

    fputs( "Connection: Keep-Alive\r\n"
           "\r\n",
           client );
}

__externC void cyg_http_start_unauthorized ( FILE *client, char *content_type,
                                             int content_length )
{
    fputs( "HTTP/1.1 401 Unauthorized\r\n"
           "Server: " CYGDAT_HTTPD_SERVER_ID "\r\n",
           client );
    ADD_ETAG ;
    if( content_type != NULL )
        fprintf( client,"Content-type: %s\r\n", content_type );

    if( content_length != 0 )
        fprintf( client, "Content-length: %d\r\n", content_length );

    fprintf( client, "WWW-Authenticate: Basic realm=\"ONU Login\"\r\n");

    fputs( "Connection: close\r\n"
           "\r\n",
           client );
}

__externC void cyg_http_finish( FILE *client )
{
    fputs( "\n\n", client );
    fflush( client );
}


/* ================================================================= */
/* HTML tag generation
 *
 * These functions generate standard HTML begin and end tags. By using
 * these rather than direct printf()s we help to reduce the number of
 * distinct strings present in the executable.
 */

__externC void cyg_html_tag_begin( FILE *client, char *tag, char *attr )
{
    char *pad = "";

    if( attr == NULL )
        attr = pad;
    else if( attr[0] != 0 )
        pad = " ";

    fprintf(client, "<%s%s%s>\n",tag,pad,attr);
}

__externC void cyg_html_tag_end( FILE *client, char *tag )
{
    fprintf( client, "<%s%s%s>\n","/",tag,"");
}

/* ================================================================= */
/* Parse form request data
 *
 * Given a form response string, we parse it into an argv/environment
 * style array of "name=value" strings. We also convert any '+'
 * separators back into spaces.
 *
 * TODO: also translate any %xx escape sequences back into real
 * characters.
 */

__externC void cyg_formdata_parse( char *data, char *list[], int size )
{
    char *p = data;
    int i = 0;

    list[i] = p;

    while( p && *p != 0 && i < size-1 )
    {
        if( *p == '&' )
        {
            *p++ = 0;
            list[++i] = p;
            continue;
        }
        if( *p == '+' )
            *p = ' ';
        p++;
    }

    list[++i] = 0;
}

/* ----------------------------------------------------------------- */
/* Search for a form response value
 *
 * Search a form response list generated by cyg_formdata_parse() for
 * the named element. If it is found a pointer to the value part is
 * returned. If it is not found a NULL pointer is returned.
 */

__externC char *cyg_formlist_find( char *list[], char *name )
{
    while( *list != 0 )
    {
        char *p = *list;
        char *q = name;

        while( *p == *q )
            p++, q++;

        if( *q == 0 && *p == '=' )
            return p+1;

        list++;
    }

    return 0;
}

/* ================================================================= */
/* Predefined page handlers
 */

/* ----------------------------------------------------------------- */
/* Send an HTML page from a single string
 *
 * This just sends the string passed as the argument with an HTTP
 * header that describes it as HTML. This is useful for sending
 * straightforward static web content.
 */

__externC cyg_bool cyg_httpd_send_html( FILE *client, char *filename,
                                        char *request, void *arg )
{
    html_begin( client );

    fwrite( arg, 1, strlen((char *)arg), client );

    html_end( client );

    return 0;
}

__externC cyg_bool cyg_httpd_send_unauthorized( FILE *client, char *filename,
                                                char *request, void *arg )
{
    html_begin_unauthorized( client );

    fwrite( arg, 1, strlen((char *)arg), client );

    html_end( client );

    return 0;
}

/* ----------------------------------------------------------------- */
/* Send arbitrary data
 *
 * This takes a pointer to a cyg_httpd_data structure as the argument
 * and sends the data therein after a header that uses the content
 * type and size from the structure. This is useful for non-HTML data
 * such a images.
 */

__externC cyg_bool cyg_httpd_send_data( FILE *client, char *filename,
                                        char *request, void *arg )
{
    cyg_httpd_data *data = (cyg_httpd_data *)arg;

    cyg_http_start( client, data->content_type, data->content_length );

    fwrite( data->data, 1, data->content_length, client );

    return 0;
}

/* ----------------------------------------------------------------- */
/* end of httpd.c                                                    */
