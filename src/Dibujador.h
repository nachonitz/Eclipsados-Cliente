#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include <iostream>
#include <vector>
#include "defs.h"
#include <algorithm>

#include "Sonido.h"

//#include "cody.h"
struct animado{
	//SDL_Texture* txt;
	bool estaActivo;
	SDL_Rect dest;
	SDL_Rect src;
	SDL_RendererFlip flip;
	int tipoEnemigo;
	int ID;
	int elementoEnMano;
	bool dibujar;
};

struct elemento{
	//SDL_Texture* txt;
	SDL_Rect dest;
	SDL_Rect src;
};

struct capa{
	//SDL_Texture* txt;
	int nivel;
	SDL_Rect dest;
	SDL_Rect src;
};


struct informacionRec{
	struct elemento elementos[MAX_CANT_ELEMENTOS];
	struct capa capas[3];
	struct animado animados[MAX_CANT_ANIMADOS];
	int scores[MAX_CLIENTES];
	int vidas[MAX_CLIENTES];
	int energia[MAX_CLIENTES];
	int cantAnimados;
	int cantElementos;
	int cantJugadores;
	int nivelActual;
};


struct credencial {
	char usuario[MAX_USER_LEN];
	char contrasenia[MAX_PASS_LEN];
	bool credencialValida;
	int myID;
};

struct renderizable {
	SDL_Texture* textura;
	SDL_Rect source;
	SDL_Rect destination;
	SDL_RendererFlip flip;
	bool soyJugador, estaActivo;
	int idxJugador;
	bool operator < (const renderizable& a) const {

		return (destination.y + destination.h) < (a.destination.y + a.destination.h);
	    }
};

class Dibujador {
public:

	void inicializar(std::vector<std::string> &nivel1, std::vector<std::string> &nivel2, std::vector<std::string> &sprites);
	void dibujar(struct informacionRec info, int ID, Sonido* reproductorMusica);
	void setearTexturas(std::vector<std::string> &nivel1, std::vector<std::string> &nivel2, std::vector<std::string> &sprites);
	void login(struct credencial* credencialCliente, bool errorAlValidar, bool esperar);
	~Dibujador();

	void mostrarPantallaEspera(int myId);
	void mostrarPantallaErrorConTexto(std::string mensaje);
	void mostrarPantallaScores (int scores[MAX_CLIENTES], int cantidadJugadores, bool finDelJuego);

private:

	SDL_Texture* crearTexturaDesdeRuta(std::string ruta);
	int obtenerIndiceBarra(int nivelEnergiaRestante);

	int frameCount;
	int timerFPS;
	Uint32 frameStart;
	SDL_Texture* tex1;
	SDL_Texture* tex2;
	SDL_Texture* tex3;

	SDL_Texture* nivel2Capa1;
	SDL_Texture* nivel2Capa2;
	SDL_Texture* nivel2Capa3;

	SDL_Texture* texCody[MAX_CLIENTES];
	SDL_Texture* texCodyCuchillo[MAX_CLIENTES];
	SDL_Texture* texCodyCanio[MAX_CLIENTES];


	SDL_Texture* texCodyInactivo;
	SDL_Texture* texEnemigo[4];
	SDL_Texture* texElemento;

	SDL_Texture* texCarasCody;
	SDL_Rect caraCodysrc[MAX_CLIENTES];
	SDL_Rect carasCodydest[MAX_CLIENTES];
	SDL_Texture* texBarrasEnergia;
	SDL_Rect barrasEnergiasrc[MAX_NIVELES_ENERGIA];
	SDL_Rect barrasEnergiadest[MAX_CLIENTES];

	SDL_Color colorTextoScore;
	TTF_Font* fontScore;
	SDL_Surface* surfaceTextoScore;
	SDL_Texture* texturaTextoScore[MAX_CLIENTES];
	SDL_Rect rectTextoScore[MAX_CLIENTES];
	int width, height, score;

	SDL_Renderer* ren;
	SDL_Window* win;
	struct informacionRec informacion;

	std::vector<renderizable> renderizables;
	std::vector<renderizable> renderizablesJugadores;


};

#endif
