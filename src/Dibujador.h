#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>

struct informacion{
	std::vector<struct animados> animados;
	std::vector<struct estaticos> estaticos;
	std::vector<struct capas> capas;
};

struct renderizable {
	SDL_Texture* textura;
	SDL_Rect source;
	SDL_Rect destination;
	SDL_RendererFlip flip;
	bool operator < (const renderizable& a) const {

		return (destination.y + destination.h) < (a.destination.y + a.destination.h);
	    }
} ;

class Dibujador {
public:
	void inicializarNivel(char const* informacion);
	void actualizarPosiciones(char const* informacion);
private:
	struct informacion informacion;
};

#endif
