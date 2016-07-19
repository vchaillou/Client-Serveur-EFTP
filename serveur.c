// Valentin CHAILLOU - Faisal CHOUDHRY
// Serveur EFTP

#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>

#include "commun.h"

#define MAX_RETRY               3
#define USER_DB_FILENAME        "users"
#define FILE_LINE_LENGTH        256

/*** DEBUT COMMANDES ***/

#include "commands_server/rls.h"
#include "commands_server/rpwd.h"
#include "commands_server/rcd.h"
#include "commands_server/upld.h"
#include "commands_server/downl.h"
commande * commandes[NB_COMMANDS_SERVER] = {
	&rlsCommande, 
	&rpwdCommande, 
	&rcdCommande, 
	&upldCommande, 
	&downlCommande
};

/*** FIN COMMANDES ***/

// Vérifie si le user existe bien en cherchant le username et le mdp dans le fichier prévu à cet effet
// Un utilisateur existe si une ligne du fichier contient bien le username suivi d'un espace puis du mdp
// Renvoie 1/vrai si c'est bon, 0/faux sinon
short int checkUser(char username[], char password[]) {
	int result = 0;
	FILE * file = fopen(USER_DB_FILENAME, "r");
	char line[FILE_LINE_LENGTH];
	char fileUsername[FILE_LINE_LENGTH];
	char filePassword[FILE_LINE_LENGTH];
	
	if(file != NULL) {
		while(fgets(line, FILE_LINE_LENGTH, file) != NULL) {
			sscanf(line, "%s %s", fileUsername, filePassword);
			if(strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
				result = 1;
				break;
			}
		}
		fclose(file);
	}
	return result;
}

// Fonction principale
int main(int argc, char ** argv) {
	struct sockaddr_in adresseServeur, adresseClient;
	socklen_t clilen = sizeof(adresseClient);

	char buffer[BUFFER_SIZE];
	char username[BUFFER_SIZE];
	char password[BUFFER_SIZE];

	int sockfd, sockfd_client;

	int numEssai = 0;
	int i;

	if(argc > 2) {
		printf("Usage : %s [Port]", argv[0]);
		exit(EXIT_ERROR_WRONG_USAGE);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	adresseServeur.sin_family = AF_INET;
	adresseServeur.sin_port = htons((argc>=2) ? atoi(argv[1]) : SERVER_PORT);
	adresseServeur.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(sockfd, (struct sockaddr *)(&adresseServeur), sizeof(adresseServeur))) {
		printf("Erreur sur le bind du socket C1\n");
		close(sockfd);
		exit(EXIT_ERROR_BIND_FAILED);
	}
	while(1) {

		// Connexion avec client
		listen(sockfd, 1);
		sockfd_client = accept(sockfd, (struct sockaddr *)(&adresseClient), &clilen);
		if(read(sockfd_client, buffer, BUFFER_SIZE) <= 0 || strcmp(buffer, CLIENT_INIT) != 0) {
			printf("L'authentification du client a échoué\n");
			close(sockfd_client);
			continue;
		}
		for( ; numEssai < MAX_RETRY ; numEssai++) {
			write(sockfd_client, SERVER_USER, strlen(SERVER_USER)+1);
			if(read(sockfd_client, username, BUFFER_SIZE) <= 0) {
				numEssai = -1;
				break;
			}
			write(sockfd_client, SERVER_PASSWORD, strlen(SERVER_PASSWORD)+1);
			if(read(sockfd_client, password, BUFFER_SIZE) <= 0) {
				numEssai = -1;
				break;
			}
			// Vérification de l'utilisateur
			if(checkUser(username, password)) {
				break;
			}
		}
		if(numEssai >= MAX_RETRY || numEssai < 0) {
			if(numEssai >= MAX_RETRY) {
				printf("Nombre maximum d'essais atteint\n");
				write(sockfd_client, SERVER_DISCONNECT, strlen(SERVER_DISCONNECT)+1);
			}
			else {
				printf("L'authentification du client a échoué\n");
			}
			close(sockfd_client);
			numEssai = 0;
			continue;
		}
		write(sockfd_client, SERVER_WELCOME, strlen(SERVER_WELCOME)+1);
		
		printf("Connexion établie\n");

		// Traitement des commandes
		do {
			if(read(sockfd_client, buffer, BUFFER_SIZE) <= 0) {
				printf("Connexion perdue avec le client\n");
				break;
			}
			for(i=0 ; i < NB_COMMANDS_SERVER ; i++) {
				if(strcmp(buffer, commandes[i]->idCommande) == 0) {
					if(commandes[i]->doCommande(sockfd_client, adresseClient.sin_addr.s_addr, 
						(argc>=2) ? atoi(argv[1]) : SERVER_PORT)) {
						printf("Une erreur s'est produite dans la commande \"%s\"\n", 
							commandes[i]->idCommande);
					}
					break;
				}
			}
		}
		while(strcmp(buffer, CLIENT_DISCONNECT) != 0);
		close(sockfd_client);
		printf("Déconnexion\n");
	}
	close(sockfd);
	
	exit(EXIT_WITH_NO_ERROR);
}

