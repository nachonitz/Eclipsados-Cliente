#include "Dibujador.h"
#include "Logger.h"
#include <string>


void Dibujador::inicializar(std::vector<std::string> &nivel1, std::vector<std::string> &nivel2, std::vector<std::string> &sprites){
	if ( SDL_Init(SDL_INIT_VIDEO) != 0); // @suppress("Suspicious semicolon")
	//Logger::getInstance()->log(ERROR, SDL_GetError());

	//Logger::getInstance()->log(INFO, "Inicializando ventana...");
	if ( SDL_CreateWindowAndRenderer(WINDOW_SIZE_HORIZONTAL, WINDOW_SIZE_VERTICAL, 0, &win, &ren) ); // @suppress("Suspicious semicolon")
	//Logger::getInstance()->log(ERROR, SDL_GetError());

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	SDL_SetWindowTitle(win, "Final Fight");

	setearTexturas(nivel1, nivel2, sprites);
}

void Dibujador::dibujar(struct informacionRec info, int ID){
	//toDo Pasar de char* a las posiciones

	//frameStart=SDL_GetTicks();
	//static int lastTime;

	int j = 0;

	renderizable renderizableActual;

	SDL_RenderClear(ren);

	if (info.capas[2].nivel == 1){
		SDL_RenderCopy(ren, tex3, &info.capas[2].src, &info.capas[2].dest);
		SDL_RenderCopy(ren, tex2, &info.capas[1].src, &info.capas[1].dest);
		SDL_RenderCopy(ren, tex1, &info.capas[0].src, &info.capas[0].dest);
	}else{
		SDL_RenderCopy(ren, nivel2Capa3, &info.capas[2].src, &info.capas[2].dest);
		SDL_RenderCopy(ren, nivel2Capa2, &info.capas[1].src, &info.capas[1].dest);
		SDL_RenderCopy(ren, nivel2Capa1, &info.capas[0].src, &info.capas[0].dest);
	}

	for(int i = info.cantJugadores; i < (info.cantAnimados); i++){
		renderizableActual.textura = texEnemigo;
		renderizableActual.source = info.animados[i].src;
		renderizableActual.destination = info.animados[i].dest;
		renderizableActual.flip = info.animados[i].flip;
		renderizableActual.soyJugador = false;
		renderizableActual.estaActivo = true;
		renderizableActual.idxJugador = info.animados[i].ID;
		renderizables.push_back(renderizableActual);
	}

	for(int i = 0; i < (info.cantJugadores); i++){
		renderizableActual.textura = texCody[i];
		renderizableActual.source = info.animados[i].src;
		renderizableActual.destination = info.animados[i].dest;
		renderizableActual.flip = info.animados[i].flip;
		renderizableActual.soyJugador = true;
		renderizableActual.estaActivo = info.animados[i].estaActivo;
		renderizableActual.idxJugador = info.animados[i].ID;
		renderizablesJugadores.push_back(renderizableActual);
		renderizables.push_back(renderizableActual);
	}

	for(int i = 0; i < (info.cantElementos); i++){
		renderizableActual.textura = texElemento;
		renderizableActual.source = info.elementos[i].src;
		renderizableActual.destination = info.elementos[i].dest;
		renderizableActual.flip = SDL_FLIP_NONE;
		renderizableActual.soyJugador = false;
		renderizableActual.estaActivo = true;
		renderizableActual.idxJugador = -1;
		renderizables.push_back(renderizableActual);
	}

	std::sort(renderizables.begin(), renderizables.end());

	for (uint i = 0; i < renderizablesJugadores.size(); i++) {
		if(renderizablesJugadores[i].idxJugador == ID){
			renderizableActual.textura = texCody[i];
			renderizableActual.source = info.animados[i].src;
			renderizableActual.destination = info.animados[i].dest;
			renderizableActual.flip = info.animados[i].flip;
			renderizableActual.soyJugador = true;
			renderizableActual.estaActivo = info.animados[i].estaActivo;
			renderizableActual.idxJugador = info.animados[i].ID;
		}
	}

	for (uint i = 0; i < renderizables.size(); i++) {
		if (renderizables[i].estaActivo){
			SDL_RenderCopyEx(ren, renderizables[i].textura, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);
		}else{
			SDL_RenderCopyEx(ren, texCodyInactivo, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);
		}
	}

/*	for (uint i = 0; i < renderizables.size(); i++) {
		if(renderizables[i].soyJugador && renderizables[i].idxJugador != ID){
			j++;
			SDL_RenderCopyEx(ren, renderizables[i].textura, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);
		}else{
			if(!(renderizables[i].soyJugador)){
				SDL_RenderCopyEx(ren, renderizables[i].textura, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);
			}else{
				if(j == 3){
					if (renderizables[i].estaActivo){
						SDL_RenderCopyEx(ren, renderizables[i].textura, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);
					}else{
						SDL_RenderCopyEx(ren, texCodyInactivo, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);
					}
				}else{
					for(uint k = i-1; k < renderizables.size(); k++){
						if(renderizables[i].destination.x == renderizables[k].destination.x && renderizables[i].destination.y == renderizables[k].destination.y && renderizables[k].soyJugador){
							if (renderizables[k].estaActivo){
								SDL_RenderCopyEx(ren, renderizables[k].textura, &renderizables[k].source, &renderizables[k].destination, 0, NULL, renderizables[k].flip);
							}else{
								SDL_RenderCopyEx(ren, texCodyInactivo, &renderizables[k].source, &renderizables[k].destination, 0, NULL, renderizables[k].flip);
							}
							if (renderizables[i].estaActivo){
								SDL_RenderCopyEx(ren, renderizables[i].textura, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);
								//std::swap(renderizables[i], renderizables[k]);
							}else{
								SDL_RenderCopyEx(ren, texCodyInactivo, &renderizables[i].source, &renderizables[i].destination, 0, NULL, renderizables[i].flip);

							}
						}
					}
				}
			}
		}
	}
*/

		for (uint i = 0; i < renderizablesJugadores.size(); i++) {
			if(renderizableActual.destination.x >= (renderizablesJugadores[i].destination.x - 10) && renderizableActual.destination.x <= (renderizablesJugadores[i].destination.x + 10) && renderizableActual.destination.y >= (renderizablesJugadores[i].destination.y - 10) && renderizableActual.destination.y <= (renderizablesJugadores[i].destination.y + 10) && renderizableActual.idxJugador != renderizablesJugadores[i].idxJugador){
				if (renderizables[i].estaActivo){
					SDL_RenderCopyEx(ren, renderizableActual.textura, &renderizableActual.source, &renderizableActual.destination, 0, NULL, renderizableActual.flip);
				}else{
					SDL_RenderCopyEx(ren, texCodyInactivo, &renderizableActual.source, &renderizableActual.destination, 0, NULL, renderizableActual.flip);
				}
			}
		}

		SDL_RenderPresent(ren);

		renderizables.clear();
		renderizablesJugadores.clear();
	/*
		if(frameDelay > lastTime){
			SDL_Delay(frameDelay - lastTime);
		}*/
	}


