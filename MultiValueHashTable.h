//
// Created by lior on 12/19/22.
//

#ifndef HW3_MULTIVALUEHASHTABLE_H
#define HW3_MULTIVALUEHASHTABLE_H

#include "HashTable.h"
#include "LinkedList.h"

typedef struct MultiValueHashTable_t *MultiValueHash;

MultiValueHash createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, EqualFunction equalValue, TransformIntoNumberFunction transformIntoNumber, int hashNumber);
status destroyMultiValueHashTable(MultiValueHash mvhs);
status addToMultiValueHashTable(MultiValueHash mvhs, Element key, Element value);
LinkedList lookUpInMultiValueHashTable(MultiValueHash mvhs, Element key);
status removeFromMultiValueHashTable(MultiValueHash mvhs, Element key, Element value);
status displayMultiValueHashElementsByKey(MultiValueHash mvhs, Element key);

#endif //HW3_MULTIVALUEHASHTABLE_H
