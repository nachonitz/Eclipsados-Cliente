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
			musicaFondoNivel1 = Mix_LoadMUS( "musica/Nivel1-musica.mp3" );
		}else{
      Logger::getInstance()->log(INFO, "Cargando música de fondo: nivel 2 (musica/Nivel2-musica.mp3)");
			musicaFondoNivel1 = Mix_LoadMUS( "musica/Nivel2-musica.mp3" );
		}
    if ( !musicaFondoNivel1 )
        Logger::getInstance()->log(ERROR, Mix_GetError());
}

Sonido::~Sonido() {

    //Libero los efectos de sonido
    /*Mix_FreeChunk( gScratch );
    Mix_FreeChunk( gHigh );
    Mix_FreeChunk( gMedium );
    Mix_FreeChunk( gLow );*/

    //Libero la musica de fondo
    Mix_FreeMusic(musicaFondoNivel1);
    Mix_Quit();

}

void Sonido::play(){

  Logger::getInstance()->log(INFO, "Reproduciendo música de fondo...");
	if ( Mix_PlayMusic(musicaFondoNivel1, -1) == -1)
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
