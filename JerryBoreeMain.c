//
// Created by lior on 12/21/22.
//

#include "MultiValueHashTable.h"
#include "KeyValuePair.h"
#include "Jerry.h"
#include <math.h>

/*** helper functions for size and hashing ***/
// this function iterates over the files and counts the number of lines related to jerries and physical characteristics in the file.
// it will be used in order to create the hash tables.
int sizeOfHash(char *configfile) {
    int sizeofhash = 0;
    FILE* config;
    char buffer[bufsize];
    int linelen = bufsize;
    config = fopen(configfile, "r");
    bool isJerry = false;
    char *title;
    title = "Jerries\n";
    /* loop that iterates all the lines of the file */
    while (fgets(buffer, linelen, config))
    {
        if (isJerry)
            sizeofhash++;
        if (strcmp(buffer, title) == 0)
            isJerry = true;
    }
    fclose(config);
    return sizeofhash;
}

// function takes in a string, and converts it to asci sum. will be used as hashing function of strings
int toNumber(char * str) {
    int asciNum = 0;
    for (int i = 0; i < strlen(str); i++)
        asciNum += (int)str[i];
    return asciNum;
}

/*** functions for hash tables - copy, print, equal and free ***/

// checks if two arrays of chars are the same
bool isSameKey(char * str1, char * str2) {
    if (str1 == NULL || str2 == NULL)
        return false;
    if (strcmp(str1, str2) == 0)
        return true;
    return false;
}

// function that takes in array of chars and prints it
status printStr(char *str) {
    if (str == NULL)
        return nullpointer;
    printf("%s : \n", str);
    return success;
}

// function that performs deep copy on array of chars and returns it
char * copyStr(char *str) {
    if (str == NULL)
        return NULL;
    char *cur_name = (char *)malloc(strlen(str)+1);
    strcpy(cur_name, str);
    return cur_name;
}

// function that frees array of chars
status freeStr(char *str) {
    if (str == NULL)
        return nullpointer;
    free(str);
    return success;
}

// function that sets pointer as null. used in order to avoid freeing elements that still exists in the system.
void setNull(Element elem) {
    elem = NULL;
}

/*** this is the function that reads the file and stores its data. it takes in the number of planets, array of planets, linked list, multi value hash table,
 * hash table, and the directory of the configuration file. it reads the files, creates planets and uses them in order to create the jerries.
 * it creates jerries and stores them in linked list, and hash table. multi value hash table.
 * if memory allocation during the function fails, the function returns failure and tries to exit and clean memory.
 * if the data is valid, the function returns success. ***/
