#include <stdio.h>
#include <stdlib.h>
#include "circular_queue.h"

// Estructura de la cola circular
struct CircularQueue {
    int* data;     // Arreglo dinámico de enteros (PIDs)
    int  capacity; // Capacidad máxima del arreglo antes de expandirse
    int  head;     // Índice del elemento frontal (Frente de extracción)
    int  tail;     // Índice de inserción (Final de la estructura)
    int  size;     // Cantidad actual de elementos almacenados
};

// Instancia la cola circular con la capacidad base seleccionada
CircularQueue* cq_create(int capacity) {
    CircularQueue* q = malloc(sizeof(CircularQueue));
    if (!q) {
        return NULL;
    }
    q->data = malloc(sizeof(int) * capacity); // Reservamos el espacio contiguo en memoria
    if (!q->data) { 
        free(q); 
        return NULL; 
    }
    q->capacity = capacity;
    q->head     = 0; // El frente inicia en la posición cero
    q->tail     = 0; // El final inicia en la posición cero
    q->size     = 0; // Inicialmente vacía
    return q;
}

int cq_is_empty(CircularQueue* q) {
    return (q == NULL || q->size == 0);
}

int cq_is_full(CircularQueue* q) {
    return (q != NULL && q->size == q->capacity);
}

// Operación Enqueue: Inserta un PID usando aritmética modular. Crece al doble si se llena.
// Complejidad Temporal: O(1) caso promedio / O(n) en caso extremo de reajuste por expansión
void cq_enqueue(CircularQueue* q, int value) {
    if (!q) {
        return;
    }
    
    // Si la estructura se llena por completo, aplicamos expansión dinámica para evitar desbordes
    if (cq_is_full(q)) {
        int  new_cap  = q->capacity * 2;            // Duplicamos el límite de almacenamiento
        int* new_data = malloc(sizeof(int) * new_cap); // Reservamos el nuevo bloque de memoria
        if (!new_data) {
            return;
        }

        // Copiamos los elementos ordenadamente eliminando el ciclo circular temporalmente
        for (int i = 0; i < q->size; i++) {
            new_data[i] = q->data[(q->head + i) % q->capacity]; // Mapeo lineal desfragmentado
        }
        free(q->data);      // Liberamos el arreglo pequeño anterior
        q->data     = new_data; // Reasignamos el puntero al nuevo bloque espacioso
        q->head     = 0;       // El frente se reinicia en el índice cero
        q->tail     = q->size; // El final apunta a la posición inmediata vacía
        q->capacity = new_cap;  // Guardamos la nueva capacidad del sistema
    }
    
    q->data[q->tail] = value;                 // Guardamos el valor en la posición del final
    q->tail = (q->tail + 1) % q->capacity;    // Avance circular mediante aritmética modular
    q->size++;                                // Incrementamos la cantidad de elementos
}

// Operación Dequeue: Extrae el elemento frontal mediante aritmética modular
// Complejidad Temporal: O(1) | Complejidad Espacial: O(1)
int cq_dequeue(CircularQueue* q) {
    if (!q || cq_is_empty(q)) {
        return -1;
    }
    int value = q->data[q->head];          // Rescatamos el valor del frente
    q->head = (q->head + 1) % q->capacity; // Desplazamos el frente usando aritmética modular
    q->size--;                             // Reducimos el contador de elementos
    return value;                          // Retornamos el valor extraído
}

// Libera de la memoria RAM todas las estructuras del arreglo circular
void cq_destroy(CircularQueue* q) {
    if (!q) {
        return;
    }
    free(q->data); // Elimina el arreglo de datos interno
    free(q);       // Elimina la estructura envolvente externa
}
