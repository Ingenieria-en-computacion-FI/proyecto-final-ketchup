#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"
#include "../include/memory_manager.h"
#include "../include/scheduler.h"
#include "../include/stack.h"

// Función principal del simulador de Sistema Operativo
int main() {
    printf("==================================================\n");
    printf("   UNAM - SIMULADOR DE SISTEMA OPERATIVO (2026-2) \n");
    printf("==================================================\n\n");

    // Inicialización de los componentes del Sistema Operativo
    MemoryManager* memoria = mm_create(1024);
    Stack* historial_auditoria = stack_create();

    // -------------------------------------------------------------------------
    // FASE 1: Lectura de Procesos desde Archivo CSV (Generado por Python)
    // -------------------------------------------------------------------------
    Process procesos_carga[256]; // Capacidad ampliada para simulaciones complejas
    int total_procesos = 0;
    
    FILE* fp_in = fopen("Procesos.csv", "r");
    if (!fp_in) {
        printf("ERROR: No se encontro 'procesos.csv'. Ejecuta el generador de Python primero.\n");
        mm_destroy(memoria);
        stack_destroy(historial_auditoria);
        return 1;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), fp_in); // Omitimos la línea de cabecera del CSV
    
    int p_id, p_burst, p_prio, p_mem;
    while(fscanf(fp_in, "%d,%d,%d,%d", &p_id, &p_burst, &p_prio, &p_mem) == 4) {
        procesos_carga[total_procesos] = (Process){p_id, p_burst, p_burst, p_prio, p_mem, READY};
        total_procesos++;
    }
    fclose(fp_in);
    printf("[Info] Se cargaron %d procesos desde el CSV.\n\n", total_procesos);

    // -------------------------------------------------------------------------
    // FASE 2: Asignación Voraz de Memoria (First Fit)
    // -------------------------------------------------------------------------
    printf("--- FASE 2: ASIGNACION VORAZ DE MEMORIA (FIRST FIT) ---\n");
    for (int i = 0; i < total_procesos; i++) {
        int dir = mm_allocate_first_fit(memoria, procesos_carga[i].pid, procesos_carga[i].memory_required);
        
        if (dir != -1) {
            printf("  [Memoria] PID=%d asignado exitosamente en el renglon. Direccion: %d MB\n", 
                   procesos_carga[i].pid, dir);
        } else {
            printf("  [Memoria] ERROR: No hay un renglon disponible para el PID=%d\n", 
                   procesos_carga[i].pid);
            procesos_carga[i].state = BLOCKED; // Se bloquea por falta de espacio físico en RAM
        }
    }
    printf("\n");
    mm_print(memoria); // Muestra el estado actual del mapa de memoria
    printf("\n");

    // -------------------------------------------------------------------------
    // FASE 3: Planificación de CPU y Exportación de Métricas a CSV
    // -------------------------------------------------------------------------
    printf("--- FASE 3: PLANIFICACION DE CPU (ROUND ROBIN INTERRUPTIBLE) ---\n");
    Scheduler* planificador = scheduler_create_rr(2); // Quantum = 2 unidades de tiempo
    
    // Creamos el reporte de salida para el análisis en Python
    FILE* fp_out = fopen("resultados.csv", "w");
    if (!fp_out) {
        printf("ERROR: No se pudo crear 'resultados.csv'\n");
        mm_destroy(memoria);
        scheduler_destroy(planificador);
        stack_destroy(historial_auditoria);
        return 1;
    }
    fprintf(fp_out, "PID,Burst_Original,Memoria_Usada,Estado_Final\n");

    // Cargamos al planificador únicamente los procesos que pasaron el filtro de memoria
    for (int i = 0; i < total_procesos; i++) {
        if (procesos_carga[i].state == READY) {
            scheduler_add_process(planificador, procesos_carga[i].pid, procesos_carga[i].burst_time);
        } else {
            // Los procesos rechazados se reportan de inmediato como bloqueados
            fprintf(fp_out, "%d,%d,%d,BLOQUEADO_MEMORIA\n", 
                    procesos_carga[i].pid, procesos_carga[i].burst_time, procesos_carga[i].memory_required);
        }
    }

    // Ciclo de ejecución del despachador
    while (!scheduler_is_done(planificador)) {
        int current_pid = scheduler_next(planificador);
        if (current_pid == -1) continue;

        // Buscamos la estructura global del proceso activo
        Process* proc = NULL;
        for (int i = 0; i < total_procesos; i++) {
            if (procesos_carga[i].pid == current_pid) {
                proc = &procesos_carga[i];
                break;
            }
        }

        if (proc != NULL) {
            proc->state = RUNNING;
            
            // Sincronización exacta del paso del tiempo en la estructura de control
            int tiempo_ejecutado = (proc->remaining_time < 2) ? proc->remaining_time : 2;
            proc->remaining_time -= tiempo_ejecutado;

            // Evaluamos el ciclo de vida del proceso interrumplible
            if (proc->remaining_time > 0) {
                proc->state = READY; // Reinterrupción y vuelta a la cola modular
            } else {
                proc->state = FINISHED;
                printf("  [Sistema] PID=%d termino su ejecucion. Liberando renglon de memoria...\n", proc->pid);
                
                // Registramos los datos limpios en nuestro reporte
                fprintf(fp_out, "%d,%d,%d,COMPLETADO\n", 
                        proc->pid, proc->burst_time, proc->memory_required);

                // Liberación de memoria dinámica física y coalescencia de fragmentación
                mm_free(memoria, proc->pid);
                mm_coalesce(memoria);
                
                // Resguardo en la pila de auditoría (LIFO)
                stack_push(historial_auditoria, proc->pid);
            }
        }
    }
    fclose(fp_out); // Cerramos el flujo del reporte una vez concluido el despacho
    printf("\n");
    mm_print(memoria); // Mapa final limpio de memoria principal
    printf("\n");

    // -------------------------------------------------------------------------
    // FASE 4: Auditoría Post-Mortem del Sistema (Historial LIFO)
    // -------------------------------------------------------------------------
    printf("--- FASE 4: AUDITORIA POST-MORTEM DEL SISTEMA (HISTORIAL LIFO) ---\n");
    printf("Orden inverso de finalizacion de procesos (extrayendo de la pila):\n");
    
    while (!stack_is_empty(historial_auditoria)) {
        int pid_concluido = stack_pop(historial_auditoria);
        printf("  -> [Pila Tope] Proceso Concluido Detectado: PID=%d\n", pid_concluido);
    }
    printf("\n");

    // -------------------------------------------------------------------------
    // FASE 5: Liberación Segura de Recursos (Evitando Memory Leaks)
    // -------------------------------------------------------------------------
    mm_destroy(memoria);
    scheduler_destroy(planificador);
    stack_destroy(historial_auditoria);

    printf("==================================================\n");
    printf("   SIMULACION CONCLUIDA CON EXITO SIN FUGAS       \n");
    printf("==================================================\n");
    return 0;
}
