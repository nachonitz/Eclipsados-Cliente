#ifndef DIBUJADOR_H_
#define DIBUJADOR_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include <iostream>
#include <vector>
#include "defs.h"

//#include "cody.h"
struct animado{
	//SDL_Texture* txt;
	bool estaActivo;
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
	int nivel;
	SDL_Rect dest;
	SDL_Rect src;
};


struct informacionRec{
	struct elemento elementos[MAX_CANT_ELEMENTOS];
	struct capa capas[3];
	struct animado animados[MAX_CANT_ANIMADOS];
	int cantAnimados;
	int cantElementos;
	int cantJugadores;
};


struct credencial {
	char usuario[MAX_USER_LEN];
	char contrasenia[MAX_PASS_LEN];
	bool credencialValida;
} ;

class Dibujador {
public:

	void inicializar(std::vector<std::string> &nivel1, std::vector<std::string> &nivel2, std::vector<std::string> &sprites);
	void dibujar(struct informacionRec info);
	void setearTexturas(std::vector<std::string> &nivel1, std::vector<std::string> &nivel2, std::vector<std::string> &sprites);
	void login(struct credencial* credencialCliente, bool errorAlValidar, bool esperar);
	~Dibujador();

private:

	SDL_Texture* crearTexturaDesdeRuta(std::string ruta);

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
	SDL_Texture* texCodyInactivo;
	SDL_Texture* texEnemigo;
	SDL_Texture* texElemento;
	SDL_Renderer* ren;
	SDL_Window* win;
	struct informacionRec informacion;




};

#endif
