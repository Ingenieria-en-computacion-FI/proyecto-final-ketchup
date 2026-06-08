#include <stdio.h>
#include <stdlib.h>
#include "../../include/scheduler.h"
#include "../../include/circular_queue.h"
#include "../../include/process.h"

#define MAX_PROCESSES 256

typedef struct {
    int pid;
    int remaining_time;
} RRProcess;

// Definición de la estructura usando el nombre exacto esperado por el header
struct Scheduler {
    CircularQueue* queue;
    RRProcess processes[MAX_PROCESSES];
    int count;
    int quantum;
};

Scheduler* scheduler_create_rr(int quantum) {
    Scheduler* rr = malloc(sizeof(Scheduler));
    if (!rr) {
        return NULL;
    }
    rr->queue = cq_create(10);
    rr->count = 0;
    rr->quantum = quantum;
    return rr;
}

void scheduler_add_process(Scheduler* rr, int pid, int burst_time) {
    if (!rr || rr->count >= MAX_PROCESSES) {
        return;
    }
    rr->processes[rr->count].pid = pid;
    rr->processes[rr->count].remaining_time = burst_time;
    rr->count++;

    cq_enqueue(rr->queue, pid);
}

int scheduler_next(Scheduler* rr) {
    if (!rr || cq_is_empty(rr->queue)) {
        return -1;
    }

    int id = cq_dequeue(rr->queue);
    RRProcess* proc = NULL;

    for (int i = 0; i < rr->count; i++) {
        if (rr->processes[i].pid == id) {
            proc = &rr->processes[i];
            break;
        }
    }
    if (!proc) {
        return -1;
    }

    int exec = (proc->remaining_time < rr->quantum) ? proc->remaining_time : rr->quantum;
    proc->remaining_time -= exec;

    printf("  [Round Robin] PID=%d ejecuto %d u | Tiempo restante: %d u\n", id, exec, proc->remaining_time);

    if (proc->remaining_time > 0) {
        cq_enqueue(rr->queue, id);
    } else {
        printf("[Round Robin] PID=%d completo su ejecucion -> TERMINADO.\n", id);
    }

    return id;
}

int scheduler_is_done(Scheduler* rr) {
    if (!rr) {
        return 1;
    }
    return cq_is_empty(rr->queue);
}

void scheduler_destroy(Scheduler* rr) {
    if (!rr) {
        return;
    }
    cq_destroy(rr->queue);
    free(rr);
}
