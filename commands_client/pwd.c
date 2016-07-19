// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "pwd" pour le client

#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>

#include "pwd.h"

unsigned short int pwdInitCommande(int numPort, unsigned long int adresseIP) {
	return EXIT_WITH_NO_ERROR;
}

void pwdDeinitCommande() {
}

unsigned short int pwdDoCommande(int sockfd_C1) {
	int * status = NULL;
	switch(fork()) {
		case -1:    // erreur
			printf("Erreur de fork dans la commande pwd");
			return EXIT_ERROR_CHILD_NOT_CREATED;
		case 0:     // fils
			execlp("pwd", "pwd", NULL);
			exit(EXIT_WITH_NO_ERROR);
		default:    // père
			wait(status);
	}
	if(WIFEXITED(status)) {
		return WEXITSTATUS(status);
	}
	return EXIT_ERROR_CHILD_INTERRUPTED;
}

commande pwdCommande = {
	"pwd",
	pwdInitCommande,
	pwdDeinitCommande,
	pwdDoCommande
};

