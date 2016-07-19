// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "ls" pour le client

#ifndef _CLIENT_LS_H
#define _CLIENT_LS_H

#include "../commun.h"

unsigned short int lsDoCommande(int sockfd_C1, unsigned long adresseIP, int port);

commande lsCommande;

#endif
