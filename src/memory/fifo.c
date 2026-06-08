#include <stdio.h>
#include <stdlib.h>
#include "../../include/scheduler.h" 
#include "../../include/queue.h"
#include "../../include/process.h"

struct Scheduler {
    Queue* queue;
    int type;
    int quantum;
};

Scheduler* scheduler_create_fifo(){
    Scheduler* s = malloc(sizeof(Scheduler));
    if (!s) {
        return NULL;
    }
    s->queue = queue_create();
    s->type = 0;
    s->quantum = 0;
    return s;
}

void scheduler_add_process(Scheduler* s, int pid, int burst_time){
    if (!s) {
        return;
    }
    (void)burst_time;
    enqueue(s->queue, pid);
}

int scheduler_next(Scheduler* s){
    if (!s) {
        return -1;
    }
    return dequeue(s->queue);
}

int scheduler_is_done(Scheduler* s){
    if (!s) {
        return 1;
    }
    return queue_is_empty(s->queue);
}

void scheduler_destroy(Scheduler* s){
    if (!s) {
        return;
    }
    queue_destroy(s->queue);
    free(s);
}
