// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "downl" pour le serveur

#ifndef _SERVER_DOWNL_H
#define _SERVER_DOWNL_H

#include "../commun.h"

unsigned short int downlDoCommande(int sockfd_C1, unsigned long adresseIP, int port);

commande downlCommande;

#endif

