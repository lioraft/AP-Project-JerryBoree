//
// Created by lior on 12/19/22.
//
#include "KeyValuePair.h"
#include "MultiValueHashTable.h"

// definition of the hash table struct, and the variables it holds.
// short description of this Generic ADT: the multi value hash table is built from a hash table.
// because there can be multiple values for every key, the value of every kvp in the linked list
// is a linked list of values.
struct MultiValueHashTable_t {
    hashTable hs;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalValue;
};

// function that returns a shallow copy of linked list. will be used only in mv hash table.
LinkedList copyLS(LinkedList list) {
    return list;
}

// this function creates multi value hash table. it takes in all the required functions for its key and its value, and its size.
// if the function can't create the mv hash table, it returns null. else, returns a pointer to it.
MultiValueHash createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, EqualFunction equalValue, TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    if (copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL || printValue == NULL || equalKey == NULL || transformIntoNumber == NULL)
        return NULL;
    // creating multi value hash table
    MultiValueHash mvhs = (MultiValueHash) malloc(sizeof(struct MultiValueHashTable_t));
    if (mvhs == NULL)
    {
        printf("Memory Problem \n");
        return NULL;
    }
    // creating hash table
    hashTable hs = createHashTable(copyKey, freeKey, printKey, (CopyFunction)copyLS, (FreeFunction)destroyList, (PrintFunction)displayList, equalKey, transformIntoNumber, hashNumber);
    if (hs == NULL) {
        free(mvhs);
        printf("Memory Problem \n");
        return NULL;
    }
    mvhs->hs = hs;
    // required functions
    mvhs->printKey = printKey;
    mvhs->copyValue = copyValue;
    mvhs->freeValue = freeValue;
    mvhs->printValue = printValue;
    mvhs->equalValue = equalValue;
    return mvhs;
}

// this function takes in mv hash table, and frees all its memory. if receives null pointer, returns nullpointer. else, returns success.
status destroyMultiValueHashTable(MultiValueHash mvhs) {
    if (mvhs == NULL)
        return nullpointer;
    destroyHashTable(mvhs->hs);
    free(mvhs);
    return success;
}

// this function takes in mv hash table and key. if the key exists in the mv hash table, function returns its list of values.
// if not exist, or if one of the pointers is null, the function returns null.
LinkedList lookUpInMultiValueHashTable(MultiValueHash mvhs, Element key) {
    if (mvhs == NULL || key == NULL)
        return NULL;
    LinkedList val_list = lookupInHashTable(mvhs->hs, key);
    return val_list;
}

// this function takes in mv hash table, key and element, and appends the element to the mv hash table.
// if pointers are null, returns nullpointer. if can't add, returns failure. else, returns success.
status addToMultiValueHashTable(MultiValueHash mvhs, Element key, Element value) {
    if (mvhs == NULL || key == NULL || value == NULL)
        return nullpointer;
    Element val_list = lookUpInMultiValueHashTable(mvhs, key);
    // if key is not in the mv hash table, the function creates a key and value pair for it
    if (val_list == NULL)
    {
        LinkedList cur_list = createLinkedList(mvhs->copyValue, mvhs->freeValue, mvhs->printValue, mvhs->equalValue);
        if (cur_list == NULL)
        {
            return failure;
        }
        addToHashTable(mvhs->hs, key, cur_list);
        val_list = cur_list;
    }
    // returns if element can be added or not, and returns it
    return appendNode(val_list, value);
}

// this function takes in mv hash table, key and value, and returns if the key and the value (or part of it) are
// in the table. if pointers are null or if can't find, returns failure. if can find, it removes it from the table and returns success.
status removeFromMultiValueHashTable(MultiValueHash mvhs, Element key, Element value) {
    if (mvhs == NULL || key == NULL || value == NULL)
        return nullpointer;
    LinkedList elem_vals = lookUpInMultiValueHashTable(mvhs, key);
    // if can't find key, returns null
    if (elem_vals == NULL)
        return failure;
    // if key is found
    else {
        // searching the value in the key's list of values
        Element elem = searchByKeyInList(elem_vals, value);
        // if element is found, delete node
        if (elem != NULL)
            deleteNode(elem_vals, value);
        // if no values left in the list, delete the list
        int *listlen = getLengthList(elem_vals);
        if (*listlen == 0)
            removeFromHashTable(mvhs->hs, key);
        free(listlen);
        return success;
    }
}

// this function takes in mv hash table and a key, and prints the key and all its values, if it has any.
// if receives null pointer, returns null. else, returns success.
status displayMultiValueHashElementsByKey(MultiValueHash mvhs, Element key) {
    if (mvhs == NULL || key == NULL)
        return nullpointer;
    // search key in multi value hash
    LinkedList elem_vals = lookUpInMultiValueHashTable(mvhs, key);
    // if can find, print
    if (elem_vals != NULL) {
        mvhs->printKey(key);
        displayList(elem_vals);
    }
    return success;
}