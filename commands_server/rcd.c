// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "rcd" pour le serveur

#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

#include "rcd.h"

unsigned short int rcdInitCommande(int numPort, unsigned long int adresseIP) {
	return EXIT_WITH_NO_ERROR;
}

void rcdDeinitCommande() {
}

unsigned short int rcdDoCommande(int sockfd_C1) {
	char buffer[BUFFER_SIZE];

	if(read(sockfd_C1, buffer, BUFFER_SIZE) <= 0) {
		printf("Connexion perdue\n");
		return EXIT_ERROR_LOST_CONNECTION;
	}

	if(chdir(buffer) != 0) {
		switch(errno) {
			case ENOTDIR:
			case ENOENT:
				printf("Répertoire inexistant\n");
				break;
			default:
				printf("Erreur inconnue. Le répertoire courant n'a pas été modifié\n");
		}
		write(sockfd_C1, SERVER_CD_FAIL, strlen(SERVER_CD_FAIL)+1);
		return EXIT_ERROR_FILE_DOES_NOT_EXIST;
	}
	write(sockfd_C1, SERVER_CD_OK, strlen(SERVER_CD_OK)+1);
	return EXIT_WITH_NO_ERROR;
}

commande rcdCommande = {
	COMMAND_RCD,
	rcdInitCommande,
	rcdDeinitCommande,
	rcdDoCommande
};

