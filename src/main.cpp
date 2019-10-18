#include "Cliente.h"
#include <pthread.h>
#include "Dibujador.h"

Cliente cliente;
char mensaje[1000];
Dibujador dibujador;


void* message_send(void*arg){
	while(1){
		cliente.enviarMensaje(mensaje);
	}
}

void* message_recieve(void*arg){
	while(1){
		//cliente.recibirMensaje(mensaje);
		struct informacion info = cliente.recibirInformacion();
		dibujador.dibujar(info);
	}
}



int main(int argc, char *argv[]){
	pthread_t hiloSendMessage;
	pthread_t hiloRecieveMessage;

	cliente.setPortAndSocket(argv[1], argv[2]);

	dibujador.inicializar();

	pthread_create(&hiloSendMessage,NULL,message_send,NULL);
	pthread_create(&hiloRecieveMessage,NULL,message_recieve,NULL);

	pthread_join(hiloSendMessage,NULL);
	pthread_join(hiloRecieveMessage,NULL);

	return 0;
}
