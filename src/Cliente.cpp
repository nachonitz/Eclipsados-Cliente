#include "Cliente.h"


Cliente::Cliente(char *puerto){

	setPortAndIP(puerto, "127.0.0.1");

}

Cliente::Cliente(){

}

Cliente::~Cliente(){

	close(sock);
	printf("Socket numero %d Closed\n", sock);

}

void Cliente::enviarInformacion(struct informacionEnv infoEnv){

	send(cliente, &infoEnv, sizeof(struct informacionEnv), 0);

}

void Cliente::recibirMensaje(char reply[1000]){

	bzero(sGuest.userName, 1000);
	bzero(sGuest.mrec, 1000);
	recv(cliente, &sGuest, sizeof(struct rec), 0);
	printf("%s:\n", sGuest.userName);
	printf("%s\n", sGuest.mrec);

}

struct informacionRec Cliente::recibirInformacion(){
	struct informacionRec infoRec;

	int infoRecibida = 0;
	while (infoRecibida < sizeof(struct informacionRec)){
		int recibido = recv(cliente, &infoRec, sizeof(struct informacionRec), 0);
		infoRecibida += recibido;
	}
	return infoRec;
}

void Cliente::setPortAndIP(char *puerto, std::string ip){
	direccionServer.sin_family = AF_INET;
	direccionServer.sin_addr.s_addr = inet_addr(ip.c_str());
	direccionServer.sin_port = htons( atoi(puerto) );

		c = sizeof(struct sockaddr_in);
		cliente = socket(AF_INET, SOCK_STREAM, 0);

		//Accept
		sock = connect( cliente, (struct sockaddr *)&direccionServer, sizeof(direccionServer));

		if (sock != 0){
			perror("Acceptance Failed. Error");
			close(sock);
			exit(-1);
		}else{
			puts("Accepting Connection...");
			puts("Connection Successful");
		}
}


bool Cliente::esperarConfirmacionDeInicio(){
	bool noDevuelveNada;
	recv(cliente, &noDevuelveNada, sizeof(bool), 0);

	return noDevuelveNada;
}

bool Cliente::validarCredenciales(struct credencial credencialesAValidar){

	std::string user = credencialesAValidar.usuario;
	std::string pass = credencialesAValidar.contrasenia;


	Logger::getInstance()->log(DEBUG, "ENVIANDO CREDENCIALES: " + user + " - " + pass);

	send(cliente, &credencialesAValidar, sizeof(struct credencial), 0);


	Logger::getInstance()->log(DEBUG, "RECIBIENDO...");
	recv(cliente, &credencialesAValidar, sizeof(struct credencial), 0);

	ID = credencialesAValidar.myID;

	Logger::getInstance()->log(DEBUG, "RESULTADO: " + std::to_string(credencialesAValidar.credencialValida));
	return (credencialesAValidar.credencialValida);

}
