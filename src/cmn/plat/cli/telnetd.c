#include <network.h>
#include "plat_common.h"
#include "cli_common.h"

struct cli_def *telnet_cli = NULL; /* Global telnet session.*/
extern cs_uint32 app_ip_changed;

extern int cur_chan;
extern void cli_usr_init(struct cli_def *cli);
extern void telnet_diag_print_register(void);
extern void telnet_diag_print_unregister(void);

cs_console_t g_console;

void console_put_char(char c)
{      
    cs_mutex_lock(g_console.lock);

    if(!g_console.full) {
        g_console.empty = 0;
        
        g_console.buf[g_console.wr_ptr] = c;
        
        g_console.wr_ptr++;
        if(g_console.wr_ptr == CS_CONSOLE_BUF_LEN) {
           g_console.wr_ptr = 0; 
        }

        if(g_console.rd_ptr == g_console.wr_ptr) {
            g_console.full = 1; /* queue become full */       
        }        
    }

    /* if buf full, drop the char */

    cs_mutex_unlock(g_console.lock); 

    cs_semaphore_post(g_console.rd_avail);
         
}

cs_uint16 console_buf_read(cs_uint8 *buf)
{
    cs_uint16 len = 0;
    cs_uint16 tot_len = 0;

    cs_semaphore_wait(g_console.rd_avail, CS_OSAL_WAIT_FOREVER);   
    
    cs_mutex_lock(g_console.lock);
    if(g_console.empty) {
        cs_mutex_unlock(g_console.lock);
        return 0;
    }

    if(g_console.full) {
        tot_len = CS_CONSOLE_BUF_LEN;

        len = CS_CONSOLE_BUF_LEN - g_console.rd_ptr;
        memcpy(buf, &g_console.buf[g_console.rd_ptr], len);
        buf += len;

        len = g_console.wr_ptr;
        if(len) {
            memcpy(buf, &g_console.buf[0], len);
        }

        g_console.rd_ptr = 0;
        g_console.wr_ptr = 0;
    }
    else {
        if(g_console.wr_ptr < g_console.rd_ptr) {
            len = CS_CONSOLE_BUF_LEN - g_console.rd_ptr;
            memcpy(buf, &g_console.buf[g_console.rd_ptr], len);
            buf += len;
            tot_len += len;

            g_console.rd_ptr = 0;
        }

        if(g_console.wr_ptr > g_console.rd_ptr) {
            len = g_console.wr_ptr - g_console.rd_ptr;
        
            memcpy(buf, &g_console.buf[g_console.rd_ptr], len);
            g_console.rd_ptr += len;
            tot_len += len;
        }    
    }        

    g_console.empty = 1;

    if(g_console.full) {        
        g_console.full = 0;
    }

    cs_mutex_unlock(g_console.lock);    

    return tot_len;
}

cs_uint8 g_telnet_out_buf[CS_CONSOLE_BUF_LEN];

void telnetd_wrt_thread_entry(cyg_addrword_t p)
{    
    cs_uint16 len = 0;        


    while(1) {
        len = console_buf_read(g_telnet_out_buf);
        if(telnet_cli && len) {
            send(telnet_cli->sockfd, g_telnet_out_buf, len, 0);
        }
    }
}

#ifdef HAVE_TELNET_CLI
void telnetd_thread_entry(cyg_addrword_t p)
{
    struct cli_def *cli = NULL;
    int s, x;
    struct sockaddr_in addr;
    struct sockaddr dummy;
    int on = 1;

    // setup socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cs_printf("socket");
        return;
    }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR|SO_KEEPALIVE, &on, sizeof(on));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(TELNETD_PORT);
    if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        cs_printf("bind");
        return;
    }

    if (listen(s, 1) < 0)
    {
        cs_printf("listen");
        return;
    }

    cs_printf("Telnetd listening on port %d\n", TELNETD_PORT);
    memset(&dummy, sizeof(dummy), 0);
    while ((x = accept(s, &dummy, 0))>=0)
    {
        socklen_t len = sizeof(addr);

        if (getpeername(x, (struct sockaddr *) &addr, &len) >= 0)
            cs_printf(" * accepted connection from %s\n", inet_ntoa(addr.sin_addr));

        gpst_cmd_tree = cli_tree_init();
        // init telnet session
        cli = cli_init(gpst_cmd_tree);
        if(NULL == cli)
        {
            cs_printf(" cannot allocate session\n");
            return;
        }

        // configure session
        cli->sockfd = x;
        cli->channel = CHANNEL_TCP;

        // configure telnet session
        cli_usr_init(cli);
#ifdef HAVE_TELNET_CLI
        cur_chan = CHANNEL_TCP;
        telnet_cli = cli;
        telnet_diag_print_register();
        app_ip_changed = 0;
#endif
        cli_loop(cli);


        close(x);
        
#ifdef HAVE_TELNET_CLI
        cur_chan = CHANNEL_SERIAL;
        telnet_diag_print_unregister();
        telnet_cli = NULL; 
#endif
    }
    if(cli)
        cli_done(cli);
    
    return;
}

void telnetd_init()
{
    cs_uint32 telnet_thread_id;
    cs_uint32 ret;

    cs_uint32 telnet_wr_th_id;

    memset(&g_console, 0, sizeof(g_console));
    cs_semaphore_init(&g_console.rd_avail, "cosonle_rd_avail", 0, 0);
    cs_mutex_init(&g_console.lock, "console_lock", 0);

    g_console.full = 0;
    g_console.empty = 1;

    ret = cs_thread_create(&telnet_wr_th_id,
                            "telnet wrt",
                            (const void *)telnetd_wrt_thread_entry, (void *)0,
                             TELNETD_THREAD_STACKSIZE/2,
                             TELNETD_THREAD_PRIORITY+1,
                             0);

    ret = cs_thread_create(&telnet_thread_id,
                            TELENTD_THREAD_NAME,
                            (const void *)telnetd_thread_entry, (void *)0,
                             TELNETD_THREAD_STACKSIZE,
                             TELNETD_THREAD_PRIORITY,
                             0);
    
    if (ret != CS_E_OK)
    {
        cs_printf("telnetd thread failed to create\n");
    }
    else
    {
        cs_printf("telnetd thread created\n");
    }
}

#endif
