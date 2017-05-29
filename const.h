#ifndef CONST
#define CONST

#define NBDRONES 10
#define LARGEUR_VAISSEAU 15
#define PROFONDEUR_SOUTE_VAISSEAU 2
#define LARGEUR_ID_COLIS 4
#define GENERAL_OFFSET_LEFT 10
#define nbTableaux (7+NBDRONES) //7 + nombre de drones	
#define drone_Y_voyage (9+4+4+2+2*PROFONDEUR_SOUTE_VAISSEAU)
#define drone_Y_atterissage (10+2*PROFONDEUR_SOUTE_VAISSEAU)
#define drone_Y_livraison (28+2*PROFONDEUR_SOUTE_VAISSEAU)
#define drone_Y_dead (8+2*PROFONDEUR_SOUTE_VAISSEAU+drone_Y_livraison)

  //int shmD[NBDRONES]; //liste des mémoires partagées des drones

#endif