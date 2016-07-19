// Valentin CHAILLOU - Faisal CHOUDHRY
// Client EFTP

#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

#include "commun.h"

#define DEFAULT_SERVER_ADDRESS "127.0.0.1"

/*** DEBUT COMMANDES ***/

#include "commands_client/ls.h"
#include "commands_client/cd.h"
#include "commands_client/rm.h"
#include "commands_client/pwd.h"
#include "commands_client/rls.h"
#include "commands_client/rpwd.h"
#include "commands_client/rcd.h"
#include "commands_client/upld.h"
#include "commands_client/downl.h"
commande * commandes[NB_COMMANDS_CLIENT] = {
	&lsCommande, 
	&cdCommande, 
	&rmCommande, 
	&pwdCommande, 
	&rlsCommande, 
	&rpwdCommande, 
	&rcdCommande, 
	&upldCommande, 
	&downlCommande
};

/*** FIN COMMANDES ***/
// Fonction principale
int main(int argc, char ** argv) {
	struct sockaddr_in adresseServeur;
	int sockfd;
	
	char buffer[BUFFER_SIZE];
	int i;

	int numPort = (argc>=3) ? atoi(argv[2]) : SERVER_PORT;

	if(argc > 3) {
		printf("Usage : %s [Adresse IP] [Port réseau]", argv[0]);
		exit(EXIT_ERROR_WRONG_USAGE);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/*** Adresses ***/

	adresseServeur.sin_addr.s_addr = 0;
	adresseServeur.sin_family = AF_INET;
	adresseServeur.sin_port = htons(numPort);
	inet_pton(AF_INET, (argc>=2) ? argv[1] : DEFAULT_SERVER_ADDRESS, &(adresseServeur.sin_addr.s_addr));

	if(adresseServeur.sin_addr.s_addr < 1) {
		printf("Adresse IP invalide\n");
		close(sockfd);
		exit(EXIT_ERROR_INVALID_IP);
	}

	// Tentative de connexion au serveur
	if(connect(sockfd, (struct sockaddr *)(&adresseServeur), sizeof(adresseServeur))) {
		printf("La connexion au serveur a échoué\n");
		close(sockfd);
		exit(EXIT_ERROR_CONNECTION_FAILED);
	}


	write(sockfd, CLIENT_INIT, strlen(CLIENT_INIT)+1);

	if(read(sockfd, buffer, BUFFER_SIZE) <= 0) {
		printf("L'authentification auprès du serveur a échoué\n");
		close(sockfd);
		exit(EXIT_ERROR_LOST_CONNECTION);
	}

	do {
		if(strcmp(buffer, SERVER_INIT_ANSWER) != 0) {
			printf("L'authentification auprès du serveur a échoué\n");
			close(sockfd);
			exit(EXIT_ERROR_LOST_CONNECTION);
		}
		printf("Username : ");
		scanf("%s", buffer);
		write(sockfd, buffer, strlen(buffer)+1);
		if(read(sockfd, buffer, BUFFER_SIZE) <= 0 || strcmp(buffer, SERVER_PASSWORD) != 0) {
			printf("L'authentification auprès du serveur a échoué\n");
			close(sockfd);
			exit(EXIT_ERROR_LOST_CONNECTION);
		}
		printf("Mot de passe : ");
		scanf("%s", buffer);
		write(sockfd, buffer, strlen(buffer)+1);
		
		if(read(sockfd, buffer, BUFFER_SIZE) <= 0) {
			printf("L'authentification auprès du serveur a échoué\n");
			close(sockfd);
			exit(EXIT_ERROR_LOST_CONNECTION);
		}
	}
	while(strcmp(buffer, SERVER_WELCOME) != 0 && strcmp(buffer, SERVER_DISCONNECT) != 0);
	if(strcmp(buffer, SERVER_WELCOME) != 0) {
		printf("Nombre maximum d'essais atteint\n");
		close(sockfd);
		exit(EXIT_ERROR_WRONG_USER_OR_PASSWORD);
	}

	// Initialisation des commandes
	for(i=0 ; i < NB_COMMANDS_CLIENT ; i++) {
		commandes[i]->initCommande(numPort, adresseServeur.sin_addr.s_addr);
	}

	// Interpréteur de commandes
	do {
		printf(" >>> ");
		scanf("%s", buffer);
		for(i=0 ; i < NB_COMMANDS_CLIENT ; i++) {
			if(strcmp(buffer, commandes[i]->idCommande) == 0) {
				if(commandes[i]->doCommande(sockfd)) {
					printf("Une erreur s'est produite dans la commande \"%s\"\n", 
						commandes[i]->idCommande);
				}
				break;
			}
		}
	}
	while(strcmp(buffer, "exit") != 0);
	write(sockfd, CLIENT_DISCONNECT, strlen(CLIENT_DISCONNECT)+1);

	// Désinitialisation des commandes
	for(i=0 ; i < NB_COMMANDS_CLIENT ; i++) {
		commandes[i]->deinitCommande();
	}
	
	close(sockfd);
	exit(EXIT_WITH_NO_ERROR);
}

