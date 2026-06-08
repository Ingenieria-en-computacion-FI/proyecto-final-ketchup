#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Estructura interna para los nodos de la cola dinámica
struct QNode {
    int value; // Almacena el ID del proceso cargado en memoria
    struct QNode* next;  // Enlace al siguiente nodo de la cola
};

// Contenedor que encapsula el frente y el final de la Cola Simple
struct Queue {
    struct QNode* head; // Apunta al primer elemento (Frente - donde se hace Dequeue)
    struct QNode* tail; // Apunta al último elemento (Final - donde se hace Enqueue)
    int size; // Contador total de elementos en espera
};

// Crea e inicializa una cola dinámica vacía
Queue* queue_create() {
    Queue* q = malloc(sizeof(Queue));
    if (!q) {
        return NULL;
    }
    q->head = NULL; // Frente inicializado en nulo
    q->tail = NULL; // Final inicializado en nulo
    q->size = 0;    // Cola vacía
    return q;
}

// Operación Enqueue: Inserta un elemento al final de la cola
// Complejidad Temporal: O(1) | Complejidad Espacial: O(1)
void enqueue(Queue* queue, int value) {
    if (!queue) {
        return;
    }
    struct QNode* node = malloc(sizeof(struct QNode)); // Reservamos memoria para el nuevo nodo
    if (!node) {
        return;
    }
    node->value = value;
    node->next  = NULL; // Al insertarse al final, su enlace frontal apunta a nulo

    if (queue->tail) {
        queue->tail->next = node; // El último nodo actual apunta al nuevo integrante
    } else {
        queue->head = node;       // Si la cola estaba vacía, el frente pasa a ser el nuevo nodo
    }
    queue->tail = node; // Actualizamos el indicador de fin de la cola
    queue->size++;      // Incrementamos el contador
}

// Operación Dequeue: Remueve y retorna el elemento del frente de la cola
// Complejidad Temporal: O(1) | Complejidad Espacial: O(1)
int dequeue(Queue* queue) {
    if (!queue || !queue->head) {
        return -1; // Retorna -1 si la cola está vacía
    }
    struct QNode* tmp = queue->head;       // Guardamos la dirección del frente actual
    int value = tmp->value;       // Rescatamos el dato del proceso
    
    queue->head = tmp->next; // El frente se desplaza hacia el elemento de atrás
    if (!queue->head) {
        queue->tail = NULL; // Si el frente quedó vacío, el final también se limpia
    }
    
    free(tmp);      // Liberamos el nodo removido
    queue->size--;  // Decrementamos el contador
    return value;   // Retornamos el valor extraído
}

// Evalúa si la cola carece de elementos
int queue_is_empty(Queue* queue) {
    return (queue == NULL || queue->head == NULL);
}

// Vacía y destruye la cola para limpiar la memoria dinámica
// Complejidad Temporal: O(n) | Complejidad Espacial: O(1)
void queue_destroy(Queue* queue) {
    if (!queue) {
        return;
    }
    while (!queue_is_empty(queue)) {
        dequeue(queue); // Remueve los nodos remanentes uno por uno
    }
    free(queue); // Elimina el contenedor base
}
