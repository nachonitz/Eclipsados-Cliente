#include "Controlador.h"

Controlador::Controlador() {
	// TODO Auto-generated constructor stub
	this->setAcciones(ACCION_CAMINAR, ACCION_PARADO, ACCION_SALTO, ACCION_SALTO_PATADA, ACCION_GOLPEAR, ACCION_AGACHADO, ACCION_SALTO_VERTICAL);
	spriteFlip = SDL_FLIP_NONE;
	infoEnv.animacionActual = parado;
	infoEnv.flip = spriteFlip;
	infoEnv.movimiento = STAND;
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
		//Cosas Para Loggear
		std::cout << "Controller Name: " << SDL_JoystickName(gameController) << std::endl;
		std::cout << "Cant Axes: " << SDL_JoystickNumAxes(gameController) << std::endl;
		std::cout << "Cant Buttons: " << SDL_JoystickNumButtons(gameController) << std::endl;
	}
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
}

void Controlador::moviendoDerecha(int setAction){
	infoEnv.movimiento = RIGHT;
	spriteFlip = SDL_FLIP_NONE;
	infoEnv.flip = spriteFlip;
	if(accionActual != setAction){
		accionActual = setAction;
		infoEnv.animacionActual = accionActual;
		infoEnv.flip = spriteFlip;
	}
}

void Controlador::moviendoIzquierda(int setAction){
	infoEnv.movimiento = LEFT;
	spriteFlip = SDL_FLIP_HORIZONTAL;
	infoEnv.flip = spriteFlip;
	if(accionActual != setAction){
		accionActual = setAction;
		infoEnv.animacionActual = accionActual;
		infoEnv.flip = spriteFlip;
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
		infoEnv.movimiento = DOWN;
	   	teclado = false;
	   	if(accionActual != caminar){
	   		accionActual = caminar;
	   		infoEnv.animacionActual = accionActual;
	   		infoEnv.flip = spriteFlip;
		}
	}
	//Down of dead zone
	if( y_move < -JOYSTICK_DEAD_ZONE && !golpeando && !agachando ){
		infoEnv.movimiento = UP;
		teclado = false;
		if(accionActual != caminar){
			accionActual = caminar;
			infoEnv.animacionActual = accionActual;
			infoEnv.flip = spriteFlip;
		}
	}
	if( ( y_move > -JOYSTICK_DEAD_ZONE && y_move < JOYSTICK_DEAD_ZONE ) && ( x_move < JOYSTICK_DEAD_ZONE && x_move > -JOYSTICK_DEAD_ZONE) && !(teclado) && !(golpeando) && !(agachando) ){
		accionActual = parado;
		infoEnv.animacionActual = accionActual;
		infoEnv.flip = spriteFlip;
		infoEnv.movimiento = STAND;
	}

}

void Controlador::preparoSalto(int tipoDeSalto, int setAction){
	tipoSalto = tipoDeSalto;
	if(accionActual != setAction){
		accionActual = setAction;
		infoEnv.animacionActual = setAction;
		infoEnv.flip = spriteFlip;
	}
}

struct informacionEnv Controlador::eventHandler(){

	//actualizar el array de estados (para GetKeyboardState)
	SDL_PumpEvents();

	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	SDL_JoystickEventState(1);
	SDL_PollEvent(&e);
	x_move = SDL_JoystickGetAxis(gameController, 0);
	y_move = SDL_JoystickGetAxis(gameController, 1);

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
		infoEnv.movimiento = UP;
		if(accionActual != caminar){
			accionActual = caminar;
			infoEnv.animacionActual = accionActual;
			infoEnv.flip = spriteFlip;
		}
	}

	if(keystates[SDL_SCANCODE_DOWN] && !golpeando && !agachando) {
		teclado = true;
		infoEnv.movimiento = DOWN;
		if(accionActual != caminar){
			accionActual = caminar;
			infoEnv.animacionActual = accionActual;
			infoEnv.flip = spriteFlip;
		}
	}

	if(keystates[SDL_SCANCODE_SPACE] && !golpeando && !agachando) {
		preparoSalto(SALTO_VERTICAL, saltoVertical);
	}

	if(keystates[SDL_SCANCODE_RIGHT] && keystates[SDL_SCANCODE_SPACE] && !golpeando && !agachando) {
		preparoSalto(SALTO_DERECHA, salto);
		spriteFlip = SDL_FLIP_NONE;
		infoEnv.flip = spriteFlip;
	}

	if(keystates[SDL_SCANCODE_RIGHT] && keystates[SDL_SCANCODE_LALT] && !golpeando && !agachando) {
		preparoSalto(SALTO_DERECHA, saltoPatada);
		spriteFlip = SDL_FLIP_NONE;
		infoEnv.flip = spriteFlip;
	}

	if(keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_SPACE] && !golpeando && !agachando) {
		preparoSalto(SALTO_IZQUIERDA, salto);
		spriteFlip = SDL_FLIP_HORIZONTAL;
		infoEnv.flip = spriteFlip;
	}
	if(keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_LALT] && !golpeando && !agachando) {
		preparoSalto(SALTO_IZQUIERDA, saltoPatada);
		spriteFlip = SDL_FLIP_HORIZONTAL;
		infoEnv.flip = spriteFlip;
	}

	if(keystates[SDL_SCANCODE_LSHIFT] && !saltando && !agachando) {
		pegar:
		if(accionActual != golpear){
			accionActual = golpear;
			infoEnv.animacionActual = accionActual;
			infoEnv.flip = spriteFlip;
		}
	}

	if(keystates[SDL_SCANCODE_LCTRL] && !saltando && !golpeando) {
		agacharse:
		if(accionActual != agachado){
			accionActual = agachado;
			infoEnv.animacionActual = accionActual;
			infoEnv.flip = spriteFlip;
		}
	}

	if(keystates[SDL_SCANCODE_ESCAPE]) {
		//setear variable para salir
		exit(0);
	}

	if(e.type == SDL_QUIT){
		//setear variable para salir
		exit(0);
	}

	if(e.type == SDL_KEYUP && !golpeando && !agachando){
		accionActual = parado;
		infoEnv.animacionActual = accionActual;
		infoEnv.flip = spriteFlip;
		infoEnv.movimiento = STAND;
	}

	return infoEnv;
}
/*
	if(e.jbutton.button == 3 && accionActual != saltoPatada || keystates[SDL_SCANCODE_LSHIFT]){
		/*jugador->setAnimacionActual(saltoPatada, spriteFlip);
		mandar al servidor que updatee la animacion
		accionActual = saltoPatada;
		infoEnv.animacionActual = accionActual;
		infoEnv.flip = spriteFlip;
	} */
	/*if(alturaActualSalto < alturaMaximaSalto ){
		//juego->movimientoSalto();	avisarle al servidor que se salto
		//alturaActualSalto = juego->getPosicionJugador()->getVertical(); pedir altura al servidor
		switch(tipoSalto){
			case SALTO_VERTICAL: return infoEnv = ACCION_SALTO;				//Salta en vertical
			break;
			case SALTO_DERECHA: infoEnv.movimiento = SALTO_RIGHT; 	//juego->movimientoDerecha();	servidor mover derecha
			break;
			case SALTO_IZQUIERDA: infoEnv.movimiento = SALTO_LEFT; //juego->movimientoIzquierda();	servidor mover izquierda
			break;
		}
		return infoEnv;
	}*/
