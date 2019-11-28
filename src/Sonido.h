#ifndef SONIDO_H_
#define SONIDO_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#include <vector>
#include <string>

#include "defs.h"

class Sonido {
public:
	Sonido(std::vector<std::string> &rutas);
	virtual ~Sonido();
	void stop();
	void resume();

	void reproducirSonidoEnemigoSegunSrc(SDL_Rect& srcEnemigo);

	void reproducirSonidoJugadorSegunSrc(SDL_Rect& srcJugador, int ELEMENTO_EN_MANO);

	void reproducirSonidoElementoSegunSrc(SDL_Rect& srcElemento);

	void pasarNivel(int numeroNivel);


private:
	//La musica de fondo
	Mix_Music *musicaFondoNivel1, *musicaFondoNivel2;

	//Efectos de sonidos
	Mix_Chunk *golpeMano, *golpeCuchillo, *golpeCanio;
	Mix_Chunk* recibirDanio= nullptr;
	Mix_Chunk* roturaElemento = nullptr;
	Mix_Chunk* agarrarElemento;
	Mix_Chunk *salto= nullptr;
	Mix_Chunk *patada= nullptr;
	Mix_Chunk *muerteJugador,*muerteEnemigo;

	int nivelActual;

	void reproducir(Mix_Chunk* sonido);

	Mix_Music* cargarMusica(const char* ruta);
	Mix_Chunk* cargarChunk(const char* ruta);


};

#endif /* SONIDO_H_ */
