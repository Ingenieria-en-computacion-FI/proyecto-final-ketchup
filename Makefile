# Variables de configuración del compilador de C
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Nombre del archivo ejecutable final que se va a generar
TARGET = simulador_os

# Lista de todos los archivos objeto necesarios para el enlace modular
OBJS = main.o first_fit.o coalescence.o queue.o circular_queue.o stack.o fifo.o round_robin.o sjf.o

# Regla principal: Compila todo el proyecto de forma automática
all: $(TARGET)

# Enlace de los objetos intermedios para producir el binario ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Reglas de compilación individuales para asegurar modularidad limpia
main.o: main.c process.h memory_manager.h circular_queue.h stack.h
	$(CC) $(CFLAGS) -c main.c

first_fit.o: first_fit.c memory_manager.h
	$(CC) $(CFLAGS) -c first_fit.c

coalescence.o: coalescence.c memory_manager.h
	$(CC) $(CFLAGS) -c coalescence.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

circular_queue.o: circular_queue.c circular_queue.h
	$(CC) $(CFLAGS) -c circular_queue.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

fifo.o: fifo.c scheduler.h queue.h process.h
	$(CC) $(CFLAGS) -c fifo.c

round_robin.o: round_robin.c circular_queue.h process.h
	$(CC) $(CFLAGS) -c round_robin.c

sjf.o: sjf.c process.h
	$(CC) $(CFLAGS) -c sjf.c

# Regla de limpieza para remover archivos intermedios y dejar la carpeta impecable
clean:
	rm -f *.o $(TARGET)
