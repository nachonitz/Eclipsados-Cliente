#include "Cliente.h"
#include <pthread.h>
#include "Dibujador.h"
#include <queue>
#include "ParserXML.h"
#include "Sonido.h"
#include <string>

Cliente cliente;
pthread_t hiloSendMessage;
pthread_t hiloRecieveMessage;
pthread_t hiloRender;
pthread_t hiloTimer;
pthread_t previeneProgramNotResponding;
Dibujador dibujador;
Controlador* controlador;
Sonido *musicaFondo;
credencial credencialesCliente;
queue <struct informacionRec> colaInfoRecibida;
pthread_mutex_t mutexQueue, mutexPantallaScores;
pthread_mutex_t mutexTimer;
string nombres[MAX_CLIENTES];
bool serverConectado, salir;
bool terminarHiloNotResponding = false;
bool nombresInicializados = false;
int tiempoEsperaSend, nivel, cantidadClientes;
bool mostrandoPantallaScores = false;

struct informacionEnv crearInfoEnvQuieto();

void* mantenerAplicacionActiva(void*arg){
	int i = 0;
	while(!terminarHiloNotResponding){
		sleep(0.5);
		i++;
	}
}

void* timer(void*arg){
	Logger::getInstance()->log(DEBUG, "Hilo Timer creado");
	while (tiempoEsperaSend <= 10){
		sleep(1);
		pthread_mutex_lock(&mutexTimer);
		tiempoEsperaSend ++;
		pthread_mutex_unlock(&mutexTimer);
	}
	Logger::getInstance()->log(ERROR, "Tiempo de espera maximo alcanzado. Desconcetando de Servidor");
	serverConectado = false;
	dibujador.mostrarPantallaErrorConTexto("Server connection lost, shutting down...");
	exit(0);
}

