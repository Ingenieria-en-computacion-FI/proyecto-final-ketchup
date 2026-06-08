#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

typedef struct DNode {
    void* data; // Puntero genérico para albergar cualquier estructura (Process, File, etc.)
    struct DNode* next;
    struct DNode* prev;
} DNode;

typedef struct {
    DNode* head;
    DNode* tail;
    int size;
} DoublyLinkedList;

DoublyLinkedList* dll_create();
void dll_append(DoublyLinkedList* list, void* data);
void dll_remove(DoublyLinkedList* list, DNode* node);
void dll_destroy(DoublyLinkedList* list);

#endif