status readCurrentFile(int numofplanets, Planet** planetsarr, LinkedList jlst, MultiValueHash mvh, hashTable hs, char *configfile) {
    /* setting objects for the functions of the file reading and storing the data */
    int planetsindex = 0;
    FILE* config;
    char *buffer;
    buffer = (char *) malloc(bufsize);
    /* if can't allocate, prints error message and returns null */
    if (!buffer) {
        return failure;
    }
    size_t linelen = bufsize;
    config = fopen(configfile, "r");
    char *title;
    title = "Planets\n";
    char *title2;
    title2 = "Jerries\n";
    bool isJerry = false;
    bool isPlanet;
    char *name = NULL;
    double x, y, z;
    char *id, *dim, *plname, *happy = NULL;
    Jerry* cur_jerry;

    /* loop that iterates all the lines of the file */
    while (getline(&buffer, &linelen, config) != -1)
    {
        /* boolean that indicate if the line is a planet */
        if (strcmp(buffer, title) == 0 || numofplanets > 0)
            isPlanet = true;
        else
            isPlanet = false;
        /* reading the section of planets */
        if (isPlanet && strcmp(buffer, title) != 0) {
            /* setting the planet's arguments */
            name = strtok(buffer, ",");
            x = strtod(strtok(NULL, ","), NULL);
            y = strtod(strtok(NULL, ","), NULL);
            z = strtod(strtok(NULL, ","), NULL);
            /* creating the planet from the arguments we received */
            Planet *ppl;
            ppl = initplanet(name, x, y, z);
            /* according to instructions, it can be assumed the input stream file is valid.
             * so if returned null, it means there was a memory allocation problem and message was printed in init. function returns failure and exits the program */
            if (!ppl) {
                return failure;
            }
            planetsarr[planetsindex] = ppl;
            numofplanets--;
            planetsindex++;
        }

        /* boolean that indicate if the line is a jerry */
        if (strcmp(buffer, title2) == 0)
            isJerry = true;
        /* reading the section of jerries */
        if (!isPlanet && isJerry && strcmp(buffer, title2) != 0) {
            /* setting jerry's arguments */
            if (strncmp(buffer, "\t", 1) != 0) {
                id = strtok(buffer, ",");
                dim = strtok(NULL, ",");
                plname = strtok(NULL, ",");
                happy = strtok(NULL, ",");
                /* finding jerry's planet */
                int whichplanet = 0;
                for (int i = 0; i < planetsindex; i++) {
                    if (strcmp(plname, planetsarr[i]->name) == 0)
                        whichplanet = i;
                }
                /* creating jerry */
                Jerry *pjer;
                pjer = initjerry(id, dim, planetsarr[whichplanet], (int)strtol(happy, NULL, 10));
                /* according to instructions, it can be assumed the input stream file is valid.
                 * so if returned null, it means there was a memory allocation problem and message was printed in init. function returns failure and exits program */
                if (!pjer) {
                    return failure;
                }
                // adding jerry to hash table
                if (addToHashTable(hs, id, pjer) == failure)
                    return failure;
                // adding to jerries linked list
                if (appendNode(jlst, pjer))
                    return failure;
                cur_jerry = pjer;
            }
            /* sets jerry's physical characteristics - the function creates the jerry and puts it in the array. in the iterations after the creation it sets
             * the physical characteristics. */
            while (buffer != NULL && strncmp(buffer, "\t", 1) == 0) {
                // removes tab at the beginning
                buffer = strtok(buffer, "\t");
                // splits the result to two arguments
                char *phyname, *value;
                phyname = strtok(buffer, ":");
                value = strtok(NULL, ",");
                /* creating physical characteristic and adding to jerry */
                if (addPhysical(cur_jerry, phyname, strtod(value, NULL)) == failure)
                    return failure;
                // adding the jerry to the jerries linked list of the physical characteristics
                if (addToMultiValueHashTable(mvh, phyname, cur_jerry) == failure)
                    return failure;
            }
        }
    }
    free(name);
    fclose(config);
    return success;
}

/*** a function that deletes all the memory created ***/
void deleteMemory(int numofplanets, Planet** planetsarr, LinkedList jlst, MultiValueHash mvh, hashTable hs) {
    destroyList(jlst);
    destroyHashTable(hs);
    destroyMultiValueHashTable(mvh);
    for (int i = 0; i < numofplanets; i++)
        destroyPlanet(planetsarr[i]);
    free(planetsarr);
}

/*** a function that prints the menu when repeating the case loop ***/
void printMenu() {
    printf("Welcome Rick, what are your Jerry's needs today ? \n");
    printf("1 : Take this Jerry away from me \n");
    printf("2 : I think I remember something about my Jerry \n");
    printf("3 : Oh wait. That can't be right \n");
    printf("4 : I guess I will take back my Jerry now \n");
    printf("5 : I can't find my Jerry. Just give me a similar one \n");
    printf("6 : I lost a bet. Give me your saddest Jerry \n");
    printf("7 : Show me what you got \n");
    printf("8 : Let the Jerries play \n");
    printf("9 : I had enough. Close this place \n");
}

