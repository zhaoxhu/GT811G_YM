/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef _PORT_H_
#define _PORT_H_


/* message priorities: currently only two */
#define EPON_MSG_PRIORITY_NORMAL        0
#define EPON_MSG_PRIORITY_URGENT        1


/* ===> function calls from iROS to environment <=== */


/* iROS initializes the environment.
   create a main loop thread, open socket to read, and
   install ethernet input hook, etc
 */
int
epon_env_init();

/* iROS sends a message.
   Used by API functions to send messages to iROS core.
   non-blocking, returning error when queue is full
 */
int
epon_env_msg_send(void *msgp, int msgsz, int prio);

/* iROS sends a frame on the management ethernet port.
   the ethernet frame is pre-formatted with DA, SA, ethertype. */
void
epon_env_ether_send(char *frame, int len);

/* iROS sends a radius message to the radius authenticatio server. */
void
epon_env_radius_send(char *msg, int len);

/* memory allocation.
   used by API functions or iROS core.
 */
void *
epon_env_malloc(int size);

/* memory free.
   used by iROS core to free non-null pointers in a message.
 */
void
epon_env_free(void *ptr);

/* memory copy */
void
epon_env_memcpy(void *dst, void *src, int size);

/* memory compare */
int
epon_env_memcmp(void *m1, void *m2, int size);

/* memory set */
void
epon_env_memset(void *m, int value, int size);

unsigned long long
htonll(unsigned long long n);

unsigned long long
ntohll(unsigned long long n);

#if !defined(epon_env_log)
/* #define epon_env_log(level,modid,args...) printf(args) */
#define epon_env_log(level, modid, args...) do { \
            extern char iros_log_mod_tab[32]; \
            if (level >= iros_log_mod_tab[modid]) { \
                printf(args); \
            }; \
        } while (0)
#endif

#if !defined(epon_env_error)
#define epon_env_error(args...) do { \
                printf(args); \
        } while (0)
#endif

/* ===> function calls from the environment to iROS <=== */


/******************************************************
 * the following functions are all non re-entrant and *
 * should run to completion                           *
 ******************************************************/

/* deliver a message to the iROS core. 
   the message is from API functions or ethernet
 */
void
epon_iros_msg_proc(void *msg);

/* provide a time tick to the iROS core */
void
epon_iros_tick();

#endif
