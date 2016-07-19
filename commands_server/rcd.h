// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "rcd" pour le serveur

#ifndef _SERVER_RCD_H
#define _SERVER_RCD_H

#include "../commun.h"

unsigned short int rcdDoCommande(int sockfd_C1, unsigned long adresseIP, int port);

commande rcdCommande;

#endif

