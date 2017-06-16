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
#include <pthread.h>


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>


#include "../gui/gui.h"
#include "../const.h"


#define FOR(p, F) for(int p = 0; p<F; ++p)


               
//enum droneState {RECHARGE, ATTENTE_DEPART, ALLER, ATTENTE_LIVRAISON, RETOUR, ATTENTE_ATTERRISSAGE, DEAD};
sem_t semB;
int millisleep(unsigned ms)
{
  return usleep(1000 * ms);
}
  
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

void bienrecut(int i)
{
  //pint(getpid(), "bien recut");
  
}

void * consommation(void* bat)
{
 int* batterie = (int*)bat;
 
  while(1)
 {
  sem_wait(&semB);
    *batterie = *batterie -1;
    /*pint(*batterie, "////////////il reste");*/
    
    if(*batterie <1)
    {
      sem_post(&semB);
      raise(SIGUSR2);
      pthread_exit(NULL);
    }
  sem_post(&semB);
  sleep(1);
  

  
  
 
 }

}


 void main(int argc, char *argv[]){
 

  
   //srand(time(NULL)); //préparation aux chiffres aléatoires
   int index = 1;
   pid_t ppid = getppid();
   char *nomSemD = calloc(9, sizeof(char));
   
   IPCDrone * shmD = shmat(atoi(argv[index++]), NULL, 0); //contient les données public du drone
   int* shmEnd = shmat(atoi(argv[index++]), NULL, 0);  //contient le nombre de drones actifs
   strcpy(nomSemD, argv[index++]);                // nom du sémaphore qui protège les données public du drone
   int msgCar = atoi(argv[index++]);                    //file de message qui contien la cargaison
   int msgDec = atoi(argv[index++]);                    //file de message de demande de decollage
   int msgAtt = atoi(argv[index++]);                    //file de message de demande d'atterrissage
   
   
     char* tmp = calloc(12,sizeof(char)), *reset = calloc(LARGEUR_ID_COLIS +1, sizeof(char));//itoa2(colis->colis.id);
  
  /*struct timespec ts;*/
  sem_t *semD = sem_open(nomSemD, O_RDWR); //semaphore qui protège les données public du drone
  sem_t *semEnd2 = sem_open("end2", O_RDWR);//semaphore qui protège la shm du nombre de drones activés
  sem_t *semEnd3 = sem_open("end3", O_RDWR);//Semaphore qui déclenche la fin du logiciel
  sem_init(&semB, 0, 1);//Semaphore  qui protège le niveau de la batterie
  
    int* batterie = malloc(sizeof(int));
    *batterie = CAPACITE_BATTERIE;
    pthread_t pcons;
  
      /*int* tmps = malloc(sizeof(int));*/
  /*sem_getvalue(semEnd2, tmps);*/
  /*pint(*tmps, "semEnd2 avant");*/
  //sem_wait(semEnd2);
  
  //    int* tmps = malloc(sizeof(int));
  
                                        void range_tout()
                                        {
                                        sem_wait(semEnd2);
                                          *shmEnd = *shmEnd -1;
                                          if(*shmEnd == 0 )
                                           {sem_post(semEnd3);
                                            /*dem->demandeur = -1;*/
                                            /*msgsnd(msgDec, (void*)dem, sizeof(Demande)-4, 0);*/
                                           }
                                        sem_post(semEnd2);
                                        
                                          sem_close(semEnd2);
                                          sem_close(semEnd3);
                                          sem_close(&semB);
                                          sem_close(semD);
                                          
                                            sem_destroy(&semB);
                                          
                                         // sem_unlink("batterie");
                                          
                                          shmdt(shmD);
                                          shmdt(shmEnd);
                                        
                                        }
                                        
                                      void dead(int osef)
                                      {
                                      //////////////  DEAD ///////////////
                                      sem_wait(semD);//-------------------- début d'une petite annimation du drone qui tombe'
                                      
                                        if(shmD->state== ATTENTE_ATTERRISSAGE)
                                        {                                            
                                          shmD->state= ALLER; 
                                          kill(ppid, SIGUSR2);
                                      sem_post(semD);
                                          
                                          millisleep(60);
                                          
                                      sem_wait(semD);
                                        }
                                          
                                        if(shmD->state & (ALLER | RETOUR) )
                                        { 
                                          shmD->state= ATTENTE_LIVRAISON; 
                                          kill(ppid, SIGUSR2);
                                      sem_post(semD);
                                        
                                          millisleep(60);
                                      sem_wait(semD);
                                        }
                                  
                                        shmD->state= DEAD; 
                                        kill(ppid, SIGUSR2);
                                      sem_post(semD);//-------------------- le drone est au sol
                                      
                                        pthread_join(pcons, NULL);
                                        range_tout();
                                        puts("dead");
                                        
                                        raise(SIGINT);
                                        
                                      }
  
  msgColis* colis = malloc(sizeof(msgColis));
  Demande* dem = malloc(sizeof(Demande));
  dem->demandeur = getpid();
  dem->type = 2;
  
   sigset_t mask;
   sigfillset(&mask);
   sigdelset(&mask, SIGUSR1);
   sigdelset(&mask, SIGUSR2);
   sigdelset(&mask, SIGINT);
   
   signal(SIGUSR2, dead);
   
   /**/
   /*sigemptyset(&mask);*/
   /*sigaddset(&mask, SIGUSR1);  */

  /*ms2ts(&ts, colis->colis.trajet*1000);*/

  ////////////////////////////////////// DEBUT MISSION /////////////////////////////////////////////////////
  
  
  while(1)
  {
  /*char* c = malloc(sizeof(char));*/
    
    
    ////// PRENDRE UN COLIS -- ATTENDE LE DEPART  ///////////////
    if( msgrcv(msgCar, (void*)colis , sizeof(msgColis)-4, -3  ,IPC_NOWAIT) == -1 )
      break;  //s'il n'y a plus de colis, alors on arrete le boulot
  sem_wait(semD);    
    shmD->id_colis = colis->colis.id;
    strcpy(shmD->colis, reset);
    strcat(shmD->colis, (itoa2(colis->colis.prio, tmp)));
    strcat(shmD->colis, "|");
    strcat(shmD->colis, (itoa2(colis->colis.id, tmp)));
    shmD->state = ATTENTE_DEPART;  //on passe le drone dans le tableau "demande de décollage"  
    kill(getppid(), SIGUSR2);
  sem_post(semD);
    signal(SIGUSR1, bienrecut);
    msgsnd(msgDec, (void*)dem, sizeof(Demande)-4, 0);
    /*signal(SIGUSR1, SIG_IGN);*/
    /*sigsuspend(&mask);*/
    pause();
    /*sleep(1);*/
  
    
    //////  ALLER -- FAIRE LE TRAJET  //////////
    pthread_create(&pcons, 0, consommation, batterie);
  sem_wait(semD);
    shmD->state= ALLER; //on passe le drone dans la zone "voyage" de l'écran
    kill(getppid(), SIGUSR2);
  sem_post(semD);
    millisleep(colis->colis.trajet);

    //////  ATTENTE_LIVRAISON //////////////////////
  sem_wait(semD);
    shmD->state= ATTENTE_LIVRAISON;
    kill(getppid(), SIGUSR2);
  sem_post(semD);
    millisleep(rand()/(RAND_MAX/(TEMPS_RECEPTION_MAX*1000 - 1000*TEMPS_RECEPTION_MIN))+TEMPS_RECEPTION_MIN*1000);
    
    /////  RETOUR--voyage retour  //////////////////
  sem_wait(semD);
    shmD->state= RETOUR;
    kill(getppid(), SIGUSR2);
  sem_post(semD);
    millisleep(colis->colis.trajet);
    
    /////   ATTENTE_ATTERRISSAGE  ////////////////////
  sem_wait(semD);
    shmD->state= ATTENTE_ATTERRISSAGE; 
    kill(getppid(), SIGUSR2);
  sem_post(semD);
    signal(SIGUSR1, bienrecut);
    msgsnd(msgAtt, (void*)dem, sizeof(Demande)-4, 0);
    /*signal(SIGUSR1, SIG_IGN);*/
    /*sigsuspend(&mask);*/
    pause();
    
    
    /////// RECHARGE ////////////
    pthread_cancel(pcons);
    
  sem_wait(semD);
    shmD->state = RECHARGE;
    kill(getppid(), SIGUSR2);
  sem_post(semD);
    sleep(1);
    *batterie = CAPACITE_BATTERIE;
  }
  

    
  range_tout();
  
  

  /*kill(getppid(), SIGUSR2);*/
  
    
  }
