#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "./gui/gui.h"

#define FOR(p, F) for(int p = 0; p<F; ++p)


Tableau** initWorld();


#define NBDRONES 3
#define LARGEUR_VAISSEAU 15
#define PROFONDEUR_SOUTE_VAISSEAU 2
#define LARGEUR_ID_COLIS 4
#define GENERAL_OFFSET_LEFT 10
	
	int nbTableaux = 0; //6 + nombre de drones	
	int drone_Y_atterissage, drone_Y_voyage, drone_Y_livraison;

  int shmD[NBDRONES]; //liste des mémoires partagées des drones


void main()
{	  



  FOR(x,NBDRONES)
	{
		shmD[x] = shmget(IPC_PRIVATE, sizeof(Tableau), 0666);
    if(shmD[x] <0)
      puts("echec creation memoire partagee pour les drones");
	}
  
	Tableau **T = initWorld();	//dessine l'univer
	draw(T, nbTableaux);
  
  
  
  FOR(x,NBDRONES)
	{
     shmdt(T[x]);
		 shmctl(shmD[x], IPC_RMID, NULL);
	}
	
}


Tableau** initWorld()	//place les tableaux des drones sur les premières cases !!! initialisez shmD !!!!
{
	if(!nbTableaux)
		nbTableaux = NBDRONES + 6;
	
	Tableau **T = malloc(nbTableaux*sizeof(Tableau*));
	
	
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
	setPos(departDrone, GENERAL_OFFSET_LEFT,stockDrone->Y+4);
	
	//délimitation des zones aériennes (attente atterissage / voyage / attente livraison)
	Label * limiteAtterrissageVoyage = createLabel("En attente d'atterrissage\n\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\nEn voyage", 0,departDrone->Y+4+5);
	Label * limiteVoyageLivraison = createLabel("En voyage\n\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\nEn attente de livraison", 0,limiteAtterrissageVoyage->Y+4+5);
	
	setPos(client, GENERAL_OFFSET_LEFT,limiteVoyageLivraison->Y+4 + 5);	

	
	//Parramétrage des drones
	drone_Y_atterissage = departDrone->Y +4;
	drone_Y_voyage = limiteAtterrissageVoyage->Y+3 +1;
	drone_Y_livraison = limiteVoyageLivraison->Y+3 +1;
  
  char droneName[4];
	
  FOR(x, NBDRONES){
    sprintf(droneName, "d%d", x);
    T[x] = createShmTableau(1,1,LARGEUR_ID_COLIS,droneName, shmD[x]);
    setPos(T[x], GENERAL_OFFSET_LEFT + x*(LARGEUR_ID_COLIS+1), drone_Y_livraison );	
  }
	
	setData(T[0], 0,0,"4|40");
	setData(T[1], 0,0,"4|41");

  int lereste = NBDRONES-1;
	T[++lereste] = vaisseau;
	T[++lereste] = stockDrone;
	T[++lereste] = departDrone;
	T[++lereste] = limiteAtterrissageVoyage;
	T[++lereste] = limiteVoyageLivraison;
	T[++lereste] = client;
	
	setData(vaisseau, 2,1,"3|00");
	setData(client, 2,1,"4|39");
	setData(departDrone, 3,0,"d4");
	setData(stockDrone, 4,0,"d5");
	setData(stockDrone, 5,0,"d6");	
	
	return T;
}



