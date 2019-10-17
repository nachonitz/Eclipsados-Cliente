/*#include "Controlador.h"

Controlador::Controlador(Juego* juego) {
	// TODO Auto-generated constructor stub
	this->juego = juego;
	jugador = juego->getJugador();
	spriteFlip = SDL_FLIP_NONE;
	saltando = false;
	golpeando = false;
	agachando = false;
	gameController = NULL;
	teclado = true;

	SDL_Init(SDL_INIT_JOYSTICK);

	//Check for joysticks
	if( SDL_NumJoysticks() < 1 ){
		//Loggear error
		printf( "Warning: No joysticks connected!\n" );
	}
	else{
		//Load joystick
		SDL_JoystickEventState(SDL_ENABLE);
		gameController = SDL_JoystickOpen( 0 );
		if( gameController == NULL ){
			//Loggear error
			printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
		}
	}
	//Cosas Para Loggear
	std::cout << "Controller Name: " << SDL_JoystickName(gameController) << std::endl;
	std::cout << "Cant Axes: " << SDL_JoystickNumAxes(gameController) << std::endl;
	std::cout << "Cant Buttons: " << SDL_JoystickNumButtons(gameController) << std::endl;
}

Controlador::~Controlador() {
	// TODO Auto-generated destructor stub

}

void Controlador::setAccionActual(int acActual){

	accionActual = acActual;
}

void Controlador::closeController(){

	SDL_JoystickClose( gameController );
	gameController = NULL;
}

void Controlador::setAcciones(int c, int p, int s, int sPatada, int g, int a, int sV){

	caminar = c;
	parado = p;
	salto = s;
	saltoPatada = sPatada;
	golpear = g;
	agachado = a;
	saltoVertical = sV;
}

void Controlador::resetearAnimaciones(){
	this->golpeando = false;
	this->saltando = false;
	this->agachando = false;
	this->alturaActualSalto = juego->getPosicionJugador()->getVertical();
	this->alturaMaximaSalto = juego->getPosicionJugador()->getVertical()+25;
}

void Controlador::moviendoDerecha(int setAction){
	juego->movimientoDerecha();
	spriteFlip = SDL_FLIP_NONE;
	if(accionActual != setAction){
		jugador->setAnimacionActual(setAction, spriteFlip);
		accionActual = setAction;
	}
}

void Controlador::moviendoIzquierda(int setAction){
	juego->movimientoIzquierda();
	spriteFlip = SDL_FLIP_HORIZONTAL;
	if(accionActual != setAction){
		jugador->setAnimacionActual(setAction, spriteFlip);
		accionActual = setAction;
	}
}

void Controlador::verificarJoystick(){

	//Joystciks

	//X axis motion
	//Right of dead zone
	if( x_move > JOYSTICK_DEAD_ZONE && !golpeando && !agachando ){
		moviendoDerecha(caminar);
		teclado = false;
	}
	//Left of dead zone
	if( x_move < -JOYSTICK_DEAD_ZONE && !golpeando && !agachando ){
	   	moviendoIzquierda(caminar);
	   	teclado = false;
	}
	//Y axis motion
	//Up of dead zone
	if( y_move > JOYSTICK_DEAD_ZONE && !golpeando && !agachando ){
	   	juego->movimientoAbajo();
	   	teclado = false;
	   	if(accionActual != caminar){
	   		jugador->setAnimacionActual(caminar, spriteFlip);
	   		accionActual = caminar;
		}
	}
	//Down of dead zone
	if( y_move < -JOYSTICK_DEAD_ZONE && !golpeando && !agachando ){
		juego->movimientoArriba();
		teclado = false;
		if(accionActual != caminar){
			jugador->setAnimacionActual(caminar, spriteFlip);
			accionActual = caminar;
		}
	}
	if( ( y_move > -JOYSTICK_DEAD_ZONE && y_move < JOYSTICK_DEAD_ZONE ) && ( x_move < JOYSTICK_DEAD_ZONE && x_move > -JOYSTICK_DEAD_ZONE) && !(teclado) && !(golpeando) && !(agachando) ){
	    jugador->setAnimacionActual(parado, spriteFlip);
	    accionActual = parado;
	}

}

void Controlador::preparoSalto(int tipoDeSalto, int setAction){
	saltando = true;
	tipoSalto = tipoDeSalto;
	alturaActualSalto = juego->getPosicionJugador()->getVertical();
	alturaMaximaSalto = juego->getPosicionJugador()->getVertical()+25;
	if(accionActual != setAction){
		jugador->setAnimacionActual(setAction, spriteFlip);
		accionActual = setAction;
	}
}

bool Controlador::eventHandler(){

	//actualizar el array de estados (para GetKeyboardState)
	SDL_PumpEvents();

	bool running = true;

	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	SDL_JoystickEventState(1);
	SDL_PollEvent(&e);
	x_move = SDL_JoystickGetAxis(gameController, 0);
	y_move = SDL_JoystickGetAxis(gameController, 1);

	if(!saltando){

		verificarJoystick();
		if(e.type == SDL_JOYBUTTONDOWN){
			switch(e.jbutton.button){
			case 0: preparoSalto(SALTO_VERTICAL, saltoVertical);
					if( x_move > JOYSTICK_DEAD_ZONE ){
						preparoSalto(SALTO_DERECHA, salto);
						teclado = false;
					}
					if( x_move < -JOYSTICK_DEAD_ZONE ){
						preparoSalto(SALTO_IZQUIERDA, salto);
						teclado = false;
					}
			break;
			case 1: goto agacharse;
			break;
			case 2: goto pegar;
			break;
			}
		}

		if(keystates[SDL_SCANCODE_RIGHT] && !(keystates[SDL_SCANCODE_SPACE]) && !golpeando && !agachando) {
			teclado = true;
			moviendoDerecha(caminar);
		}

		if(keystates[SDL_SCANCODE_LEFT] && !(keystates[SDL_SCANCODE_SPACE]) && !golpeando && !agachando) {
			teclado = true;
			moviendoIzquierda(caminar);
		}

		if(keystates[SDL_SCANCODE_UP] && !golpeando && !agachando) {
			teclado = true;
			juego->movimientoArriba();
			if(accionActual != caminar){
				jugador->setAnimacionActual(caminar, spriteFlip);
				accionActual = caminar;
			}
		}

		if(keystates[SDL_SCANCODE_DOWN] && !golpeando && !agachando) {
			teclado = true;
			juego->movimientoAbajo();
			if(accionActual != caminar){
				jugador->setAnimacionActual(caminar, spriteFlip);
				accionActual = caminar;
			}
		}

		if(keystates[SDL_SCANCODE_SPACE] && !golpeando && !agachando) {
			preparoSalto(SALTO_VERTICAL, saltoVertical);
		}

		if( ( keystates[SDL_SCANCODE_RIGHT] && keystates[SDL_SCANCODE_SPACE] && !golpeando && !agachando ) ) {
			preparoSalto(SALTO_DERECHA, salto);
			spriteFlip = SDL_FLIP_NONE;
		}

		if(keystates[SDL_SCANCODE_RIGHT] && keystates[SDL_SCANCODE_LALT] && !golpeando && !agachando) {
			preparoSalto(SALTO_DERECHA, saltoPatada);
			spriteFlip = SDL_FLIP_NONE;
		}

		if(keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_SPACE] && !golpeando && !agachando) {
			preparoSalto(SALTO_IZQUIERDA, salto);
			spriteFlip = SDL_FLIP_HORIZONTAL;
		}

		if(keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_LALT] && !golpeando && !agachando) {
			preparoSalto(SALTO_IZQUIERDA, saltoPatada);
			spriteFlip = SDL_FLIP_HORIZONTAL;
		}

		if(keystates[SDL_SCANCODE_LSHIFT] && !saltando && !agachando) {
			pegar:
			golpeando = true;
			if(accionActual != golpear){
				jugador->setAnimacionActual(golpear, spriteFlip);
				accionActual = golpear;
			}
		}
		if((4 - jugador->getTicks()) < 0 && golpeando){
			golpeando = false;
			jugador->setAnimacionActual(parado, spriteFlip);
			accionActual = parado;
		}

		if(keystates[SDL_SCANCODE_LCTRL] && !saltando && !golpeando) {
			agacharse:
				if(accionActual != agachado){
					jugador->setAnimacionActual(agachado, spriteFlip);
					accionActual = agachado;
					agachando = true;
				}
			}
		if((2 - jugador->getTicks()) < 0 && agachando){
				agachando = false;
				jugador->setAnimacionActual(parado, spriteFlip);
				accionActual = parado;
		}

		if(keystates[SDL_SCANCODE_ESCAPE]) {
			running = false;
			return running;
		}

		if(e.type == SDL_QUIT){
			running =false;
			return running;
		}

		if(e.type == SDL_KEYUP && !golpeando && !agachando){
			jugador->setAnimacionActual(parado, spriteFlip);
			accionActual = parado;
		}

		return running;
	}
	if(e.jbutton.button == 3 && accionActual != saltoPatada || keystates[SDL_SCANCODE_LSHIFT]){
		jugador->setAnimacionActual(saltoPatada, spriteFlip);
		accionActual = saltoPatada;
	}
	if(alturaActualSalto < alturaMaximaSalto ){
		juego->movimientoSalto();
		alturaActualSalto = juego->getPosicionJugador()->getVertical();
		switch(tipoSalto){
			case SALTO_VERTICAL: return running;				//Salta en vertical
			break;
			case SALTO_DERECHA: juego->movimientoDerecha();
			break;
			case SALTO_IZQUIERDA: juego->movimientoIzquierda();
			break;
		}
		return running;
	}
	juego->terminadoSalto();
	saltando = false;
	accionActual = parado;
	jugador->setAnimacionActual(accionActual, spriteFlip);
	return running;
}
*/