/*** case functions ***/
// this function takes in an array of planets and the number of planets, a linked list of jerries and hash table of jerries, and adds a jerry to the hash table and linked list.
// if fails to allocate memory, it returns failure. if succeeds, returns success.
status takeJerryCase1(int numofplanets, Planet **planetsarr, LinkedList jerrieslist, hashTable jerryhash) {
    printf("What is your Jerry's ID ? \n");
    char idinput[bufsize];
    scanf("%s", idinput);
        // searching the jerry in hashtable by id
        if (lookupInHashTable(jerryhash, idinput) == NULL) {
            printf("What planet is your Jerry from ? \n");
            char plinput[bufsize];
            scanf("%s", plinput);
            for (int i = 0; i < numofplanets; i++) {
                // if planet exist
                if (strcmp(planetsarr[i]->name, plinput) == 0) {
                    // input of dimension
                    printf("What is your Jerry's dimension ? \n");
                    char diminput[bufsize];
                    scanf("%s", diminput);
                    // input of happiness
                    printf("How happy is your Jerry now ? \n");
                    char hapinput[bufsize];
                    scanf("%s", hapinput);
                    // creating jerry and adding to hash table and linked list
                    Jerry *pjer = initjerry(idinput, diminput, planetsarr[i], (int)strtol(hapinput, NULL, 10));
                    if (pjer == NULL)
                        return failure;
                    if (addToHashTable(jerryhash, idinput, pjer) != success)
                        return failure;
                    if (appendNode(jerrieslist, pjer) != success)
                        return failure;
                    // print jerry after adding
                    if (printJerry(pjer) != success)
                        return failure;
                    return success;
                }
            }
            // if planet doesn't exist
            printf("%s is not a known planet ! \n", plinput);
            return success;
    }
    // if can't find the specified jerry
    printf("Rick did you forgot ? you already left him here ! \n");
    return success;
}

// this function takes in hash table of jerries and multi hash table of physical characteristics with the jerries who has them, and adds a physical characteristic to a specified jerry.
// if fails to allocate memory, it returns failure. if succeeds, returns success.
status somethingAboutJerryCase2(hashTable jerryhash, MultiValueHash multihash) {
    printf("What is your Jerry's ID ? \n");
    char idinput[bufsize];
    scanf("%s", idinput);
    // searching the jerry in hashtable by id
    Jerry *pjer = lookupInHashTable(jerryhash, idinput);
    if (pjer != NULL) {
        printf("What physical characteristic can you add to Jerry - %s ? \n", idinput);
        char phyinput[bufsize];
        scanf("%s", phyinput);
        for (int i = 0; i < pjer->numofphy; i++) {
            // if jerry already has physical characteristic
            if (strcmp(pjer->phyarr[i]->name, phyinput) == 0) {
                printf("The information about his %s already available to the daycare ! \n", phyinput);
                return success;
            }
        }
        printf("What is the value of his %s ? \n", phyinput);
        char value[bufsize];
        scanf("%s", value);
        // add physical characteristic to jerry
        if (addPhysical(pjer, phyinput, strtod(value, NULL)) != success)
            return failure;
        // add to multi hash table
        if (addToMultiValueHashTable(multihash, phyinput, pjer) != success)
            return failure;
        // print jerries with this physical characteristic
        if (displayMultiValueHashElementsByKey(multihash, phyinput) != success)
            return failure;
        return success;
    }
    // if can't find the specified jerry
    printf("Rick this Jerry is not in the daycare ! \n");
    return success;
}

// this function takes in hash table of jerries and multi hash table of physical characteristics with the jerries who has them, and adds a physical characteristic to a specified jerry.
// if fails to allocate memory, it returns failure. if succeeds, returns success.
status ohWaitCase3(hashTable jerryhash, MultiValueHash multihash) {
    printf("What is your Jerry's ID ? \n");
    char idinput[bufsize];
    scanf("%s", idinput);
    // searching the jerry in hashtable by id
    Jerry *pjer = lookupInHashTable(jerryhash, idinput);
    if (pjer != NULL) {
        printf("What physical characteristic do you want to remove from Jerry - %s ? \n", idinput);
        char phyinput[bufsize];
        scanf("%s", phyinput);
        for (int i = 0; i < pjer->numofphy; i++) {
            // searching for physical characteristic
            if (strcmp(pjer->phyarr[i]->name, phyinput) == 0) {
                // deletes if finds
                if (deletePhysical(pjer, phyinput) != success)
                    return failure;
                if (removeFromMultiValueHashTable(multihash, phyinput, pjer) != success)
                    return failure;
                if (printJerry(pjer) != success)
                    return failure;
                return success;
            }
        }
        // if can't find physical characteristic
        printf("The information about his %s not available to the daycare ! \n", phyinput);
        return success;
    }
    // if can't find the specified jerry
    printf("Rick this Jerry is not in the daycare ! \n");
    return success;
}

