#include <stdlib.h>
#include "memory_manager.h"

// Coalescencia: Une bloques libres adyacentes de forma consecutiva
// Complejidad Temporal: O(n) | Complejidad Espacial: O(1)
// Recurrencia: T(n) = T(n-1) + O(1) -> Demostración matemática lineal por método de sustitución
void mm_coalesce(MemoryManager* mm) {
    if (!mm || !mm->head) {
        return;
    }

    MemoryBlock* current = mm->head; // Empezamos a evaluar desde la cabeza de la lista

    // Recorremos la lista evaluando parejas adyacentes de nodos (O(n))
    while (current != NULL && current->next != NULL) {
        // Si el renglón actual y el nodo de adelante están vacíos, se unifican de inmediato
        if (current->free && current->next->free) {
            
            MemoryBlock* to_merge = current->next; // Apuntamos temporalmente al nodo excedente

            current->size += to_merge->size; // El bloque actual absorbe la capacidad de su vecino
            current->next  = to_merge->next;  // Saltamos el nodo absorbido apuntando al siguiente de adelante

            // Gracias a la lista doblemente ligada, reconectamos hacia atrás en O(1)
            if (to_merge->next != NULL) {
                to_merge->next->prev = current; 
            }

            free(to_merge); // Eliminamos el nodo duplicado de la memoria del sistema
            // Nota: No se avanza 'current' porque el nuevo vecino frontal podría estar libre también
        } else {
            current = current->next; // Si no hay fusión posible, avanzamos de forma ordinaria
        }
    }
}
