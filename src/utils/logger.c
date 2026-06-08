#include <stdio.h>
#include <stdarg.h>

void log_event(const char* format, ...) {
    va_list args;
    
    // Imprimir en consola
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    // Opcional: Guardar en archivo (data/logs/simulacion.log)
    FILE* log_file = fopen("data/logs/simulacion.log", "a");
    if (log_file) {
        va_start(args, format);
        vfprintf(log_file, format, args);
        va_end(args);
        fclose(log_file);
    }
}