// this function takes in linked list of jerries, hash table of jerries and multi hash table of physical characteristics with the jerries who has them,
// and removes a jerry from the system. if fails to delete, it returns failure. if succeeds, returns success.
status takeJerryBackCase4(LinkedList jerrieslist, hashTable jerryhash, MultiValueHash multihash) {
    printf("What is your Jerry's ID ? \n");
    char idinput[bufsize];
    scanf("%s", idinput);
    // searching the jerry in hashtable by id
    Jerry *pjer = lookupInHashTable(jerryhash, idinput);
    if (pjer != NULL) {
        for (int i = 0; i < pjer->numofphy; i++)
            // delete jerry from physical characteristics in multi hash
            if (removeFromMultiValueHashTable(multihash, pjer->phyarr[i]->name, pjer) != success)
                return failure;
        // delete jerry from hash
        if (removeFromHashTable(jerryhash, pjer->id) != success)
            return failure;
        // delete jerry from linked list
       if (deleteNode(jerrieslist, pjer) != success)
           return failure;
        // return jerry message
        printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
        return success;
    }
    // if can't find the specified jerry
    printf("Rick this Jerry is not in the daycare ! \n");
    return success;
}

// this function takes in linked list of jerries, hash table of jerries and multi hash table of physical characteristics with the jerries who has them,
// and asks the user for a physicalv characteristic and value. it then searches among the jerries who has this physical characteristic, who has the closest value to the
// value the user entered as input. if function fails due to memory allocation, it returns failure. if succeeds, returns success.
status similarJerryCase5(LinkedList jerrieslist, hashTable jerryhash, MultiValueHash multihash) {
    printf("What do you remember about your Jerry ? \n");
    char phyinput[bufsize];
    scanf("%s", phyinput);
    // searching the physical characteristic in multi value hashtable
    LinkedList elemlist = lookUpInMultiValueHashTable(multihash, phyinput);
    if (elemlist != NULL) {
        // if physical characteristic exists
        printf("What do you remember about the value of his %s ? \n", phyinput);
        char valinput[bufsize];
        scanf("%s", valinput);
        // converting value of physical characteristic to double
        double val = strtod(valinput, NULL);
        // pointers of current jerry and closest jerry to value. current closest is the first jerry in the list
        Jerry *closestjerry = getDataByIndex(elemlist, 1);
        int minVal;
        for (int k = 0; k < closestjerry->numofphy; k++)
            if (strcmp(closestjerry->phyarr[k]->name, phyinput) == 0)
                minVal = min(fabs(closestjerry->phyarr[k]->val - val), fabs(val - closestjerry->phyarr[k]->val));
        Jerry *curjerry;
        int *listlen = getLengthList(elemlist);
        // iterating through all the jerries with that physical characteristic
        for (int i = 2; i <= *listlen; i++)
        {
            curjerry = getDataByIndex(elemlist, i);
            // iterating through each jerry's physical characteristics array
            for (int j = 0; j < curjerry->numofphy; j++) {
                //if same physical characteristic, checks the value of it. if value is closest, update the closest jerry to be the current jerry
                if (strcmp(curjerry->phyarr[j]->name, phyinput) == 0) {
                    int curVal = min(fabs(curjerry->phyarr[j]->val - val), fabs(val - curjerry->phyarr[j]->val));
                    if (curVal < minVal) {
                        minVal = curVal;
                        closestjerry = curjerry;
                    }
                }
            }
        }
        free(listlen);
        // after finding the closest jerry, print it
        printf("Rick this is the most suitable Jerry we found : \n");
        if (printJerry(closestjerry) != success)
            return failure;
        for (int i = 0; i < closestjerry->numofphy; i++)
            // delete jerry from physical characteristics in multi hash
            if (removeFromMultiValueHashTable(multihash, closestjerry->phyarr[i]->name, closestjerry) != success)
                return failure;
        // delete jerry from hash
        if (removeFromHashTable(jerryhash, closestjerry->id) != success)
            return failure;
        // delete jerry from linked list
        if (deleteNode(jerrieslist, closestjerry) != success)
            return failure;
        // message for returning the jerry
        printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
        return success;
    }
    // if there are no jerries with this physical characteristic
    printf("Rick we can not help you - we do not know any Jerry's %s ! \n", phyinput);
    return success;
}

