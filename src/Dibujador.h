#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>
#include "defs.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
//#include "cody.h"
struct animado{
	//SDL_Texture* txt;
	SDL_Rect dest;
	SDL_Rect src;
	SDL_RendererFlip flip;
};

struct elemento{
	//SDL_Texture* txt;
	SDL_Rect dest;
	SDL_Rect src;
};

struct capa{
	//SDL_Texture* txt;
	SDL_Rect dest;
	SDL_Rect src;
};

struct informacion{
	std::vector<struct animado> animados;
	std::vector<struct elemento> elementos;
	std::vector<struct capa> capas;
};

class Dibujador {
public:
	void inicializar();
	void dibujar(struct informacion info);
	void setearCapas();
private:
	SDL_Texture* tex;
	SDL_Renderer* ren;
	SDL_Window* win;
	struct informacion informacion;
};

#endif
