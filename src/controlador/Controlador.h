/*#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_


#include "../modelo/Juego.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Controlador {
public:
	Controlador(Juego* juego);
	~Controlador();
	bool eventHandler();

	void setAccionActual(int acActual);
	void setAcciones(int c, int p, int s, int sPatada, int g, int a, int sV);
	void resetearAnimaciones();
	void closeController();
	void moviendoDerecha(int setAction);
	void moviendoIzquierda(int setAction);
	void verificarJoystick();
	void preparoSalto(int tipoDeSalto, int setAction);

private:
	Juego* juego;
	Personaje* jugador;
	SDL_Event e;
	SDL_RendererFlip spriteFlip;
	SDL_Joystick* gameController;
	int caminar;
	int parado;
	int salto;
	int saltoPatada;
	int golpear;
	int agachado;
	int saltoVertical;
	int accionActual;
	int setAction;
	bool saltando;
	bool golpeando;
	bool agachando;
	int tipoSalto;
	float alturaActualSalto;
	float alturaMaximaSalto;
	int tmp;
	const int JOYSTICK_DEAD_ZONE = 8000;
	int x_move;
	int y_move;
	bool teclado;
};

#endif /* CONTROLADOR_H_ */
