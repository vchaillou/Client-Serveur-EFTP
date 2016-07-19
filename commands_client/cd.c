// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "cd" pour le client

#include<stdio.h>
#include<unistd.h>
#include<errno.h>

#include "cd.h"

unsigned short int cdDoCommande(int sockfd_C1, unsigned long adresseIP, int port) {
	char nomRepertoire[TAILLE_NOM_FICHIER];

	printf("Veuillez entrer le nom du répertoire : ");
	scanf("%s", nomRepertoire);
	if(chdir(nomRepertoire) != 0) {
		switch(errno) {
			case ENOTDIR:
			case ENOENT:
				printf("Répertoire inexistant\n");
				break;
			default:
				printf("Erreur inconnue. Le répertoire courant n'a pas été modifié\n");
		}
		return EXIT_ERROR_FILE_DOES_NOT_EXIST;
	}
	return EXIT_WITH_NO_ERROR;
}

commande cdCommande = {
	"cd",
	cdDoCommande
};

