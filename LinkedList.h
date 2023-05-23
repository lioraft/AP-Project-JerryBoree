//
// Created by lior on 12/17/22.
//

#ifndef HW3_LINKEDLIST_H
#define HW3_LINKEDLIST_H

#include "Defs.h"

// definition of the Linked List as a Generic ADT
typedef struct LinkedList_t *LinkedList;

// definitions of all the functions required for this ADT
LinkedList createLinkedList(CopyFunction copyElem, FreeFunction freeElem, PrintFunction printElem, EqualFunction EqElem);
status destroyList(LinkedList ls);
status appendNode(LinkedList ls, Element elem);
status deleteNode(LinkedList ls, Element elem);
status displayList(LinkedList ls);
Element getDataByIndex(LinkedList ls, int index);
int* getLengthList(LinkedList ls);
Element searchByKeyInList(LinkedList ls, Element elem);


#endif //HW3_LINKEDLIST_H
