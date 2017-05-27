#ifndef TAB
#define TAB
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>




typedef struct tabl{
	char* titre;

	int L,H, l, X,Y, Lchar,Hchar;
	char *** data;
	char **tab;
} Tableau;

typedef Tableau Label;

// créer le tableau
Tableau* createTableau(int Largeur, int Hauteur, int LargeurCase, char* titre);
Tableau* createShmTableau(int Largeur, int Hauteur, int LargeurCase, char* titre, int shmId);

// créer une zone de text
Label* createLabel(char* text,int X,int Y); // X et Y sont la position sur la console


//accès aux cases
void setData(Tableau *this, int x,int y,char* str);
char* getData(Tableau *this, int x,int y);
void setPos(Tableau *tab, int X, int Y);



#endif
