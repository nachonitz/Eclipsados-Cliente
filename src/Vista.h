/*#ifndef VISTA_H_
#define VISTA_H_

#include "../defs.h"
#include "../modelo/Juego.h"
#include "../controlador/Controlador.h"
#include "../modelo/nivel/Capa.h"
#include "../modelo/nivel/Nivel.h"
#include "../AsignadorDeTexturas.h"
#include "../modelo/entidadUbicada/dibujable/Enemigo.h"
#include <stdlib.h>
#include <time.h>
#include <algorithm>

struct renderizable {
	SDL_Texture* textura;
	SDL_Rect source;
	SDL_Rect destination;
	SDL_RendererFlip flip;
	bool operator < (const renderizable& a) const {

		return (destination.y + destination.h) < (a.destination.y + a.destination.h);
	    }
} ;

class Vista {
public:
	Vista(Juego* modelo, Controlador* controlador, AsignadorDeTexturas& asignador);
	virtual ~Vista();
	void update();
	void render();
	void Draw();
	void loop();

private:
	void prepararCapa(Capa* capa,char const* imagen);
	void prepararSegundaCapa(Capa* capa,char const* imagen);
	void inicializarVistaParaNivel();


	Juego* juego;
	Controlador* controlador;
	Personaje* jugador;
	SDL_Renderer* ren;
	SDL_Window* win;
	bool running;
	int FPS = 60;
	int frameCount;
	int frameDelay = 1000/FPS;
	int timerFPS;
	Uint32 frameStart;
	int posicionActual;
	int accionActual;
	int parado;
	int caminar;
	int salto;
	int saltoPatada;
	int golpear;
	int agachado;
	int saltoVertical;
	float posicionX;
	float posicionY;

	int nivelActual;

	Capa* capa1;
	Capa* capa2;
	Capa* capa3;
	vector<EntidadUbicada*> elementos;
	vector<EntidadUbicada*> enemigos;
	AsignadorDeTexturas asignador;
	//Nivel* nivel;
	vector<renderizable> renderizables;
};

#endif /* VISTA_H_ */