// this function takes in linked list of jerries, hash table of jerries and multi hash table of physical characteristics with the jerries who has them,
// and returns the jerry who has the minimum happiness level.
// if fails to delete and "return" to rick, it returns failure. if succeeds, returns success.
status saddestJerryCase6(LinkedList jerrieslist, hashTable jerryhash, MultiValueHash multihash) {
    int *listlen = getLengthList(jerrieslist);
    if (*listlen == 0) {
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        free(listlen);
        return success;
    }
    else {
        // default will be the first jerry
        Jerry *sadjerry = getDataByIndex(jerrieslist, 1);
        Jerry *curjerry;
        // iterating all jerries in order to find the saddest one
        for (int i = 2; i <= *listlen; i++) {
            curjerry = getDataByIndex(jerrieslist, i);
            if (curjerry->happy < sadjerry->happy)
                sadjerry = curjerry;
            }
        free(listlen);
        // printing jerry
        printf("Rick this is the most suitable Jerry we found : \n");
        if (printJerry(sadjerry) != success)
            return failure;
        // delete jerry from physical characteristics in multi hash
        for (int k = 0; k < sadjerry->numofphy; k++) {
            if (removeFromMultiValueHashTable(multihash, sadjerry->phyarr[k]->name, sadjerry) != success)
                return failure;
        }
        // delete jerry from hash
        if (removeFromHashTable(jerryhash, sadjerry->id) != success)
            return failure;
        // delete jerry from linked list
        if (deleteNode(jerrieslist, sadjerry) != success)
            return failure;
        // return jerry message
        printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
        return success;
    }
}

// this function takes in the number of planets, array of planets, linked list of jerries and multi value hash of physical characteristics.
// it receives option input from the user: if 1, prints all jerries. if 2, prints jerries by certain physical characteristic. if 3, prints all planets.
// any other option will cause an error message to be printed. if at any point there is memory allocation problem, the function returns failure.
// if action is successful, returns success.
status showMeCase7(int numofplanets, Planet **planetsarr, LinkedList jerrieslist, MultiValueHash multihash) {
    // printing options
    printf("What information do you want to know ? \n1 : All Jerries \n2 : All Jerries by physical characteristics \n3 : All known planets \n");
    // choosing option
    char option[bufsize];
    scanf("%s", option);
    int opnum = (int)strtol(option, NULL, 10);
    if (!(opnum >= 1 && opnum <= 3)) { // if option is not valid, print error message and return to menu
        printf("Rick this option is not known to the daycare ! \n");
        return success;
    }
    else {
        // print all jerries
        if (opnum == 1) {
            int *listlen = getLengthList(jerrieslist);
            if (*listlen == 0) {
                printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                free(listlen);
                return success;
            }
            free(listlen);
            if (displayList(jerrieslist) != success)
                return failure;
            return success;
        }
        // print jerries by physical characteristic
        if (opnum == 2) {
            printf("What physical characteristics ? \n");
            char phyinput[bufsize];
            scanf("%s", phyinput);
            if (lookUpInMultiValueHashTable(multihash, phyinput) == NULL) {
                // if there are no jerries with this physical characteristic
                printf("Rick we can not help you - we do not know any Jerry's %s ! \n", phyinput);
                return success;
            }
            if (displayMultiValueHashElementsByKey(multihash, phyinput) != success)
                return failure;
            return success;
        }
        if (opnum == 3) {
            // print all planets
            for (int i = 0; i < numofplanets; i++)
                if (printPlanet(planetsarr[i]) != success)
                    return failure;
            return success;
        }
    }
}

