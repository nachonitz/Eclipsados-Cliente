#include "Cliente.h"
#include <pthread.h>
#include "Dibujador.h"
#include <queue>

Cliente cliente;
char mensaje[1000];
Dibujador dibujador;
Controlador* controlador;
queue <struct informacionRec> colaInfoRecibida;
pthread_mutex_t mutexPush;
pthread_mutex_t mutexPop;


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
		pthread_mutex_lock(&mutexPush);
		colaInfoRecibida.push(info);
		pthread_mutex_unlock(&mutexPush);
	}
}

void* render_vista(void*arg){
	while(1){
		if(!colaInfoRecibida.empty()){
			pthread_mutex_lock(&mutexPop);
			struct informacionRec info = colaInfoRecibida.front();
			pthread_mutex_unlock(&mutexPop);

			dibujador.dibujar(info);
			colaInfoRecibida.pop();
		}
	}
}



int main(int argc, char *argv[]){
	pthread_t hiloSendMessage;
	pthread_t hiloRecieveMessage;
	pthread_t hiloRender;

	pthread_mutex_init(&mutexPop,NULL);
	pthread_mutex_init(&mutexPush,NULL);

	controlador = new Controlador();

	cliente.setPortAndSocket(argv[1]);

	dibujador.inicializar();

	cliente.recibirMensaje(mensaje);

	pthread_create(&hiloRecieveMessage,NULL,message_recieve,NULL);
	pthread_create(&hiloSendMessage,NULL,message_send,NULL);
	pthread_create(&hiloRender,NULL,render_vista,NULL);

	pthread_join(hiloRecieveMessage,NULL);
	pthread_join(hiloSendMessage,NULL);

	return 0;
}
