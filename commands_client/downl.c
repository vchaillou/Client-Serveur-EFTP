// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "downl" pour le client

#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

#include "downl.h"

int sockfd_D1;
struct sockaddr_in adresseClient;

unsigned short int downlInitCommande(int numPort, unsigned long int adresseIP) {
	
	// Nouvelle connexion au serveur
	adresseClient.sin_addr.s_addr = htonl(INADDR_ANY);
	adresseClient.sin_family = AF_INET;
	adresseClient.sin_port = htons(numPort + 1);
	
	sockfd_D1 = socket(AF_INET, SOCK_STREAM, 0);
	
	if(bind(sockfd_D1, (struct sockaddr *)(&adresseClient), sizeof(adresseClient))) {
		printf("Erreur sur le bind du socket D1\n");
		close(sockfd_D1);
		return EXIT_ERROR_BIND_FAILED;
	}
	listen(sockfd_D1, 1);

	return EXIT_WITH_NO_ERROR;
}

void downlDeinitCommande() {
	close(sockfd_D1);
}

unsigned short int downlDoCommande(int sockfd_C1) {
	struct sockaddr_in adresseServeur;
	int sockfd_D1_server;
	socklen_t servlen = sizeof(adresseServeur);
	char buffer[BUFFER_SIZE];
	char nomFichier[BUFFER_SIZE];
	int nbOctetsLus;
	FILE * file;

	printf("Veuiller entrer le nom du fichier : ");
	scanf("%s", nomFichier);

	file = fopen(nomFichier, "w");
	if(file == NULL) {
		printf("Le client ne peut pas créer le fichier \"%s\"\n", nomFichier);
		return EXIT_ERROR_CANNOT_CREATE_FILE;
	}
	
	write(sockfd_C1, COMMAND_DOWNL, strlen(COMMAND_DOWNL)+1);

	// On attend que le serveur soit prêt
	if(read(sockfd_C1, buffer, BUFFER_SIZE) <= 0 || (strcmp(buffer, SERVER_READY) != 0)) {
		printf("Connexion perdue\n");
		return EXIT_ERROR_LOST_CONNECTION;
	}
	write(sockfd_C1, nomFichier, strlen(nomFichier)+1);

	sockfd_D1_server = accept(sockfd_D1, (struct sockaddr *)(&adresseServeur), &servlen);

	if((nbOctetsLus = read(sockfd_D1_server, buffer, BUFFER_SIZE)) <= 0) {
		printf("Commande inconnue\n");
		close(sockfd_D1_server);
		return EXIT_ERROR_LOST_CONNECTION;
	}

	if(strcmp(buffer, SERVER_UNKNOWN) == 0) {
		printf("Le serveur ne peut pas ouvrir le fichier\n");
		fclose(file);
		remove(nomFichier);
		close(sockfd_D1_server);
		return EXIT_ERROR_CANNOT_OPEN_FILE;
	}

	do {
		fwrite(buffer, sizeof(char), nbOctetsLus, file);
	}
	while((nbOctetsLus = read(sockfd_D1_server, buffer, BUFFER_SIZE)) > 0);
	
	fclose(file);
	close(sockfd_D1_server);
	return EXIT_WITH_NO_ERROR;
}

commande downlCommande = {
	"downl",
	downlInitCommande,
	downlDeinitCommande,
	downlDoCommande
};