status jerriesPlayCase8(LinkedList jerrieslist) {
    // if there are no jerries
    int *listlen = getLengthList(jerrieslist);
    if (*listlen == 0) {
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        free(listlen);
        return success;
    }
    // if there are jerries in system, print options
    printf("What activity do you want the Jerries to partake in ? \n1 : Interact with fake Beth \n2 : Play golf \n3 : Adjust the picture settings on the TV \n");
    // choose option
    char option[bufsize];
    scanf("%s", option);
    int opnum = (int)strtol(option, NULL, 10);
    if (!(opnum >= 1 && opnum <= 3)) { // if option is not valid, print error message and return to menu
        printf("Rick this option is not known to the daycare ! \n");
        free(listlen);
        return success;
    }
    else { // valid option
        Jerry *curjerry;
            // iterating through all jerries
            for (int i = 1; i <= *listlen; i++) {
                curjerry = getDataByIndex(jerrieslist, i);
                if (opnum == 1) { // interact with beth
                // when happiness level is equal/above 20
                if (curjerry->happy >= 20) {
                    curjerry->happy = min(curjerry->happy+15, 100);
                }
                else {
                    // when happiness level is below 20
                    curjerry->happy = max(curjerry->happy-5, 0);
                    }
                }
                if (opnum == 2) { // play golf
                    // when happiness level is equal/above 50
                    if (curjerry->happy >= 50) {
                        curjerry->happy = min(curjerry->happy+10, 100);
                    }
                    else {
                        // when happiness level is below 50
                        curjerry->happy = max(curjerry->happy-10, 0);
                    }
                }
                if (opnum == 3) { // adjust tv
                    // adding 20 points to happiness level
                    curjerry->happy = min(curjerry->happy+20, 100);
                }
        }
        free(listlen);
        // print all jerries after update
        printf("The activity is now over ! \n");
        if (displayList(jerrieslist) != success)
            return failure;
        return success;
    }
}