void Dibujador::setearTexturas(std::vector<std::string> &nivel1, std::vector<std::string> &nivel2, std::vector<std::string> &sprites) {

	tex1 = crearTexturaDesdeRuta(nivel1.at(0).c_str());
	tex2 = crearTexturaDesdeRuta(nivel1.at(1).c_str());
	tex3 = crearTexturaDesdeRuta(nivel1.at(2).c_str());

	nivel2Capa1 = crearTexturaDesdeRuta(nivel2.at(0).c_str());
	nivel2Capa2 = crearTexturaDesdeRuta(nivel2.at(1).c_str());
	nivel2Capa3 = crearTexturaDesdeRuta(nivel2.at(2).c_str());

	texCody[0] = crearTexturaDesdeRuta(sprites.at(0).c_str());
	texCody[1] = crearTexturaDesdeRuta(sprites.at(1).c_str());
	texCody[2] = crearTexturaDesdeRuta(sprites.at(2).c_str());
	texCody[3] = crearTexturaDesdeRuta(sprites.at(3).c_str());
	texCodyInactivo = crearTexturaDesdeRuta(sprites.at(8).c_str());

	texEnemigo = crearTexturaDesdeRuta(sprites.at(6).c_str());
	texElemento = crearTexturaDesdeRuta(sprites.at(4).c_str());

}

