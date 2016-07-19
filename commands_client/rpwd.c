// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "rpwd" pour le client

#include<stdio.h>
#include<string.h>
#include<unistd.h>

#include "rpwd.h"

unsigned short int rpwdInitCommande(int numPort, unsigned long int adresseIP) {
	return EXIT_WITH_NO_ERROR;
}

void rpwdDeinitCommande() {
}

unsigned short int rpwdDoCommande(int sockfd_C1) {
	int nbOctetsLus;
	char buffer[BUFFER_SIZE];

	write(sockfd_C1, COMMAND_RPWD, strlen(COMMAND_RPWD)+1);

	while((nbOctetsLus = read(sockfd_C1, buffer, BUFFER_SIZE)) > 0) {
		printf("%s", buffer);
		// dernier élément du buffer réservé au '\0'
		// l'avant-dernier élément du buffer correspond au dernier élément du fichier
		if(nbOctetsLus < BUFFER_SIZE || buffer[BUFFER_SIZE-2] == EOF) {
			break;
		}
	}
	return EXIT_WITH_NO_ERROR;
}

commande rpwdCommande = {
	"rpwd",
	rpwdInitCommande,
	rpwdDeinitCommande,
	rpwdDoCommande
};

