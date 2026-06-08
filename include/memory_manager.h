#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

// Nodo elemental para el manejo de la memoria mediante una Lista Doblemente Ligada
typedef struct MemoryBlock {
    int start; // Dirección física inicial del bloque en el mapa
    int size;  // Tamaño del renglón o bloque en MB
    int free;  // Bandera booleana: 1 si está libre, 0 si está asignado
    int pid;   // PID del proceso dueño del renglón (-1 si está libre)
    struct MemoryBlock* next;  // Enlace apuntando al bloque de memoria frontal
    struct MemoryBlock* prev;  // Enlace apuntando al bloque de memoria trasero
} MemoryBlock;

// Estructura contenedora global del espacio de memoria
typedef struct {
    MemoryBlock* head;         // Puntero al primer renglón de la memoria
    int total_memory; // Capacidad máxima total asignada al simulador
} MemoryManager;

// Prototipos de las funciones globales para el manejo de memoria
MemoryManager* mm_create(int total_memory);
int mm_allocate_first_fit(MemoryManager* mm, int pid, int size);
void mm_free(MemoryManager* mm, int pid);
void mm_coalesce(MemoryManager* mm);
void mm_destroy(MemoryManager* mm);
void mm_print(MemoryManager* mm);

int mm_allocate_best_fit(MemoryManager* mm, int pid, int size);
int mm_allocate_worst_fit(MemoryManager* mm, int pid, int size);
void mm_compact(MemoryManager* mm);
#endif
