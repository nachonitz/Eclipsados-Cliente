/*#include "Vista.h"
#include "../Logger.h"

Vista::Vista(Juego* modelo, Controlador* controlador, AsignadorDeTexturas& unAsignador) {
	this->juego = modelo;
	this->controlador = controlador;
	this->asignador = unAsignador;
	this->running = true;
	this->nivelActual = 1;

  Logger::getInstance()->log(INFO, "Inicializando subsistema de video...");
	if ( SDL_Init(SDL_INIT_VIDEO) != 0)
    Logger::getInstance()->log(ERROR, SDL_GetError());

  Logger::getInstance()->log(INFO, "Inicializando ventana...");
	if ( SDL_CreateWindowAndRenderer(WINDOW_SIZE_HORIZONTAL, WINDOW_SIZE_VERTICAL, 0, &win, &ren) )
    Logger::getInstance()->log(ERROR, SDL_GetError());

	SDL_SetWindowTitle(win, "Final Fight");

	inicializarVistaParaNivel();

	loop();
}

void Vista::inicializarVistaParaNivel(){

	this->jugador = juego->getJugador();


	//personaje, interactua con modelo
	PosicionGlobal* posicionJugador = juego->getPosicionJugador();

	posicionX = posicionJugador->getHorizontal();
	posicionY = posicionJugador->getVertical();

	jugador->setImageWith(asignador,ren);
	jugador->setDest(posicionX, posicionY, JUGADOR_SIZE_HORIZONTAL, JUGADOR_SIZE_VERTICAL); //TODO

	parado = jugador->crearCiclo(1, 120, 120, 1, 10);
	caminar = jugador->crearCiclo(2, 120, 120, 12, 4);
	salto = jugador->crearCiclo(3, 120, 120, 8, 6);
	golpear = jugador->crearCiclo(4, 120, 120, 9, 5);
	saltoPatada = jugador->crearCiclo(5, 120, 120, 6, 9);
	agachado = jugador->crearCiclo(1, 120, 120, 4, 5);
	saltoVertical = jugador->crearCiclo(6, 120, 120, 6, 8);


	accionActual = parado;
	jugador->setAnimacionActual(accionActual, SDL_FLIP_NONE);

//se crea la funcion inicializar para que cuando se cambia de nivel en medio de una accion no la siga
	controlador->resetearAnimaciones();
	controlador->setAcciones(caminar, parado, salto, saltoPatada, golpear, agachado, saltoVertical);
	controlador->setAccionActual(accionActual);

	//nivel = juego->getNivel();

	capa1 = juego->getCapa1();
	capa2 = juego->getCapa2();
	capa3 = juego->getCapa3();

	prepararCapa(capa3, asignador.getNivel1()->at(2).c_str());
	prepararCapa(capa2, asignador.getNivel1()->at(1).c_str());
	prepararCapa(capa1, asignador.getNivel1()->at(0).c_str());

	elementos = juego->getElementos();
	enemigos = juego->getEnemigos();

	for (uint i = 0; i < elementos.size(); i++) {
		Dibujable* dibujable = elementos[i]->getDibujable();
		PosicionGlobal* posicion = elementos[i]->getPosicionGlobal();

		posicionX = posicion->getHorizontal();
		posicionY = posicion->getVertical();

		dibujable->setImageWith(asignador, ren);

//-->> Ojo que no todos van a tener las mismas dimensiones (source y dest cambian de acuredo al obj)
// -> solucion: cada objeto setea su source y se le pide el ancho y el alto para dibujarlo
		dibujable->setDest(posicionX, posicionY, dibujable->getWidth(),dibujable->getHeight());
		//dibujable->setSource(200, 190, 100, 100);
	}
	srand(time(NULL));
	for (uint i = 0; i < enemigos.size(); i++) {
		Dibujable* dibujable = enemigos[i]->getDibujable();
		PosicionGlobal* posicion = enemigos[i]->getPosicionGlobal();

		posicionX = posicion->getHorizontal();
		posicionY = posicion->getVertical();

		dibujable->setImageWith(asignador, ren);

//-->> Ojo que no todos van a tener las mismas dimensiones (source y dest cambian de acuredo al obj)
// -> solucion: cada objeto setea su source y se le pide el ancho y el alto para dibujarlo
		dibujable->setDest(posicionX, posicionY, dibujable->getWidth()*2.2,dibujable->getHeight()*2.2);
		//dibujable->setSource(200, 190, 100, 100);

		Enemigo* enemigoActual = (Enemigo*) enemigos[i]->getDibujable();

		int caminarEnemigo = enemigoActual->crearCiclo(2, 100, 125, 6, 5);


		int x = rand()%2;

		if (x == 0){
			enemigoActual->setAnimacionActual(caminarEnemigo, SDL_FLIP_NONE);
		}
		else{
			enemigoActual->setAnimacionActual(caminarEnemigo, SDL_FLIP_HORIZONTAL);
		}
	}
}

void Vista::prepararCapa(Capa* capa,char const* imagen){
	capa->setImage(ren,imagen);
	capa->setSource(0,0,ANCHO_CAPA_PIXELES ,WINDOW_SIZE_VERTICAL+10);
	capa->setDest(0,0,ANCHO_CAPA_PIXELES_ESCALADA,WINDOW_SIZE_VERTICAL+10);
}
void Vista::prepararSegundaCapa(Capa* capa,char const* imagen){
	capa->setImage(ren,imagen);
	capa->setSource(0,0,ANCHO_CAPA_PIXELES ,WINDOW_SIZE_VERTICAL+10);
	capa->setDest(0,0,ANCHO_CAPA_PIXELES_ESCALADA,WINDOW_SIZE_VERTICAL+10);
}

Vista::~Vista() {

	Mix_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
    IMG_Quit();
	SDL_Quit();

}

void Vista::render() {

	SDL_RenderClear(ren);

	Draw();

	SDL_RenderPresent(ren);

}

void Vista::Draw(){

	// Capa1
	SDL_Rect destinationFondo1 = capa1->getDest();
	SDL_Rect sourceFondo1 = capa1->getSource();

	// Capa2
	SDL_Rect destinationFondo2 = capa2->getDest();
	SDL_Rect sourceFondo2 = capa2->getSource();

	// Capa3
	SDL_Rect destinationFondo3 = capa3->getDest();
	SDL_Rect sourceFondo3 = capa3->getSource();

	// Cody
	SDL_Rect destinationJugador = jugador->getDest();
	SDL_Rect sourceJugador = jugador->getSource();


	SDL_RenderCopy(ren, capa3->getTexture(), &sourceFondo3, &destinationFondo3);
	SDL_RenderCopy(ren, capa2->getTexture(), &sourceFondo2, &destinationFondo2);
	SDL_RenderCopy(ren, capa1->getTexture(), &sourceFondo1, &destinationFondo1);

	for (uint i = 0; i < elementos.size(); i++) {
		Dibujable* dibujable = elementos[i]->getDibujable();
		renderizable renderizableActual;
		renderizableActual.textura = dibujable->getTexture();
		renderizableActual.source = dibujable->getSource();
		renderizableActual.destination = dibujable->getDest();
		renderizableActual.flip = SDL_FLIP_NONE;
		renderizables.push_back(renderizableActual);

//		SDL_Rect destinationElemento = dibujable->getDest();
//		SDL_Rect sourceElemento = dibujable->getSource();

//		SDL_RenderCopy(ren, dibujable->getTexture(), &sourceElemento, &destinationElemento);
	}

	for (uint i = 0; i < enemigos.size(); i++) {
		Dibujable* dibujable = enemigos[i]->getDibujable();
//		SDL_Rect destinationEnemigo = dibujable->getDest();
//		SDL_Rect sourceEnemigo = dibujable->getSource();
		Enemigo* enemigoActual = (Enemigo*) enemigos[i]->getDibujable();

		renderizable renderizableActual;
		renderizableActual.textura = dibujable->getTexture();
		renderizableActual.source = dibujable->getSource();
		renderizableActual.destination = dibujable->getDest();
		renderizableActual.flip = enemigoActual->getFlip();
		renderizables.push_back(renderizableActual);

//		SDL_RenderCopyEx(ren, dibujable->getTexture(), &sourceEnemigo, &destinationEnemigo, 0, NULL, enemigoActual->getFlip());
	}

//	SDL_RenderCopyEx(ren, jugador->getTexture(), &sourceJugador, &destinationJugador, 0, NULL, jugador->getFlip());
	renderizable renderizableActual;
	renderizableActual.textura = jugador->getTexture();
	renderizableActual.source = jugador->getSource();
	renderizableActual.destination = jugador->getDest();
	renderizableActual.flip = jugador->getFlip();
	renderizables.push_back(renderizableActual);

	//tengo todos los renderizables en un vector, ordeno de acuerdo a la posicion vertical
	std::sort(renderizables.begin(), renderizables.end());


	for (uint i = 0; i < renderizables.size(); i++) {
		SDL_RenderCopyEx(ren, renderizables[i].textura, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);
	}

	renderizables.clear();
}


void Vista::update(){

	//PosicionGlobal posicionJugador = juego->getPosicionJugador();
	//posicionX = posicionJugador.getHorizontal();
	//posicionY = posicionJugador.getVertical();
	//jugador->updateDest(posicionX, posicionY);
	jugador->updateAnim();
	for (uint i = 0; i<enemigos.size();i++){
		Enemigo* enemigoActual = (Enemigo*) enemigos[i]->getDibujable();
		enemigoActual->updateAnim();
	}

}


void Vista::loop() {

	//ciclo infinito de renderizacion
	while(running){

		frameStart=SDL_GetTicks();
		static int lastTime;

		render();

		//llamo a controlador a ver si toco alguna tecla
		running = controlador->eventHandler();
		juego->moverEnemigos();

		if(juego->terminoElNivel() && nivelActual == 1){

			juego->cambiarDeNivel();
			inicializarVistaParaNivel();
			prepararSegundaCapa(capa3, asignador.getNivel2()->at(2).c_str());
			prepararSegundaCapa(capa2, asignador.getNivel2()->at(1).c_str());
			prepararSegundaCapa(capa1, asignador.getNivel2()->at(0).c_str());

			nivelActual = 2;
		}

		update();

		if(frameDelay > lastTime){
			SDL_Delay(frameDelay - lastTime);
		}

	}
}
*/
