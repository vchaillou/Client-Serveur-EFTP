// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "rpwd" pour le serveur

#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<fcntl.h>

#include "rpwd.h"

unsigned short int rpwdDoCommande(int sockfd_C1, unsigned long adresseIP, int port) {
	int nbOctetsLus;
	char buffer[BUFFER_SIZE];

	int * status = NULL;
	int fd;

	switch(fork()) {
		case -1:    // erreur
			printf("Erreur de fork dans la commande rpwd");
			return EXIT_ERROR_CHILD_NOT_CREATED;
		case 0:     // fils
			fd = open(NOM_FICHIER_TEMP, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR);
			dup2(fd, STDOUT_FILENO);
			execlp("pwd", "pwd", NULL);
			close(fd);
			exit(EXIT_WITH_NO_ERROR);
		default:    // père
			wait(status);
	}
	fd = open(NOM_FICHIER_TEMP, O_RDONLY);
	while((nbOctetsLus = read(fd, buffer, BUFFER_SIZE-1)) > 0) {
		buffer[nbOctetsLus] = '\0';	// A ajouter car il n'est pas contenu dans la lecture du fichier
		write(sockfd_C1, buffer, nbOctetsLus+1);
	}
	close(fd);
	
	// Pour éviter les conflits sur les prochaines commandes
	remove(NOM_FICHIER_TEMP);

	if(WIFEXITED(status)) {
		return WEXITSTATUS(status);
	}
	return EXIT_ERROR_CHILD_INTERRUPTED;
}

commande rpwdCommande = {
	COMMAND_RPWD,
	rpwdDoCommande
};

