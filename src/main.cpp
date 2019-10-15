#include "Cliente.h"
#include "Vista.h"
#include "Dibujador.h"

int main(int argc, char *argv[]){

	char mensaje[1000];

	Cliente cliente(argv[1], argv[2]);

	Dibujador* dibujador = new Dibujador();

	while(1){

		cliente.recibirMensaje(mensaje);
		dibujador->actualizarPosiciones(cliente.getMensaje());

		//Esto seria enviar cuando tocas algun boton, pero nose si se hace siempre
		//se deberia hacer en controlador creo
		cliente.enviarMensaje(mensaje);

		if( strcmp(mensaje, "quit\n") == 0){
			break;
		}

	}

	return 0;
}
