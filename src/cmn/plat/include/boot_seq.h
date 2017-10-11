#ifndef _BOOT_SEQ_H_
#define _BOOT_SEQ_H_

#define CS_MAX_INIT_SEQ_PRI 32
#define CS_MAX_DEP_PRI      8
typedef struct _cs_init_seq_t
{
    int init_flag;
    int dep_pri[CS_MAX_DEP_PRI];
    int registered;
}cs_init_seq_t;

extern void cs_init_seq_init();
extern void cs_reg_init_seq(int pri, int dep_pri);
extern void cs_init_seq_done(int pri);
extern void cs_init_seq_wait(int pri);

#endif

