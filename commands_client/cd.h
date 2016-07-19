// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "cd" pour le client

#ifndef _CLIENT_CD_H
#define _CLIENT_CD_H

#include "../commun.h"

unsigned short int cdDoCommande(int sockfd_C1, unsigned long adresseIP, int port);

commande cdCommande;

#endif
