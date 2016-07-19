// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "rm" pour le client

#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>

#include "rm.h"

unsigned short int rmDoCommande(int sockfd_C1, unsigned long adresseIP, int port) {
	int * status = NULL;
	char nomFichier[256];

	printf("Veuillez entrer le nom du fichier à supprimer : ");
	scanf("%s", nomFichier);

	switch(fork()) {
		case -1:    // erreur
			printf("Erreur de fork dans la commande rm");
			return EXIT_ERROR_CHILD_NOT_CREATED;
		case 0:     // fils
			execlp("rm", "rm", nomFichier, NULL);
			exit(EXIT_WITH_NO_ERROR);
		default:    // père
			wait(status);
	}
	if(WIFEXITED(status)) {
		return WEXITSTATUS(status);
	}
	return EXIT_ERROR_CHILD_INTERRUPTED;
}

commande rmCommande = {
	"rm",
	rmDoCommande
};

