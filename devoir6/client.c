
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include "common.h"

int main (int argc ,char* argv[]){

	if(argc<3){
		printf("Erreur : nombre d'arguments insuffisant \n");
		printf("Usage : %s Adresse_IP port\n", argv[0]);
		return 0;

	}

	//creation d'une socket 
	int socketClient = socket (AF_INET,SOCK_STREAM,0);

	if(socketClient == -1){
		perror ("Erreur de creation de la socket \n");
		return 0;
	}
	printf("Creation de la socket avec succes \n");

 	// initialisation de la structure 

	struct sockaddr_in addrServer;

	memset(&addrServer,0,sizeof(struct sockaddr_in));

	addrServer.sin_family = AF_INET;
	addrServer.sin_port=htons(atoi(argv[2]));
	addrServer.sin_addr.s_addr =inet_addr(argv[1]);

	// appel de connect 
	
	int resultat =connect(socketClient ,(struct sockaddr *) &addrServer ,sizeof(struct sockaddr_in));

	if(resultat == -1){
		perror("Erreur d'ouverture de la connexion\n");
		return 0;
	}



	// logique de l'application
	// buffers

	 int counter;
    	char buffer[BUF_SIZE];
	int i=0;
	int nbReceive;
	while(1){

		memset(buffer, 0, BUF_SIZE);
		nbReceive = recv(socketClient,buffer, BUF_SIZE, 0);

		if(nbReceive>0)
		{
			printf("[%d]Compteur reçu du serveur:%s \n",i,buffer);
		}

		counter = atoi(buffer);
		counter++;

		sleep(1);

		memset(buffer, 0, BUF_SIZE);
		sprintf(buffer, "%d", counter);
		send(socketClient,buffer,strlen(buffer),0);
		printf(" Compteur envoyé au serveur %s \n",buffer);
		i++;
	}
	close(socketClient);
	return 0;
}