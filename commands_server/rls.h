// Valentin CHAILLOU et Faisal CHOUDHRY
// Commande "rls" pour le serveur

#ifndef _SERVER_RLS_H
#define _SERVER_RLS_H

#include "../commun.h"

unsigned short int rlsDoCommande(int sockfd_C1, unsigned long adresseIP, int port);

commande rlsCommande;

#endif

