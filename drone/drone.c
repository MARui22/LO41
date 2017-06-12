#define _POSIX_SOURCE //199309L
#define _XOPEN_SOURCE 
#define _XOPEN_SOURCE_EXTENDED
#define _BSD_SOURCE 500
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
#include <time.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>


#include "../gui/gui.h"
#include "../const.h"

#define FOR(p, F) for(int p = 0; p<F; ++p)


int millisleep(unsigned ms)
{
  return usleep(1000 * ms);
}
               
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
   int index = 1;
   
   IPCDrone * shmD = shmat(atoi(argv[index++]), NULL, 0); //controle la position Y du drone dans l'univer
   int* shmEnd = shmat(atoi(argv[index++]), NULL, 0);  //controle l'affichage du nom du colis
   int semEnd = atoi(argv[index++]);                    // semaphore de signal de sin de logiciel
   int msgCar = atoi(argv[index++]);                    //file de message qui contien la cargaison
   int msgDec = atoi(argv[index++]);                    //file de message de demande de decollage
   int msgAtt = atoi(argv[index++]);                    //file de message de demande d'atterrissage
   
   
     char* tmp = calloc(12,sizeof(char)), *reset = calloc(LARGEUR_ID_COLIS +1, sizeof(char));//itoa2(colis->colis.id);
  
  /*struct timespec ts;*/
  
  sem_t *semEnd2 = sem_open("end2", O_RDWR);
  sem_t *semEnd3 = sem_open("end3", O_RDWR);

  
      int* tmps = malloc(sizeof(int));
  sem_getvalue(semEnd2, tmps);
  pint(*tmps, "semEnd2 avant");
  //sem_wait(semEnd2);
  
  //    int* tmps = malloc(sizeof(int));
  
  msgColis* colis = malloc(sizeof(msgColis));
  Demande* dem = malloc(sizeof(Demande));
  dem->demandeur = getpid();
  dem->type = 1;
  
   sigset_t mask;
   sigfillset(&mask);
   sigdelset(&mask, SIGUSR1);
   
  

  /*ms2ts(&ts, colis->colis.trajet*1000);*/
  
  int cargo_non_vide = 1;
  
    while(1)
  {
  /*char* c = malloc(sizeof(char));*/
      
    /////// RECHARGE ////////////
    shmD->state = RECHARGE;
     kill(getppid(), SIGCONT);
    millisleep(colis->colis.trajet);
    
    ////// PRENDRE UN COLIS -- ATTENDE LE DEPART  ///////////////
    if( msgrcv(msgCar, (void*)colis , sizeof(msgColis)-4, -3  ,IPC_NOWAIT) == -1 )
      break;  //s'il n'y a plus de colis, alors on arrete le boulot
      
    strcpy(shmD->colis, reset);
    strcat(shmD->colis, (itoa2(colis->colis.prio, tmp)));
    strcat(shmD->colis, "|");
    strcat(shmD->colis, (itoa2(colis->colis.id, tmp)));
    shmD->id_colis = colis->colis.id;
    shmD->state = ATTENTE_DEPART;  //on passe le drone dans le tableau "demande de décollage"  
    kill(getppid(), SIGCONT);
    msgsnd(msgDec, (void*)dem, sizeof(Demande)-4, 0);
    /*signal(SIGUSR1, SIG_IGN);*/
    /*sigsuspend(&mask);*/
    sleep(1);
    //pause();
    
    
    
    //////  ALLER -- FAIRE LE TRAJET  //////////
    shmD->state= ALLER; //on passe le drone dans la zone "voyage" de l'écran
    kill(getppid(), SIGCONT);
    millisleep(colis->colis.trajet);

    //////  ATTENTE_LIVRAISON //////////////////////
    shmD->state= ATTENTE_LIVRAISON;
    kill(getppid(), SIGCONT);
    millisleep(colis->colis.trajet);
    
    /////  RETOUR--voyage retour  //////////////////
    shmD->state= RETOUR;
    kill(getppid(), SIGCONT);
    millisleep(colis->colis.trajet);
    
    /////   ATTENTE_ATTERRISSAGE  ////////////////////
    shmD->state= ATTENTE_ATTERRISSAGE; 
    kill(getppid(), SIGCONT);
    millisleep(colis->colis.trajet);
    
    
  }
  
  /*P(semEnd, 0);*/
  /**shmEnd = *shmEnd -1; */
  /**/
  /*V(semEnd, 0);*/
  
  sem_getvalue(semEnd2, tmps);
  pint(*tmps, "semEnd2 fin drone ******************************************");
  
  sem_wait(semEnd2);
    *shmEnd = *shmEnd -1;
    if(*shmEnd == 0 )
     sem_post(semEnd3);
    pint(*shmEnd, "***********************vel shmend");// 
      
  sem_post(semEnd2);
  
  sem_close(semEnd2);
  sem_close(semEnd3);
  
  shmdt(shmD);
  shmdt(shmEnd);
  /*kill(getppid(), SIGUSR2);*/
  
    
  }