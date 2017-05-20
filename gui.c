#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tab.h"



#define FOR(p, F) for(int p = 0; p<F; ++p)
#define IN(infInc, val, maxExc) infInc <= val && val < maxExc

// launch with : gcc -std=c99 -o gui tab.h gui.c tab.c

extern void pint(int t, char* c);
// {
	// printf("%s : %d\n",c,t);
// }
char** screen = NULL;

int cmpfunc (const void * a, const void * b)
{
   return  ( *(int*)b - *(int*)a ); 
}

int countScreenChar(char* str)
{
	int c = 0, i = 0;
	// printf("\n%d", str[i]);
	while(str[i] != '\0')
	{	//printf("\n%d", str[i]);
		if(str[i++] < 0)
			i+=2;
		++c;
	}
	return c;
}

int getFromScreenPosChar(char* str, int pos)
{
	int c = 0, ind = 0;
		pint(pos, "position screen");
	FOR(x, pos){
		//printf("\n%d", str[c]);
		if(str[c] < 0)
			c+=3;
		else
			c++;
		
	}
	pint(c, "position char");
	return c;
}

void Insert(char* c, Tableau* tab,int y)
{
	int line = y - tab->Y;
	char* str;
	
	if(line <0)
		str = tab->titre;
	else
		str = tab->tab[line];
	
	pint(strlen(c), "largeur screen");
	//pint(getFromScreenPosChar(c,tab->X),"pos à atteindre");
	int max = getFromScreenPosChar(c,tab->X);
	for(int x = strlen(c); x < max; ++x)
		if(c[x] == '\0')
			strcat(c," ");
	pint(strlen(c), "largeur screen atteinte");

	
	FOR(i,strlen(str))
		c[i+max] = str[i];

		
	//pint(strlen(str), screen[y+1]);
}

void draw(Tableau**T, int nbT)
{

	int maxXFREE[nbT],  maxY[nbT], Largest = 0;
	FOR(t, nbT){
		maxY[t] = T[t]->Y + T[t]->Hchar;
		maxXFREE[t] = T[t]->X + T[t]->Lchar;
		Largest += T[t]->Lchar;
		if(T[t]->titre)
			++maxY[t];
	}
	qsort(maxXFREE, nbT, sizeof(int), cmpfunc);
	qsort(maxY, nbT, sizeof(int), cmpfunc);
	
	if (!screen)
	{
		screen = malloc(maxY[0]*sizeof(char*));	
		
		FOR(y,maxY[0])
			screen[y] = malloc((Largest + maxXFREE[0])*sizeof(char));
	}
	// int x;
	// pint(maxY[0], "max long screen");
	// pint(T[1]->Y, "pos tab2 Y");
	// pint(T[1]->Y + T[1]->Hchar, "fintab2 Y");
	
	
	
	FOR(y,maxY[0]){
		FOR(t,nbT){
			 //printf("\ny = %d t = %d", y,t);
			if(T[t]->titre){
				
				if(IN(T[t]->Y, y, T[t]->Y + T[t]->Hchar +1))	//si ce tableau est dans cette ligne
				{
					Insert(screen[y], T[t],y-1);
				}					
			}
			else{	//idem, mais sans titre
				if(IN(T[t]->Y, y, T[t]->Y + T[t]->Hchar))
					Insert(screen[y], T[t],y);		
			}
			
			
		}
		
	}
	
	// if (T[0]->titre)
		// printf("\n%s\n", T[0]->titre);
	
	FOR(i,maxY[0]){
		printf("\n%s", screen[i]);
	}
	printf("\n\n");
}

void main()
{
	Tableau **T = malloc(2*sizeof(Tableau*));
	Tableau *tab1 = createTableau(3,5,3,"Semaphore");
	Tableau *tab2 = createTableau(4,5,3,"BLBLBLBLBL");
	setPos(tab2, 30,8);
	
	T[0] = tab1;
	T[1] = tab2;
	
	setData(tab1, 2,3,"0123456789");
	setData(tab2, 2,1,"456");
	draw(T, 2);
	//pint(countScreenChar("\u2550\u2556\n\t "), "un \\ ?");
}