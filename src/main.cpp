#include "../../../../eclipse-workspace/Cliente/Cliente/src/Cliente.h"
#include <pthread.h>

Cliente cliente;
char mensaje[1000];

void* message_send(void*arg){
	while(1){
		cliente.enviarMensaje(mensaje);
	}
}

void* message_recieve(void*arg){
	while(1){
		cliente.recibirMensaje(mensaje);
	}
}



int main(int argc, char *argv[]){
	pthread_t hiloSendMessage;
	pthread_t hiloRecieveMessage;

	cliente.setPortAndSocket(argv[1], argv[2]);

	pthread_create(&hiloSendMessage,NULL,message_send,NULL);
	pthread_create(&hiloRecieveMessage,NULL,message_recieve,NULL);

	pthread_join(hiloSendMessage,NULL);
	pthread_join(hiloRecieveMessage,NULL);
/*
	while(1){

		cliente.recibirMensaje(mensaje);
		cliente.enviarMensaje(mensaje);

		if( strcmp(mensaje, "quit\n") == 0){
			break;
		}

	}
*/
	return 0;
}
