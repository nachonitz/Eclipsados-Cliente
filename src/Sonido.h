#ifndef SONIDO_H_
#define SONIDO_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

class Sonido {
public:
	Sonido(int numeroNivel);
	virtual ~Sonido();
	void play();
	void stop();
	void resume();

private:
	//La musica de fondo
	Mix_Music *musicaFondoNivel1;

	//Efectos de sonidos
	/*Mix_Chunk *golpe;
	Mix_Chunk *salto;
	Mix_Chunk *patada;
	Mix_Chunk *especial;*/


};

#endif /* SONIDO_H_ */
