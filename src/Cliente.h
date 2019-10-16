#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
using namespace std;

class Cliente {
public:
	Cliente(char *puerto, char* serverSocket);
	~Cliente();
	void enviarMensaje(char replay[1000]);
	void recibirMensaje(char replay[1000]);

private:
	struct sockaddr_in direccionServer;
	int c;
	int sock;
	int cliente;
	char reply[1000];
	char name[1000];
	int socketServidor;

};



#endif /* CLIENTE_H_ */
