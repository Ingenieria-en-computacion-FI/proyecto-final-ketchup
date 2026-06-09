#include <stdio.h>
#include <stdlib.h>
#include "process.h"

// Lee el archivo CSV ignorando la primera línea (cabecera)
int parse_processes(const char* filename, Process* procesos_array, int max_capacity) {
    FILE* fp_in = fopen(filename, "r");
    if (!fp_in) {
        return -1; // Retorna error si no encuentra el archivo
    }

    // --- CORRECCIÓN: Saltar la línea de cabecera ---
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), fp_in) == NULL) {
        fclose(fp_in);
        return 0; // Archivo vacío
    }

    int total_procesos = 0;
    int p_id, p_burst, p_prio, p_mem;
    
    // Lectura directa de los datos crudos a partir de la segunda línea
    while(fscanf(fp_in, "%d,%d,%d,%d", &p_id, &p_burst, &p_prio, &p_mem) == 4) {
        if (total_procesos >= max_capacity) break;
        
        procesos_array[total_procesos] = (Process){
            p_id, p_burst, p_burst, p_prio, p_mem, READY
        };
        total_procesos++;
    }
    
    fclose(fp_in);
    return total_procesos;
}
