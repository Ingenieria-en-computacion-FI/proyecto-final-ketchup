#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "memory_manager.h"
#include "scheduler.h"
#include "stack.h"
#include "algorithms.h"

// Prototipos de utilidades (src/utils/) para garantizar una compilación limpia
void log_event(const char* format, ...);
int parse_processes(const char* filename, Process* procesos_array, int max_capacity);

// Declaraciones de las funciones avanzadas de memoria si no se han agregado a memory_manager.h
int mm_allocate_best_fit(MemoryManager* mm, int pid, int size);
int mm_allocate_worst_fit(MemoryManager* mm, int pid, int size);
void mm_compact(MemoryManager* mm);

int main() {
    log_event("==================================================\n");
    log_event("   UNAM - SIMULADOR DE SISTEMA OPERATIVO (2026-2) \n");
    log_event("==================================================\n\n");

    // Inicialización del administrador de memoria central y la pila de auditoría
    MemoryManager* memoria = mm_create(1024);
    Stack* historial_auditoria = stack_create();
    
    if (!memoria || !historial_auditoria) {
        log_event("ERROR: Fallo crítico al inicializar las estructuras base del Kernel.\n");
        return 1;
    }

    // -------------------------------------------------------------------------
    // FASE 1: Lectura Modularizada de Procesos (Parser Utility)
    // -------------------------------------------------------------------------
    Process procesos_carga[256];
    // Invoca al parser diseñado para archivos crudos sin cabeceras tradicionales
    int total_procesos = parse_processes("Procesos.csv", procesos_carga, 256);
    
    if (total_procesos == -1) {
        log_event("ERROR: No se encontró 'Procesos.csv'. Ejecuta el generador de Python primero.\n");
        mm_destroy(memoria);
        stack_destroy(historial_auditoria);
        return 1;
    }
    log_event("[Info] Parser cargó exitosamente %d procesos desde el CSV.\n\n", total_procesos);

    // -------------------------------------------------------------------------
    // FASE 2: Pre-análisis y Búsqueda Exhaustiva (Backtracking & Fuerza Bruta)
    // -------------------------------------------------------------------------
    log_event("--- FASE 2: ANÁLISIS DE FACTIBILIDAD Y EXPLORACIÓN EXHAUSTIVA ---\n");
    
    // 1. Simulación previa por Backtracking para verificar si existe un acomodo global válido
    log_event("  [Backtracking] Evaluando espacio de estados para asignación masiva...\n");
    int es_factible = algo_backtracking_batch_alloc(memoria, procesos_carga, total_procesos, 0);
    
    if (es_factible) {
        log_event("  [Backtracking] ¡ÉXITO! Se encontró una distribución teórica válida para el lote.\n");
    } else {
        log_event("  [Backtracking] ADVERTENCIA: El lote completo no cabe de forma simultánea. Se requerirá compactación dinámica.\n");
    }
    
    // 2. Demostración analítica de Fuerza Bruta para mapear el estado inicial de los huecos
    MemoryBlock* primer_hueco = algo_brute_force_find(memoria, 64);
    if (primer_hueco) {
        log_event("  [Fuerza Bruta] Verificación de bloques libres activa. Primer hueco viable detectado en offset: %d MB\n", primer_hueco->start);
    }
    log_event("\n");

    // -------------------------------------------------------------------------
    // FASE 3: Asignación de Memoria con Políticas Avanzadas y Compactación
    // -------------------------------------------------------------------------
    log_event("--- FASE 3: ASIGNACIÓN DE MEMORIA PRINCIPAL (POLÍTICA: BEST FIT) ---\n");
    
    for (int i = 0; i < total_procesos; i++) {
        // Se opta por Best Fit para reducir la fragmentación externa de manera voraz
        int dir = mm_allocate_best_fit(memoria, procesos_carga[i].pid, procesos_carga[i].memory_required);
        
        // Si falla por fragmentación, aplicamos el enfoque de Divide y Vencerás para reorganizar la RAM
        if (dir == -1) {
            log_event("  [Memoria] RAM fragmentada para PID=%d (%d MB). Iniciando compactación D&C...\n", 
                      procesos_carga[i].pid, procesos_carga[i].memory_required);
            
            mm_compact(memoria); // Reordena nodos con D&C y recalcula offsets físicos
            
            // Reintento post-compactación
            dir = mm_allocate_best_fit(memoria, procesos_carga[i].pid, procesos_carga[i].memory_required);
        }
        
        if (dir != -1) {
            log_event("  [Memoria] PID=%d asignado de forma óptima en la dirección física: %d MB\n", 
                      procesos_carga[i].pid, dir);
        } else {
            log_event("  [Memoria] RECHAZADO: PID=%d no puede ser cargado ni tras compactación. Estado: BLOQUEADO\n", 
                      procesos_carga[i].pid);
            procesos_carga[i].state = BLOCKED;
        }
    }
    log_event("\n");
    mm_print(memoria); // Imprime el mapa de memoria inicial consolidado
    log_event("\n");

    // -------------------------------------------------------------------------
    // FASE 4: Planificación de CPU y Exportación de Resultados
    // -------------------------------------------------------------------------
    log_event("--- FASE 4: PLANIFICACIÓN DE CPU (ROUND ROBIN INTERRUPTIBLE) ---\n");
    Scheduler* planificador = scheduler_create_rr(2); // Quantum = 2 unidades de tiempo
    
    FILE* fp_out = fopen("resultados.csv", "w");
    if (!fp_out) {
        log_event("ERROR: No se pudo generar el reporte de métricas 'resultados.csv'\n");
        mm_destroy(memoria);
        scheduler_destroy(planificador);
        stack_destroy(historial_auditoria);
        return 1;
    }
    fprintf(fp_out, "PID,Burst_Original,Memoria_Usada,Estado_Final\n");

    // Carga de procesos aprobados al planificador de ráfagas
    for (int i = 0; i < total_procesos; i++) {
        if (procesos_carga[i].state == READY) {
            scheduler_add_process(planificador, procesos_carga[i].pid, procesos_carga[i].burst_time);
        } else {
            fprintf(fp_out, "%d,%d,%d,BLOQUEADO_MEMORIA\n", 
                    procesos_carga[i].pid, procesos_carga[i].burst_time, procesos_carga[i].memory_required);
        }
    }

    // Despacho de hilos en la CPU simulada
    while (!scheduler_is_done(planificador)) {
        int current_pid = scheduler_next(planificador);
        if (current_pid == -1) continue;

        Process* proc = NULL;
        for (int i = 0; i < total_procesos; i++) {
            if (procesos_carga[i].pid == current_pid) {
                proc = &procesos_carga[i];
                break;
            }
        }

        if (proc != NULL) {
            proc->state = RUNNING;
            int tiempo_ejecutado = (proc->remaining_time < 2) ? proc->remaining_time : 2;
            proc->remaining_time -= tiempo_ejecutado;

            if (proc->remaining_time > 0) {
                proc->state = READY; 
            } else {
                proc->state = FINISHED;
                log_event("  [Sistema] PID=%d concluyó ráfagas. Volcando métricas y liberando RAM...\n", proc->pid);
                
                fprintf(fp_out, "%d,%d,%d,COMPLETADO\n", 
                        proc->pid, proc->burst_time, proc->memory_required);

                // Liberación estructural y fusión contigua de bloques libres (coalescencia)
                mm_free(memoria, proc->pid);
                mm_coalesce(memoria);
                
                // Resguardo de auditoría cronológica inversa en la Pila LIFO
                stack_push(historial_auditoria, proc->pid);
            }
        }
    }
    fclose(fp_out);
    log_event("\n");
    mm_print(memoria); // Mapa final limpio
    log_event("\n");

    // -------------------------------------------------------------------------
    // FASE 5: Auditoría Post-Mortem del Sistema (Historial LIFO)
    // -------------------------------------------------------------------------
    log_event("--- FASE 5: AUDITORÍA POST-MORTEM DEL SISTEMA (HISTORIAL LIFO) ---\n");
    log_event("Orden inverso de finalización registrado en la estructura de auditoría:\n");
    
    while (!stack_is_empty(historial_auditoria)) {
        int pid_concluido = stack_pop(historial_auditoria);
        log_event("  -> [Pila Tope] Registro Recuperado: PID=%d\n", pid_concluido);
    }
    log_event("\n");

    // -------------------------------------------------------------------------
    // FASE 6: Cierre Seguro de Descriptores y Destrucción de Recursos
    // -------------------------------------------------------------------------
    mm_destroy(memoria);
    scheduler_destroy(planificador);
    stack_destroy(historial_auditoria);

    log_event("==================================================\n");
    log_event("   SIMULACIÓN CONCLUIDA CON ÉXITO SIN FUGAS       \n");
    log_event("==================================================\n");
    return 0;
}
