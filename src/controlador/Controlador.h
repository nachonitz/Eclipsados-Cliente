#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../defs.h"
#include "../Logger.h"
#include "../Sonido.h"

#include "../Dibujador.h"

struct informacionEnv{
	int animacionActual;
	int movimiento;
	SDL_RendererFlip flip;
	int numeroCliente;
};


class Controlador {
public:
	Controlador();
	~Controlador();
	struct informacionEnv eventHandler(Sonido* musica, bool *salir);

	void setAccionActual(int acActual);
	void setAcciones(int c, int p, int s, int sPatada, int g, int a, int sV);
	void resetearAnimaciones();
	void closeController();
	void moviendoDerecha(int setAction);
	void moviendoIzquierda(int setAction);
	void verificarJoystick(SDL_Event e, Sonido* musica, bool *salir);
	void preparoSalto(int tipoDeSalto, int setAction);
	void agacharse(SDL_RendererFlip spriteFlip);
	void pegar(SDL_RendererFlip spriteFlip);


private:
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
	bool teclado, musicPlaying;
	struct informacionEnv infoEnv;

	bool musicaYaApretada = false, testModeYaApretado = false;

	int timeoutT = 50, timeoutM = 50;
};

#endif /* CONTROLADOR_H_ */
