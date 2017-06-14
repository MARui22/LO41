
#define _POSIX_SOURCE
#define _XOPEN_SOURCE 
#define _BSD_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
/*#include <math.h>*/
#include <signal.h>
#include <pthread.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>


#include "./gui/gui.h"
#include "const.h"

#define FOR(p, F) for(int p = 0; p<F; ++p)


#define index_cargaison NBDRONES
#define index_drone_charge (NBDRONES+1)
#define index_drone_decollage (NBDRONES+2)
#define index_maison (NBDRONES+5)

Tableau** initWorld();
int initsem();
void finish(int i);


  int shmDId[NBDRONES];
  int shmEndId;
  
  IPCDrone *shmD[NBDRONES]; //liste des mémoires partagées de la hauteur des drones*/
  int *shmEnd;
  
  Tableau **T;
  
  int nbDroneTravail = NBDRONES; 
  sem_t* semD[NBDRONES];
  
Colis genereColis(){
  static int i = 0;
  Colis c;
  
    
  c.id = i++;
  c.prio = rand()/(RAND_MAX/3)+1;
  c.trajet = rand()/(RAND_MAX/(TRAJET_MAX*1000 - 1000*TRAJET_MIN))+TRAJET_MIN*1000;
   
  return c;
}

msgColis *genereMsgColis()
{
  msgColis *m = malloc(sizeof(msgColis));
  m->colis = genereColis();
  m->type = m->colis.prio;
  
  return m;
}


void drawUnivers(int i)
{
   /*signal(SIGUSR1, SIG_IGN);*/
   int index_num_livraison = 0;
  
/*RECHARGE, ATTENTE_DEPART, ALLER, ATTENTE_LIVRAISON, RETOUR, ATTENTE_ATTERRISSAGE, DEAD};*/
  int pos;  
  
  FOR(x, NBDRONES){
sem_wait(semD[x]); 
  char* ncolis = shmD[x]->colis;
  
  /*on gère au cas par drone :*/
  enum droneState ds = shmD[x]->state ;
    
  if(ds & (RECHARGE|ATTENTE_DEPART))
  {   
    pos=drone_Y_repos;
    if(ds & ATTENTE_DEPART)//- on récupere le numéro de colis du drone pour le suprimer de la cargaison
    {
          
      if(shmD[x]->id_colis !=-1)
      {
        index_num_livraison = shmD[x]->id_colis; // num contient le numéro de colis
        //supprimons le colis n°index_num_livraison de la cargaison :
        setData(T[index_cargaison], index_num_livraison%LARGEUR_VAISSEAU,index_num_livraison/LARGEUR_VAISSEAU, "");
        
        //on place le colis dans le drone n°x:    
        setData(T[x], 0,0, ncolis);
      }
      setData(T[index_drone_decollage], x, 0, T[x]->titre);
      setData(T[index_drone_charge], x, 0, " ");
    }
    else
      setData(T[index_drone_charge], x, 0, T[x]->titre);
  }
  else if(ds & ATTENTE_ATTERRISSAGE )
  {  pos=drone_Y_atterissage;
  
  }
  else if(ds & (ALLER | RETOUR))
  {  pos=drone_Y_voyage;
  
  
    if(ds & RETOUR)//- on place le colis dans une maison
    {      
    //on place le colis du drone dans la maison
        
    
      if(shmD[x]->id_colis !=-1)
      {
        
        index_num_livraison = shmD[x]->id_colis; // num contient le numéro de colis
    
  
        setData(T[index_maison], index_num_livraison%LARGEUR_VAISSEAU,(index_num_livraison)/LARGEUR_VAISSEAU,
          ncolis);
        
        shmD[x]->id_colis = -1;
      }

    /*getData(T[x], 0,0)*/
    //on clean le colis de l'affichage du drone
      setData(T[x], 0,0, " ");
    }
    else
    {
      setData(T[index_drone_decollage], x, 0, " ");
    }
  
  
  }
  else if(ds |ATTENTE_LIVRAISON)
  {  pos=drone_Y_livraison;
  
  }
  else if(ds |DEAD)
  {  pos=drone_Y_dead;
  
  }
  else
    pos = drone_Y_repos;
  
    /*on place la hauteur du drone */
    *(T[x]->Y) = pos;
sem_post(semD[x]);      
  } //fin de l'actualisation des drones
  
  draw(T,nbTableaux);  
  /*signal(SIGUSR1, drawUnivers);*/
  
    
  //Gestion de signaux :
  sigset_t mask;
  struct sigaction act;
  
    sigemptyset (&mask); 
  sigaddset (&mask, SIGUSR2);
  
  act.sa_handler = drawUnivers;
  act.sa_mask = mask;
  
  sigaction(SIGUSR2, &act, NULL);
}


char* itoa(int i){
  char* str = calloc(12, sizeof(char));
  sprintf(str, "%d", i);
  return str;
}

