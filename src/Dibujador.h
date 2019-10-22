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
	//vector<struct animado> animados;
	//vector<struct elemento> elementos;
	//vector<struct capa> capas;
	struct elemento elementos[15];
	struct capa capas[3];
	struct animado animados[9];
	int cantAnimados;
	int cantElementos;
};

class Dibujador {
public:
	void inicializar();
	void dibujar(struct informacion info);
	void setearTexturas();
private:
	int FPS = 60;
	int frameCount;
	int frameDelay = 1000/FPS;
	int timerFPS;
	Uint32 frameStart;
	SDL_Texture* tex1;
	SDL_Texture* tex2;
	SDL_Texture* tex3;
	SDL_Texture* texCody;
	SDL_Texture* texEnemigo;
	SDL_Texture* texElemento;
	SDL_Renderer* ren;
	SDL_Window* win;
	struct informacion informacion;
};

#endif
