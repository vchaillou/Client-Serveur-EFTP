// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "rls" pour le client

#include<stdio.h>
#include<string.h>
#include<unistd.h>

#include "rls.h"

unsigned short int rlsDoCommande(int sockfd_C1, unsigned long adresseIP, int port) {
	int nbOctetsLus;
	char buffer[BUFFER_SIZE];

	write(sockfd_C1, COMMAND_RLS, strlen(COMMAND_RLS)+1);
	
	// dernier élément du buffer réservé au '\0'
	// l'avant-dernier élément du buffer correspond au dernier élément du fichier
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

commande rlsCommande = {
	"rls",
	rlsDoCommande
};

