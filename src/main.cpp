#include "Cliente.h"
#include "Vista.h"

int main(int argc, char *argv[]){

	char mensaje[1000];

	Cliente cliente(argv[1], argv[2]);

	while(1){

		cliente.recibirMensaje(mensaje);
		cliente.enviarMensaje(mensaje);

		if( strcmp(mensaje, "quit\n") == 0){
			break;
		}

	}

	return 0;
}
