// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "rcd" pour le client

#include<stdio.h>
#include<string.h>
#include<unistd.h>

#include "rcd.h"

unsigned short int rcdInitCommande(int numPort, unsigned long int adresseIP) {
	return EXIT_WITH_NO_ERROR;
}

void rcdDeinitCommande() {
}

unsigned short int rcdDoCommande(int sockfd_C1) {
	char buffer[BUFFER_SIZE];

	write(sockfd_C1, COMMAND_RCD, strlen(COMMAND_RCD)+1);

	printf("Veuillez entrer le nom du répertoire : ");
	scanf("%s", buffer);

	write(sockfd_C1, buffer, strlen(buffer)+1);

	if(read(sockfd_C1, buffer, BUFFER_SIZE) <= 0 || (strcmp(buffer, SERVER_CD_OK) != 0 && strcmp(buffer, SERVER_CD_FAIL) != 0)) {
		printf("Réponse incorrecte du serveur\n");
		return EXIT_ERROR_LOST_CONNECTION;
	}
	if(strcmp(buffer, SERVER_CD_OK) != 0) {
		printf("Erreur dans le changement de répertoire\n");
	}

	return EXIT_WITH_NO_ERROR;
}

commande rcdCommande = {
	"rcd",
	rcdInitCommande,
	rcdDeinitCommande,
	rcdDoCommande
};

