#include "Cliente.h"
#include <pthread.h>
#include "Dibujador.h"
#include <queue>

Cliente cliente;
char mensaje[1000];
Dibujador dibujador;
Controlador* controlador;
queue <struct informacionRec> colaInfoRecibida;
pthread_mutex_t mutexQueue;


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
		pthread_mutex_lock(&mutexQueue);
		colaInfoRecibida.push(info);
		pthread_mutex_unlock(&mutexQueue);
	}
}

void* render_vista(void*arg){
	while(1){
		if(!colaInfoRecibida.empty()){
			pthread_mutex_lock(&mutexQueue);
			struct informacionRec info = colaInfoRecibida.front();
			pthread_mutex_unlock(&mutexQueue);

			dibujador.dibujar(info);
			colaInfoRecibida.pop();
		}
	}
}



int main(int argc, char *argv[]){
	pthread_t hiloSendMessage;
	pthread_t hiloRecieveMessage;
	pthread_t hiloRender;

	pthread_mutex_init(&mutexQueue,NULL);

	controlador = new Controlador();

	if (argc >= 3)
		cliente.setPortAndIP(argv[1], argv[2]);
	else
		cliente.setPortAndIP(argv[1], "127.0.0.1");

	dibujador.inicializar();

	dibujador.login();
	//-> Ventana Login

	//dibujador.login(&cliente);
	// Deberia esperar a que todos los clientes terminen el login antes de lanzar los hilos del juego
	//cliente.esperarConfirmacionDeInicio();


	pthread_create(&hiloRecieveMessage,NULL,message_recieve,NULL);
	pthread_create(&hiloSendMessage,NULL,message_send,NULL);
	pthread_create(&hiloRender,NULL,render_vista,NULL);

	pthread_join(hiloRecieveMessage,NULL);
	pthread_join(hiloSendMessage,NULL);
	pthread_join(hiloRender, NULL);

	return 0;
}
