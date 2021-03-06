#include "Cliente.h"
#include <csignal>


Cliente::Cliente(char *puerto){

	setPortAndIP(puerto, "127.0.0.1");

}

void signalHandler(int signum){

	Logger::getInstance()->log(ERROR, "Se ha capturado una excepcion SIGPIPE. Comportamiento Handler: no se hace nada");
}

Cliente::Cliente(){
}

Cliente::~Cliente(){

	printf("Socket numero %d Closed\n", cliente);
	Logger::getInstance()->log(INFO, "Cerrando socket: " + std::to_string(cliente));
	close(cliente);

}

int Cliente::enviarInformacion(struct informacionEnv infoEnv){
	int enviado = 0;
	int actual = 0;
	signal(SIGPIPE, signalHandler);
	while (enviado < sizeof(struct informacionEnv)){
		actual = send(cliente, &infoEnv+enviado, sizeof(struct informacionEnv)-enviado, 0);
		enviado += actual;
		if (actual <= 0){
			Logger::getInstance()->log(ERROR, "Servidor desconectado (error en Send). Desconectando cliente: " + std::to_string(ID));
			return -1;
		}
	}
	return 1;


}


struct informacionRec Cliente::recibirInformacion(){
	struct informacionRec infoRec;
	bool estadoTransferencia = true;

	int recibido = 0;
	while (recibido < sizeof(int)){
		int temp = recv(cliente, &infoRec.nivelActual+recibido, sizeof(infoRec.nivelActual)-recibido, 0);
		if (temp <= 0) {
			infoRec.nivelActual = -1;
			return infoRec;
		}
		recibido += temp;
	}

	recibido = 0;
	while (recibido < sizeof(int)){
		int temp = recv(cliente, &infoRec.cantAnimados+recibido, sizeof(infoRec.cantAnimados)-recibido, 0);
		if (temp <= 0) {
			infoRec.cantJugadores = -1;
			return infoRec;
		}
		recibido += temp;
	}
	recibido = 0;
	while (recibido < sizeof(int)){
		int temp = recv(cliente, &infoRec.cantJugadores+recibido, sizeof(infoRec.cantJugadores)-recibido, 0);
		if (temp <= 0) {
			infoRec.cantJugadores = -1;
			return infoRec;
		}
		recibido += temp;
	}
	recibido = 0;
	while (recibido < sizeof(int)){
		int temp= recv(cliente, &infoRec.cantElementos+recibido, sizeof(infoRec.cantElementos)-recibido, 0);
		if (temp <= 0) {
			infoRec.cantJugadores = -1;
			return infoRec;
		}
		recibido += temp;
	}

	for(int i=0; i < infoRec.cantAnimados; i++){
		recibido = 0;
		while (recibido < sizeof(struct animado)){
			int temp= recv(cliente, &infoRec.animados[i]+recibido, sizeof(infoRec.animados[i])-recibido, 0);
			if (temp <= 0) {
				infoRec.cantJugadores = -1;
				return infoRec;
			}
			recibido += temp;
		}

	}

	for(int i=0; i < 3; i++){
		recibido = 0;
		while (recibido < sizeof(struct capa)){
			int temp= recv(cliente, &infoRec.capas[i]+recibido, sizeof(infoRec.capas[i])-recibido, 0);
			if (temp <= 0) {
				infoRec.cantJugadores = -1;
				return infoRec;
			}
			recibido += temp;
		}
	}

	for(int i=0; i < infoRec.cantElementos; i++){
		recibido = 0;
		while (recibido < sizeof(struct elemento)){
			int temp= recv(cliente, &infoRec.elementos[i]+recibido, sizeof(infoRec.elementos[i])-recibido, 0);
			if (temp <= 0) {
				infoRec.cantJugadores = -1;
				return infoRec;
			}
			recibido += temp;
		}
	}

	for(int i=0; i < infoRec.cantJugadores; i++){
		recibido = 0;
		while (recibido < sizeof(int)){
			int temp= recv(cliente, &infoRec.scores[i]+recibido, sizeof(infoRec.scores[i])-recibido, 0);
			if (temp <= 0) {
				infoRec.cantJugadores = -1;
				return infoRec;
			}
			recibido += temp;
		}
	}
	for(int i=0; i < infoRec.cantJugadores; i++){
		recibido = 0;
		while (recibido < sizeof(int)){
			int temp= recv(cliente, &infoRec.vidas[i]+recibido, sizeof(infoRec.vidas[i])-recibido, 0);
			if (temp <= 0) {
				infoRec.cantJugadores = -1;
				return infoRec;
			}
			recibido += temp;
		}
	}
	for(int i=0; i < infoRec.cantJugadores; i++){
		recibido = 0;
		while (recibido < sizeof(int)){
			int temp= recv(cliente, &infoRec.energia[i]+recibido, sizeof(infoRec.energia[i])-recibido, 0);
			if (temp <= 0) {
				infoRec.cantJugadores = -1;
				return infoRec;
			}
			recibido += temp;
		}
	}
	for(int i=0; i < infoRec.cantJugadores; i++){
		recibido = 0;
		while (recibido < sizeof(struct credencial)){
			int temp= recv(cliente, &infoRec.credenciales[i]+recibido, sizeof(infoRec.credenciales[i])-recibido, 0);
			if (temp <= 0) {
				infoRec.cantJugadores = -1;
				return infoRec;
			}
			recibido += temp;
		}
	}
	recibido = 0;
	while (recibido < sizeof(bool)){
		int temp= recv(cliente, &infoRec.perdieronTodos+recibido, sizeof(infoRec.perdieronTodos)-recibido, 0);
		if (temp <= 0) {
			infoRec.cantJugadores = -1;
			return infoRec;
		}
		recibido += temp;
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
		Logger::getInstance()->log(ERROR, "Servidor con IP: " + ip + " y puerto " + std::string(puerto) + " no encontrado.");
		perror("Acceptance Failed. Error");
		close(cliente);
		exit(-1);
	}else{
		Logger::getInstance()->log(INFO, "Conexion con servidor exitosa!");

		puts("Accepting Connection...");
		puts("Connection Successful");
	}
}


bool Cliente::esperarConfirmacionDeInicio(int* bufferError){
	bool confirmacion = true;

	*bufferError = recv(cliente, &confirmacion, sizeof(bool), 0);

	return confirmacion;
}

bool Cliente::validarCredenciales(struct credencial credencialesAValidar){

	std::string user = credencialesAValidar.usuario;
	std::string pass = credencialesAValidar.contrasenia;


	Logger::getInstance()->log(DEBUG, "ENVIANDO CREDENCIALES: " + user + " - " + pass);

	send(cliente, &credencialesAValidar, sizeof(struct credencial), MSG_NOSIGNAL);


	Logger::getInstance()->log(DEBUG, "RECIBIENDO...");
	recv(cliente, &credencialesAValidar, sizeof(struct credencial), 0);

	ID = credencialesAValidar.myID;

	Logger::getInstance()->log(DEBUG, "RESULTADO: " + std::to_string(credencialesAValidar.credencialValida));
	return (credencialesAValidar.credencialValida);

}
