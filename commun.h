// Valentin CHAILLOU - Faisal CHOUDHRY
// Valeurs communes Client/serveur EFTP

#ifndef _COMMUN_H
#define _COMMUN_H

/*** COMMANDES ***/

typedef struct {
	char * idCommande;
	unsigned short int (*doCommande)(int sockfd_C1, unsigned long adresseIP, int port);
} commande;

#define COMMAND_UPLD           "upld"
#define COMMAND_DOWNL          "downl"
#define COMMAND_RLS            "rls"
#define COMMAND_RCD            "rcd"
#define COMMAND_RPWD           "rpwd"

/*** VALEURS FIXES/PAR DEFAUT ***/

#define SERVER_PORT 23456
#define BUFFER_SIZE 1024
#define TAILLE_NOM_FICHIER 256
#define NOM_FICHIER_TEMP "TEMP"

/*** CODES DE NEGOCIATION CLIENT/SERVEUR ***/

#define CLIENT_INIT            "BONJ"
#define SERVER_INIT_ANSWER     "WHO"
#define SERVER_PASSWORD        "PASSWD"
#define SERVER_WELCOME         "WELC"
#define SERVER_DISCONNECT      "BYE"

#define SERVER_READY           "RDY"
#define SERVER_NOT_READY       "FBDN"
#define SERVER_UNKNOWN         "UNKWN"
#define SERVER_CD_OK           "CDOk"
#define SERVER_CD_FAIL         "NOCD"

#define SERVER_USER SERVER_INIT_ANSWER
#define CLIENT_DISCONNECT SERVER_DISCONNECT

/*** CODES D'ERREUR ***/

#define EXIT_WITH_NO_ERROR                         0

#define EXIT_ERROR_INVALID_IP                      1
#define EXIT_ERROR_CONNECTION_FAILED               2
#define EXIT_ERROR_LOST_CONNECTION                 3
#define EXIT_ERROR_WRONG_USER_OR_PASSWORD          4
#define EXIT_ERROR_CHILD_NOT_CREATED               5
#define EXIT_ERROR_CHILD_INTERRUPTED               6
#define EXIT_ERROR_BIND_FAILED                     7
#define EXIT_ERROR_WRONG_USAGE                     8
#define EXIT_ERROR_FILE_DOES_NOT_EXIST             9
#define EXIT_ERROR_CANNOT_CREATE_FILE             10
#define EXIT_ERROR_CANNOT_OPEN_FILE               11

#endif
