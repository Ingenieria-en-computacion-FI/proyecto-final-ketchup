#include <stdlib.h>
#include "doubly_linked_list.h"

DoublyLinkedList* dll_create() {
    DoublyLinkedList* list = malloc(sizeof(DoublyLinkedList));
    if (list) {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }
    return list;
}

void dll_append(DoublyLinkedList* list, void* data) {
    if (!list) return;
    DNode* new_node = malloc(sizeof(DNode));
    if (!new_node) return;
    
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = list->tail;
    
    if (list->tail) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
    list->size++;
}

void dll_remove(DoublyLinkedList* list, DNode* node) {
    if (!list || !node) return;
    
    // Reconectar el nodo anterior
    if (node->prev) node->prev->next = node->next;
    else list->head = node->next;
    
    // Reconectar el nodo siguiente
    if (node->next) node->next->prev = node->prev;
    else list->tail = node->prev;
    
    free(node);
    list->size--;
}

void dll_destroy(DoublyLinkedList* list) {
    if (!list) return;
    DNode* current = list->head;
    while (current) {
        DNode* next = current->next;
        free(current); // Evita fugas de memoria
        current = next;
    }
    free(list);
}
