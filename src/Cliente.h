#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "Dibujador.h"
using namespace std;

class Cliente {
public:
	Cliente(char *puerto, char* serverSocket);
	Cliente();
	~Cliente();
	void enviarMensaje(char replay[1000]);
	void recibirMensaje(char replay[1000]);
	struct informacion recibirInformacion();
	void setPortAndSocket(char *puerto, char* serverSocket);

private:
	struct sockaddr_in direccionServer;
	int c;
	int sock;
	int cliente;
	char receive[1000];
	struct rec{
		char userName[1000];
		char mrec[1000];
	};
	char* miUsuario;
	rec sGuest, sHost;
	int socketServidor;

};



#endif /* CLIENTE_H_ */
