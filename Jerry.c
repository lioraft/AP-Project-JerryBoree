#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Jerry.h"
#include "Defs.h"

/* an array of existing planets, and the number of planets it holds */
char **planets = NULL;
int numofPlanets = 0;

/* helper function that iterates over the array of planet names and returns whether the planet
 * already exists or not. if exists, returns true. otherwise, returns false. */
bool searchPlanet(char *name) {
    for (int i = 0; i < numofPlanets; i++) {
        if (strcmp(planets[i], name) == 0)
            return true;
    }
    return false;
}

/* this function takes in the name of the planet as a string and its coordinates (x,y,z) as doubles.
 * if the planet doesn't already exist, it creates an object of type Planet, allocates memory for its variables,
 * initializes the input and returns a pointer to the object. if the planet exists, if can't allocate memory, or if one of the arguments is NULL, the function returns null. */
Planet* initplanet(char *name, double x, double y, double z) {
    /* if pointer is null the function returns null */
    if (name == NULL) {
        return NULL;
    }
    if (!searchPlanet(name)) {
        /* if not exist, initialize the planet */
        Planet *pplanet;
        pplanet = (Planet *) malloc(sizeof(Planet));
        /* if can't allocate, returns null */
        if (!pplanet) {
            return NULL;
        }
        /* allocating memory for name and setting all variables properly */
        pplanet->name = (char *) malloc(strlen(name)+1);
        /* if can't allocate, frees memory and returns null */
        if (!pplanet->name){
            free(pplanet);
            return NULL;
        }
        strcpy(pplanet->name, name);
        pplanet->x = x;
        pplanet->y = y;
        pplanet->z = z;
        /* adding the planet to list of planets and increasing the counter by 1. if list isn't initialized, function initializes it.
         * if already initialized, function increases the memory. */
        if (numofPlanets == 0) {
            planets = (char **) malloc(sizeof(char *));
            /* if can't allocate, frees memory and returns null */
            if (!planets) {
                free(pplanet->name);
                free(pplanet);
                return NULL;
            }
            planets[0] = pplanet->name;
        }
        else {
            planets = (char **) realloc(planets, (numofPlanets+1)*sizeof(char *));
            /* if can't allocate, frees memory and returns null */
            if (!planets){
                free(pplanet->name);
                free(pplanet);
                return NULL;
            }
            planets[numofPlanets] = pplanet->name;
        }
        numofPlanets++;
        return pplanet;
    }
    else
        return NULL;
}

/* this function takes in a pointer of a planet and the name of its dimension as a string.
 * it creates origin, allocates memory for its variables,
 * initializes the input and returns a pointer to the object. if one of the arguments is NULL, or if can't allocate memory, it returns null. */
Origin* initorigin(Planet* pplanet, char *dim) {
    /* if one of the pointers is null the function returns null */
    if (pplanet == NULL || dim == NULL) {
        return NULL; }
    /* initializing object */
    Origin *porigin;
    porigin = (Origin *) malloc(sizeof(Origin));
    /* if can't allocate, returns null */
    if (!porigin){
        return NULL;
    }
    /* allocating memory for dimension and setting it properly */
    porigin->dim = (char *) malloc(strlen(dim)+1);
    /* if can't allocate, frees memory returns null */
    if (!porigin->dim){
        free(porigin);
        return NULL;
    }
    strcpy(porigin->dim, dim);
    /* setting the planet pointer */
    porigin->pl = pplanet;
    return porigin;
}

/* this function takes in the name of the physical characteristic as a string and its value as a double.
 * it creates an object of type Physical, allocates memory for its variables,
 * initializes the input and returns a pointer to the object. if one of the arguments is NULL, or if can't allocate memory, returns null. */
Physical* initphysical(char *name, double val) {
    /* if pointer is null the function returns null */
    if (name == NULL) {
        return NULL; }
    /* initializing object */
    Physical* pphysical;
    pphysical = (Physical *) malloc(sizeof(Physical));
    /* if can't allocate, returns null */
    if (!pphysical){
        return NULL;
    }
    /* allocating memory for name and setting it properly */
    pphysical->name = (char *) malloc(strlen(name)+1);
    /* if can't allocate, frees memory and returns null */
    if (!pphysical->name) {
        free(pphysical);
        return NULL;
    }
    strcpy(pphysical->name, name);
    /* setting value */
    pphysical->val = val;
    return pphysical;
}

