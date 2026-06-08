#ifndef PROCESS_H
#define PROCESS_H

// Definición de los estados del proceso solicitados por la profesora
typedef enum {
    READY,      // Proceso en la cola de listos esperando asignación de CPU
    RUNNING,    // Proceso ocupando activamente el procesador simulado
    BLOCKED,    // Proceso detenido o pausado por falta de renglones de memoria
    FINISHED    // Proceso que completó sus ráfagas y liberó la memoria
} ProcessState;

// Estructura principal del proceso con sus atributos obligatorios
typedef struct {
    int pid;             // Identificador único y secuencial de la tarea
    int burst_time;      // Tiempo total de CPU requerido para completarse
    int remaining_time;  // Tiempo que le falta al proceso para terminar
    int priority;        // Nivel de prioridad asignado
    int memory_required; // Memoria solicitada en Megabytes (MB)
    ProcessState state;           // Estado actual dentro del ciclo de vida
} Process;

#endif
