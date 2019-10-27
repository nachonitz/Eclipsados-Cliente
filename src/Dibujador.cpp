#include "Dibujador.h"
#include "Logger.h"


void Dibujador::inicializar(){
	if ( SDL_Init(SDL_INIT_VIDEO) != 0);
	//Logger::getInstance()->log(ERROR, SDL_GetError());

	//Logger::getInstance()->log(INFO, "Inicializando ventana...");
	if ( SDL_CreateWindowAndRenderer(WINDOW_SIZE_HORIZONTAL, WINDOW_SIZE_VERTICAL, 0, &win, &ren) );
	//Logger::getInstance()->log(ERROR, SDL_GetError());

	SDL_SetWindowTitle(win, "Final Fight");

	setearTexturas();
}

void Dibujador::dibujar(struct informacionRec info){
	//toDo Pasar de char* a las posiciones

	//frameStart=SDL_GetTicks();
	//static int lastTime;

	SDL_RenderClear(ren);

	SDL_RenderCopy(ren, tex3, &info.capas[2].src, &info.capas[2].dest);
	SDL_RenderCopy(ren, tex2, &info.capas[1].src, &info.capas[1].dest);
	SDL_RenderCopy(ren, tex1, &info.capas[0].src, &info.capas[0].dest);

	for(int i = 0; i < (info.cantJugadores); i++){
		SDL_RenderCopyEx(ren, texCody[i], &info.animados[i].src, &info.animados[i].dest, 0, NULL, info.animados[i].flip);
	}
	for(int i = info.cantJugadores; i < (info.cantAnimados); i++){
		SDL_RenderCopyEx(ren, texEnemigo, &info.animados[i].src, &info.animados[i].dest, 0, NULL, info.animados[i].flip);
	}

	for(int i = 0; i < (info.cantElementos); i++){
		SDL_RenderCopy(ren, texElemento, &info.elementos[i].src, &info.elementos[i].dest);
	}
	SDL_RenderPresent(ren);

	/*
	std::sort(renderizables.begin(), renderizables.end());
	for (uint i = 0; i < renderizables.size(); i++) {
		SDL_RenderCopyEx(ren, renderizables[i].textura, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);
	}
	renderizables.clear();

	if(frameDelay > lastTime){
		SDL_Delay(frameDelay - lastTime);
	}*/
}

void Dibujador::setearTexturas(){

	SDL_Surface* surfCapa1 = IMG_Load("../sprites/Nivel1-fondo1-XL.png");
	SDL_Surface* surfCapa2 = IMG_Load("../sprites/Nivel1-fondo2-XL.png");
	SDL_Surface* surfCapa3 = IMG_Load("../sprites/Nivel1-fondo3-XL.png");

	SDL_Surface* surfCody1 = IMG_Load("../sprites/SpriteCodyCompleto1-120x120.png");
	SDL_Surface* surfCody2 = IMG_Load("../sprites/SpriteCodyCompleto2-120x120.png");
	SDL_Surface* surfCody3 = IMG_Load("../sprites/SpriteCodyCompleto3-120x120.png");
	SDL_Surface* surfCody4 = IMG_Load("../sprites/SpriteCodyCompleto4-120x120.png");

	SDL_Surface* surfEnemigo = IMG_Load("../sprites/AndoreJr.png");
	SDL_Surface* surfElemento = IMG_Load("../sprites/objetos_varios.png");

	/*if (surf == nullptr) {

	    tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 500, 500);

	    Uint32 * pixels = new Uint32[500* 500];

	    memset(pixels, 50*(this->velocidad), 500* 500* sizeof(Uint32));

	    SDL_UpdateTexture(tex, NULL, pixels, 500* sizeof(Uint32));

	    Logger::getInstance()->log(ERROR, "No se encuentra el sprite para capa, se mostrara una textura erronea.");
	}
	else*/
		tex1 = SDL_CreateTextureFromSurface(ren, surfCapa1);
		tex2 = SDL_CreateTextureFromSurface(ren, surfCapa2);
		tex3 = SDL_CreateTextureFromSurface(ren, surfCapa3);

		texCody[0] = SDL_CreateTextureFromSurface(ren, surfCody1);
		texCody[1] = SDL_CreateTextureFromSurface(ren, surfCody2);
		texCody[2] = SDL_CreateTextureFromSurface(ren, surfCody3);
		texCody[3] = SDL_CreateTextureFromSurface(ren, surfCody4);

		texEnemigo = SDL_CreateTextureFromSurface(ren, surfEnemigo);
		texElemento = SDL_CreateTextureFromSurface(ren, surfElemento);

}
