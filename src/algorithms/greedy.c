#include <stddef.h>
#include "../../include/algorithms.h"

// Best Fit: Minimiza la fragmentación externa eligiendo el hueco más ajustado
MemoryBlock* algo_greedy_best_fit(MemoryManager* mm, int size) {
    MemoryBlock* current = mm->head;
    MemoryBlock* best_block = NULL;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (best_block == NULL || current->size < best_block->size) {
                best_block = current;
            }
        }
        current = current->next;
    }
    return best_block;
}

// Worst Fit: Maximiza el residuo dejando el hueco sobrante más grande posible
MemoryBlock* algo_greedy_worst_fit(MemoryManager* mm, int size) {
    MemoryBlock* current = mm->head;
    MemoryBlock* worst_block = NULL;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (worst_block == NULL || current->size > worst_block->size) {
                worst_block = current;
            }
        }
        current = current->next;
    }
    return worst_block;
}
