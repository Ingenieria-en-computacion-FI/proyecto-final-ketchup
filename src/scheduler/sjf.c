#include <stdio.h>
#include <stdlib.h>
#include "../../include/scheduler.h"
#include "../../include/process.h"

#define MAX_SJF 256

struct Scheduler {
    Process processes[MAX_SJF];
    int count;
};

Scheduler* scheduler_create_sjf(){
    Scheduler* s = malloc(sizeof(Scheduler));
    if (!s) {
        return NULL;
    }
    s->count = 0;
    return s;
}

void scheduler_add_process(Scheduler* s, int pid, int burst_time){
    if (!s || s->count >= MAX_SJF) {
        return;
    }

    Process p;
    p.pid = pid;
    p.burst_time = burst_time;
    p.remaining_time = burst_time;
    p.priority = 0;
    p.memory_required = 0;
    p.state = READY;

    int i = s->count - 1;
    while (i >= 0 && s->processes[i].burst_time > burst_time) {
        s->processes[i + 1] = s->processes[i];
        i--;
    }
    s->processes[i + 1] = p;
    s->count++;
}

int scheduler_next(Scheduler* s) {
    if (!s || s->count == 0) {
        return -1;
    }

    int pid = s->processes[0].pid;

    for (int i = 0; i < s->count - 1; i++) {
        s->processes[i] = s->processes[i + 1];
    }
    s->count--;

    return pid;
}

int scheduler_is_done(Scheduler* s) {
    if (!s) {
        return 1;
    }
    return s->count == 0;
}

void scheduler_destroy(Scheduler* s) {
    if (!s) {
        return;
    }
    free(s);
}
