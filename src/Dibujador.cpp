#include "Dibujador.h"
#include "Logger.h"


void Dibujador::inicializar(){
	if ( SDL_Init(SDL_INIT_VIDEO) != 0)
	//Logger::getInstance()->log(ERROR, SDL_GetError());

	//Logger::getInstance()->log(INFO, "Inicializando ventana...");
	if ( SDL_CreateWindowAndRenderer(WINDOW_SIZE_HORIZONTAL, WINDOW_SIZE_VERTICAL, 0, &win, &ren) )
	//Logger::getInstance()->log(ERROR, SDL_GetError());

	SDL_SetWindowTitle(win, "Final Fight");

}

void Dibujador::dibujar(struct informacion info){
	//toDo Pasar de char* a las posiciones
	SDL_RenderClear(ren);

	SDL_RenderCopy(ren, info.capas[0].txt, &info.capas[0].src, &info.capas[0].dest);
	SDL_RenderCopy(ren, info.capas[1].txt, &info.capas[1].src, &info.capas[1].dest);
	SDL_RenderCopy(ren, info.capas[2].txt, &info.capas[2].src, &info.capas[2].dest);

	SDL_RenderPresent(ren);

	/*
	std::sort(renderizables.begin(), renderizables.end());
	for (uint i = 0; i < renderizables.size(); i++) {
		SDL_RenderCopyEx(ren, renderizables[i].textura, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);
	}
	renderizables.clear();
	*/
}
