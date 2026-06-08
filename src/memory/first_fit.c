#include <stdio.h>
#include <stdlib.h>
#include "../../include/memory_manager.h"

// Inicializa el administrador creando un único renglón con toda la capacidad libre
// Complejidad Temporal: O(1) | Complejidad Espacial: O(1)
MemoryManager* mm_create(int total_memory) {
    // Reservamos memoria para la estructura controladora del administrador
    MemoryManager* mm = malloc(sizeof(MemoryManager));
    if (!mm) {
        return NULL; // Protección en caso de fallo de asignación
    }

    // Reservamos el nodo inicial de la lista doblemente ligada
    MemoryBlock* block = malloc(sizeof(MemoryBlock));
    if (!block) { 
        free(mm); 
        return NULL; 
    }

    block->start = 0;               // El mapa de memoria inicia en la posición 0
    block->size  = total_memory;    // Al inicio, este bloque abarca todo el tamaño
    block->free  = 1;               // El bloque inicial nace completamente libre
    block->pid   = -1;              // No pertenece a ningún proceso
    block->next  = NULL;            // Es el único nodo, no hay elemento frontal
    block->prev  = NULL;            // Es el único nodo, no hay elemento trasero

    mm->head         = block;       // La cabeza de la memoria apunta a este bloque libre
    mm->total_memory = total_memory; // Registramos el tamaño total del sistema
    return mm;                      // Retornamos el administrador listo
}

// Estrategia Greedy (First Fit): Busca el primer renglón libre que sea suficiente
// Complejidad Temporal: O(n) | Complejidad Espacial: O(1)
// Recurrencia de recorrido lineal: T(n) = T(n-1) + O(1) -> Solución por sustitución: O(n)
int mm_allocate_first_fit(MemoryManager* mm, int pid, int size) {
    if (!mm) {
        return -1;
    }
    MemoryBlock* current = mm->head; // Comenzamos la búsqueda desde el primer nodo

    // Recorrido lineal sobre la lista doblemente ligada (O(n))
    while (current != NULL) {
        // Criterio Voraz: Evaluamos si el bloque está libre y tiene tamaño adecuado
        if (current->free && current->size >= size) {
            
            // Si el bloque es más grande, lo fragmentamos para salvar el sobrante libre
            if (current->size > size) {
                // Creamos un nuevo nodo para el residuo de memoria libre
                MemoryBlock* new_block = malloc(sizeof(MemoryBlock));
                if (!new_block) {
                    return -1;
                }

                // Calculamos la dirección del nuevo bloque usando un desplazamiento
                new_block->start = current->start + size;
                // El tamaño restante es la resta de la capacidad original menos lo pedido
                new_block->size  = current->size - size;
                new_block->free  = 1;  // El bloque sobrante queda marcado como libre
                new_block->pid   = -1; // Sin proceso asignado

                // Reajustamos los punteros para insertar el nuevo bloque adelante de current
                new_block->next = current->next;
                new_block->prev = current;

                if (current->next != NULL) {
                    current->next->prev = new_block; // Enlazamos de regreso el nodo posterior
                }
                current->next = new_block; // El bloque actual apunta ahora al sobrante libre
                current->size = size;      // Ajustamos el tamaño del bloque actual a la medida exacta
            }

            // Marcamos el renglón actual como ocupado por el proceso solicitante
            current->free = 0;
            current->pid  = pid;
            return current->start; // Retorna la dirección física inicial de la asignación
        }
        current = current->next; // Avanzamos al siguiente nodo de la lista
    }
    return -1; // Retorna -1 si no se encontró ningún renglón continuo disponible
}

// Busca el bloque ocupado por el PID indicado para regresarlo al estado libre
// Complejidad Temporal: O(n) | Complejidad Espacial: O(1)
void mm_free(MemoryManager* mm, int pid) {
    if (!mm) {
        return;
    }
    MemoryBlock* current = mm->head; // Iniciamos la búsqueda desde la cabeza de la lista

    // Recorremos los bloques para identificar al dueño
    while (current != NULL) {
        if (!current->free && current->pid == pid) {
            current->free = 1;  // Reestablecemos el estado a libre
            current->pid  = -1; // Limpiamos el ID del proceso
            break;              // Salimos del bucle ya que los PIDs son únicos
        }
        current = current->next; // Continuamos la búsqueda lineal
    }
}

// Imprime el mapa completo de renglones de la memoria principal
void mm_print(MemoryManager* mm) {
    if (!mm) {
        return;
    }
    MemoryBlock* b = mm->head; // Apuntamos al inicio de la memoria
    printf("Mapa de Memoria [Capacidad Total=%d MB]:\n", mm->total_memory);
    while (b) {
        if (b->free) {
            printf("  [Renglon LIBRE ] Inicio fisica=%-4d Tamano=%-4d MB\n", b->start, b->size);
        } else {
            printf("  [Renglon PID=%-2d] Inicio fisica=%-4d Tamano=%-4d MB\n", b->pid, b->start, b->size);
        }
        b = b->next; // Avanzamos al bloque contiguo
    }
}

// Libera secuencialmente la memoria dinámica para evitar fugas (Memory Leaks)
void mm_destroy(MemoryManager* mm) {
    if (!mm) {
        return;
    }
    MemoryBlock* current = mm->head;
    while (current) {
        MemoryBlock* tmp = current->next; // Guardamos el enlace al siguiente antes de borrar
        free(current);                    // Liberamos el nodo actual
        current = tmp;                    // Pasamos al temporal guardado
    }
    free(mm); // Liberamos el contenedor externo del administrador
}
