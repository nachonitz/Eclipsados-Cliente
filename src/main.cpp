#include "Cliente.h"


char mensajeEntrada[1000];
char mensajeSalida[1000];
pthread_mutex_t mutex;
pthread_t recibir;
pthread_t enviar;
Cliente* cliente;

void* ExecuteThread1(void* HiloUno){

	//pthread_mutex_lock(&mutex);
	cliente->recibirMensaje(mensajeEntrada);
	//printf("%s\n", mensajeEntrada);
	//pthread_mutex_unlock(&mutex);
	sleep(5);

}

void* ExecuteThread2(void* HiloDos){

	pthread_mutex_lock(&mutex);
	cliente->enviarMensaje(mensajeSalida);
	pthread_mutex_unlock(&mutex);

}



int main(int argc, char *argv[]){


	pthread_mutex_init(&mutex, NULL);

	cliente = new Cliente(argv[1], argv[2]);

	while(1){

		//pthread_create( &recibir, NULL, ExecuteThread1, (void *) "Hilo Uno");
		cliente->recibirMensaje(mensajeEntrada);
		//pthread_create( &enviar, NULL, ExecuteThread2, (void *) "Hilo Dos");
		cliente->enviarMensaje(mensajeSalida);


	}

	return 0;
}
