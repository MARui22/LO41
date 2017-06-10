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
   
     char* tmp = calloc(12,sizeof(char));//itoa2(colis->colis.id);
  msgColis* colis = malloc(sizeof(msgColis));
  msgrcv(msgCar, (void*)colis , sizeof(msgColis)-4, -3  , 0);
  /*msgTest* c  = malloc(sizeof(msgTest ));*/
  
  
  
  
  /*msgrcv(msgCar, (void*)c, sizeof(msgTest), 0  , 0);*/
  /*pint(colis->colis.prio, "prio");*/
  /*pint(colis->colis.trajet, "trajet");*/
  /*pint(colis->colis.id, "id");*/
  /*pint(c->test, "test");*/
  //tmp = strcast(itoa2(colis->colis.id, tmp), "|")
   
  sleep(colis->colis.trajet);
  shmD->posYDrone = drone_Y_voyage;  //on passe le drone dans la zone "voyage" de l'écran
  strcpy(shmD->colis, "");
  strcat(shmD->colis, (itoa2(colis->colis.prio, tmp)));
  strcat(shmD->colis, "|");
  strcat(shmD->colis, (itoa2(colis->colis.id, tmp)));
  
  kill(getppid(), SIGUSR1);

  sleep(colis->colis.trajet);
  shmD->posYDrone = drone_Y_livraison; //on passe le drone dans la zone "livraison" de l'écran
  kill(getppid(), SIGUSR1);
  
  sleep(colis->colis.trajet);
  shmD->posYDrone = drone_Y_atterissage;
  strcpy(shmD->colis, "  "); //on décharge le colis !!! ATTENTION !!! TOUJOURS utiliser strcpy(), car un "=" fait sauter la référence à la mémoire partagée !!!!!!
  kill(getppid(), SIGUSR1);
  
  
  sleep(colis->colis.trajet);
  shmD->posYDrone = drone_Y_dead;  //on passe le drone dans la zone "mort" de l'écran

  kill(getppid(), SIGUSR1);
  
  P(semEnd);
  *shmEnd = *shmEnd -1; 
  
  V(semEnd);
  
  shmdt(shmD);
  shmdt(shmEnd);
  /*kill(getppid(), SIGUSR2);*/
  
    
  }