SDL_Texture* Dibujador::crearTexturaDesdeRuta(std::string ruta) {

	SDL_Texture* tex = nullptr;

	SDL_Surface* surf = IMG_Load(ruta.c_str());

	if (surf == nullptr) {

		tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 500, 500);

		Uint32 * pixels = new Uint32[500* 500];

		memset(pixels, 50*ruta.length(), 500* 500* sizeof(Uint32));

		SDL_UpdateTexture(tex, NULL, pixels, 500* sizeof(Uint32));

		Logger::getInstance()->log(ERROR, "No se encuentra la imagen en " + ruta + ", se mostrara una textura erronea.");
	}
	else {
		tex = SDL_CreateTextureFromSurface(ren, surf);
	}

	return tex;
}

void Dibujador::login(struct credencial* credencialCliente, bool errorAlValidar, bool esperar){

	SDL_Texture* texturaFondoLogin1;
	SDL_Surface* surfaceFondoLogin1;
	SDL_Rect rectFondo1source;
	SDL_Rect rectFondo1dest;
	SDL_Texture* texturaFondoLogin2;
	SDL_Surface* surfaceFondoLogin2;
	SDL_Rect rectFondo2source;
	SDL_Rect rectFondo2dest;
	TTF_Font* font;
	TTF_Font* fontAyuda;

	SDL_Texture* texturaFlecha;
	SDL_Surface* surfaceFlecha;
	SDL_Rect rectFlechaSource;
	SDL_Rect rectFlechaDest;

	SDL_Surface* surfaceTexto1;
	SDL_Surface* surfaceTexto2;
	SDL_Surface* surfaceTexto3;
	SDL_Surface* surfaceTexto4;
	SDL_Surface* surfaceTextoError;
	SDL_Surface* surfaceTextoEsperando;
	SDL_Texture* texturaTextoLogin1;
	SDL_Texture* texturaTextoLogin2;
	SDL_Texture* texturaTextoLogin3;
	SDL_Texture* texturaTextoLogin4;
	SDL_Texture* texturaTextoLoginError;
	SDL_Texture* texturaTextoEsperando;
	SDL_Rect rectTexto1;
	SDL_Rect rectTexto2;
	SDL_Rect rectTexto3;
	SDL_Rect rectTexto4;
	SDL_Rect rectTextoError;
	SDL_Rect rectTextoEsperando;
	SDL_Color colorTextoAyuda;
	SDL_Color colorTexto;

	SDL_Surface* surfaceTextoInput1;
	SDL_Surface* surfaceTextoInput2;
	SDL_Texture* texturaTextoInput1;
	SDL_Texture* texturaTextoInput2;
	SDL_Rect rectTextoInput1;
	SDL_Rect rectTextoInput2;
	SDL_Color colorTextoInput;
	SDL_Color colorTextoError;
	TTF_Font* fontInput;

	std::string textoIngresadoUsuario = "";
	std::string textoIngresadoContrasenia = "";

	int width,height;
	bool isLogginIn = true;
	bool needToRender = true;
	bool inputEnUsuario = true;
	SDL_Event event;
	const Uint8* keystates;


	rectFlechaSource.x=0;rectFlechaSource.y=0;rectFlechaSource.w=850;rectFlechaSource.h=850;
	rectFlechaDest.x=20;rectFlechaDest.y=390;rectFlechaDest.w=70;rectFlechaDest.h =70;

	surfaceFlecha = IMG_Load("sprites/flecha.png");
	texturaFlecha = SDL_CreateTextureFromSurface(ren,surfaceFlecha);



	rectFondo1source.x=0;rectFondo1source.y=0;rectFondo1source.w=620;rectFondo1source.h=390;
	rectFondo1dest.x=0;rectFondo1dest.y=0;rectFondo1dest.w=WINDOW_SIZE_HORIZONTAL;rectFondo1dest.h=WINDOW_SIZE_VERTICAL;
	surfaceFondoLogin1 = IMG_Load("sprites/Login-FondoAlt.jpg");
	texturaFondoLogin1 = SDL_CreateTextureFromSurface(ren,surfaceFondoLogin1);

	rectFondo2source.x=0;rectFondo2source.y=0;rectFondo2source.w=640;rectFondo2source.h=394;
	rectFondo2dest.x=100;rectFondo2dest.y=0;rectFondo2dest.w=650;rectFondo2dest.h=400;
	surfaceFondoLogin2 = IMG_Load("sprites/FinalFight.png");
	texturaFondoLogin2 = SDL_CreateTextureFromSurface(ren,surfaceFondoLogin2);
	

	if(TTF_Init());

	font = TTF_OpenFont("sprites/Sansation-Bold.ttf", 30);
	fontInput = TTF_OpenFont("sprites/Sansation-Bold.ttf", 20);
	colorTextoInput.r = 200; colorTextoInput.g = 100; colorTextoInput.b = 100;
	colorTexto.r = 190; colorTexto.g = 60; colorTexto.b = 32;
	colorTextoAyuda.r = 170; colorTextoAyuda.g = 30; colorTextoAyuda.b = 10;
	colorTextoError.r = 220; colorTextoError.g = 0; colorTextoError.b = 0;

	surfaceTexto1 = TTF_RenderText_Solid(font, "Username", colorTexto);
	texturaTextoLogin1 = SDL_CreateTextureFromSurface(ren, surfaceTexto1);
	SDL_QueryTexture(texturaTextoLogin1,NULL,NULL,&width,&height);
	rectTexto1.x=100;rectTexto1.y=410;rectTexto1.w=width;rectTexto1.h=height;
	

	surfaceTexto2 = TTF_RenderText_Solid(font, "Password", colorTexto);
	texturaTextoLogin2 = SDL_CreateTextureFromSurface(ren, surfaceTexto2);
	SDL_QueryTexture(texturaTextoLogin2,NULL,NULL,&width,&height);
	rectTexto2.x=100;rectTexto2.y=515;rectTexto2.w=width;rectTexto2.h=height;
	
	surfaceTexto3 = TTF_RenderText_Solid(fontInput, "Use arrows to move", colorTextoAyuda);
	texturaTextoLogin3 = SDL_CreateTextureFromSurface(ren, surfaceTexto3);
	SDL_QueryTexture(texturaTextoLogin3,NULL,NULL,&width,&height);
	rectTexto3.x=600;rectTexto3.y=550;rectTexto3.w=width;rectTexto3.h=height;

	surfaceTexto4 = TTF_RenderText_Solid(fontInput, "Press Spacebar to login", colorTextoAyuda);
	texturaTextoLogin4 = SDL_CreateTextureFromSurface(ren, surfaceTexto4);
	SDL_QueryTexture(texturaTextoLogin4,NULL,NULL,&width,&height);
	rectTexto4.x=580;rectTexto4.y=575;rectTexto4.w=width;rectTexto4.h=height;

	surfaceTextoError = TTF_RenderText_Solid(fontInput, "Invalid username or password. Try again", colorTextoError);
	texturaTextoLoginError = SDL_CreateTextureFromSurface(ren, surfaceTextoError);
	SDL_QueryTexture(texturaTextoLoginError,NULL,NULL,&width,&height);
	rectTextoError.x=120;rectTextoError.y=570;rectTextoError.w=width;rectTextoError.h=height;

	surfaceTextoEsperando = TTF_RenderText_Solid(fontInput, "Waiting for other users", colorTextoError);
	texturaTextoEsperando = SDL_CreateTextureFromSurface(ren, surfaceTextoEsperando);
	SDL_QueryTexture(texturaTextoEsperando,NULL,NULL,&width,&height);
	rectTextoEsperando.x=120;rectTextoEsperando.y=570;rectTextoEsperando.w=width;rectTextoEsperando.h=height;

	SDL_StartTextInput();






	while(isLogginIn){

		SDL_PumpEvents();

		keystates = SDL_GetKeyboardState(NULL);

		while(SDL_PollEvent(&event)){


			if(event.type == SDL_QUIT ){
				exit(0);
				isLogginIn = false;
			}
			else if(event.type == SDL_TEXTINPUT){
				if(inputEnUsuario && textoIngresadoUsuario.length()<18){
					textoIngresadoUsuario += event.text.text;
				}
				else if(!inputEnUsuario && textoIngresadoContrasenia.length()<18){
					textoIngresadoContrasenia += event.text.text;
				}

				needToRender = true;

			}
			if(keystates[SDL_SCANCODE_DOWN]){
				inputEnUsuario = false;
				rectFlechaDest.y=495;
			}
			if(keystates[SDL_SCANCODE_UP]){
				inputEnUsuario = true;
				rectFlechaDest.y=390;
			}
			if(keystates[SDL_SCANCODE_SPACE]){
				isLogginIn = false;
				//textoIngresadoUsuario.clear();
				//textoIngresadoContrasenia.clear();
				//needToRender = true;
			}
			if(keystates[SDL_SCANCODE_BACKSPACE]){
				if(inputEnUsuario && textoIngresadoUsuario.length()>0){
					textoIngresadoUsuario.pop_back();
					needToRender = true;
				}
				else if(!inputEnUsuario &&textoIngresadoContrasenia.length()>0){
					textoIngresadoContrasenia.pop_back();
					needToRender = true;
				}
			}
		}
		
		if(needToRender){

			surfaceTextoInput1 = TTF_RenderText_Solid(fontInput, textoIngresadoUsuario.c_str(), colorTextoInput);
			texturaTextoInput1 = SDL_CreateTextureFromSurface(ren, surfaceTextoInput1);
			SDL_QueryTexture(texturaTextoInput1,NULL,NULL,&width,&height);
			rectTextoInput1.x=270;rectTextoInput1.y=415;rectTextoInput1.w=width;rectTextoInput1.h=height;

			surfaceTextoInput2 = TTF_RenderText_Solid(fontInput, textoIngresadoContrasenia.c_str(), colorTextoInput);
			texturaTextoInput2 = SDL_CreateTextureFromSurface(ren, surfaceTextoInput2);
			SDL_QueryTexture(texturaTextoInput2,NULL,NULL,&width,&height);
			rectTextoInput2.x=270;rectTextoInput2.y=520;rectTextoInput2.w=width;rectTextoInput2.h=height;

			needToRender = false;
		}

		SDL_RenderClear(ren);


		SDL_RenderCopy(ren, texturaFondoLogin1,&rectFondo1source,&rectFondo1dest);
		SDL_RenderCopy(ren, texturaFondoLogin2,&rectFondo2source,&rectFondo2dest);
		SDL_RenderCopy(ren, texturaTextoLogin1,NULL,&rectTexto1);
		SDL_RenderCopy(ren, texturaTextoLogin2,NULL,&rectTexto2);
		SDL_RenderCopy(ren, texturaTextoLogin3,NULL,&rectTexto3);
		SDL_RenderCopy(ren, texturaTextoLogin4,NULL,&rectTexto4);

		SDL_RenderCopy(ren, texturaFlecha,&rectFlechaSource,&rectFlechaDest);
		SDL_RenderCopy(ren, texturaTextoInput1,NULL,&rectTextoInput1);
		SDL_RenderCopy(ren, texturaTextoInput2,NULL,&rectTextoInput2);

		if(errorAlValidar){
			SDL_RenderCopy(ren, texturaTextoLoginError,NULL,&rectTextoError);
		}

/*		if(esperar){
			SDL_RenderCopy(ren, texturaTextoEsperando,NULL,&rectTextoEsperando);
			SDL_RenderPresent(ren);
			break;
		}*/


		SDL_RenderPresent(ren);
		

	}

	// quitar espacio del final
	textoIngresadoContrasenia[textoIngresadoContrasenia.find(' ')] = '\0';

	strcpy(credencialCliente->usuario, textoIngresadoUsuario.c_str());
	strcpy(credencialCliente->contrasenia, textoIngresadoContrasenia.c_str());


	SDL_StopTextInput();
	SDL_DestroyTexture(texturaFondoLogin1);
	SDL_DestroyTexture(texturaFondoLogin2);
	SDL_DestroyTexture(texturaFlecha);
	SDL_DestroyTexture(texturaTextoLogin1);
	SDL_DestroyTexture(texturaTextoLogin2);
	SDL_DestroyTexture(texturaTextoLogin3);
	SDL_DestroyTexture(texturaTextoLogin4);
	SDL_DestroyTexture(texturaTextoLoginError);
	SDL_DestroyTexture(texturaTextoEsperando);
	SDL_DestroyTexture(texturaTextoInput1);
	SDL_DestroyTexture(texturaTextoInput2);

	Logger::getInstance()->log(DEBUG, "DIBUJADOR OBTUVO CREDENCIALES: " + std::string(credencialCliente->usuario) + " - " + std::string(credencialCliente->contrasenia));

}

