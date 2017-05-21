#ifndef TAB
#define TAB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct tabl{
	char* titre;

	int L,H, l, X,Y, Lchar,Hchar;
	char *** data;
	char **tab;
} Tableau;

typedef Tableau Label;

// créer le tableau
Tableau* createTableau(int Largeur, int Hauteur, int LargeurCase, char* titre);

Label* createLabel(char* text,int X,int Y);
//accès aux cases
void setData(Tableau *this, int x,int y,char* str);
char* getData(Tableau *this, int x,int y);
void setPos(Tableau *tab, int X, int Y);



#endif
