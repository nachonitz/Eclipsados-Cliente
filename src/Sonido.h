#ifndef SONIDO_H_
#define SONIDO_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#include "defs.h"

class Sonido {
public:
	Sonido(int numeroNivel);
	virtual ~Sonido();
	void play();
	void stop();
	void resume();

	void reproducirSonidoEnemigoSegunSrc(SDL_Rect& srcEnemigo);

	void reproducirSonidoJugadorSegunSrc(SDL_Rect& srcJugador, int ELEMENTO_EN_MANO);

	void reproducirSonidoElementoSegunSrc(SDL_Rect& srcElemento);


private:
	//La musica de fondo
	Mix_Music *musicaFondoNivel;

	//Efectos de sonidos
	Mix_Chunk *golpeMano, *golpeCuchillo, *golpeCanio;
	Mix_Chunk* recibirDanio= nullptr;
	Mix_Chunk* roturaElemento = nullptr;
	Mix_Chunk* agarrarElemento;
	Mix_Chunk *salto= nullptr;
	Mix_Chunk *patada= nullptr;
	Mix_Chunk *muerteJugador,*muerteEnemigo;

	void reproducir(Mix_Chunk* sonido);


};

#endif /* SONIDO_H_ */
