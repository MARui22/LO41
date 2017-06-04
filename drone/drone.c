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
char* itoa(int i){
  char* str = calloc(12, sizeof(char));
  sprintf(str, "%d", i);
  return str;
}

 void main(int argc, char *argv[]){
 
   //srand(time(NULL)); //préparation aux chiffres aléatoires
   
   IPCDrone * shmD = shmat(atoi(argv[1]), NULL, 0); //controle la position Y du drone dans l'univer
   int* shmEnd = shmat(atoi(argv[2]), NULL, 0);  //controle l'affichage du nom du colis
   int semEnd = atoi(argv[3]);
   int msgCar = atoi(argv[4]);
   
     
  Colis* colis = malloc(sizeof(Colis));
  msgrcv(msgCar, (void*)colis , sizeof(Colis), 0  , 0);
   
  sleep(colis->trajet);
  shmD->posYDrone = drone_Y_voyage;  //on passe le drone dans la zone "voyage" de l'écran
  kill(getppid(), SIGUSR1);
  strcpy(shmD->colis, itoa(colis->id));

  sleep(colis->trajet);
  shmD->posYDrone = drone_Y_livraison; //on passe le drone dans la zone "livraison" de l'écran
  kill(getppid(), SIGUSR1);
  
  sleep(colis->trajet);
  shmD->posYDrone = drone_Y_atterissage;
  strcpy(shmD->colis, "  "); //on décharge le colis !!! ATTENTION !!! TOUJOURS utiliser strcpy(), car un "=" fait sauter la référence à la mémoire partagée !!!!!!
  kill(getppid(), SIGUSR1);
  
  
  sleep(colis->trajet);
  shmD->posYDrone = drone_Y_dead;  //on passe le drone dans la zone "mort" de l'écran

  kill(getppid(), SIGUSR1);
  
  P(semEnd);
  *shmEnd = *shmEnd -1; 
  
  V(semEnd);
  
  shmdt(shmD);
  shmdt(shmEnd);
  /*kill(getppid(), SIGUSR2);*/
  
    
  }