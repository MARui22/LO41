#ifndef CONST
#define CONST



#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <semaphore.h>

#include <fcntl.h> 


#define NBDRONES 30
#define LARGEUR_VAISSEAU 30
#define PROFONDEUR_SOUTE_VAISSEAU 2 //ATTENTION !!! DOIT IMPÉRATIVEMENT ÊTRE UN NOMBRE PAIRE !!!
#define GENERAL_OFFSET_LEFT 10

#define TRAJET_MIN 1          //temps de trajet minimu du vaisseau au ballon
#define TRAJET_MAX 2
#define TEMPS_RECEPTION_MIN 1 //temps de réaction minimum du client qui receptione son colis
#define TEMPS_RECEPTION_MAX 4
#define TEMPS_MANOEUVRE_DECOLLAGE 250 // en milliseconde
#define TEMPS_MANOEUVRE_ATTERRISSAGE 300 // en milliseconde
#define CAPACITE_BATTERIE 4             //Temps avant que la batterie ne s'épuise
#define FACTEUR_RECHARGEMENT_BATTERIE 1 //ex : 2 => rechargement 2 fois plus lent que la consommation

#if LARGEUR_VAISSEAU*PROFONDEUR_SOUTE_VAISSEAU >100
  #define LARGEUR_ID_COLIS 5
#else
  #define LARGEUR_ID_COLIS 4
#endif

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
  long type;
  pid_t demandeur;
  } Demande;
  
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

void sem_set(sem_t *sem, int i, int* buff)
{
sem_getvalue(sem, buff);
  
  while( *buff !=i)
  {
    if(*buff <i)
      sem_post(sem);
    else if(*buff>i)
      sem_wait(sem);
    
    sem_getvalue(sem, buff);
  }

}

int shmDelNCreat(size_t size)
{int shmId;
  if((shmId = shmget(IPC_PRIVATE, size, 0666|IPC_CREAT|IPC_EXCL) )== -1)
  {
    shmId = shmget(IPC_PRIVATE, size, 0666);
    shmctl(shmId, IPC_RMID, 0);
    return shmget(IPC_PRIVATE, size, 0666|IPC_CREAT);
  }
  return shmId;

}

sem_t* semDelNCreat(char* name, int init)
{sem_t* semId;
  if((semId = sem_open(name, O_CREAT|O_EXCL, 0600, init))== SEM_FAILED)
  {
    sem_unlink(name);
    return sem_open(name, O_CREAT, 0600, init);
  }
  return semId;

}


  
#endif