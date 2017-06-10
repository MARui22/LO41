#define _XOPEN_SOURCE
#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>


#include "../gui/gui.h"
#include "../const.h"

#define FOR(p, F) for(int p = 0; p<F; ++p)

//enum droneState {RECHARGE, ATTENTE_DEPART, ALLER, ATTENTE_LIVRAISON, RETOUR, ATTENTE_ATTERRISSAGE, DEAD};

char* itoa2(int i, char* str){
  /*char* str = calloc(12, sizeof(char));*/
  sprintf(str, "%d", i);
  return str;
}

void myStrcpy(char*i, char*c)
{
int a = 0;
  while(*c!='\0')
  {
    i[a] = c[a++];
    
  }
}

 void main(int argc, char *argv[]){
 
   //srand(time(NULL)); //préparation aux chiffres aléatoires
   
   IPCDrone * shmD = shmat(atoi(argv[1]), NULL, 0); //controle la position Y du drone dans l'univer
   int* shmEnd = shmat(atoi(argv[2]), NULL, 0);  //controle l'affichage du nom du colis
   int semEnd = atoi(argv[3]);
   int msgCar = atoi(argv[4]);
   
     char* tmp = calloc(12,sizeof(char)), *reset = calloc(LARGEUR_ID_COLIS +1, sizeof(char));//itoa2(colis->colis.id);
  
  msgColis* colis = malloc(sizeof(msgColis));
  int cargo_non_vide = 1;
  
  while(1)
{
/*char* c = malloc(sizeof(char));*/
    
  /////// RECHARGE ////////////
  shmD->state = RECHARGE;
   kill(getppid(), SIGUSR1);
  sleep(colis->colis.trajet);
  
  ////// PRENDRE UN COLIS -- ATTENDE LE DEPART  ///////////////
  if( msgrcv(msgCar, (void*)colis , sizeof(msgColis)-4, -3  ,IPC_NOWAIT) == -1 )
    break;
  strcpy(shmD->colis, reset);
  strcat(shmD->colis, (itoa2(colis->colis.prio, tmp)));
  strcat(shmD->colis, "|");
  strcat(shmD->colis, (itoa2(colis->colis.id, tmp)));
  shmD->id_colis = colis->colis.id;
  shmD->state = ATTENTE_DEPART;  //on passe le drone dans la zone "voyage" de l'écran  
  kill(getppid(), SIGUSR1);
  sleep(colis->colis.trajet);
  
  //////  ALLER -- FAIRE LE TRAJET  //////////
  shmD->state= ALLER; //on passe le drone dans la zone "livraison" de l'écran
  kill(getppid(), SIGUSR1);
  sleep(colis->colis.trajet);

  //////  ATTENTE_LIVRAISON //////////////////////
  shmD->state= ATTENTE_LIVRAISON;
  kill(getppid(), SIGUSR1);
  sleep(colis->colis.trajet);
   
  /////  RETOUR--voyage retour  //////////////////
  shmD->state= RETOUR;
  kill(getppid(), SIGUSR1);
  sleep(colis->colis.trajet);
  
  /////   ATTENTE_ATTERRISSAGE  ////////////////////
  shmD->state= ATTENTE_ATTERRISSAGE; 
  kill(getppid(), SIGUSR1);
  sleep(colis->colis.trajet);
  
  
}
  
  P(semEnd, 0);
  *shmEnd = *shmEnd -1; 
  
  V(semEnd, 0);
  
  shmdt(shmD);
  shmdt(shmEnd);
  /*kill(getppid(), SIGUSR2);*/
  
    
  }