#include <stdio.h>
#include <stdlib.h>


#include "./gui/gui.h"

	const int nbDrones = 3;
	const int Largeur_Vaisseau = 15;
	const int Profondeur_soute_Vaisseau = 2;
	const int Largeur_Id_Colis = 4;
	const int general_offset_left = 10;
	
	int nbTableaux = 0; //6 + nombre de drones	
	int drone_Y_atterissage, drone_Y_voyage, drone_Y_livraison;

Tableau** initWorld()	//place les tableaux des drones sur les premières cases
{
	if(!nbTableaux)
		nbTableaux = nbDrones + 6;
	
	Tableau **T = malloc(nbTableaux*sizeof(Tableau*));
	
	
	//création de l'environnement
	Tableau *vaisseau = createTableau(Largeur_Vaisseau,
							Profondeur_soute_Vaisseau,Largeur_Id_Colis,"Cargaison Vaisseau");
	Tableau *stockDrone = createTableau(Largeur_Vaisseau,1,
							Largeur_Id_Colis,"Drones en charge");
	Tableau *departDrone = createTableau(Largeur_Vaisseau,1,
							Largeur_Id_Colis,"Drones en attente de decollage");
	Tableau *client = createTableau(Largeur_Vaisseau,Profondeur_soute_Vaisseau,
							Largeur_Id_Colis, "Ville -- Colis livrees");
							
	//Positionnnement de l'environnement
	setPos(vaisseau, general_offset_left,0);
	setPos(stockDrone, general_offset_left,Profondeur_soute_Vaisseau*2+2);
	setPos(departDrone, general_offset_left,stockDrone->Y+4);
	
	//délimitation des zones aériennes (attente atterissage / voyage / attente livraison)
	Label * limiteAtterrissageVoyage = createLabel("En attente d'atterrissage\n\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\nEn voyage", 0,departDrone->Y+4+5);
	Label * limiteVoyageLivraison = createLabel("En voyage\n\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\nEn attente de livraison", 0,limiteAtterrissageVoyage->Y+4+5);
	
	setPos(client, general_offset_left,limiteVoyageLivraison->Y+4 + 5);	

	
	//Parramétrage des drones
	drone_Y_atterissage = departDrone->Y +4;
	drone_Y_voyage = limiteAtterrissageVoyage->Y+3 +1;
	drone_Y_livraison = limiteVoyageLivraison->Y+3 +1;
	
	Tableau *drone1= createTableau(1,1,Largeur_Id_Colis,"d1");
	Tableau *drone2= createTableau(1,1,Largeur_Id_Colis,"d2");
	Tableau *drone3= createTableau(1,1,Largeur_Id_Colis,"d3");
	
	setPos(drone1, general_offset_left, drone_Y_livraison );	
	setPos(drone2, general_offset_left+Largeur_Id_Colis+1, drone_Y_voyage );	
	setPos(drone3, general_offset_left+2*Largeur_Id_Colis+2, drone_Y_atterissage );	
	
	setData(drone1, 0,0,"4|40");
	setData(drone2, 0,0,"4|41");


	T[0] = drone1;
	T[1] = drone2;
	T[2] = drone3;	
	T[3] = vaisseau;
	T[4] = stockDrone;
	T[5] = departDrone;
	T[6] = limiteAtterrissageVoyage;
	T[7] = limiteVoyageLivraison;
	T[8] = client;
	
	setData(vaisseau, 2,1,"3|00");
	setData(client, 2,1,"4|39");
	setData(departDrone, 3,0,"d4");
	setData(stockDrone, 4,0,"d5");
	setData(stockDrone, 5,0,"d6");	
	
	return T;
}

void main()
{	
	Tableau **T = initWorld();	//dessine l'univer
	draw(T, nbTableaux);
	
	
}

