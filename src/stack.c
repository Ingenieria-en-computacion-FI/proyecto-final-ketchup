#include <stdio.h>
#include <stdlib.h>
#include "../../include/stack.h"

// Nodo dinámico lineal interno de la pila
struct Node {
    int value; // Almacena el PID concluido
    struct Node* next;  // Enlace apuntando al nodo inferior de la pila
};

// Tipo abstracto de datos que encapsula el tope de la Pila (Estructura LIFO)
struct Stack {
    struct Node* top;  // Puntero directo al nodo superior (Corona de la pila)
    int size; // Cantidad de elementos apilados
};

// Inicializa una estructura de pila vacía
Stack* stack_create() {
    Stack* stack = malloc(sizeof(Stack));
    if (!stack) {
        return NULL;
    }
    stack->top  = NULL; // Al nacer, el tope apunta a nulo
    stack->size = 0;    // Pila con cero elementos
    return stack;
}

// Operación Push: Inserta un elemento directamente sobre el tope de la pila
// Complejidad Temporal: O(1) | Complejidad Espacial: O(1)
void stack_push(Stack* stack, int value) {
    if (!stack) {
        return;
    }
    struct Node* node = malloc(sizeof(struct Node)); // Reservamos memoria para el nuevo eslabón
    if (!node) {
        return;
    }
    node->value = value;
    node->next  = stack->top; // El nuevo nodo se apoya sobre el antiguo tope de la pila
    stack->top  = node;       // Actualizamos la corona de la pila apuntando al nuevo integrante
    stack->size++;            // Incrementamos el contador
}

// Operación Pop: Retira y devuelve el elemento del tope
// Complejidad Temporal: O(1) | Complejidad Espacial: O(1)
int stack_pop(Stack* stack) {
    if (!stack || !stack->top) {
        return -1; // Retorna -1 si la pila está totalmente vacía
    }
    struct Node* tmp = stack->top;         // Guardamos la dirección del tope actual
    int          value = tmp->value;       // Rescatamos el valor numérico
    stack->top   = tmp->next;              // El tope desciende un peldaño al nodo inferior
    free(tmp);                             // Liberamos el nodo retirado de la memoria RAM
    stack->size--;                         // Decrementamos el contador de elementos
    return value;                          // Retornamos el valor extraído
}

int stack_is_empty(Stack* stack) {
    return (stack == NULL || stack->top == NULL);
}

// Vacía y elimina la estructura completa de la pila
void stack_destroy(Stack* stack) {
    if (!stack) {
        return;
    }
    while (!stack_is_empty(stack)) {
        stack_pop(stack); // Vacía los elementos remanentes secuencialmente
    }
    free(stack); // Libera el puntero base del contenedor
}
