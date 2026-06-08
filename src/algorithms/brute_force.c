#include <stdio.h>
#include "../../include/algorithms.h"

MemoryBlock* algo_brute_force_find(MemoryManager* mm, int size) {
    if (!mm || !mm->head) return NULL;
    
    MemoryBlock* current = mm->head;
    MemoryBlock* valid_candidate = NULL;

    // Exploración exhaustiva: recorre absolutamente toda la estructura
    while (current != NULL) {
	if(current -> free){
            // Registramos el primer candidato viable encontrado en la exploración
            if (current->size >= size && valid_candidate == NULL) {
                valid_candidate = current;
            }
        }
        current = current->next;
    }
    
    // El algoritmo evaluó todo el espacio antes de retornar
    return valid_candidate;
}
