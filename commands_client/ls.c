// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "ls" pour le client

#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>

#include "ls.h"

unsigned short int lsDoCommande(int sockfd_C1, unsigned long adresseIP, int port) {
	int * status = NULL;
	switch(fork()) {
		case -1:    // erreur
			printf("Erreur de fork dans la commande ls");
			return EXIT_ERROR_CHILD_NOT_CREATED;
		case 0:     // fils
			execlp("ls", "ls", NULL);
			exit(EXIT_WITH_NO_ERROR);
		default:    // père
			wait(status);
	}
	if(WIFEXITED(status)) {
		return WEXITSTATUS(status);
	}
	return EXIT_ERROR_CHILD_INTERRUPTED;
}

commande lsCommande = {
	"ls",
	lsDoCommande
};