void* message_send(void*arg){
	serverConectado = true;
	salir = false;

	while(serverConectado && !salir){
		//Logger::getInstance()->log(DEBUG, "Tomando input usuario para luego enviar...");
		pthread_mutex_lock(&mutexPantallaScores);

		struct informacionEnv infoEnv;

		if (!mostrandoPantallaScores)
			infoEnv = controlador->eventHandler(musicaFondo, &salir);
		else
			infoEnv = crearInfoEnvQuieto();

		pthread_mutex_unlock(&mutexPantallaScores);

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
	salir = false;
	tiempoEsperaSend = 0;
	pthread_create(&hiloTimer,NULL,timer,NULL);
	while(serverConectado && !salir){
		//Logger::getInstance()->log(DEBUG, "Recibiendo info de servidor...");
		struct informacionRec info = cliente.recibirInformacion();

		if(!nombresInicializados){
			for(int i = 0; i< info.cantJugadores;i++){
				nombres[i] = info.credenciales[i].usuario;
			}
			nombresInicializados = true;
		}

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
	salir = false;
	while(serverConectado && !salir){
		if(!colaInfoRecibida.empty()){
			pthread_mutex_lock(&mutexQueue);
			struct informacionRec info = colaInfoRecibida.front();
			pthread_mutex_unlock(&mutexQueue);

			if(nivel != info.nivelActual){
				if (nivel != 0){
					//delete(musicaFondo);
				}
				if(info.nivelActual == 2){
					dibujador.mostrarPantallaScores(info.scores, info.cantJugadores, false, info.nivelActual);

					pthread_mutex_lock(&mutexPantallaScores);
					mostrandoPantallaScores = true;
					pthread_mutex_unlock(&mutexPantallaScores);

					sleep(7);

					pthread_mutex_lock(&mutexQueue);
					mostrandoPantallaScores = false;
					pthread_mutex_unlock(&mutexQueue);
				}
				if(info.nivelActual == 3){
					//nivel 3 va a ser cuando termina la partida
					dibujador.mostrarPantallaGameOver(info.scores,nombres,info.cantJugadores,info.perdieronTodos);
					sleep(5);
					if(nivel == 1){
						dibujador.mostrarPantallaScores(info.scores, info.cantJugadores, true,1);
					}else{
						dibujador.mostrarPantallaScores(info.scores, info.cantJugadores, true, 2);
					}

					sleep(7);
					salir = true;
				}
				musicaFondo->pasarNivel(info.nivelActual);
				nivel = info.nivelActual;
			}
			dibujador.dibujar(info, cliente.getID(), musicaFondo);
			colaInfoRecibida.pop();
		}
	}
}



int main(int argc, char *argv[]){

	Logger::getInstance()->createLogFile();

	Logger::getInstance()->setLevel(DEBUG);

	std::string customXmlPath = "xmlClienteCustom.xml";

	ParserXML parser(customXmlPath);

	std::vector<std::string> nivel1, nivel2, sprites, sonidos;

	parser.parsearConfig(nivel1, nivel2, sprites, sonidos);

	nivel = 0;

	credencialesCliente.credencialValida = false;

	pthread_mutex_init(&mutexQueue,NULL);
	pthread_mutex_init(&mutexTimer,NULL);
	pthread_mutex_init(&mutexPantallaScores,NULL);


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

	musicaFondo = new Sonido(sonidos);

	dibujador.mostrarIntroEclipsados(musicaFondo);

	dibujador.login(& credencialesCliente, false,false);
	//-> Ventana Login

	//dibujador.login(& credencialesCliente, false);
	Logger::getInstance()->log(DEBUG, "COMENZANDO LOOP CON: " + std::string(credencialesCliente.usuario) + " - " + std::string(credencialesCliente.contrasenia));

	while(!cliente.validarCredenciales(credencialesCliente)){
		dibujador.login(&credencialesCliente, true,false);
	}

	dibujador.mostrarPantallaEspera(cliente.getID());

	int bytesRecibidos;

	// Espera a que todos los clientes terminen el login antes de lanzar los hilos del juego
	bool confirmacion = cliente.esperarConfirmacionDeInicio(&bytesRecibidos);

	Logger::getInstance()->log(DEBUG, "RECIBIDA CONFIRMACION: " + std::to_string(confirmacion));

	if (!confirmacion && bytesRecibidos > 0) {
		Logger::getInstance()->log(ERROR, "Server en puerto " + std::string((char*)argv[1]) + " con IP: " + puerto + " se encuentra lleno! Desconectando....");
		dibujador.mostrarPantallaErrorConTexto("Server full! Disconnecting...");
		return 0;
	}

	else if (!confirmacion && bytesRecibidos <= 0) {
		Logger::getInstance()->log(ERROR, "Server en puerto " + std::string((char*)argv[1]) + " con IP: " + puerto + " caido! (no se encuentra el server) Desconectando...");
		dibujador.mostrarPantallaErrorConTexto("Server connection lost, shutting down...");
		return 0;
	}

	Logger::getInstance()->log(INFO, "Listo para comenzar, iniciando escenario!");

	musicaFondo->pasarNivel(1);

	pthread_create(&hiloSendMessage,NULL,message_send,NULL);
	pthread_create(&hiloRecieveMessage,NULL,message_recieve,NULL);
	pthread_create(&hiloRender,NULL,render_vista,NULL);

	pthread_join(hiloSendMessage,NULL);


	Logger::getInstance()->log(ERROR, "Server en puerto " + std::string((char*)argv[1]) + " con IP: " + puerto + " caido! (no se encuentra el server) Desconectando...");

	if(salir){
		dibujador.mostrarPantallaErrorConTexto("Saliendo del Juego...");
	}else{
		dibujador.mostrarPantallaErrorConTexto("Server connection lost, shutting down...");
	}

	delete(musicaFondo);

	return 0;
}



struct informacionEnv crearInfoEnvQuieto() {
	struct informacionEnv infoEnv;

	infoEnv.animacionActual = ACCION_PARADO;
	infoEnv.flip = SDL_FLIP_NONE;
	infoEnv.movimiento = STAND;

	return infoEnv;
}
