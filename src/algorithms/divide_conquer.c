#include <stddef.h>
#include "../../include/algorithms.h"

// Función auxiliar para separar nodos ocupados a la izquierda y libres a la derecha
static void partition_and_merge(MemoryBlock** head_ref) {
    if (*head_ref == NULL || (*head_ref)->next == NULL) return;

    MemoryBlock *occupied_head = NULL, *occupied_tail = NULL;
    MemoryBlock *free_head = NULL, *free_tail = NULL;
    MemoryBlock *curr = *head_ref;

    // Partición O(n)
    while (curr != NULL) {
        MemoryBlock* next_node = curr->next;
        curr->next = NULL;
        curr->prev = NULL;

        if (!curr->free) {
            if (!occupied_head) { occupied_head = curr; occupied_tail = curr; }
            else { occupied_tail->next = curr; curr->prev = occupied_tail; occupied_tail = curr; }
        } else {
            if (!free_head) { free_head = curr; free_tail = curr; }
            else { free_tail->next = curr; curr->prev = free_tail; free_tail = curr; }
        }
        curr = next_node;
    }

    // Fusión (Merge)
    if (occupied_head) {
        *head_ref = occupied_head;
        if (free_head) {
            occupied_tail->next = free_head;
            free_head->prev = occupied_tail;
        }
    } else {
        *head_ref = free_head;
    }
}

void algo_divide_conquer_compact(MemoryBlock** head_ref) {
    // La estrategia Divide y Vencerás aplica la partición para reordenar
    partition_and_merge(head_ref);
}
