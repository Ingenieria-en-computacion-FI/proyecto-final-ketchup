#include <stdlib.h>
#include "memory_manager.h"
#include "algorithms.h"

int mm_allocate_best_fit(MemoryManager* mm, int pid, int size) {
    if (!mm) return -1;

    // 1. Delegamos la decisión al algoritmo Greedy: BEST FIT
    MemoryBlock* target_block = algo_greedy_best_fit(mm, size);

    if (target_block != NULL) {
        // 2. Lógica del Kernel: Fragmentar el bloque si sobra espacio
        if (target_block->size > size) {
            MemoryBlock* new_block = malloc(sizeof(MemoryBlock));
            new_block->start = target_block->start + size;
            new_block->size  = target_block->size - size;
            new_block->free  = 1;
            new_block->pid   = -1;

            new_block->next = target_block->next;
            new_block->prev = target_block;
            if (target_block->next) {
                target_block->next->prev = new_block;
            }
            target_block->next = new_block;
            target_block->size = size;
        }

        // 3. Asignación oficial
        target_block->free = 0;
        target_block->pid  = pid;
        return target_block->start;
    }
    return -1; // Retorna -1 si no hubo espacio
}