/* this function takes in id, dimension, planet and happiness level.
 * it creates an object of type Jerry, allocates memory for its variables,
 * initializes the input and returns a pointer to the object. if one of the arguments is NULL, or if can't allocate memory, returns null. */
Jerry* initjerry(char *id, char *dim, Planet* pplanet, int happy) {
    /* if one of the pointers is null the function returns null */
    if (id == NULL || pplanet == NULL || dim == NULL) {
        return NULL; }
    /* initializing object */
    Jerry* pjerry;
    pjerry = (Jerry *) malloc(sizeof(Jerry));
    /* if can't allocate, returns null */
    if (!pjerry) {
        return NULL;
    }
    /* allocating memory for name and setting it properly */
    pjerry->id  = (char *) malloc(strlen(id)+1);
    /* if can't allocate, frees memory and returns null */
    if (!pjerry->id) {
        free(pjerry);
        return NULL;
    }
    strcpy(pjerry->id, id);
    /* setting happiness level */
    pjerry->happy = happy;
    /* creating the origin by using its init function and setting it properly */
    Origin *orig;
    orig = initorigin(pplanet, dim);
    /* if can't allocate, it already freed memory and printed message in init origin, so only returns null */
    if (!orig) {
        return NULL;
    }
    pjerry->orig = orig;
    /* setting list of physical characteristics as NULL when initializing */
    pjerry->phyarr = NULL;
    /* current number of physical characteristic is 0 */
    pjerry->numofphy = 0;
    return pjerry;
}

/* this function takes in a jerry and a name of physical characteristic and its value, and adds the physical characteristic
 * to the jerry's array of physical characters. if one of the arguments is NULL, the function returns nullpointer.
 * if can't allocate memory, returns failure and prints memory allocation error. if succeeds, returns success. */
status addPhysical(Jerry* pjerry, char *name, double val) {
    if (pjerry == NULL || name == NULL) {
        return nullpointer;
    }
    else {
        Physical * pphysical;
        /* creating new physical characteristic */
        pphysical = initphysical(name, val);
        /* null variables were already tested. so if struct is null, it means there was a memory allocation problem, and the function returns failure
         * (it already printed message in physical init) */
        if (!pphysical) {
            return failure;
        }
        /* reallocating the memory for jerry's physical characteristics */
        pjerry->phyarr = (Physical **) realloc(pjerry->phyarr, sizeof(Physical*) * (pjerry->numofphy + 1));
        /* if can't allocate, returns failure */
        if (!pjerry->phyarr) {
            return failure;
        }
        /* setting the new physical characteristic and increasing counter by 1 */
        pjerry->phyarr[pjerry->numofphy] = pphysical;
        pjerry->numofphy++;
        return success;
    }
}


/* this function takes in a jerry and a name of physical characteristic, and if possible it deletes the
 * physical characteristic from the jerry's array of physical characters. if one of the arguments is NULL, returns nullpointer.
 * if can't allocate memory, returns failure. if deletes properly, returns success. */
status deletePhysical(Jerry* pjerry, char *phy) {
    if (pjerry == NULL || phy == NULL) {
        return nullpointer;
    }
    else {
        for (int i = 0; i < pjerry->numofphy; i++) {
            /* searching the specified characteristic */
            if (strcmp(pjerry->phyarr[i]->name, phy) == 0) {
                /* deleting the characteristic and advancing all the physical characteristics after the deleted one, so it can decrease the size of the array safely */
                destroyPhysical(pjerry->phyarr[i]);
                for (int k = i; k < pjerry->numofphy - 1; k++)
                    pjerry->phyarr[k] = pjerry->phyarr[k + 1];
                /* decreasing number of characteristic and reallocate the memory */
                pjerry->numofphy--;
                pjerry->phyarr = (Physical **) realloc(pjerry->phyarr, sizeof(Physical*) * (pjerry->numofphy));
                /* if can't allocate, returns failure */
                if (!pjerry->phyarr && pjerry->numofphy > 0) {
                    return failure;
                }
            }
        }
    }
    return success;
}

