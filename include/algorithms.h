#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "memory_manager.h"
#include "process.h"

// --- Fuerza Bruta ---
MemoryBlock* algo_brute_force_find(MemoryManager* mm, int size);

// --- Algoritmos Greedy (Voraces) ---
MemoryBlock* algo_greedy_best_fit(MemoryManager* mm, int size);
MemoryBlock* algo_greedy_worst_fit(MemoryManager* mm, int size);

// --- Búsqueda Incremental ---
MemoryBlock* algo_incremental_next_fit(MemoryManager* mm, int size, MemoryBlock** last_allocated);

// --- Backtracking (Recuperación de Estados) ---
int algo_backtracking_batch_alloc(MemoryManager* mm, Process* proc_array, int total_procs, int current_idx);

// --- Divide y Vencerás ---
void algo_divide_conquer_compact(MemoryBlock** head_ref);

#endif
