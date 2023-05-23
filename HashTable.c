//
// Created by lior on 12/17/22.
//
#include "LinkedList.h"
#include "KeyValuePair.h"
#include "HashTable.h"

// definition of the hash table struct, and the variables it holds.
// short description of this Generic ADT: the hash table consists of indexed array (for chaining). inside each one of the cells there is
// a linked list that consists of nodes that are key and value pairs.
struct hashTable_s {
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    TransformIntoNumberFunction transformIntoNumber;
    int size;
    LinkedList *ls;
};

// this function performs shallow copy (to avoid cloning) on a kvp and will be used only in hash table
KeyValuePair kvpCopy(KeyValuePair kvp) {
    return kvp;
}

// this function prints a kvp and will be used only in hash table
status kvpPrint(KeyValuePair kvp) {
    if (kvp == NULL)
        return nullpointer;
    displayKey(kvp);
    displayValue(kvp);
    return success;
}

// this function takes in size of hash table and all the required functions for this struct, creates it and returns a pointer to it.
// if can't create, or if one of the arguments is null, the function returns null.
hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    if (copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL || printValue == NULL || equalKey == NULL || transformIntoNumber == NULL)
        return NULL;
    hashTable hs = (hashTable) malloc(sizeof(struct hashTable_s));
    // if can't allocate memory for hash table
    if (hs == NULL) {
        return NULL;
    }
    LinkedList *ls = (LinkedList *)malloc(sizeof(LinkedList) * hashNumber);
    // if can't allocate memory for array of linked lists of the hash table
    if (ls == NULL) {
        free(hs);
        return NULL;
    }
    // creating a linked list for each index. if can't allocate memory for array of linked lists of the hash table, frees all memory
    for (int i = 0; i < hashNumber; i++) {
        ls[i] = createLinkedList((CopyFunction)kvpCopy, (FreeFunction)destroyKeyValuePair, (PrintFunction)kvpPrint, (EqualFunction)isKeyEqual);
        if (ls[i] == NULL) {
            for (int j = 0; j < i; j++)
                destroyList(ls[j]);
            free(ls);
            free(hs);
            return NULL;
        }
    }
    // required functions
    hs->copyKey = copyKey;
    hs->freeKey = freeKey;
    hs->printKey = printKey;
    hs->copyValue = copyValue;
    hs->freeValue = freeValue;
    hs->printValue = printValue;
    hs->equalKey = equalKey;
    hs->transformIntoNumber = transformIntoNumber;
    hs->size = hashNumber;
    hs->ls = ls;
    return hs;
}

// this function takes in a pointer to a hash table, and frees it and the memory of its elements.
// if the pointer is null, the function returns nullpointer.
status destroyHashTable(hashTable hs) {
    if (hs == NULL)
        return nullpointer;
    for (int i = 0; i < hs->size; i++) {
        destroyList(hs->ls[i]);
    }
    free(hs->ls);
    free(hs);
    return success;
}

// function takes in hash table, key and element, sets them as key value pair and adds the pair to the linked list of the hash table.
// if one of the pointers is null, returns null. if can't create, returns failure. else. returns success.
status addToHashTable(hashTable hs, Element key, Element value) {
    if (hs == NULL || key == NULL || value == NULL)
        return nullpointer;
    // getting the index for chaining
    int transformedKey = hs->transformIntoNumber(key) % hs->size;
    // creating kvp and chaining it in the list
    KeyValuePair kvp = createKeyValuePair(key, value, hs->copyKey, hs->freeKey, hs->printKey, hs->equalKey, hs->copyValue, hs->freeValue, hs->printValue);
    if (kvp == NULL)
        return failure;
    return appendNode(hs->ls[transformedKey], kvp);
}

// function takes in hash table and key, and returns the element of that key. if there's no such key, returns null.
Element lookupInHashTable(hashTable hs, Element key) {
    if (hs == NULL || key == NULL)
        return NULL;
    // getting the index it was chained in
    int transformedKey = hs->transformIntoNumber(key) % hs->size;
    // searching for the key value pair with the search by key function.
    KeyValuePair pair = searchByKeyInList(hs->ls[transformedKey], key);
    if (pair == NULL)
        return NULL;
    return getValue(pair);
}

// this function takes in hash table and a key. it looks for the key value pair of this key.
// if not exist, returns null. if exists, deletes it from hash table and returns success.
status removeFromHashTable(hashTable hs, Element key) {
    if (hs == NULL || key == NULL)
        return nullpointer;
    // if doesn't exist
    Element value = lookupInHashTable(hs, key);
    if (value == NULL)
        return failure;
    // if exists, get the index it was chained in
    int transformedKey = hs->transformIntoNumber(key) % hs->size;
    // delete the node
    return deleteNode(hs->ls[transformedKey], key);
}

// this function takes in hash table and prints all of its elements.
status displayHashElements(hashTable hs) {
    if (hs == NULL)
        return nullpointer;
    // print all nodes in every list
    for (int i = 0; i < hs->size; i++) {
        if (displayList(hs->ls[i]) == nullpointer)
            return nullpointer; }
    return success;
}

