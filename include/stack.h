#ifndef STACK_H
#define STACK_H

// Declaración del tipo abstracto para preservar la modularidad opaca de la pila
typedef struct Stack Stack;

// Operaciones elementales de la estructura de auditoría LIFO
Stack* stack_create();
void stack_push(Stack* stack, int value);
int stack_pop(Stack* stack);
int stack_is_empty(Stack* stack);
void stack_destroy(Stack* stack);

#endif
