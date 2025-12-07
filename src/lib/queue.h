#ifndef AOC_LIB_QUEUE_H
#define AOC_LIB_QUEUE_H

#include "lib/vec.h"
#include <stddef.h>

#define Queue(type)             \
    union {                     \
        QueueInternal internal; \
        type *payload;          \
    }

#define queue_new() {.internal = __queue_new()}

#define queue_destroy(queue) __queue_destroy(&(queue)->internal)

#define queue_size(queue) __queue_size(&(queue)->internal)

#define queue_is_empty(queue) __queue_is_empty(&(queue)->internal)

#define queue_enqueue(queue, data) \
    __queue_enqueue(&(queue)->internal, true ? data : (queue)->payload, sizeof(*(queue)->payload))

#define queue_dequeue(queue, out) \
    __queue_dequeue(&(queue)->internal, true ? out : (queue)->payload, sizeof(*(queue)->payload))

#define queue_peek(queue) \
    ((typeof((queue)->payload))__queue_peek(&(queue)->internal, sizeof(*(queue)->payload)))

#define queue_clear(queue) __queue_clear(&(queue)->internal)

typedef struct {
    VecInternal data;
    size_t head;
    size_t tail;
} QueueInternal;

QueueInternal __queue_new(void);

void __queue_destroy(QueueInternal *queue);

size_t __queue_size(const QueueInternal *queue);

bool __queue_is_empty(const QueueInternal *queue);

void __queue_enqueue(QueueInternal *queue, const void *data, size_t item_size);

void __queue_dequeue(QueueInternal *queue, void *out, size_t item_size);

void *__queue_peek(QueueInternal *queue, size_t item_size);

void __queue_clear(QueueInternal *queue);

#endif
