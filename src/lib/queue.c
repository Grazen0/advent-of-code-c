#include "lib/queue.h"
#include "lib/macros.h"
#include "lib/vec.h"
#include <stddef.h>
#include <string.h>

static constexpr size_t QUEUE_INIT_CAPACITY = 32;

QueueInternal __queue_new(void)
{
    return (QueueInternal){
        .data = __vec_new(),
        .head = 0,
        .tail = 0,
    };
}

void __queue_destroy(QueueInternal *const queue)
{
    __vec_destroy(&queue->data);
    queue->head = 0;
    queue->tail = 0;
}

size_t __queue_size(const QueueInternal *const queue)
{
    const size_t data_size = __vec_size(&queue->data);
    return (queue->tail + data_size - queue->head) % data_size;
}

bool __queue_is_empty(const QueueInternal *queue)
{
    return queue->head == queue->tail;
}

void __queue_enqueue(QueueInternal *const queue, const void *const data, const size_t item_size)
{
    if (__vec_is_empty(&queue->data))
        __vec_resize(&queue->data, QUEUE_INIT_CAPACITY, item_size);

    size_t new_tail = (queue->tail + 1) % __vec_size(&queue->data);

    if (new_tail == queue->head) {
        const size_t prev_size = __queue_size(queue);
        const size_t new_size = 2 * prev_size;

        VecInternal new_data = __vec_new();
        __vec_resize(&new_data, new_size, item_size);

        for (size_t i = 0; i < prev_size; ++i) {
            const size_t prev_idx = (queue->head + i) % queue->data.size;
            memcpy(__vec_get(&new_data, i, item_size), __vec_get(&queue->data, prev_idx, item_size),
                   item_size);
        }

        __vec_destroy(&queue->data);
        queue->data = new_data;
        queue->head = 0;
        queue->tail = prev_size;
        new_tail = (queue->tail + 1) % new_size;
    }

    memcpy(__vec_get(&queue->data, queue->tail, item_size), data, item_size);
    queue->tail = new_tail;
}

void __queue_dequeue(QueueInternal *const queue, void *const out, const size_t item_size)
{
    PANIC_IF(__queue_is_empty(queue), "cannot dequeue from empty queue");

    memcpy(out, __vec_get(&queue->data, queue->head, item_size), item_size);
    queue->head = (queue->head + 1) % __vec_size(&queue->data);
}

void *__queue_peek(QueueInternal *const queue, const size_t item_size)
{
    PANIC_IF(__queue_is_empty(queue), "cannot peek an empty queue");
    return __vec_get(&queue->data, queue->head, item_size);
}

void __queue_clear(QueueInternal *const queue)
{
    __vec_clear(&queue->data);
    queue->head = 0;
    queue->tail = 0;
}
