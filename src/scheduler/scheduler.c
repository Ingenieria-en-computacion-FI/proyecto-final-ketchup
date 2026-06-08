#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "queue.h"
#include "circular_queue.h"
#include "process.h"

#define MAX_SJF 256
#define MAX_RR 256

typedef struct {
    int pid;
    int remaining_time;
} RRProcess;

// La estructura única global del Planificador para todo el sistema
struct Scheduler {
    int type;       // 0 = FIFO, 1 = Round Robin, 2 = SJF
    Queue* fifo_queue; // Cola simple para FIFO
    CircularQueue* rr_queue;   // Cola circular para Round Robin
    RRProcess rr_proc_table[MAX_RR]; // Tabla interna de ráfagas para RR
    int rr_count;
    int quantum;
    Process sjf_table[MAX_SJF];    // Arreglo para ordenamiento SJF
    int sjf_count;
};

// Constructor FIFO
Scheduler* scheduler_create_fifo(void) {
    Scheduler* s = malloc(sizeof(Scheduler));
    if (!s) return NULL;
    s->type       = 0;
    s->fifo_queue = queue_create();
    s->quantum    = 0;
    return s;
}

// Constructor Round Robin
Scheduler* scheduler_create_rr(int quantum) {
    Scheduler* s = malloc(sizeof(Scheduler));
    if (!s) return NULL;
    s->type     = 1;
    s->rr_queue = cq_create(10);
    s->rr_count = 0;
    s->quantum  = quantum;
    return s;
}

// Constructor Shortest Job First
Scheduler* scheduler_create_sjf(void) {
    Scheduler* s = malloc(sizeof(Scheduler));
    if (!s) return NULL;
    s->type      = 2;
    s->sjf_count = 0;
    s->quantum   = 0;
    return s;
}

// Operación genérica de inserción
void scheduler_add_process(Scheduler* s, int pid, int burst_time) {
    if (!s) return;

    if (s->type == 0) {
        // Lógica FIFO
        enqueue(s->fifo_queue, pid);
    } 
    else if (s->type == 1) {
        // Lógica Round Robin
        if (s->rr_count < MAX_RR) {
            s->rr_proc_table[s->rr_count].pid = pid;
            s->rr_proc_table[s->rr_count].remaining_time = burst_time;
            s->rr_count++;
            cq_enqueue(s->rr_queue, pid);
        }
    } 
    else if (s->type == 2) {
        // Lógica SJF (Insertion Sort Greedy)
        if (s->sjf_count >= MAX_SJF) return;
        Process p = { .pid = pid, .burst_time = burst_time, .remaining_time = burst_time, .state = READY };
        int i = s->sjf_count - 1;
        while (i >= 0 && s->sjf_table[i].burst_time > burst_time) {
            s->sjf_table[i + 1] = s->sjf_table[i];
            i--;
        }
        s->sjf_table[i + 1] = p;
        s->sjf_count++;
    }
}

// Operación genérica de despacho (Siguiente tick/proceso)
int scheduler_next(Scheduler* s) {
    if (!s) return -1;

    if (s->type == 0) {
        // Despacho FIFO
        return dequeue(s->fifo_queue);
    } 
    else if (s->type == 1) {
        // Despacho Round Robin con Quantum e interrupción
        if (cq_is_empty(s->rr_queue)) return -1;
        int id = cq_dequeue(s->rr_queue);
        RRProcess* proc = NULL;

        for (int i = 0; i < s->rr_count; i++) {
            if (s->rr_proc_table[i].pid == id) {
                proc = &s->rr_proc_table[i];
                break;
            }
        }
        if (!proc) return -1;

        int exec = (proc->remaining_time < s->quantum) ? proc->remaining_time : s->quantum;
        proc->remaining_time -= exec;

        printf("  [Round Robin] PID=%d ejecuto %d u | Tiempo restante: %d u\n", id, exec, proc->remaining_time);

        if (proc->remaining_time > 0) {
            cq_enqueue(s->rr_queue, id);
        } else {
            printf("  [Round Robin] PID=%d completo su ejecucion -> TERMINADO.\n", id);
        }
        return id;
    } 
    else if (s->type == 2) {
        // Despacho SJF (Extrae el primero por ser el más corto)
        if (s->sjf_count == 0) return -1;
        int pid = s->sjf_table[0].pid;
        for (int i = 0; i < s->sjf_count - 1; i++) {
            s->sjf_table[i] = s->sjf_table[i + 1];
        }
        s->sjf_count--;
        return pid;
    }
    return -1;
}

// Operación genérica de verificación de término
int scheduler_is_done(Scheduler* s) {
    if (!s) return 1;
    if (s->type == 0) return queue_is_empty(s->fifo_queue);
    if (s->type == 1) return cq_is_empty(s->rr_queue);
    if (s->type == 2) return s->sjf_count == 0;
    return 1;
}

// Destructor seguro de recursos del planificador
void scheduler_destroy(Scheduler* s) {
    if (!s) return;
    if (s->type == 0) queue_destroy(s->fifo_queue);
    if (s->type == 1) cq_destroy(s->rr_queue);
    free(s);
}
