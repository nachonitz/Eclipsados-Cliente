#include "Dibujador.h"
#include "Logger.h"


void Dibujador::inicializar(){
	if ( SDL_Init(SDL_INIT_VIDEO) != 0);
	//Logger::getInstance()->log(ERROR, SDL_GetError());

	//Logger::getInstance()->log(INFO, "Inicializando ventana...");
	if ( SDL_CreateWindowAndRenderer(WINDOW_SIZE_HORIZONTAL, WINDOW_SIZE_VERTICAL, 0, &win, &ren) );
	//Logger::getInstance()->log(ERROR, SDL_GetError());

	SDL_SetWindowTitle(win, "Final Fight");

	setearCapas();
}

void Dibujador::dibujar(struct informacion info){
	//toDo Pasar de char* a las posiciones

	frameStart=SDL_GetTicks();
	static int lastTime;

	SDL_RenderClear(ren);

	//setearCapas();

	SDL_RenderCopy(ren, tex3, &info.elementos[2].src, &info.elementos[2].dest);
	SDL_RenderCopy(ren, tex2, &info.elementos[1].src, &info.elementos[1].dest);
	SDL_RenderCopy(ren, tex1, &info.elementos[0].src, &info.elementos[0].dest);
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

void Dibujador::setearCapas(){

	SDL_Surface* surfCapa1 = IMG_Load("../sprites/Nivel1-fondo1-XL.png");
	SDL_Surface* surfCapa2 = IMG_Load("../sprites/Nivel1-fondo2-XL.png");
	SDL_Surface* surfCapa3 = IMG_Load("../sprites/Nivel1-fondo3-XL.png");

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

}
