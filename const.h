#ifndef CONST
#define CONST

#include <sys/ipc.h>
#include <sys/sem.h>
//#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>

#define NBDRONES 5
#define LARGEUR_VAISSEAU 5
#define PROFONDEUR_SOUTE_VAISSEAU 2
#define LARGEUR_ID_COLIS 4
#define GENERAL_OFFSET_LEFT 10

#define TRAJET_MIN 1
#define TRAJET_MAX 4

#define NBCOLIS (LARGEUR_VAISSEAU*PROFONDEUR_SOUTE_VAISSEAU)
#define nbTableaux (7+NBDRONES) //7 + nombre de drones	
#define drone_Y_voyage (9+4+4+2+2*PROFONDEUR_SOUTE_VAISSEAU)
#define drone_Y_atterissage (10+2*PROFONDEUR_SOUTE_VAISSEAU)
#define drone_Y_livraison (28+2*PROFONDEUR_SOUTE_VAISSEAU)
#define drone_Y_dead (8+2*PROFONDEUR_SOUTE_VAISSEAU+drone_Y_livraison)
#define drone_Y_repos PROFONDEUR_SOUTE_VAISSEAU

enum droneState {RECHARGE =1, ATTENTE_DEPART=2, ALLER=4, ATTENTE_LIVRAISON=8, RETOUR=16, ATTENTE_ATTERRISSAGE=32, DEAD=64};

typedef struct {
  enum droneState state;
  char colis[LARGEUR_ID_COLIS +1];
  int id_colis;
} IPCDrone;
typedef unsigned short ushort;

typedef struct {
  int id;
  int prio;
  int trajet;
  } Colis;
  
  typedef struct {
  long type;
  int test;
  }msgTest;

typedef struct {
  long type;
  Colis colis;
  }msgColis;
  //int shmD[NBDRONES]; //liste des mémoires partagées des drones

  
struct sembuf sem_oper_P ;  /* Operation P */
struct sembuf sem_oper_V ;  /* Operation V */


 void P(int sem_id, int numShm) {
sem_oper_P.sem_num = numShm;
sem_oper_P.sem_op  = -1 ;
sem_oper_P.sem_flg = 0 ;
semop(sem_id, &sem_oper_P, 1);
}

void V(int sem_id, int numShm) {

sem_oper_V.sem_num = numShm;
sem_oper_V.sem_op  = 1 ;
sem_oper_V.sem_flg  = 0 ;
semop(sem_id, &sem_oper_V, 1);
} 
  
  
#endif