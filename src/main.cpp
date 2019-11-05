#include "Cliente.h"
#include <pthread.h>
#include "Dibujador.h"
#include <queue>
#include "ParserXML.h"

Cliente cliente;
pthread_t hiloSendMessage;
pthread_t hiloRecieveMessage;
pthread_t hiloRender;
pthread_t hiloTimer;
pthread_t previeneProgramNotResponding;
Dibujador dibujador;
Controlador* controlador;
credencial credencialesCliente;
queue <struct informacionRec> colaInfoRecibida;
pthread_mutex_t mutexQueue;
pthread_mutex_t mutexTimer;
bool serverConectado;
bool terminarHiloNotResponding = false;
int tiempoEsperaSend;

void* mantenerAplicacionActiva(void*arg){
	int i = 0;
	while(!terminarHiloNotResponding){
		sleep(0.5);
		i++;
	}
}

void* timer(void*arg){
	Logger::getInstance()->log(DEBUG, "Hilo Timer creado");
	while (tiempoEsperaSend <= 5){
		sleep(1);
		pthread_mutex_lock(&mutexTimer);
		tiempoEsperaSend ++;
		pthread_mutex_unlock(&mutexTimer);
	}
	Logger::getInstance()->log(ERROR, "Tiempo de espera maximo alcanzado. Desconcetando de Servidor");
	serverConectado = false;
	dibujador.mostrarPantallaConTextoYCerrarCliente("Server connection lost, shutting down...");
	exit(0);
}

void* message_send(void*arg){
	serverConectado = true;


	while(serverConectado){
		//Logger::getInstance()->log(DEBUG, "Tomando input usuario para luego enviar...");
		struct informacionEnv infoEnv = controlador->eventHandler();

		int resultadoSend = cliente.enviarInformacion(infoEnv);

		SDL_Delay(FRAME_DELAY);
		if (resultadoSend <= 0){
			serverConectado = false;

		}
		//dibujador.dibujar(info);
	}
}

void* message_recieve(void*arg){
	serverConectado = true;
	tiempoEsperaSend = 0;
	pthread_create(&hiloTimer,NULL,timer,NULL);
	while(serverConectado){
		//Logger::getInstance()->log(DEBUG, "Recibiendo info de servidor...");
		struct informacionRec info = cliente.recibirInformacion();
		pthread_mutex_lock(&mutexQueue);
		colaInfoRecibida.push(info);
		pthread_mutex_unlock(&mutexQueue);
		//terminar hilo timer
		pthread_mutex_lock(&mutexTimer);
		tiempoEsperaSend =0;
		pthread_mutex_unlock(&mutexTimer);
	}
}

void* render_vista(void*arg){
	serverConectado = true;

	while(serverConectado){
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


	credencialesCliente.credencialValida = false;

	pthread_mutex_init(&mutexQueue,NULL);
	pthread_mutex_init(&mutexTimer,NULL);

	controlador = new Controlador();

	std::string puerto;

	if (argc >= 3) {
		Logger::getInstance()->log(INFO, "Iniciando conexion a puerto: " + std::string((char*)argv[1]) + ". Con IP: " + std::string((char*)argv[2]));
		cliente.setPortAndIP(argv[1], argv[2]);
		puerto =  std::string((char*)argv[2]);
	} else {
		Logger::getInstance()->log(INFO, "Iniciando conexion a puerto: " + std::string((char*)argv[1]) + ". Con IP: 127.0.0.1 (localhost)");
		cliente.setPortAndIP(argv[1], "127.0.0.1");
		puerto = "127.0.0.1";
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

	//dibujador.login(&credencialesCliente,false,true);
	dibujador.mostrarPantallaEspera(cliente.getID());

	int bytesRecibidos;

	pthread_create(&previeneProgramNotResponding,NULL,mantenerAplicacionActiva,NULL);
	// Deberia esperar a que todos los clientes terminen el login antes de lanzar los hilos del juego
	bool confirmacion = cliente.esperarConfirmacionDeInicio(&bytesRecibidos);
	terminarHiloNotResponding = true;
	pthread_join(previeneProgramNotResponding,NULL);

	Logger::getInstance()->log(DEBUG, "RECIBIDA CONFIRMACION: " + std::to_string(confirmacion));

	if (!confirmacion && bytesRecibidos > 0) {
		Logger::getInstance()->log(ERROR, "Server en puerto " + std::string((char*)argv[1]) + " con IP: " + puerto + " se encuentra lleno! Desconectando....");
		dibujador.mostrarPantallaConTextoYCerrarCliente("Server full! Disconnecting...");
		return 0;
	}

	else if (!confirmacion && bytesRecibidos <= 0) {
		Logger::getInstance()->log(ERROR, "Server en puerto " + std::string((char*)argv[1]) + " con IP: " + puerto + " caido! (no se encuentra el server) Desconectando...");
		dibujador.mostrarPantallaConTextoYCerrarCliente("Server connection lost, shutting down...");
		return 0;
	}

	Logger::getInstance()->log(INFO, "Listo para comenzar, iniciando escenario!");

	pthread_create(&hiloSendMessage,NULL,message_send,NULL);
	pthread_create(&hiloRecieveMessage,NULL,message_recieve,NULL);
	pthread_create(&hiloRender,NULL,render_vista,NULL);

	pthread_join(hiloSendMessage,NULL);


	Logger::getInstance()->log(ERROR, "Server en puerto " + std::string((char*)argv[1]) + " con IP: " + puerto + " caido! (no se encuentra el server) Desconectando...");

	dibujador.mostrarPantallaConTextoYCerrarCliente("Server connection lost, shutting down...");




	return 0;
}
