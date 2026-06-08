#include <stdio.h>
#include <stdlib.h>
#include "../../include/process.h"

// Lee el archivo CSV asumiendo que contiene unicamente datos en crudo (sin linea de cabecera)
int parse_processes(const char* filename, Process* procesos_array, int max_capacity) {
    FILE* fp_in = fopen(filename, "r");
    if (!fp_in) {
        return -1;
    }

    int total_procesos = 0;
    int p_id, p_burst, p_prio, p_mem;
    
    // Lectura directa asumiendo la ausencia de cabeceras tradicionales
    while(fscanf(fp_in, "%d,%d,%d,%d", &p_id, &p_burst, &p_prio, &p_mem) == 4) {
        if (total_procesos >= max_capacity) break;
        
        procesos_array[total_procesos] = (Process){
            p_id, p_burst, p_burst, p_prio, p_mem, READY
        }; //
        total_procesos++;
    }
    
    fclose(fp_in);
    return total_procesos;
}
