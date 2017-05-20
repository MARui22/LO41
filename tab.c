#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tab.h"


#define FOR(p, F) for(int p = 0; p<F; ++p)

#define FIRST (int)0
#define MID 1
#define END	2

// launch with: gcc -std=c99 -o tab tab.c

// typedef struct {
//	 char* titre;
//
//	 int L,H, l, X,Y, Lchar,Hchar;
//	 char *** data;
//	 char **tab;
// } Tableau;

void pint(int t, char* c)
{
	printf("%s : %d\n",c,t);
}

void genereDataVoid(Tableau *this){
	this->data = malloc(this->H*sizeof(char**));

	for(int y = 0; y < this->H; ++y){
		this->data[y] = malloc(this->L*sizeof(char*));
		FOR(x,this->L){
			this->data[y][x] = malloc((this->l+1)*sizeof(char));
			FOR(z, this->l)
				this->data[y][x][z] = ' ';
			//printf("%d", (int)strlen(this->data[y][x]));
		}
	}
}

void genereLine(Tableau *this,char* tab, int pos){
	char *gauche;
	char *mid ;
	char *droit;
	char *cross;


	if(pos == FIRST){
		gauche = "\u2554";
		mid = "\u2550";
		cross = "\u2566";
		droit = "\u2557";
	}else if(pos == MID){
		gauche = "\u2560";
		mid =    "\u2550";
		cross = "\u256C";
		droit =  "\u2563";
	}else{
		gauche = "\u255A";
		mid =    "\u2550";
		cross =  "\u2569";
		droit =  "\u255D";
	}

	int p = 0, x, xl;
	for(x = 0; x < this->L; ++x){
		p = x*(this->L + 1);
		if(x)
			strcat(tab, cross);
		else
			strcat(tab, gauche);

		FOR(xl,this->l)
			strcat(tab, mid);


	}
	strcat(tab, droit);
}

void genereDataLine(Tableau *this, int y)
{
	int ychar = 2*y+1;
	FOR(x,this->L){
		strcat(this->tab[ychar], "\u2551");
		 strcat(this->tab[ychar], this->data[y][x]);
	}
	strcat(this->tab[ychar], "\u2551");
}

void genere(Tableau *this)
{
	int ychar;
	FOR(y,this->H){
		ychar = 2*y;

		if(y)
			genereLine(this,this->tab[ychar], MID);
		else
			genereLine(this,this->tab[ychar], FIRST);

		genereDataLine(this,y);
	}
	genereLine(this,this->tab[this->Hchar-1], END);
	// strcat(this->tab[0], "\u2557");

}

void setData(Tableau *this, int x,int y,char* str)
{
	if(this->L <= x || this->H <= y){
		printf("\ntu palce tes data hors des cases du tableau !!!\n");
		return;
	}
	int endStr = 0, xl = 0;
	while(xl < this->l && str[xl] != '\0'){
		this->data[y][x][xl] = str[xl++];
	}
	while(xl < this->l){
		this->data[y][x][xl++] = ' ';
	}

	int ydata = 2*y+1;
	strcpy(this->tab[ydata], "");
	genereDataLine(this,y);

	//genere(this);
}

void setPos(Tableau *tab, int X, int Y)
{
	tab->X = X;
	tab->Y = Y;
}

char* getData(Tableau *this, int x,int y)
{
		if(this->L <= x || this->H <= y){
		printf("\ntu cherche tes data hors des cases du tableau !!!\n");
		return "";
	}
	int z = this->l;
	while(this->data[y][x][z] == ' ' || this->data[y][x][z] == '\0')
		--z;
	++z;
	char* str = malloc((z+1)*sizeof(char));
	FOR(i,z)
		str[i] = this->data[y][x][i];

	return str;

}

Tableau* createTableau(int Largeur, int Hauteur, int LargeurCase, char* titre)
{
	Tableau *tab = malloc(sizeof(Tableau));
	
	tab->titre = titre;
	tab->H = Hauteur;
	tab->L = Largeur;
	tab->l = LargeurCase;
	tab->Hchar = (2*tab->H +1);
	tab->Lchar = ((tab->l+1)*tab->L +1)*3+1;
	tab->X = 0;
	tab->Y = 0;

	tab->tab = malloc(tab->Hchar*sizeof(char*));
	FOR(y,tab->Hchar)
		tab->tab[y] = malloc(tab->Lchar*sizeof(char));


	genereDataVoid(tab);
	genere(tab);

	return tab;

}



 // void main()
 // {
	 // Tableau* tab = createTableau(7,4,20, "HO ! le beau tableau !!");
////	 printf("%d\n", tab->Hchar);
//	 setData(tab, 3,6,"0123456789");
//	  setData(tab, 3,6,"456");
//
//	 if (tab->titre)
//		 printf("\n%s\n", tab->titre);

	 // FOR(i,tab->Hchar)
		 // printf("%s\n", tab->tab[i]);

	 // printf("%lu\n", sizeof("123"));
//	 printf("%s\n", tab.tab[1]);
//	 printf("%s\n", tab.tab[2]);

 // }