/*void* operateur_dec(void* tour)*/
/*{*/
/**/
  /*Demande *dem = malloc(sizeof(Demande));*/
  /*int Tour = *((int*)tour);*/
  /*pint(Tour, "msg");*/
  /*while(1)*/
  /*{*/
    /*free(dem);*/
    /*dem = malloc(sizeof(Demande));*/
    /*msgrcv( Tour, (void*) dem, sizeof(Demande)-4, -3, 0);*/
    /*pint((int)dem->demandeur, "recieve");*/
    /**/
    /*sleep(1);*/
    /**/
    /*if(dem->demandeur < 0)*/
      /*pthread_exit(NULL); //si la mission est terminée*/
    /**/
    /*puts("recieve");*/
    /*kill(dem->demandeur, SIGCONT);*/
  /*}*/
/*}*/

void main()
{	  

  
  
  //////////////////////////INIT MEMOIRE PARTAGEE
  //parametre des drones
  FOR(x,NBDRONES)
	{
		shmDId[x] = shmDelNCreat(sizeof(IPCDrone));//shmget(IPC_PRIVATE, sizeof(IPCDrone), 0666|IPC_CREAT);
    if(shmD[x] <0)
      puts("echec creation memoire partagee pour les drones");
    
    shmD[x] = shmat(shmDId[x], NULL, 0);
    shmD[x]->state = RECHARGE;
    
	}
  
  //controle de fin de logiciel, pricipalement pour initier le netoyage des IPC
  shmEndId = shmDelNCreat(sizeof(int));//shmget(IPC_PRIVATE, sizeof(int), 0666|IPC_CREAT);
  if(shmEndId <0)
      puts("echec creation memoire partagee pour la fin du programme");
  shmEnd = shmat(shmEndId, NULL, 0);
  *shmEnd = NBDRONES;
  
//////////////////////////////////////INIT SEMAPHORE 
  int semEnd = initsem();
  sem_t *semEnd2 = semDelNCreat("end2", 1);
  /*sem_close(semEnd2);*/
  
  /*semEnd2 = sem_open("end2", O_CREAT, 0644, 1);*/
  sem_t *semEnd3 = semDelNCreat("end3",0);
  
    //Semaphore de la memoire partagé des IPCDrone
  char** nomSemD= malloc(NBDRONES*sizeof(char*));
  
  
  FOR(x, NBDRONES)
  {
    nomSemD[x] = calloc(9, sizeof(char));
    sprintf(nomSemD[x], "drone%d", x);
    semD[x] = semDelNCreat(nomSemD[x], 1);
    
  }
  
  
  /*sem_post(semEnd2);*/
    /*sem_post(semEnd2);*/
  /*sem_set(semEnd2, 1, malloc(sizeof(int)));*/
  
  /*sem_set(semEnd3, 0, malloc(sizeof(int)));*/
  
  /**/
  /*int* tmps = malloc(sizeof(int));*/
  /*sem_getvalue(semEnd2, tmps);*/
  /*pint(*tmps, "semEnd2 creation");*/


  
  
	T = initWorld();	//dessine l'univer
  
  
//////////////////////////////////////INIT FILE MESSAGE 
  // la cargaison du vaisseau, et son contenu
  int msgCarId = msgget(IPC_PRIVATE, 0666|IPC_CREAT);
  srand(time(NULL)); //préparation aux chiffres aléatoires
  FOR(y, PROFONDEUR_SOUTE_VAISSEAU)
  FOR(x, LARGEUR_VAISSEAU){
    msgColis *c = genereMsgColis();
    
    char*buff = calloc(LARGEUR_ID_COLIS+1, sizeof(char));
    
      strcat(buff, itoa(c->colis.prio));
    strcat(buff, "|");
    strcat(buff, itoa(c->colis.id));
    setData(T[index_cargaison], x, y,buff);  //On remplie la soute
    msgsnd(msgCarId, (void*)c, sizeof(msgColis)-4, 0);

  }
  
  //Demande de décollage :
    int msgDecId = msgget(IPC_PRIVATE, 0666|IPC_CREAT);
  
  //Demande d'atterrissage :
    int msgAttId = msgget(IPC_PRIVATE, 0666|IPC_CREAT);
  
  

  /*pthread_t operateurDec;*/
  /*int * tmp = malloc(sizeof(int));*/
  /**tmp = msgDecId;*/
  /*if(pthread_create(&operateurDec, NULL, operateur_dec, tmp) == -1) {*/
	/*perror("erreur a la creation de la tour de controle de decollage");*/
	/*return ;*/
    /*}*/
/**/
	/*draw(T, nbTableaux);*/
  
  
  //------------------ FONCTION DE CLEAN
                void range_tout(int i)
                {

                  Demande* dem2 = malloc(sizeof(Demande));
                  dem2->type = 1;
                  dem2->demandeur = -1;
                  msgsnd(msgDecId, (void*) dem2, sizeof(Demande)-4, 0); //on indique à la tour de controle de décollage que la mission est terminée
                  msgsnd(msgAttId, (void*) dem2, sizeof(Demande)-4, 0); //on indique à la tour de controle d'atterrissage que la mission est terminée

                  sleep(1);
                  
                  shmdt(shmEnd);
                  shmctl(shmEndId, IPC_RMID, NULL);
                  
                  semctl(semEnd, 0, IPC_RMID, NULL);
                  sem_close(semEnd2);
                  sem_close(semEnd3);
                  
                  sem_unlink("end2");
                  sem_unlink("end3");
                  
                  
                  
                  
                  FOR(x,NBDRONES)
                  {
                     shmdt(shmD[x]);
                      
                     shmctl(shmDId[x], IPC_RMID, NULL);
                  }
                  
                  if(i != 0)
                  {
                    signal(SIGINT, SIG_DFL);
                    raise(SIGINT);
                    
                  }
                //pthread_join(operateurDec, NULL);  
	
                msgctl(msgCarId, IPC_RMID, NULL);
                msgctl(msgAttId, IPC_RMID, NULL);  
                msgctl(msgDecId, IPC_RMID, NULL);
   
                }
                  
                  
  /////////////////////////////////////////////GESTION DE SIGNAUX :
  sigset_t mask;
  struct sigaction act;
  
  sigemptyset (&mask); 
  sigaddset (&mask, SIGUSR2);
  
  act.sa_handler = drawUnivers;
  act.sa_mask = mask;
  
  sigaction(SIGUSR2, &act, NULL);
  signal(SIGINT, range_tout);
  
  ////////////////////////////////////////------FORK
  //- CRÉATION DES TOURS DE CONTROLES POUR L'ATTERRISSAGE ET DÉCOLLAGE DES DRONES :
  
  
  pid_t pidTourDec;
  pidTourDec = fork();
  if(pidTourDec == 0)    //fils tour de controle decollage
  {
      execlp("tour_de_controle/tour_controle.elf", "tour_controle.elf", itoa(msgDecId), itoa(TEMPS_MANOEUVRE_DECOLLAGE), (char*)0);
      perror("pas de tour de decollage");
      
      exit(5);
  }
  
  pid_t pidTourAtt;
  pidTourAtt = fork();
  if(pidTourAtt == 0)    //fils tour de controle atterrissage
  {
      execlp("tour_de_controle/tour_controle.elf", "tour_controle.elf", itoa(msgAttId), itoa(TEMPS_MANOEUVRE_ATTERRISSAGE), (char*)0);
      perror("pas de tour d'atterrissage");
      
      exit(5);
  }

    //- CREATION DES DRONES :
    
  pid_t pidD[NBDRONES];
  FOR(x, NBDRONES){
    pidD[x]= fork();
    if(pidD[x] == 0){ //Fils n°x ------------------- RECOUVREMENT DRONE ------------------------------//
      execlp("drone/drone.elf", "drone.elf", itoa(shmDId[x]), itoa(shmEndId), nomSemD[x], itoa(msgCarId), itoa(msgDecId), itoa(msgAttId), (char*)0);
      perror("erreur recouvrement drone");
      exit(5);
    }
  }
  
  /*signal(SIGUSR1, drawUnivers); */
/**/
  /*int* error = malloc(sizeof(int));*/
  /**error = 0;*/

  
  wait(NULL);
  int* tmps = malloc(sizeof(int));
*tmps =-1 ;
  while(*shmEnd != 0)
    {sem_wait(semEnd3);
    
      *tmps = *tmps +1;
    }

        //sem_getvalue(semEnd3, tmps);
  pint(*tmps, "Nombre de proc de fin de logiciel inexpliquable stopé");
  /*puts("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");*/
      /*sem_getvalue(semEnd3, tmps);*/
  /*pint(*tmps, "semEnd3 fin3");*/
  /*puts("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");*/
  /**/
  /*sem_wait(semEnd3);*/
    /*sem_wait(semEnd3);*/
  /*sem_wait(semEnd3);*/
  

  

  drawUnivers(0);
  

  
  
  range_tout(0);
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
    if ((semid_init = semget(IPC_PRIVATE, 2, 0666|IPC_CREAT)) > 0) {
		
	    	ushort array[2] = {1,0};
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
	
  char lineOfDead[vaisseau->Lchar];
  strcpy(lineOfDead, "Dead\t");
  FOR(osef, vaisseau->Lchar/3-10)
    strcat(lineOfDead, "\u2501");
    
  Label * limiteDead = createLabel(lineOfDead, 0,
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
    setPos(T[x], GENERAL_OFFSET_LEFT + x*(LARGEUR_ID_COLIS+1), drone_Y_repos );
  }
  

  int lereste = NBDRONES;
	T[lereste++] = vaisseau;
	T[lereste++] = stockDrone;
	T[lereste++] = departDrone;
	T[lereste++] = limiteAtterrissageVoyage;
	T[lereste++] = limiteVoyageLivraison;
	T[lereste++] = client;
  T[lereste++] = limiteDead;
	
	return T;
}



