#include "Cliente.h"


Cliente::Cliente(char *puerto, char* serverSocket){

	direccionServer.sin_family = AF_INET;
	direccionServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServer.sin_port = htons( atoi(puerto) );
	socketServidor = atoi(serverSocket);

	c = sizeof(struct sockaddr_in);
	cliente = socket(AF_INET, SOCK_STREAM, 0);

	//Accept
	sock = connect( 3, (struct sockaddr *)&direccionServer, sizeof(direccionServer));

	if (sock != 0){
		perror("Acceptance Failed. Error");
		close(sock);
		exit(-1);
	}else{
		puts("Connection Accepted");

	}
	recv(cliente, name, 100, 0);
	printf("%s\n", name);
	fgets(name, 100, stdin);
	send(cliente, name, strlen(name), 0);
	bzero(name, 100);

}

Cliente::~Cliente(){

	//close(sock);
	//printf("Socket numero %d Closed\n", sock);

}

void Cliente::enviarMensaje(char reply[1000]){

	bzero(reply, 1000);
	fgets(reply, 1000, stdin);
	//printf("Yo:\n");
	send(cliente, reply, strlen(reply), 0);

}

void Cliente::recibirMensaje(char mensajeEntrada[1000]){

	bzero(name, 1000);
	bzero(reply, 1000);
	recv(cliente, name, 100, 0);
	//printf("%s\n", name);
	recv(cliente, reply, 1000, 0);
	//printf("%s\n", reply);
	if(strlen(name) != 0){
		printf("%s\n", name);
		printf("%s\n", reply);
	}

}
