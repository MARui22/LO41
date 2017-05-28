#ifndef CONST
#define CONST

#define NBDRONES 3
#define LARGEUR_VAISSEAU 15
#define PROFONDEUR_SOUTE_VAISSEAU 2
#define LARGEUR_ID_COLIS 4
#define GENERAL_OFFSET_LEFT 10
	
	int nbTableaux = 0; //6 + nombre de drones	
	int drone_Y_atterissage, drone_Y_voyage, drone_Y_livraison, drone_Y_dead;

  int shmD[NBDRONES]; //liste des mémoires partagées des drones

#endif