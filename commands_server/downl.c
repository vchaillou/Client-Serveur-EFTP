// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "downl" pour le serveur

#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

#include "downl.h"

unsigned short int downlDoCommande(int sockfd_C1, unsigned long adresseIP, int port) {
	struct sockaddr_in adresseClient;
	int sockfd_D1;
	char buffer[BUFFER_SIZE];
	int nbOctetsLus;
	FILE * file;

	if(read(sockfd_C1, buffer, BUFFER_SIZE) <= 0) {
		printf("Connexion perdue\n");
		return EXIT_ERROR_LOST_CONNECTION;
	}

	// Nouvelle connexion au client
	adresseClient.sin_addr.s_addr = adresseIP;
	adresseClient.sin_family = AF_INET;
	adresseClient.sin_port = htons(port+1);
	
	sockfd_D1 = socket(AF_INET, SOCK_STREAM, 0);

	if(connect(sockfd_D1, (struct sockaddr *)(&adresseClient), sizeof(adresseClient))) {
		printf("La connexion au client pour le transfert de fichier a échoué\n");
		close(sockfd_D1);
		return EXIT_ERROR_CONNECTION_FAILED;
	}

	file = fopen(buffer, "r");
	if(file == NULL) {
		// un fichier existe déjà
		write(sockfd_D1, SERVER_UNKNOWN, strlen(SERVER_UNKNOWN)+1);
		close(sockfd_D1);
		return EXIT_ERROR_CANNOT_OPEN_FILE;
	}

	while((nbOctetsLus = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
		write(sockfd_D1, buffer, nbOctetsLus);
	}
	
	fclose(file);
	close(sockfd_D1);
	return EXIT_WITH_NO_ERROR;
}

commande downlCommande = {
	COMMAND_DOWNL,
	downlDoCommande
};

