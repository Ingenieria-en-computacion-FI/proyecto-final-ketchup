#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct Scheduler Scheduler;

// Constructores para cada política de planificación uniforme
Scheduler* scheduler_create_fifo();
Scheduler* scheduler_create_rr(int quantum);
Scheduler* scheduler_create_sjf();

void scheduler_add_process(Scheduler* s, int pid, int burst_time);
int scheduler_next(Scheduler* s);
int scheduler_is_done(Scheduler* s);
void scheduler_destroy(Scheduler* s);

#endif
