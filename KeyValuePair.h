//
// Created by lior on 12/17/22.
//

#ifndef HW3_KEYVALUEPAIR_H
#define HW3_KEYVALUEPAIR_H

#include "Defs.h"

// definition of the Key Value Pair as a Generic ADT
typedef struct KeyValuePair_t *KeyValuePair;

// definitions of all the functions required for this ADT
KeyValuePair createKeyValuePair(Element key, Element value, CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, EqualFunction equalKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue);
status destroyKeyValuePair(KeyValuePair kvp);
status displayValue(KeyValuePair kvp);
status displayKey(KeyValuePair kvp);
Element getValue(KeyValuePair kvp);
Element getKey(KeyValuePair kvp);
bool isKeyEqual(KeyValuePair kvp, Element key);

#endif //HW3_KEYVALUEPAIR_H
