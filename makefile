CC:=gcc
OPTS:=-Wall
SERVEUR:=serveur
CLIENT:=client
CCOBJ:=gcc -c

COMMANDS_SERVER_DIR:=commands_server
COMMANDS_SERVER:=${shell find ${COMMANDS_SERVER_DIR}/*.h -printf "${COMMANDS_SERVER_DIR}/%f " | tr '.h' '.o'}
OPTS_SERVER:=-DNB_COMMANDS_SERVER=${shell ls ${COMMANDS_SERVER_DIR}/*.h | wc -l}

COMMANDS_CLIENT_DIR:=commands_client
COMMANDS_CLIENT:=${shell find ${COMMANDS_CLIENT_DIR}/*.h -printf "${COMMANDS_CLIENT_DIR}/%f " | tr '.h' '.o'}
OPTS_CLIENT:=-DNB_COMMANDS_CLIENT=${shell ls ${COMMANDS_CLIENT_DIR}/*.h | wc -l}

all: ${SERVEUR} ${CLIENT}

${SERVEUR}: serveur.o ${COMMANDS_SERVER}
	${CC} ${OPTS} $+ -o $@

serveur.o: serveur.c commun.h
	${CCOBJ} ${OPTS} ${OPTS_SERVER} serveur.c -o $@

${CLIENT}: client.o ${COMMANDS_CLIENT}
	${CC} ${OPTS} $+ -o $@

client.o: client.c commun.h
	${CCOBJ} ${OPTS} ${OPTS_CLIENT} client.c -o $@

%.o: %.c %.h
	${CCOBJ} ${OPTS} $< -o $@

clean:
	rm -rf *.o && rm -rf commands_client/*.o && rm -rf commands_server/*.o

