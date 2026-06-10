#ifndef QUEUE_H
#define QUEUE_H

// Declaración incompleta de la estructura para ocultar los detalles de implementación (Encapsulamiento)
typedef struct Queue Queue;

// Funciones de control de la estructura lineal FIFO
Queue* queue_create();
void enqueue(Queue* queue, int value);
int dequeue(Queue* queue);
int queue_is_empty(Queue* queue);
void queue_destroy(Queue* queue);

#endif
