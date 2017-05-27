#ifndef GUI
#define GUI
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tab.h"

extern void pint(int t, char* c);

void draw(Tableau**T, int nbT);	//dessine les nbT tableaux de T

#endif