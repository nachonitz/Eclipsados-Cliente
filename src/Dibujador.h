#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_

#include <vector.h>

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
	vector <struct renderizable> enemigos;
	vector <struct renderizable> elementos;
	vector <struct renderizable> capas;
	vector <struct renderizable> personajes;
};
