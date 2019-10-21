#include "Cliente.h"


Cliente::Cliente(char *puerto){

	setPortAndSocket(puerto);

}

Cliente::Cliente(){

}

Cliente::~Cliente(){

	close(sock);
	printf("Socket numero %d Closed\n", sock);

}

void Cliente::enviarMensaje(char reply[1000]){


	bzero(sHost.userName, 1000);
	bzero(sHost.mrec, 1000);
	printf("%s:\n", sHost.userName);
	fgets(sHost.mrec, 1000, stdin);
	send(cliente, &sHost, sizeof(struct rec), 0);

}

void Cliente::recibirMensaje(char reply[1000]){

	bzero(sGuest.userName, 1000);
	bzero(sGuest.mrec, 1000);
	recv(cliente, &sGuest, sizeof(struct rec), 0);
	printf("%s:\n", sGuest.userName);
	printf("%s\n", sGuest.mrec);

}

struct informacion Cliente::recibirInformacion(){
	struct informacion info;
	recv(cliente, &info, sizeof(struct informacion), 0);
	//recv(cliente, &sGuest.mrec,strlen(sGuest.mrec),0);
	//printf("%s",sGuest.mrec);
	return info;
}

void Cliente::setPortAndSocket(char *puerto){
	direccionServer.sin_family = AF_INET;
	direccionServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServer.sin_port = htons( atoi(puerto) );

		c = sizeof(struct sockaddr_in);
		cliente = socket(AF_INET, SOCK_STREAM, 0);

		//Accept
		sock = connect( cliente, (struct sockaddr *)&direccionServer, sizeof(direccionServer));

		if (sock != 0){
			perror("Acceptance Failed. Error");
			close(sock);
			exit(-1);
		}else{
			puts("Accepting Connection...");
			puts("Connection Successful");
		}
}