void Dibujador::mostrarPantallaEspera() {

	SDL_Surface* surfaceTextoEsperando;

	SDL_Color colorTextoError;

	int width, height;

	SDL_Texture* texturaTextoEsperando;

	SDL_Rect rectTextoEsperando;

	TTF_Font* fontInput;

	SDL_Texture* texturaFondoLogin1;
	SDL_Surface* surfaceFondoLogin1;
	SDL_Rect rectFondo1source;
	SDL_Rect rectFondo1dest;

	SDL_RenderClear(ren);

	rectFondo1source.x=0;rectFondo1source.y=0;rectFondo1source.w=620;rectFondo1source.h=390;
	rectFondo1dest.x=0;rectFondo1dest.y=0;rectFondo1dest.w=WINDOW_SIZE_HORIZONTAL;rectFondo1dest.h=WINDOW_SIZE_VERTICAL;
	surfaceFondoLogin1 = IMG_Load("sprites/Login-Fondo.jpg");
	texturaFondoLogin1 = SDL_CreateTextureFromSurface(ren,surfaceFondoLogin1);

	fontInput = TTF_OpenFont("sprites/Sansation-Bold.ttf", 30);
	colorTextoError.r = 220; colorTextoError.g = 0; colorTextoError.b = 0;


	surfaceTextoEsperando = TTF_RenderText_Solid(fontInput, "Waiting for other players...", colorTextoError);
	texturaTextoEsperando = SDL_CreateTextureFromSurface(ren, surfaceTextoEsperando);
	SDL_QueryTexture(texturaTextoEsperando,NULL,NULL,&width,&height);
	rectTextoEsperando.x=225;rectTextoEsperando.y=300;rectTextoEsperando.w=width;rectTextoEsperando.h=height;

	SDL_SetRenderDrawColor(ren, 25, 25,25, 200);

	SDL_Rect banner;
	banner.h=80;
	banner.w=WINDOW_SIZE_HORIZONTAL;
	banner.x= 0;
	banner.y= WINDOW_SIZE_VERTICAL / 2 - banner.h/4;

	SDL_RenderCopy(ren, texturaFondoLogin1,&rectFondo1source,&rectFondo1dest);

	SDL_RenderFillRect(ren, &banner);

	SDL_RenderCopy(ren, texturaTextoEsperando,NULL,&rectTextoEsperando);
	SDL_RenderPresent(ren);

}

Dibujador::~Dibujador(){
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_DestroyTexture(tex1);
	SDL_DestroyTexture(tex2);
	SDL_DestroyTexture(tex3);
	SDL_DestroyTexture(texCody[0]);
	SDL_DestroyTexture(texCody[1]);
	SDL_DestroyTexture(texCody[2]);
	SDL_DestroyTexture(texCody[3]);
	SDL_DestroyTexture(texEnemigo);
	SDL_DestroyTexture(texElemento);

}
