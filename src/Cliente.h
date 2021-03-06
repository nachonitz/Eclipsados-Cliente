#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <csignal>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "Dibujador.h"
#include "controlador/Controlador.h"
#include "Logger.h"
using namespace std;

class Cliente {
public:
	Cliente(char *puerto);
	Cliente();
	~Cliente();
	void recibirMensaje(char replay[1000]);
	int enviarInformacion(struct informacionEnv info);
	struct informacionRec recibirInformacion();
	void setPortAndIP(char *puerto, std::string ip);
	bool esperarConfirmacionDeInicio(int* bufferError);
	bool validarCredenciales(struct credencial credencialesAValidar);
	void setID(int idx){ID = idx;}
	int getID(){return ID;}

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
	int ID;
	struct informacionRec infoRec;

};



#endif /* CLIENTE_H_ */
