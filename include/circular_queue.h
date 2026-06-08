#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

// Declaración incompleta del tipo abstracto para la cola circular
typedef struct CircularQueue CircularQueue;

// Operaciones de la cola indexada modularmente
CircularQueue* cq_create(int capacity);
void cq_enqueue(CircularQueue* q, int value);
int cq_dequeue(CircularQueue* q);
int cq_is_empty(CircularQueue* q);
int cq_is_full(CircularQueue* q);
void cq_destroy(CircularQueue* q);

#endif