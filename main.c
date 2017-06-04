#define _POSIX_SOURCE
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
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
int initsem();
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
  int shmDId[NBDRONES];
  int shmEndId;
  
  IPCDrone *shmD[NBDRONES]; //liste des mémoires partagées de la hauteur des drones*/
  int *shmEnd;
  
  Tableau **T;
  
  int nbDroneTravail = NBDRONES; // !!!!!!!!!!!!!!!!!!!!!!!! en nominial, ça vaut le nombre de drones !

Colis* genereColis(){
  static int i = 0;
  Colis* c = malloc(sizeof(Colis));
  
    
  c->id = i++;
  c->prio = rand()/(RAND_MAX/2);
  c->trajet = rand()/(RAND_MAX/(TRAJET_MAX - TRAJET_MIN))+TRAJET_MIN;
  
  
  return c;
}

void drawUnivers(int i)
{
   signal(SIGUSR1, SIG_IGN);
   
  
  FOR(x, NBDRONES){
    setData(T[x], 0,0, shmD[x]->colis);
    *(T[x]->Y) = shmD[x]->posYDrone;
  }
  
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

  
  
  //INIT MEMOIRE PARTAGEE
  //parametre des drones
  FOR(x,NBDRONES)
	{
		shmDId[x] = shmget(IPC_PRIVATE, sizeof(IPCDrone), 0666|IPC_CREAT);
    if(shmD[x] <0)
      puts("echec creation memoire partagee pour les drones");
    
    shmD[x] = shmat(shmDId[x], NULL, 0);
    
	}
  
  //controle de fin de logiciel, pricipalement pour initier le netoyage des IPC
  shmEndId = shmget(IPC_PRIVATE, sizeof(int), 0666|IPC_CREAT);
  if(shmEndId <0)
      puts("echec creation memoire partagee pour la fin du programme");
  shmEnd = shmat(shmEndId, NULL, 0);
  *shmEnd = NBDRONES;
  
  //INIT SEMAPHORE  -- controle de la fin de logiciel
  int semEnd = initsem();
  pint(semctl(semEnd, 0, GETVAL, 0), "semEnd");
  
	T = initWorld();	//dessine l'univer
  
  
  //INIT FILE MESSAGE 
  // contient la cargaison du vaisseau
  int msgCarId = msgget(IPC_PRIVATE, 0666|IPC_CREAT);
  srand(time(NULL)); //préparation aux chiffres aléatoires
  FOR(y, PROFONDEUR_SOUTE_VAISSEAU)
  FOR(x, LARGEUR_VAISSEAU){
    Colis *c = genereColis();
    setData(T[NBDRONES], x, y, itoa(c->id));  //On remplie la soute
    msgsnd(msgCarId, (void*)c, sizeof(Colis), c->prio);
    
  }
  
  
  

	draw(T, nbTableaux);
   
  
  pid_t pid[NBDRONES];
  FOR(x, NBDRONES){
    pid[x]= fork();
    if(pid[x] == 0){ //Fils n°x ------------------- RECOUVREMENT DRONE ------------------------------//
      execlp("drone/drone.elf", "drone.elf", itoa(shmDId[x]), itoa(shmEndId), itoa(semEnd), itoa(msgCarId), (char*)0);
      exit(5);
    }
  }
  
  
  signal(SIGUSR1, drawUnivers); 
  signal(SIGUSR2, finTravail);

  int* error = malloc(sizeof(int));
  *error = 0;
  
  int loop = 1;
  
  while(loop)
  {
    P(semEnd);
    if(*shmEnd != 0)
    {
      V(semEnd);
      sleep(1);
    }else{
      V(semEnd);
      loop = 0;
    }
  }

  /*while(nbDroneTravail){*/
    /*pause();*/
    /*}*/
 // wait(NULL);
 // pint(semctl(semEnd, 0, GETVAL, 0), "semEnd");

 //msgget, msgctl
  shmdt(shmEnd);
  shmctl(shmEndId, IPC_RMID, NULL);
  
  semctl(semEnd, 0, IPC_RMID, NULL);
  
  
  FOR(x,NBDRONES)
  {
     shmdt(shmD[x]);
      
     shmctl(shmDId[x], IPC_RMID, NULL);
  }
	
  msgctl(msgCarId, IPC_RMID, NULL);
  
  
}

int initsem() 
{
    
	int status = 0;		
	int semid_init;
   	union semun {
		int val;
		struct semid_ds *stat;
		ushort * array;
	} ctl_arg;
    if ((semid_init = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT)) > 0) {
		
	    	ushort array[1] = {1};
	    	ctl_arg.array = array;
	    	status = semctl(semid_init, 0, SETALL, ctl_arg);
    }
   if (semid_init == -1 || status == -1) { 
	perror("Erreur initsem");
	return (-1);
    } else return (semid_init);
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
    setPos(T[x], GENERAL_OFFSET_LEFT + x*(LARGEUR_ID_COLIS+1), drone_Y_dead );
  }
  
	strcpy(shmD[0]->colis,"4|40");
	strcpy(shmD[1]->colis,"4|41");

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



