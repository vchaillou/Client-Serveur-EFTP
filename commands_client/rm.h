// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "rm" pour le client

#ifndef _CLIENT_RM_H
#define _CLIENT_RM_H

#include "../commun.h"

unsigned short int rmDoCommande(int sockfd_C1, unsigned long adresseIP, int port);

commande rmCommande;

#endif
