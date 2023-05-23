//
// Created by lior on 12/17/22.
//
#include "LinkedList.h"

// definition of a node struct. this struct will be used in the linked list. it has an element of data, and a pointer that references the next node in the list
typedef struct Node_t {
    Element data;
    struct Node_t *next;
}Node;

// creating a struct of type Node: the function takes in copy function, and data as element.
// it sets a new node and returns a pointer to it.
Node* createNode(CopyFunction copyElem, Element elem) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = copyElem(elem);
    node->next = NULL;
    return node;
}

// definition of the linked list struct, and the variables it holds. head is the starting node of the list.
// size is the number of nodes the list currently has.
struct LinkedList_t {
    Node *head;
    int size;
    CopyFunction copyElem;
    FreeFunction freeElem;
    PrintFunction printElem;
    EqualFunction EqElem;
};

// this function takes in all the required functions for this struct, creates it and returns a pointer to it.
// if can't create, or if one of the arguments is null, the function returns null.
LinkedList createLinkedList(CopyFunction copyElem, FreeFunction freeElem, PrintFunction printElem, EqualFunction EqElem) {
    // checks if one of the pointers is null
    if (copyElem == NULL || freeElem == NULL || printElem == NULL || EqElem == NULL)
        return NULL;
    LinkedList ls;
    ls = (LinkedList) malloc(sizeof(struct LinkedList_t));
    // if can't allocate memory, returns null
    if (ls == NULL) {
        return NULL;
    }
    // list is empty when created, so initializing size to 0 and head pointer to null
    ls->size = 0;
    ls->head = NULL;
    // initializing functions
    ls->copyElem = copyElem;
    ls->freeElem = freeElem;
    ls->printElem = printElem;
    ls->EqElem = EqElem;
    return ls;
}

// this function takes in a pointer to a list, and frees it and the memory of all its elements.
// if the pointer is null, the function returns nullpointer.
status destroyList(LinkedList ls) {
    if (ls == NULL)
        return nullpointer;
    // iterating all the nodes and freeing them while advancing the current node to the next
    Node *cur_node = ls->head;
    while (cur_node != NULL) {
        Node *next_node = cur_node->next;
        ls->freeElem(cur_node->data);
        free(cur_node);
        cur_node = next_node;
    }
    free(ls);
    return success;
}

// this function takes in a pointer to a list, and an element, and adds the element to the end of the list.
// if one of the arguments is null, the function returns nullpointer.
status appendNode(LinkedList ls, Element elem) {
    if (ls == NULL || elem == NULL)
        return nullpointer;
    Node *newNode = createNode(ls->copyElem, elem);
    if (newNode == NULL) {
        return failure;
    }
    // if size is 0 function need to initialize node, and sets as head
    if (ls->size == 0) {
        ls->head = newNode;
    }
    // if list is not empty, append node at the end
    else {
        Node *cur_node = ls->head;
        // advance until reach last node
        while (cur_node->next != NULL)
            cur_node = cur_node->next;
        // append node
        cur_node->next = newNode;
    }
    ls->size++;
    return success;
}

// this function take in a pointer to a list and an element, and deletes the element from the list.
// if one of the arguments is null, the function returns nullpointer. if can't delete due to memory allocation, or if can't find node, returns failure.
// if can delete, returns success.
status deleteNode(LinkedList ls, Element elem) {
    if (ls == NULL || elem == NULL)
        return nullpointer;
    // if node is head
    Node *cur_node = ls->head;
    if (ls->EqElem(ls->head->data, elem)) {
        ls->head = ls->head->next;
        ls->freeElem(cur_node->data);
        free(cur_node);
        ls->size--;
        return success;
    } else {
        // search for node to delete
        while (cur_node != NULL && !ls->EqElem(cur_node->next->data, elem))
            cur_node = cur_node->next;
        // if reach the end of the linked list
        if (cur_node == NULL || cur_node->next == NULL)
            return failure;
        else {
            // if node is in the middle of the list
            Node *temp = cur_node->next;
            cur_node->next = cur_node->next->next;
            ls->freeElem(temp->data);
            free(temp);
            ls->size--;
            return success;
        }
    }
}

// this function takes in a pointer to a list and prints all of its elements.
// if the pointer is null, returns nullpointer. if can print, returns success.
status displayList(LinkedList ls) {
    if (ls == NULL)
        return nullpointer;
    Node *cur_node = ls->head;
    while (cur_node != NULL) {
        ls->printElem(cur_node->data);
        cur_node = cur_node->next;
    }
    return success;
}

// this function takes in an index and returns the element in that index.
// if the list is null, or if the index is out of range, function returns null.
Element getDataByIndex(LinkedList ls, int index) {
    if (ls == NULL || index < 1 || index > ls->size) {
        return NULL;
    }
    Node *cur_node = ls->head;
    // advancing node until it reaches the node in the required index
    int i = 1;
    while (i < index) {
        cur_node = cur_node->next;
        i++;
    }
    // return copy of the node
    Element cpyelem = ls->copyElem(cur_node->data);
    if (cpyelem == NULL) {
        return NULL;
    }
    return cpyelem;
}

// this function takes in a linked list, and returns the number of elements it holds. if pointer is null, returns null pointer.
int* getLengthList(LinkedList ls) {
    if (ls == NULL)
        return NULL;
    // creating pointer to the size variable and returning it
    int *copySize = (int*)malloc(sizeof(int));
    if (copySize == NULL) {
        return NULL;
    }
    *copySize = ls->size;
    return copySize;
}

 // function takes in a linked list, and a part of an element and checks if one of the nodes in the list holds the element.
 // if so, returns element. if not, returns null.
Element searchByKeyInList(LinkedList ls, Element elem) {
    if (ls == NULL || elem == NULL)
        return NULL;
     Node *cur_node = ls->head;
     // comparing nodes and searching for correct node
     while (cur_node != NULL && !ls->EqElem(cur_node->data, elem)) {
         cur_node = cur_node->next;
     }
     // if can't find, returns null
     if (cur_node == NULL)
         return NULL;
     // if found, tries to return copy of it. if can't allocate memory, returns null.
     Element cpyelem = ls->copyElem(cur_node->data);
     if (cpyelem == NULL) {
         return NULL;
     }
     return cpyelem;
}