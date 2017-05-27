#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tab.h"
#include "gui.h"




#define FOR(p, F) for(int p = 0; p<F; ++p)
#define IN(infInc, val, maxExc) infInc <= val && val < maxExc

// launch with : gcc -std=c99 -o gui tab.h gui.c tab.c

int degub = 0;
void debug()
{
	printf("%d\n", degub++);
}

char** screen = NULL;
int HeightScreen = 0; //HeightScreen est le nombre de lignes verticale de l'affichage

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
	FOR(x, pos){
		//printf("\n%d", str[c]);
		if(str[c] < 0)
			c+=3;
		else
			c++;
		
	}
	return c;
}

void Add2End(char* target, char* arrow,int posChar)
{
	int charLenArrow = strlen(arrow), CharLenTarget = strlen(target);
	
	for(int x = CharLenTarget; x < posChar; ++x)
		if(target[x] == '\0')
			strcat(target," ");


	FOR(i,charLenArrow)
		target[i+posChar] = arrow[i];
}

void RecouvreCharInLine(char* Target, char* arrow,int posChar)
{	
	if(strlen(Target) <1)
		return;	
	
	int CharLenTarget = strlen(Target), charLenArrow = strlen(arrow);
	int screenLenArrow = countScreenChar(arrow);
		
	int index = posChar;
	
	//on compte le nombre de char à être écrasés
	FOR(osef,screenLenArrow)
	{
		if(Target[index] < 0)
			index+=3;
		else
			index++;
	}	
	int toDel = index-posChar;
	int nbr_case_to_decal = charLenArrow-toDel;
	
	
		//on décale les char
	for(int x = CharLenTarget-1; x>=posChar; --x)
	{
		Target[x+nbr_case_to_decal] = Target[x];
	}

		//on écrit dessus
	FOR(X,charLenArrow)
	{
		Target[X+posChar] = arrow[X];
	}	
}

void Insert(char* c, Tableau* tab,int y)
{
	int line = y - tab->Y;
	char* str;

	if(line <0)
		str = tab->titre;
	else
		str = tab->tab[line];
	//pint(line,"ligne du tab");
	// debug();
	//pint(getFromScreenPosChar(c,tab->X),"pos à atteindre");
	int posTab = getFromScreenPosChar(c,tab->X), lenC = strlen(c);
	
	if(lenC-1 >= posTab)
		RecouvreCharInLine(c, str, posTab);
	else
		Add2End(c, str, posTab);
	
}

void deleteScreen(){
	
}

void draw(Tableau**T, int nbT)
{
	//debug();
	if (screen) //on nétoie l'écran
	{
		FOR(y,HeightScreen)
			free(screen[y]);
		free(screen);
	}
	
	int maxXFREE[nbT],  maxY[nbT], Largest = 0;
	FOR(t, nbT){
		maxY[t] = T[t]->Y + T[t]->Hchar;
		maxXFREE[t] = T[t]->X;
		Largest += T[t]->Lchar;
		if(T[t]->titre)
			++maxY[t];
	}
	qsort(maxXFREE, nbT, sizeof(int), cmpfunc);
	qsort(maxY, nbT, sizeof(int), cmpfunc);
	HeightScreen = maxY[0];
	
		

	screen = malloc(HeightScreen*sizeof(char*));	
	
	FOR(y,maxY[0])
		screen[y] = calloc(10*(Largest + maxXFREE[0]),sizeof(char));

	// int x;
	// pint(maxY[0], "max long screen");
	// pint(T[1]->Y, "pos tab2 Y");
	// pint(T[1]->Y + T[1]->Hchar, "fintab2 Y");
	
	
	//debug();
	FOR(y,maxY[0]){
		FOR(t,nbT){
			 //printf("\ny = %d t = %d", y,t);
			if(T[t]->titre){
				
				if(IN(T[t]->Y, y, T[t]->Y + T[t]->Hchar +1))	//si ce tableau est dans cette ligne
				{//debug();
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

// void main()
// {
	// const int nbTableaux = 2;
	// Tableau **T = malloc(nbTableaux*sizeof(Tableau*));
	// Tableau *tab1 = createTableau(5,5,2,"Semaphore");
	// Tableau *tab2 = createTableau(5,5,1,"BLBLBLBLBL");
		
	// Label * lab = createLabel("blblb\nfesi", 0,0);
	// setPos(tab2, 10,0);
	
	// Tableau *tab3 = createTableau(5,5,1,"tab3");
	// setPos(tab3, 10,13);
	
	// T[0] = lab;
	// T[2] = tab2;
	// T[1] = tab2;
	
	// setData(tab1, 2,3,"0123456789");
	// setData(tab2, 2,1,"456");
	// draw(T, nbTableaux);
	// pint(countScreenChar("\u2550\u2556\n\t "), "un \\ ?");
// }