#include <stdio.h>
#include "../../include/algorithms.h"

// Retorna 1 si logra acomodar el lote completo, 0 si falla y revierte el estado
int algo_backtracking_batch_alloc(MemoryManager* mm, Process* proc_array, int total_procs, int current_idx) {
    // Caso base: todos los procesos han sido asignados exitosamente
    if (current_idx == total_procs) return 1;

    Process* p = &proc_array[current_idx];
    if (p->state != READY) {
        // Si no está listo, saltamos al siguiente proceso
        return algo_backtracking_batch_alloc(mm, proc_array, total_procs, current_idx + 1);
    }

    MemoryBlock* curr = mm->head;
    while (curr != NULL) {
        if (curr->free && curr->size >= p->memory_required) {
            
            // --- GUARDADO DE ESTADO (Simulación de asignación) ---
            curr->free = 0;
            curr->pid = p->pid;
            int original_size = curr->size;
            curr->size = p->memory_required; // Reducción lógica, no creamos nodo nuevo para evitar fugas en recursión
            
            // --- RECURSIÓN ---
            if (algo_backtracking_batch_alloc(mm, proc_array, total_procs, current_idx + 1)) {
                return 1; // Camino válido encontrado
            }
            
            // --- RECUPERACIÓN DE ESTADO (Backtrack) ---
            curr->free = 1;
            curr->pid = -1;
            curr->size = original_size;
        }
        curr = curr->next;
    }
    
    return 0; // No hay ruta válida desde esta configuración
}
