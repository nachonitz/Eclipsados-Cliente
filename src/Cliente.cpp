#include "Cliente.h"


Cliente::Cliente(char *puerto, char* serverSocket){

	setPortAndSocket(puerto, serverSocket);

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
	strcat(sHost.userName, miUsuario);
	printf("%s:\n", sHost.userName);
	fgets(sHost.mrec, 1000, stdin);
	send(cliente, &sHost, sizeof(struct rec), 0);

}

void Cliente::recibirMensaje(char reply[1000]){

	bzero(sGuest.userName, 1000);
	bzero(sGuest.mrec, 1000);
	recv(socketServidor, &sGuest, sizeof(struct rec), 0);
	printf("%s:\n", sGuest.userName);
	printf("%s\n", sGuest.mrec);
}

void Cliente::setPortAndSocket(char *puerto, char* serverSocket){
	direccionServer.sin_family = AF_INET;
	direccionServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServer.sin_port = htons( atoi(puerto) );
	socketServidor = atoi(serverSocket);

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
