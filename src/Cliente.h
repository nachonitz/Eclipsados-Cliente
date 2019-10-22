#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "Dibujador.h"
#include "controlador/Controlador.h"
using namespace std;

class Cliente {
public:
	Cliente(char *puerto);
	Cliente();
	~Cliente();
	void recibirMensaje(char replay[1000]);
	void enviarInformacion(struct informacionEnv info);
	struct informacionRec recibirInformacion();
	void setPortAndSocket(char *puerto);

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
