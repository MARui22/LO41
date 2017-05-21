#include <stdio.h>
#include <stdlib.h>


#include "./gui/gui.h"




void main()
{
	const int nbTableaux = 6;
	const int Largeur_Vaisseau = 15;
	const int Profondeur_soute_Vaisseau = 2;
	const int Largeur_Id_Colis = 4;
	
	Tableau **T = malloc(nbTableaux*sizeof(Tableau*));
	
	
	Tableau *vaisseau = createTableau(Largeur_Vaisseau,
							Profondeur_soute_Vaisseau,Largeur_Id_Colis,"Cargaison Vaisseau");
	Tableau *stockDrone = createTableau(Largeur_Vaisseau,1,
							Largeur_Id_Colis,"Drones en charge");
	Tableau *departDrone = createTableau(Largeur_Vaisseau,1,
							Largeur_Id_Colis,"Drones en attente de decollage");
	Tableau *client = createTableau(Largeur_Vaisseau,Profondeur_soute_Vaisseau,
							Largeur_Id_Colis, "Ville -- Colis livrees");
	//
	setPos(vaisseau, 10,0);
	setPos(stockDrone, 10,Profondeur_soute_Vaisseau*2+2);
	setPos(departDrone, 10,stockDrone->Y+4);
	
	
	
	Label * limiteRetourVoyage = createLabel("En attente d'atterrissage\n\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\nEn voyage", 0,departDrone->Y+4+5);
	Label * limiteVoyageClient = createLabel("En voyage\n\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\nEn attente de livraison", 0,limiteRetourVoyage->Y+3+5);
	
	setPos(client, 10,limiteVoyageClient->Y+3 + 5);	

	
	
	T[0] = vaisseau;
	T[1] = stockDrone;
	T[2] = departDrone;
	T[3] = limiteRetourVoyage;
	T[4] = limiteVoyageClient;
	T[5] = client;
	
	
	setData(vaisseau, 2,1,"0123456789");
	setData(client, 2,1,"456");
	draw(T, nbTableaux);
	//pint(countScreenChar("\u2550\u2556\n\t "), "un \\ ?");
	
}

