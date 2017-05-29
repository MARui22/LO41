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
 
 void main(int argc, char *argv[]){
 
   srand(time(NULL)); //préparation aux chiffres aléatoires
   
   int * y = shmat(atoi(argv[1]), NULL, 0); //controle la position Y du drone dans l'univer
   char* Id_colis = shmat(atoi(argv[2]), NULL, 0);  //controle l'affichage du nom du colis
   
  sleep(rand()/(RAND_MAX/2)+2);
  *y = drone_Y_voyage;  //on passe le drone dans la zone "voyage" de l'écran
  kill(getppid(), SIGUSR1);


  sleep(rand()/(RAND_MAX/2)+2);
  *y = drone_Y_livraison; //on passe le drone dans la zone "livraison" de l'écran
  kill(getppid(), SIGUSR1);
  
  sleep(rand()/(RAND_MAX/2)+2);
  *y = drone_Y_atterissage;
  strcpy(Id_colis, "0123"); //on décharge le colis !!! ATTENTION !!! TOUJOURS utiliser strcpy(), car un "=" fait sauter la référence à la mémoire partagée !!!!!!
  kill(getppid(), SIGUSR1);
  
  
  sleep(rand()/(RAND_MAX/2)+2);
  *y = drone_Y_dead;  //on passe le drone dans la zone "mort" de l'écran

  kill(getppid(), SIGUSR1);
  
  
  shmdt(y);
  shmdt(Id_colis);
  kill(getppid(), SIGUSR2);
  }