/* printing the variables of a jerry. if pointer is NULL, returns nullpointer. if can print jerry, returns success. */
status printJerry(Jerry* pjerry) {
    if (pjerry == NULL) {
        return nullpointer;
    }
    else {
        printf("Jerry , ID - %s : \nHappiness level : %d \nOrigin : %s \n", pjerry->id, pjerry->happy,
               pjerry->orig->dim);
        printPlanet(pjerry->orig->pl);
        if (pjerry->numofphy != 0) {
            printf("Jerry's physical Characteristics available : \n\t");
            for (int i = 0; i < pjerry->numofphy - 1; i++)
                printf("%s : %.2f , ", pjerry->phyarr[i]->name, pjerry->phyarr[i]->val);
            printf("%s : %.2f \n", pjerry->phyarr[pjerry->numofphy - 1]->name,
                   pjerry->phyarr[pjerry->numofphy - 1]->val);
        }
        return success;
    }
}

/* printing the variables of a planet. if pointer is NULL, returns nullpointer. if can print jerry, returns success. */
status printPlanet(Planet* pplanet) {
    if (pplanet == NULL) {
        return nullpointer;
    }
    else {
        printf("Planet : %s (%.2f,%.2f,%.2f) \n", pplanet->name, pplanet->x, pplanet->y, pplanet->z);
        return success;
    }
}

/* destroying objects */

/* destroying a planet - this function takes in a pointer of a planet, releases its memory and removes it from list of existing planets.
 * if pointer is NULL, returns nullpointer. if can't delete, returns failure. if can delete planet, returns success. */
status destroyPlanet(Planet* pplanet) {
    if (pplanet == NULL) {
        return nullpointer;
    }
    else {
        /* searches for planet in list of planets */
        for (int i = 0; i < numofPlanets; i++) {
            if (strncmp(pplanet->name, planets[i], strlen(pplanet->name)) == 0) {
                /* deleting the planet and advancing all the planets after the deleted one, so it can decrease the size of the array safely */
                for (int k = i; k < numofPlanets - 1; k++) {
                    planets[k] = planets[k+1];
                }
                /* decreasing number of planets and reallocate the memory */
                numofPlanets--;
                planets = (char **) realloc(planets, numofPlanets * sizeof(char *));
                /* if can't allocate, returns failure */
                if (!planets && numofPlanets > 0) {
                    return failure;
                }
            }
        }
        free(pplanet->name);
        free(pplanet);
        return success;
    }
}

/* destroying an origin - this function is used by the jerry of the origin. it releases the memory of the origin
 * if pointer is NULL, returns nullpointer. if can delete origin, returns success. */
status destroyOrigin(Origin* porigin) {
    if (porigin == NULL) {
        return nullpointer;
    }
    else {
        free(porigin->dim);
        free(porigin);
        return success;
    }
}

/* destroying a physical characteristic - this function is used by the jerry who possess the physicaL characteristic. it releases the memory of the characteristic.
 * if pointer is NULL, returns nullpointer. if can delete origin, returns success. */
status destroyPhysical(Physical* pphysical) {
    if (pphysical == NULL) {
        return nullpointer;
    }
    else {
        free(pphysical->name);
        free(pphysical);
        return success;
    }
}

/* destroying a jerry - this function destroys a jerry. it releases the memory of his variables and then releases its pointer.
 * if pointer is NULL, returns nullpointer. if can delete the jerry, returns success. */
status destroyJerry(Jerry* pjerry) {
    if (pjerry == NULL) {
        return nullpointer;
    }
    else {
        free(pjerry->id);
        destroyOrigin(pjerry->orig);
        for (int i = 0; i < pjerry->numofphy; i++)
            destroyPhysical(pjerry->phyarr[i]);
        free(pjerry->phyarr);
        free(pjerry);
        return success;
    }
}

// function that takes in a pointer to a jerry, and returns it. will be used to shallow copy jerries.
Jerry* copyJerry(Jerry* pjer) {
    return pjer;
}

// function that takes in 2 jerries, and returns if they are the same jerry by comparing their IDs.
// if identical, returns true. else, returns false.
bool isSameJerry(Jerry* pjer1, Jerry* pjer2) {
    if (pjer1 == NULL || pjer2 == NULL)
        return false;
    if (strcmp(pjer1->id, pjer2->id)== 0)
        return true;
    return false;
}