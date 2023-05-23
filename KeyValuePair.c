//
// Created by lior on 12/17/22.
//
#include "Defs.h"
#include "KeyValuePair.h"
// definition of the key value pair struct, and the variables it holds
struct KeyValuePair_t {
    Element key;
    Element value;
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    EqualFunction equalKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
};

// this function takes in key and value as elements, and all the required functions for this struct, creates it and returns a pointer to it.
// if can't create, or if one of the arguments is null, the function returns null.
KeyValuePair createKeyValuePair(Element key, Element value, CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, EqualFunction equalKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue) {
    if (key == NULL || value == NULL || copyKey == NULL || freeKey == NULL || printKey == NULL || equalKey == NULL || copyValue == NULL || freeValue == NULL || printValue == NULL)
        return NULL;
    KeyValuePair kvp;
    kvp = (KeyValuePair) malloc(sizeof(struct KeyValuePair_t));
    if (kvp == NULL) {
        return NULL;
    }
    kvp->key = copyKey(key);
    kvp->value = copyValue(value);
    kvp->copyKey = copyKey;
    kvp->freeKey = freeKey;
    kvp->printKey = printKey;
    kvp->equalKey = equalKey;
    kvp->copyValue = copyValue;
    kvp->freeValue = freeValue;
    kvp->printValue = printValue;
    return kvp;
}

// this function takes in a pointer to a key value pair, and frees it and the memory of its elements.
// if the pointer is null, the function returns nullpointer.
status destroyKeyValuePair(KeyValuePair kvp) {
    if (kvp == NULL)
        return nullpointer;
    kvp->freeKey(kvp->key);
    kvp->freeValue(kvp->value);
    free(kvp);
    return success;
}

// this function takes in a pointer to a keyvaluepair, and prints its value.
// if pointer is null, the function returns nullpointer.
status displayValue(KeyValuePair kvp) {
    if (kvp == NULL)
        return nullpointer;
    kvp->printValue(kvp->value);
    return success;
}

// this function takes in a pointer to a keyvaluepair, and prints its key.
// if pointer is null, the function returns nullpointer.
status displayKey(KeyValuePair kvp) {
    if (kvp == NULL)
        return nullpointer;
    kvp->printKey(kvp->key);
    return success;
}

// this function takes in a pointer to a keyvaluepair, and returns its value.
// if pointer is null, or if can't allocate memory for copy, the function returns null.
Element getValue(KeyValuePair kvp) {
    if (kvp == NULL)
        return NULL;
    // return copy of value
    Element cpyelem = kvp->copyValue(kvp->value);
    if (cpyelem == NULL) {
        return NULL;
    }
    return cpyelem;
}

// this function takes in a pointer to a keyvaluepair, and returns its key.
// if pointer is null, or if can't allocate memory for copy, the function returns null.
Element getKey(KeyValuePair kvp) {
    if (kvp == NULL)
        return NULL;
    // return copy of key
    Element cpyelem = kvp->copyKey(kvp->key);
    if (cpyelem == NULL) {
        return NULL;
    }
    return cpyelem;
}

// this function takes in a pointer to a keyvaluepair, and an element, and returns if the key is equal to the key of the keyvaluepair.
// if equal, returns true. if not equal or if one of the pointers is null, the function returns false.
bool isKeyEqual(KeyValuePair kvp, Element key) {
    if (kvp == NULL || key == NULL)
        return false;
    return kvp->equalKey(kvp->key, key);
}
