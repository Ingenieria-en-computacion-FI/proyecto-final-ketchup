#include <stddef.h>
#include "../../include/algorithms.h"

MemoryBlock* algo_incremental_next_fit(MemoryManager* mm, int size, MemoryBlock** last_allocated) {
    if (!mm || !mm->head) return NULL;
    
    MemoryBlock* start_node = (*last_allocated != NULL) ? *last_allocated : mm->head;
    MemoryBlock* current = start_node;

    // Primer barrido desde el punto incremental
    while (current != NULL) {
        if (current->free && current->size >= size) {
            *last_allocated = current;
            return current;
        }
        current = current->next;
    }

    // Si llega al final, hace un barrido circular desde el inicio hasta el punto incremental
    current = mm->head;
    while (current != start_node) {
         if (current->free && current->size >= size) {
            *last_allocated = current;
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}
