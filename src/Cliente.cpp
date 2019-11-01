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
	bool estadoTransferencia = true;

	int infoRecibida = 0;

	//while (infoRecibida < sizeof(struct informacionRec)){
		int recibido4 = recv(cliente, &infoRec.cantAnimados, sizeof(infoRec.cantAnimados), 0);
		int recibido5 = recv(cliente, &infoRec.cantJugadores, sizeof(infoRec.cantJugadores), 0);
		int recibido6 = recv(cliente, &infoRec.cantElementos, sizeof(infoRec.cantElementos), 0);
		for(int i=0; i < infoRec.cantAnimados; i++){
			int recibido1 = recv(cliente, &infoRec.animados[i], sizeof(infoRec.animados[i]), 0);
		}
		for(int i=0; i < 3; i++){
			int recibido2 = recv(cliente, &infoRec.capas[i], sizeof(infoRec.capas[i]), 0);
		}
		for(int i=0; i < infoRec.cantElementos; i++){
			int recibido3 = recv(cliente, &infoRec.elementos[i], sizeof(infoRec.elementos[i]), 0);
		}
		//int recibido = recibido1 + recibido2 + recibido3 + recibido4 + recibido5 + recibido6;
		//int recibido = recv(cliente, &infoRec, sizeof(struct informacionRec), 0);
		//infoRecibida += recibido;
	//}
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
