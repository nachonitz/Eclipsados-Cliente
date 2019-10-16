#include "../../../../eclipse-workspace/Cliente/Cliente/src/Cliente.h"


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

	printf("Client:\n");
	bzero(reply, 1000);
	fgets(reply, 1000, stdin);
	send(cliente, reply, strlen(reply), 0);

}

void Cliente::recibirMensaje(char reply[1000]){

	bzero(reply, 1000);
	recv(socketServidor, reply, 1000, 0);
	printf("Server:\n");
	printf("%s\n", reply);
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
			puts("Connection Accepted");
			puts("Connection Successful");
		}
}
