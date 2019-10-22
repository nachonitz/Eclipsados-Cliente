#include "Cliente.h"
#include <pthread.h>
#include "Dibujador.h"

Cliente cliente;
char mensaje[1000];
Dibujador dibujador;
Controlador* controlador;


void* message_send(void*arg){
	while(1){
		struct informacionEnv infoEnv = controlador->eventHandler();
		cliente.enviarInformacion(infoEnv);
		SDL_Delay(1000/60);
		//dibujador.dibujar(info);
	}
}

void* message_recieve(void*arg){
	while(1){
		struct informacionRec info = cliente.recibirInformacion();
		dibujador.dibujar(info);
	}
}



int main(int argc, char *argv[]){
	pthread_t hiloSendMessage;
	pthread_t hiloRecieveMessage;

	controlador = new Controlador();

	cliente.setPortAndSocket(argv[1]);

	dibujador.inicializar();

	cliente.recibirMensaje(mensaje);

	pthread_create(&hiloRecieveMessage,NULL,message_recieve,NULL);
	pthread_create(&hiloSendMessage,NULL,message_send,NULL);

	pthread_join(hiloRecieveMessage,NULL);
	pthread_join(hiloSendMessage,NULL);

	return 0;
}
