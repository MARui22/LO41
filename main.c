#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "./gui/gui.h"
#include "const.h"

#define FOR(p, F) for(int p = 0; p<F; ++p)


Tableau** initWorld();
void finish(int i);


/*#define NBDRONES 3*/
/*#define LARGEUR_VAISSEAU 15*/
/*#define PROFONDEUR_SOUTE_VAISSEAU 2*/
/*#define LARGEUR_ID_COLIS 4*/
/*#define GENERAL_OFFSET_LEFT 10*/
	/**/
	/*int nbTableaux = 0; //6 + nombre de drones	*/
	/*int drone_Y_atterissage, drone_Y_voyage, drone_Y_livraison, drone_Y_dead;*/
/**/
  int shmDY[NBDRONES]; //liste des mémoires partagées de la hauteur des drones*/
  int shmDC[NBDRONES]; //liste des mémoires partagées du chargement des drones
  char* dataDrone[NBDRONES];  //Liste des identifants colis des drones en temps réel
  Tableau **T;
  
  int nbDroneTravail = NBDRONES; // !!!!!!!!!!!!!!!!!!!!!!!! en nominial, ça vaut le nombre de drones !

void drawUnivers(int i)
{
   signal(SIGUSR1, SIG_IGN);
   
  
  FOR(x, NBDRONES)
    setData(T[x], 0,0, dataDrone[x]);
  
  draw(T,nbTableaux);  
  signal(SIGUSR1, drawUnivers);
}

void finTravail(int i)
{
  signal(SIGUSR2, finTravail); 
  
  --nbDroneTravail;
  
}

char* itoa(int i){
  char* str = calloc(12, sizeof(char));
  sprintf(str, "%d", i);
  return str;
}

void main()
{	  
  FOR(x,NBDRONES)
	{
		shmDY[x] = shmget(IPC_PRIVATE, sizeof(int), 0666|IPC_CREAT);
    if(shmDY[x] <0)
      puts("echec creation memoire partagee pour les drones");
    
    shmDC[x] = shmget(IPC_PRIVATE, (1+LARGEUR_ID_COLIS)*sizeof(char), 0666|IPC_CREAT);
    if(shmDC[x] <0)
      puts("echec creation memoire partagee pour les drones");
	}
  
	T = initWorld();	//dessine l'univer
	draw(T, nbTableaux);
   
  
  pid_t pid[NBDRONES];
  FOR(x, NBDRONES){
    pid[x]= fork();
    if(pid[x] == 0){ //Fils n°x ------------------- RECOUVREMENT DRONE ------------------------------//
      execlp("drone/drone.elf", "drone.elf", itoa(shmDY[x]), itoa(shmDC[x]), (char*)0);
      exit(5);
    }
  }
  
  
  signal(SIGUSR1, drawUnivers); 
  signal(SIGUSR2, finTravail);

  int* error = malloc(sizeof(int));
  *error = 0;
  
  while(nbDroneTravail){
    pause();
    }


  FOR(x,NBDRONES)
  {
     shmdt(T[x]->Y);
     shmdt(dataDrone[x]);
     
     shmctl(shmDY[x], IPC_RMID, NULL);
     shmctl(shmDC[x], IPC_RMID, NULL);
  }
	
}



Tableau** initWorld()	//place les tableaux des drones sur les premières cases !!! initialisez shmD !!!!
{
	/*if(!nbTableaux)*/
		/*nbTableaux = NBDRONES + 7;*/
	
	Tableau **T = malloc(nbTableaux*sizeof(Tableau*));
	
	//int shmid = shmget(IPC_PRIVATE, nbTableaux*sizeof(Tableau*), 0666);
	//création de l'environnement
	Tableau *vaisseau = createTableau(LARGEUR_VAISSEAU,
							PROFONDEUR_SOUTE_VAISSEAU,LARGEUR_ID_COLIS,"Cargaison Vaisseau");
	Tableau *stockDrone = createTableau(LARGEUR_VAISSEAU,1,
							LARGEUR_ID_COLIS,"Drones en charge");
	Tableau *departDrone = createTableau(LARGEUR_VAISSEAU,1,
							LARGEUR_ID_COLIS,"Drones en attente de decollage");
	Tableau *client = createTableau(LARGEUR_VAISSEAU,PROFONDEUR_SOUTE_VAISSEAU,
							LARGEUR_ID_COLIS, "Ville -- Colis livrees");
							
	//Positionnnement de l'environnement
	setPos(vaisseau, GENERAL_OFFSET_LEFT,0);
	setPos(stockDrone, GENERAL_OFFSET_LEFT,PROFONDEUR_SOUTE_VAISSEAU*2+2);
	setPos(departDrone, GENERAL_OFFSET_LEFT,*stockDrone->Y+4);
	
	//délimitation des zones aériennes (attente atterissage / voyage / attente livraison)
	Label * limiteAtterrissageVoyage = createLabel("En attente d'atterrissage\n\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\nEn voyage", 0,*departDrone->Y+4+5);
	Label * limiteVoyageLivraison = createLabel("En voyage\n\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\nEn attente de livraison", 0,*limiteAtterrissageVoyage->Y+4+5);
	Label * limiteDead = createLabel("Dead\t\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501", 0,
      *limiteVoyageLivraison->Y+4 + 5+2*PROFONDEUR_SOUTE_VAISSEAU+2+4);
	
  setPos(client, GENERAL_OFFSET_LEFT,*limiteVoyageLivraison->Y+4 + 5);	

	
	//Parramétrage des drones
	/*drone_Y_atterissage = *departDrone->Y +4;*/
	//drone_Y_voyage = *limiteAtterrissageVoyage->Y+3 +1;
	/*drone_Y_livraison = *limiteVoyageLivraison->Y+3 +1;*/
  /*drone_Y_dead = *limiteDead->Y-3;*/
  
  char droneName[4];
	
  FOR(x, NBDRONES){
    sprintf(droneName, "d%d", x);
    T[x] = createTableau(1,1,LARGEUR_ID_COLIS,droneName);
    
    T[x]->Y = shmat(shmDY[x], NULL, 0);
    dataDrone[x] = shmat(shmDC[x], NULL, 0); //fair un free() avant nn ?
    setPos(T[x], GENERAL_OFFSET_LEFT + x*(LARGEUR_ID_COLIS+1), drone_Y_dead );
  }
  
	strcpy(dataDrone[0],"4|40");
	strcpy(dataDrone[1],"4|41");

  int lereste = NBDRONES;
	T[lereste++] = vaisseau;
	T[lereste++] = stockDrone;
	T[lereste++] = departDrone;
	T[lereste++] = limiteAtterrissageVoyage;
	T[lereste++] = limiteVoyageLivraison;
	T[lereste++] = client;
  T[lereste++] = limiteDead;
	
	setData(vaisseau, 2,1,"3|00");
	setData(client, 2,1,"4|39");
	setData(departDrone, 3,0,"d4");
	setData(stockDrone, 4,0,"d5");
	setData(stockDrone, 5,0,"d6");	
	
	return T;
}



