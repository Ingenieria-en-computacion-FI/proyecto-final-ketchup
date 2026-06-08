#include <stdlib.h>
#include "../../include/memory_manager.h"
#include "../../include/algorithms.h"

void mm_compact(MemoryManager* mm) {
    if (!mm || !mm->head) return;

    // 1. Delegamos el reordenamiento estructural al algoritmo Divide y Vencerás
    algo_divide_conquer_compact(&(mm->head));

    // 2. Lógica del Kernel: Recalcular direcciones físicas (start offsets) secuencialmente
    int current_offset = 0;
    MemoryBlock* current = mm->head;
    
    while (current != NULL) {
        current->start = current_offset;
        current_offset += current->size;
        current = current->next;
    }

    // 3. Unificar todos los bloques libres que ahora quedaron juntos al final
    mm_coalesce(mm);
}
