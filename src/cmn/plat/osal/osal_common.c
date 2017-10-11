#include "cs_types.h"
#include "osal_common.h"
#include "cs_timer.h"

extern cs_int32 sys_mempool_num;
extern int isdigit(int __c);

void cs_sys_capability_show()
{
    cs_printf("\r\n============================================");
    cs_printf("\r\n%-12s %-15s %-15s", "Type", "Max-number", "Current-number");
    cs_printf("\r\n--------------------------------------------");
    cs_printf("\r\n%-12s %-15d %-15d", "Thread", CS_OSAL_MAX_THREAD, cs_thread_number());
    cs_printf("\r\n%-12s %-15d %-15d", "Semaphore", CS_OSAL_MAX_COUNT_SEM, cs_semaphore_number());
    cs_printf("\r\n%-12s %-15d %-15d", "Mutex", CS_OSAL_MAX_MUTEX, cs_mutex_number());
    cs_printf("\r\n%-12s %-15d %-15d", "Queue", CS_OSAL_MAX_QUEUE, cs_queue_number());
    cs_printf("\r\n%-12s %-15d %-15d", "Timer", CS_MAX_TIMER_NUMBER, cs_count_timer());
    cs_printf("\r\n%-12s %-15d %-15d", "Mempool", sys_mempool_num, cs_pool_number());
    cs_printf("\r\n============================================\r\n");
}

cs_int8 bitMask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
void cs_bit_set(cs_int8 *pbuf , cs_int32 bit_pos)
{
    pbuf[bit_pos/8] |= bitMask[bit_pos%8];
}

void cs_bit_clr(cs_int8 *pbuf , cs_int32 bit_pos)
{
    pbuf[bit_pos/8] &= ~(bitMask[bit_pos%8]);
}

cs_int32 cs_bit_tst(cs_int8 *pbuf , cs_int32 bit_pos)
{
    if (pbuf[bit_pos/8] & bitMask[bit_pos%8])
        return 1;
    else
        return 0;
}

cs_int32 cs_bit_get_0(cs_int8 *pbuf , cs_int32 total_bit)
{
    cs_int32 intNum = 0;
    cs_int32 left_bits = 0;
    cs_int32 *pInt = (cs_int32 *)pbuf;
    int i , j;

    intNum = total_bit / 32;
    left_bits = total_bit % 32;
    for (i = 0 ; i < intNum ; i++) {
        if (pInt[i] == 0xFFFFFFFF)
            continue;

        for (j = 0 ; j < 32 ; j++) {
            if (!cs_bit_tst((cs_int8 *)&pInt[i] , j))
                return (i*32 + j);
        }
    }

    for (j = 0 ; j < left_bits ; j++) {
        if (!cs_bit_tst((cs_int8 *)&pInt[intNum] , j))
            return (intNum*32 + j);
    }

    return -1;
}

cs_int32 cs_bit_get_1(cs_int8 *pbuf , cs_int32 total_bit)
{
    cs_int32 intNum = 0;
    cs_int32 left_bits = 0;
    cs_int32 *pInt = (cs_int32 *)pbuf;
    int i , j;

    intNum = total_bit / 32;
    left_bits = total_bit % 32;
    for (i = 0 ; i < intNum ; i++) {
        if (pInt[i] == 0)
            continue;

        for (j = 0 ; j < 32 ; j++) {
            if (cs_bit_tst((cs_int8 *)&pInt[i] , j))
                return (i*32 + j);
        }
    }

    for (j = 0 ; j < left_bits ; j++) {
        if (cs_bit_tst((cs_int8 *)&pInt[intNum] , j))
            return (intNum*32 + j);
    }

    return -1;
}

cs_int32 cs_bit_tst_all(cs_int8 *pbuf , cs_int32 total_bit)
{
    if (cs_bit_get_0(pbuf , total_bit) == -1)
        return 1;
    else
        return 0;
}


