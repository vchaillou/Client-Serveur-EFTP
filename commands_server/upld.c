// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "upld" pour le serveur

#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

#include "upld.h"

int sockfd_D2;
struct sockaddr_in adresseServeur;

unsigned short int upldInitCommande(int numPort, unsigned long int adresseIP) {
	// Nouvelle connexion au client
	adresseServeur.sin_addr.s_addr = htonl(INADDR_ANY);
	adresseServeur.sin_family = AF_INET;
	adresseServeur.sin_port = htons(numPort+2);
	
	sockfd_D2 = socket(AF_INET, SOCK_STREAM, 0);
	
	if(bind(sockfd_D2, (struct sockaddr *)(&adresseServeur), sizeof(adresseServeur))) {
		printf("Erreur sur le bind du socket D2\n");
		close(sockfd_D2);
		return EXIT_ERROR_BIND_FAILED;
	}
	listen(sockfd_D2, 1);
	return EXIT_WITH_NO_ERROR;
}

void upldDeinitCommande() {
	close(sockfd_D2);
}

unsigned short int upldDoCommande(int sockfd_C1) {
	struct sockaddr_in adresseClient;
	int sockfd_D2_client;
	socklen_t clilen = sizeof(adresseClient);
	char buffer[BUFFER_SIZE];
	int nbOctetsLus;
	FILE * file;
	
	// Pour éviter les deadlocks
	write(sockfd_C1, SERVER_READY, strlen(SERVER_READY)+1);

	if(read(sockfd_C1, buffer, BUFFER_SIZE) <= 0) {
		printf("Connexion perdue\n");
		return EXIT_ERROR_LOST_CONNECTION;
	}

	file = fopen(buffer, "r");
	if(file != NULL) {
		// un fichier existe déjà
		fclose(file);
		write(sockfd_C1, SERVER_NOT_READY, strlen(SERVER_NOT_READY)+1);
		return EXIT_ERROR_CANNOT_CREATE_FILE;
	}

	file = fopen(buffer, "w");
	if(file == NULL) {
		// Pas les droits nécessaires
		write(sockfd_C1, SERVER_NOT_READY, strlen(SERVER_NOT_READY)+1);
		return EXIT_ERROR_CANNOT_CREATE_FILE;
	}
		
	write(sockfd_C1, SERVER_READY, strlen(SERVER_READY)+1);

	sockfd_D2_client = accept(sockfd_D2, (struct sockaddr *)(&adresseClient), &clilen);

	while((nbOctetsLus = read(sockfd_D2_client, buffer, BUFFER_SIZE)) > 0) {
		fwrite(buffer, sizeof(char), nbOctetsLus, file);
	}
	
	fclose(file);
	close(sockfd_D2_client);
	return EXIT_WITH_NO_ERROR;
}

commande upldCommande = {
	COMMAND_UPLD,
	upldInitCommande,
	upldDeinitCommande,
	upldDoCommande
};

