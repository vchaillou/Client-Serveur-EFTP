// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "rpwd" pour le client

#ifndef _CLIENT_RPWD_H
#define _CLIENT_RPWD_H

#include "../commun.h"

unsigned short int rpwdDoCommande(int sockfd_C1, unsigned long adresseIP, int port);

commande rpwdCommande;

#endif