int main (int argc, char* argv[]) {
    /* setting an array that stores all the planets */
    int numofplanets = (int)strtol(argv[1], NULL, 10);
    Planet** planetsarr;
    planetsarr = (Planet**)malloc(numofplanets*sizeof(Planet *));

    // this is going to be the size of the hash. the number of lines is number of jerries + number of total physical characteristics.
    // it is chosen as the size of the hash because the goal is to find a jerry in o(1) average time. if all the lines are jerries (worst case), this is the optimal
    // size in order to create the most spread distribution in hash table of jerries. if there is one jerry and all the other lines are physical characteristics, this is the optimal
    // size for multi value hash of physical characteristics. if there are not many lines, set the size to be 11, which is a prime number that will deliver spread division.
    int hashNum = max(sizeOfHash(argv[2]), 11);

    // creating a multi value hash table, hash table and linked list, in order to store information from the file: in the hash table the keys are jerry ids and the
    // values are pointers to actual jerries. in the multi value hash table, the keys are physical characteristics names and the values are
    // linked lists of the jerries who has the physical characteristic. the linked list is going to store the jerries in the order they were added to the system.
    MultiValueHash mvh = createMultiValueHashTable((CopyFunction)copyStr, (FreeFunction)freeStr, (PrintFunction)printStr, (CopyFunction)copyJerry, (FreeFunction)setNull, (PrintFunction)printJerry,
                              (EqualFunction)isSameKey, (EqualFunction)isSameJerry, (TransformIntoNumberFunction)toNumber, hashNum);
    hashTable hs = createHashTable((CopyFunction)copyStr, (FreeFunction)freeStr, (PrintFunction)printStr, (CopyFunction)copyJerry, (FreeFunction)setNull, (PrintFunction)printJerry, (EqualFunction)isSameKey, (TransformIntoNumberFunction)toNumber, hashNum);
    LinkedList jerrieslist = createLinkedList((CopyFunction)copyJerry, (FreeFunction)destroyJerry, (PrintFunction)printJerry, (EqualFunction)isSameJerry);

    /* if there was a problem with memory allocation of ADTs, the program prints memory problem, frees all memory created and exit */
    if (!planetsarr || !mvh || !hs || !jerrieslist) {
        printf("Memory Problem \n");
        deleteMemory(numofplanets, planetsarr, jerrieslist, mvh, hs);
        return 0;
    }

    /* reading the file using readCurrentFile function. if the function returns failure, it means there was a problem in the memory allocation,
     * and the program prints error message, cleans memory and exits.
   * if success, it means the file was read and the information was stored properly */
    if (readCurrentFile(numofplanets, planetsarr, jerrieslist, mvh, hs, argv[2]) == failure) {
        printf("Memory Problem \n");
        deleteMemory(numofplanets, planetsarr, jerrieslist, mvh, hs);
        return 0;
    }
    else /*if reading the file was successful, proceed to menu */
        {
            /* main: after reading the file and storing the data, the system prints menu for the user and takes requests for functions. */
            char *opstr; /* string input option */
            opstr = (char *) malloc(bufsize);
            int opnum = -1; /* variable that is used to convert the option to integer so it can be used in switch case */
            status isSucceed = nullpointer; /* status of current function the user chose. */
            while (opnum != 9 && isSucceed != failure) { /* breaking out of the switch case when the user chooses to exit or when a function fails */
                printMenu(); // printing menu
                scanf("%s", opstr);
                opnum = (int)strtol(opstr, NULL, 10);
                if (!(opnum >= 1 && opnum <= 9) && strlen(opstr) == 1) /* if option is valid, it's converted it to integer */
                    opnum = -1;
                switch (opnum) {
                    case 1:   /* take this jerry away from me */
                        isSucceed = takeJerryCase1(numofplanets, planetsarr, jerrieslist, hs);
                        break;

                    case 2: /* i think i remember something about my jerry */
                        isSucceed = somethingAboutJerryCase2(hs, mvh);
                        break;
                    case 3: /* oh wait. that can't be right */
                        isSucceed = ohWaitCase3(hs, mvh);
                        break;

                    case 4: /* i guess i will take my jerry back now */
                        isSucceed = takeJerryBackCase4(jerrieslist, hs, mvh);
                        break;

                    case 5: /* i can't find my jerry. just give me a similar one */
                        isSucceed = similarJerryCase5(jerrieslist, hs, mvh);
                        break;

                    case 6: /* i lost a bet. give me your saddest jerry */
                        isSucceed = saddestJerryCase6(jerrieslist, hs, mvh);
                        break;

                    case 7: /* show me what you got */
                        isSucceed = showMeCase7(numofplanets, planetsarr, jerrieslist, mvh);
                        break;

                    case 8: /* let the jerries play */
                        isSucceed = jerriesPlayCase8(jerrieslist);
                        break;

                    case 9: /* i had enough. close this place */
                        break;

                    default: /* if input is invalid */
                        printf("Rick this option is not known to the daycare ! \n");
                }
            }
            free(opstr);
            // if option 9 was chosen last, it means the user chose to quit the system. if a different option was chosen, it means the case loop ended due to
            // failure in a function, therefore there was a memory allocation problem
            if (opnum == 9)
                printf("The daycare is now clean and close ! \n");
            else
                printf("Memory Problem \n");
        }
        /* the program deletes all the memory created properly and exits */
        deleteMemory(numofplanets, planetsarr, jerrieslist, mvh, hs);
        return 0;
}