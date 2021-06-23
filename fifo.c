/* FIFO */

#include "bootpack.h"

#define FLAGS_OVERRUN 0x0001

void fifo32_init(struct FIFO32* fifo, int size, int* buf, struct TASK* task)
{
    /* FIFO初始化 */
    fifo->size = size;
    fifo->buf = buf;
    fifo->free = size; /* 空 */
    fifo->flags = 0;
    fifo->p = 0; /* 写入位置 */
    fifo->q = 0; /* 读取位置 */
    fifo->task = task;
    return;
}

int fifo32_put(struct FIFO32* fifo, int data)
{
    /* FIFO存 */
    if (fifo->free == 0) {
        /* 满了 */
        fifo->flags |= FLAGS_OVERRUN;
        return -1;
    }
    fifo->buf[fifo->p] = data;
    fifo->p++;
    if (fifo->p == fifo->size) {
        fifo->p = 0;
    }
    fifo->free--;
    if (fifo->task != 0) {
        if (fifo->task->flags != 2) { /* �^�X�N���Q�Ă����� */
            task_run(fifo->task, -1, 0); /* �N�����Ă����� */
        }
    }
    return 0;
}

int fifo32_get(struct FIFO32* fifo)
{
    /* FIFO取 */
    int data;
    if (fifo->free == fifo->size) {
        /* 空了 */
        return -1;
    }
    data = fifo->buf[fifo->q];
    fifo->q++;
    if (fifo->q == fifo->size) {
        fifo->q = 0;
    }
    fifo->free++;
    return data;
}

int fifo32_status(struct FIFO32* fifo)
{
    /* 存了多少数据 */
    return fifo->size - fifo->free;
}
