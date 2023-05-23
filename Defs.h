//
// Created by lior on 12/17/22.
//

#ifndef HW3_DEFS_H
#define HW3_DEFS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* the size of the reading buffer allowed in this project */
#define bufsize 300

// macro to find minimum value between two values
#define min(x, y) (x<y ? x:y)

// macro to find maximum value between two values
#define max(x, y) (x>y ? x:y)

typedef enum e_bool { false, true } bool;
typedef enum e_status { success, failure, nullpointer } status;

typedef void * Element;

typedef Element(*CopyFunction) (Element);
typedef status(*FreeFunction) (Element);
typedef status(*PrintFunction) (Element);
typedef int(*TransformIntoNumberFunction) (Element);
typedef bool(*EqualFunction) (Element, Element);

#endif //HW3_DEFS_H
