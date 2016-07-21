// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "upld" pour le client

#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

#include "upld.h"

int upldNumPort = 0;
unsigned long int upldAdresseIP;

unsigned short int upldInitCommande(int numPort, unsigned long int adresseIP) {
	upldNumPort = numPort + 2;
	upldAdresseIP = adresseIP;
	return EXIT_WITH_NO_ERROR;
}

void upldDeinitCommande() {
}

unsigned short int upldDoCommande(int sockfd_C1) {
	struct sockaddr_in adresseServeur;
	int sockfd_D2;
	char nomFichier[TAILLE_NOM_FICHIER];
	char buffer[BUFFER_SIZE];
	FILE * file;
	int nbOctetsLus;

	printf("Veuiller entrer le nom du fichier : ");
	scanf("%s", nomFichier);
	file = fopen(nomFichier, "r");
	if(file == NULL) {
		printf("Le fichier \"%s\" n'existe pas\n", nomFichier);
		return EXIT_ERROR_FILE_DOES_NOT_EXIST;
	}

	write(sockfd_C1, COMMAND_UPLD, strlen(COMMAND_UPLD)+1);
	
	// Pour éviter les deadlocks
	if(read(sockfd_C1, buffer, BUFFER_SIZE) <= 0 || (strcmp(buffer, SERVER_READY) != 0 && 
		strcmp(buffer, SERVER_NOT_READY) != 0)) {
		printf("Le serveur n'a pas répondu correctement\n");
		return EXIT_ERROR_LOST_CONNECTION;
	}

	write(sockfd_C1, nomFichier, strlen(nomFichier)+1);

	if(read(sockfd_C1, buffer, BUFFER_SIZE) <= 0 || (strcmp(buffer, SERVER_READY) != 0 && 
		strcmp(buffer, SERVER_NOT_READY) != 0)) {
		printf("Le serveur n'a pas répondu correctement\n");
		return EXIT_ERROR_LOST_CONNECTION;
	}
	if(strcmp(buffer, SERVER_READY) != 0) {
		printf("Le serveur ne peut pas écrire le fichier\n");
		return EXIT_ERROR_CANNOT_CREATE_FILE;
	}

	// Nouvelle connexion au serveur
	adresseServeur.sin_addr.s_addr = upldAdresseIP;
	adresseServeur.sin_family = AF_INET;
	adresseServeur.sin_port = htons(upldNumPort);
	
	sockfd_D2 = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(sockfd_D2, (struct sockaddr *)(&adresseServeur), sizeof(adresseServeur))) {
		printf("La connexion au serveur pour le transfert de fichier a échoué\n");
		close(sockfd_D2);
		return EXIT_ERROR_CONNECTION_FAILED;
	}

	while((nbOctetsLus = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
		write(sockfd_D2, buffer, nbOctetsLus);
	}

	fclose(file);
	close(sockfd_D2);
	return EXIT_WITH_NO_ERROR;
}

commande upldCommande = {
	"upld",
	upldInitCommande,
	upldDeinitCommande,
	upldDoCommande
};

