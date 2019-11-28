#include "Sonido.h"
#include "Logger.h"


Sonido::Sonido(int numeroNivel) {

    Logger::getInstance()->log(INFO, "Inicializando subsistema de sonido...");
	if ( SDL_Init(SDL_INIT_AUDIO) != 0)
        Logger::getInstance()->log(ERROR, SDL_GetError());

    Logger::getInstance()->log(INFO, "Inicializando mezclador...");
    if ( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
        Logger::getInstance()->log(ERROR, Mix_GetError());

    if (numeroNivel == 1){
		Logger::getInstance()->log(INFO, "Cargando música de fondo: nivel 1 (musica/Nivel1-musica.mp3)");
		musicaFondoNivel = Mix_LoadMUS( "musica/Nivel1-musica.mp3" );
	}else{
		Logger::getInstance()->log(INFO, "Cargando música de fondo: nivel 2 (musica/Nivel2-musica.mp3)");
		musicaFondoNivel = Mix_LoadMUS( "musica/Nivel2-musica.mp3" );
	}
    if ( !musicaFondoNivel )
    	Logger::getInstance()->log(ERROR, Mix_GetError());

    Mix_VolumeMusic(VOLUMEN_MUSICA);

    golpeMano = Mix_LoadWAV("musica/acciones/golpeMano.wav");
    Mix_VolumeChunk(golpeMano, VOLUMEN_ACCIONES);

    golpeCanio = Mix_LoadWAV("musica/acciones/golpeCanio.wav");
    Mix_VolumeChunk(golpeCanio, VOLUMEN_ACCIONES);

    golpeCuchillo = Mix_LoadWAV("musica/acciones/golpeCanio.wav");
    Mix_VolumeChunk(golpeCuchillo, VOLUMEN_ACCIONES);

    agarrarElemento = Mix_LoadWAV("musica/acciones/agarrarElemento.wav");
    Mix_VolumeChunk(agarrarElemento, VOLUMEN_ACCIONES);

    muerteJugador = Mix_LoadWAV("musica/acciones/muerteJugador.wav");
    Mix_VolumeChunk(muerteJugador, VOLUMEN_ACCIONES);

    muerteEnemigo = Mix_LoadWAV("musica/acciones/muerteEnemigo.wav");
    Mix_VolumeChunk(muerteEnemigo, VOLUMEN_ACCIONES);


}

Sonido::~Sonido() {

    //Libero los efectos de sonido
    Mix_FreeChunk( golpeCanio);
    Mix_FreeChunk( golpeCuchillo);
    Mix_FreeChunk( golpeMano);
    Mix_FreeChunk( agarrarElemento);

    //Libero la musica de fondo
    Mix_FreeMusic(musicaFondoNivel);
    Mix_Quit();

}

void Sonido::reproducirSonidoEnemigoSegunSrc(SDL_Rect& srcEnemigo) {
	int x = srcEnemigo.x;
	int y = srcEnemigo.y;

	if (x != 220) // reproduce sonido solo si esta en el tick=1
		return;

	switch (y) {
	case 264*2:	// golpe
		this->reproducir(golpeMano);
		break;

	case 264*3:	// recibir danio
	case 264*4:	// muerte :(
		this->reproducir(muerteEnemigo);
		break;

	}
}

void Sonido::reproducirSonidoJugadorSegunSrc(SDL_Rect& srcJugador, int ELEMENTO_EN_MANO) {

	int x = srcJugador.x;
	int y = srcJugador.y;

	if (x != 230) // reproduce sonido solo si esta en el tick=1
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

	case 280*6: // recibir golpe
		this->reproducir(muerteJugador);
	}

}

void Sonido::reproducirSonidoElementoSegunSrc(SDL_Rect& srcElemento) {

}


void Sonido::reproducir(Mix_Chunk* sonido) {
    Mix_PlayChannel( -1, sonido, 0 );
}

void Sonido::play(){

  Logger::getInstance()->log(INFO, "Reproduciendo música de fondo...");
	if ( Mix_PlayMusic(musicaFondoNivel, -1) == -1)
    Logger::getInstance()->log(ERROR, Mix_GetError());
}

void Sonido::stop(){

  Logger::getInstance()->log(INFO, "Deteniendo música de fondo...");
  Mix_PauseMusic();
}

void Sonido::resume(){

  Logger::getInstance()->log(INFO, "Reanudando música de fondo...");
  Mix_ResumeMusic();
}
