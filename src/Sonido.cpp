#include "Sonido.h"
#include "Logger.h"


Sonido::Sonido(std::vector<std::string> &rutasParseadas) {

    Logger::getInstance()->log(INFO, "Inicializando subsistema de sonido...");
	if ( SDL_Init(SDL_INIT_AUDIO) != 0)
        Logger::getInstance()->log(ERROR, SDL_GetError());

    Logger::getInstance()->log(INFO, "Inicializando mezclador...");
    if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
        Logger::getInstance()->log(ERROR, Mix_GetError());

    musicaFondoNivel1 = cargarMusica(rutasParseadas[0].c_str());

    musicaFondoNivel2 = cargarMusica( rutasParseadas[1].c_str() );

    Mix_VolumeMusic(VOLUMEN_MUSICA);

    golpeMano = cargarChunk(rutasParseadas[2].c_str());
    if (golpeMano)
    	Mix_VolumeChunk(golpeMano, 30);

    golpeCanio = cargarChunk(rutasParseadas[3].c_str());
    if (golpeCanio)
    	Mix_VolumeChunk(golpeCanio, VOLUMEN_ACCIONES);

    golpeCuchillo = cargarChunk(rutasParseadas[4].c_str());
    if (golpeCuchillo)
    	Mix_VolumeChunk(golpeCuchillo, VOLUMEN_ACCIONES + 30);

    agarrarElemento = cargarChunk(rutasParseadas[5].c_str());
    if (agarrarElemento)
    	Mix_VolumeChunk(agarrarElemento, VOLUMEN_ACCIONES);

    muerteJugador = cargarChunk(rutasParseadas[6].c_str());
    if (muerteJugador)
    	Mix_VolumeChunk(muerteJugador, VOLUMEN_ACCIONES);

    muerteEnemigo = cargarChunk(rutasParseadas[7].c_str());
    if (muerteEnemigo)
    	Mix_VolumeChunk(muerteEnemigo, 30);

    recibirDanio = cargarChunk(rutasParseadas[8].c_str());
    if (recibirDanio)
    	Mix_VolumeChunk(recibirDanio, 30);

    intro = cargarChunk(rutasParseadas[9].c_str());
    if (intro)
    	Mix_VolumeChunk(intro, 60);

    saltoConPatada = cargarChunk(rutasParseadas[10].c_str());
    if (saltoConPatada)
    	Mix_VolumeChunk(saltoConPatada, 40);

    finNivel1 = cargarChunk(rutasParseadas[11].c_str());
    if (finNivel1)
    	Mix_VolumeChunk(finNivel1, 80);

    roturaElemento = cargarChunk(rutasParseadas[12].c_str());
    if (roturaElemento)
    	Mix_VolumeChunk(roturaElemento, 20);

}

Mix_Music* Sonido::cargarMusica(const char* ruta) {

	Logger::getInstance()->log(INFO, "Cargando música de fondo: " + std::string(ruta));
	Mix_Music* temp = Mix_LoadMUS( ruta );

	if ( !temp )
		Logger::getInstance()->log(ERROR, Mix_GetError());

	return temp;
}

Mix_Chunk* Sonido::cargarChunk(const char* ruta) {

	Logger::getInstance()->log(INFO, "Cargando sonido: " + std::string(ruta));
	Mix_Chunk* temp = Mix_LoadWAV( ruta );

	if ( !temp )
		Logger::getInstance()->log(ERROR, Mix_GetError());

	return temp;
}


Sonido::~Sonido() {

    //Libero los efectos de sonido
    Mix_FreeChunk( golpeCanio);
    Mix_FreeChunk( golpeCuchillo);
    Mix_FreeChunk( golpeMano);
    Mix_FreeChunk( agarrarElemento);
    Mix_FreeChunk( muerteEnemigo);
    Mix_FreeChunk( muerteJugador);
    Mix_FreeChunk( recibirDanio);

    //Libero la musica de fondo
    Mix_FreeMusic(musicaFondoNivel1);

    Mix_FreeMusic(musicaFondoNivel2);
    Mix_Quit();

}

void Sonido::reproducirSonidoIntro() {
	this->reproducir(intro);
}

void Sonido::reproducirSonidoFinNivel() {
	this->reproducir(finNivel1);
}



void Sonido::pasarNivel(int numeroNivel) {

	switch(numeroNivel) {
	case 1:
		Mix_PlayMusic(musicaFondoNivel1, -1);
		break;

	case 2:
		Mix_PlayMusic(musicaFondoNivel2, -1);
		break;
	}
}


void Sonido::reproducirSonidoEnemigoSegunSrc(SDL_Rect& srcEnemigo) {
	int x = srcEnemigo.x;
	int y = srcEnemigo.y;

	if (x != 0) // reproduce sonido solo si esta en el tick=1
		return;

	switch (y) {
	case 264*2:	// golpe
		//this->reproducir(golpeMano);
		break;

	case 264*3:	// recibir danio
		this->reproducir(recibirDanio); // no reproducir nada de momento
		break;

	case 264*4:	// muerte :(
		this->reproducir(muerteEnemigo);
		break;

	}
}

void Sonido::reproducirSonidoJugadorSegunSrc(SDL_Rect& srcJugador, int ELEMENTO_EN_MANO) {

	int x = srcJugador.x;
	int y = srcJugador.y;

	if (!(x == 230 || (x == 230*4 && y == 280*6) || (x == 0 && y == 280*6))) // reproduce sonido solo si esta en el tick=1, o en tick 4 y muriendo
		return;

	switch (y) {
	case 0:
		if (ELEMENTO_EN_MANO != ELEMENTO_MANO)
			this->reproducir(agarrarElemento);
		break;

	case 840: // golpeando
		if (ELEMENTO_EN_MANO == ELEMENTO_MANO)
			this->reproducir(golpeMano);
		else if (ELEMENTO_EN_MANO == ELEMENTO_CANIO)
			this->reproducir(golpeCanio);
		else if (ELEMENTO_EN_MANO == ELEMENTO_CUCHILLO)
			this->reproducir(golpeCuchillo);
		break;

	case 280*6: // recibir golpe o  morir
		if (x == 0)
			this->reproducir(recibirDanio);
		else if (x == 230*4)
			this->reproducir(muerteJugador);
		break;

	case 280*4:	// saltando con patada
		this->reproducir(saltoConPatada);
		break;
	}

}

void Sonido::reproducirSonidoElementoSegunSrc(SDL_Rect& srcElemento) {
	int x = srcElemento.x;
	int y = srcElemento.y;

	if (y == 100*3)
		this->reproducir(roturaElemento);
}


void Sonido::reproducir(Mix_Chunk* sonido) {
    if (sonido)
    	Mix_PlayChannel( -1, sonido, 0 );
}

void Sonido::stop(){

  Logger::getInstance()->log(INFO, "Deteniendo música de fondo...");
  Mix_PauseMusic();
}

void Sonido::resume(){

  Logger::getInstance()->log(INFO, "Reanudando música de fondo...");
  Mix_ResumeMusic();
}
