#include "Cliente.h"
#include <pthread.h>
#include "Dibujador.h"
#include <queue>

#include "ParserXML.h"

Cliente cliente;
char mensaje[1000];
Dibujador dibujador;
Controlador* controlador;
credencial credencialesCliente;
queue <struct informacionRec> colaInfoRecibida;
pthread_mutex_t mutexQueue;


void* message_send(void*arg){
	while(1){
		//Logger::getInstance()->log(DEBUG, "Tomando input usuario para luego enviar...");
		struct informacionEnv infoEnv = controlador->eventHandler();
		cliente.enviarInformacion(infoEnv);
		SDL_Delay(1000/60);
		//dibujador.dibujar(info);
	}
}

void* message_recieve(void*arg){
	while(1){
		//Logger::getInstance()->log(DEBUG, "Recibiendo info de servidor...");
		struct informacionRec info = cliente.recibirInformacion();
		pthread_mutex_lock(&mutexQueue);
		colaInfoRecibida.push(info);
		pthread_mutex_unlock(&mutexQueue);
	}
}

void* render_vista(void*arg){
	while(1){
		if(!colaInfoRecibida.empty()){
			pthread_mutex_lock(&mutexQueue);
			struct informacionRec info = colaInfoRecibida.front();
			pthread_mutex_unlock(&mutexQueue);

			dibujador.dibujar(info, cliente.getID());
			colaInfoRecibida.pop();
		}
	}
}



int main(int argc, char *argv[]){

	Logger::getInstance()->createLogFile();

	Logger::getInstance()->setLevel(DEBUG);

	std::string customXmlPath = "xmlClienteCustom.xml";

	ParserXML parser(customXmlPath);

	std::vector<std::string> nivel1, nivel2, sprites;

	parser.parsearConfig(nivel1, nivel2, sprites);

	pthread_t hiloSendMessage;
	pthread_t hiloRecieveMessage;
	pthread_t hiloRender;

	credencialesCliente.credencialValida = false;

	pthread_mutex_init(&mutexQueue,NULL);

	controlador = new Controlador();

	if (argc >= 3) {
		Logger::getInstance()->log(INFO, "Iniciando conexion a puerto: " + std::string((char*)argv[1]) + ". Con IP: " + std::string((char*)argv[2]));
		cliente.setPortAndIP(argv[1], argv[2]);
	} else {
		Logger::getInstance()->log(INFO, "Iniciando conexion a puerto: " + std::string((char*)argv[1]) + ". Con IP: 127.0.0.1 (localhost)");
		cliente.setPortAndIP(argv[1], "127.0.0.1");
	}

	dibujador.inicializar(nivel1, nivel2, sprites);

	dibujador.login(& credencialesCliente, false,false);
	//-> Ventana Login

	//dibujador.login(& credencialesCliente, false);
	Logger::getInstance()->log(DEBUG, "COMENZANDO LOOP CON: " + std::string(credencialesCliente.usuario) + " - " + std::string(credencialesCliente.contrasenia));

	while(!cliente.validarCredenciales(credencialesCliente)){
		dibujador.login(&credencialesCliente, true,false);
	}

	//cliente.setID(credencialesCliente.myID);

	dibujador.login(&credencialesCliente,false,true);

	// Deberia esperar a que todos los clientes terminen el login antes de lanzar los hilos del juego
	bool puedoEntrar = cliente.esperarConfirmacionDeInicio();

	Logger::getInstance()->log(DEBUG, "RECIBIDA CONFIRMACION: " + std::to_string(puedoEntrar));

	if (!puedoEntrar) {

		return 0;
	}


	pthread_create(&hiloRecieveMessage,NULL,message_recieve,NULL);
	pthread_create(&hiloSendMessage,NULL,message_send,NULL);
	pthread_create(&hiloRender,NULL,render_vista,NULL);

	pthread_join(hiloRecieveMessage,NULL);
	pthread_join(hiloSendMessage,NULL);
	pthread_join(hiloRender, NULL);

	return 0;
}
