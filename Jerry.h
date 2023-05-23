#ifndef HW3_JERRY_H
#define HW3_JERRY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h" /*definitions header*/

/* definition of a planet */
typedef struct Planet_t {
    char *name;
    double x;
    double y;
    double z;
}Planet;

/* definition of an origin */
typedef struct Origin_t {
    Planet *pl;
    char *dim;
}Origin;

/* definition of a physical characteristic */
typedef struct Physical_t {
    char *name;
    double val;
}Physical;

/* definition of a jerry */
typedef struct Jerry_t {
    char *id;
    int happy;
    Origin *orig;
    Physical **phyarr;
    int numofphy;
}Jerry;

/* functions of creating objects - if creating objects succeed, returns pointer. if not, returns null */
Planet* initplanet(char *, double, double, double);
Origin* initorigin(Planet*, char *);
Physical* initphysical(char *, double);
Jerry* initjerry(char *, char *, Planet*, int);

/* add and delete physical characteristics of a jerry */
status addPhysical(Jerry*, char *, double);
status deletePhysical(Jerry*, char *);

/* functions for printing jerries and planets */
status printJerry(Jerry*);
status printPlanet(Planet*);

/* helper function - searches if a planet exist */
bool searchPlanet(char *);

/* destroy objects and free memory */
status destroyPlanet(Planet*);
status destroyOrigin(Origin*);
status destroyPhysical(Physical*);
status destroyJerry(Jerry*);

/* shallow copy function of jerry */
Jerry* copyJerry(Jerry*);

/* equal function of jerry */
bool isSameJerry(Jerry*, Jerry*);

#endif //HW3_JERRY_H
