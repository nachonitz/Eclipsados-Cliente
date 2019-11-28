#include "Dibujador.h"
#include "Logger.h"
#include <string>

int Dibujador::obtenerIndiceBarra(int nivelEnergiaRestante){
	switch(nivelEnergiaRestante){
		case 100:
			return 0;
		case 80:
			return 1;
		case 60:
			return 2;
		case 40:
			return 3;
		case 20:
			return 4;
		case 0:
			return 5;
	}
	return -1;
}

void Dibujador::inicializar(std::vector<std::string> &nivel1, std::vector<std::string> &nivel2, std::vector<std::string> &sprites){
	if ( SDL_Init(SDL_INIT_VIDEO) != 0); // @suppress("Suspicious semicolon")
	//Logger::getInstance()->log(ERROR, SDL_GetError());

	//Logger::getInstance()->log(INFO, "Inicializando ventana...");
	if ( SDL_CreateWindowAndRenderer(WINDOW_SIZE_HORIZONTAL, WINDOW_SIZE_VERTICAL, 0, &win, &ren) ); // @suppress("Suspicious semicolon")
	//Logger::getInstance()->log(ERROR, SDL_GetError());
	if(TTF_Init());

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	SDL_SetWindowTitle(win, "Final Fight");

	setearTexturas(nivel1, nivel2, sprites);

	for(int i=0; i< MAX_CLIENTES; i++){
		caraCodysrc[i].x= 95*i; caraCodysrc[i].y= 0; caraCodysrc[i].w= 95; caraCodysrc[i].h= 95;
		carasCodydest[i].x= 200*i + 20; carasCodydest[i].y= 20; carasCodydest[i].w= 70; carasCodydest[i].h= 70;
		barrasEnergiadest[i].x= 200*i + 100; barrasEnergiadest[i].y= 30; barrasEnergiadest[i].w= 95; barrasEnergiadest[i].h= 25;
	}
	for(int i=0; i< MAX_NIVELES_ENERGIA;i++){
		barrasEnergiasrc[i].x= 0; barrasEnergiasrc[i].y= 200*i; barrasEnergiasrc[i].w= 900; barrasEnergiasrc[i].h= 200;
	}

	fontScore = TTF_OpenFont("sprites/Sansation-Bold.ttf", 20);
	colorTextoScore.r = 225; colorTextoScore.g = 225; colorTextoScore.b = 20;

	Logger::getInstance()->log(DEBUG, "Inicializado SDL");


}

void Dibujador::dibujar(struct informacionRec info, int ID, Sonido* reproductorMusica){
	//toDo Pasar de char* a las posiciones

	//frameStart=SDL_GetTicks();
	//static int lastTime;

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

	int scoresCopy [MAX_CLIENTES] = {0,0,0,0};

	for(int i=0; i< (info.cantJugadores); i++){
		scoresCopy[i] = info.scores[i];
	}


	for(int i=0; i< (info.cantJugadores); i++){
		SDL_RenderCopy(ren, texCarasCody, &caraCodysrc[i] , &carasCodydest[i]);
		//jugador obtener energia restante

		int indice = obtenerIndiceBarra(info.energia[i]);
		SDL_RenderCopy(ren, texBarrasEnergia, &barrasEnergiasrc[indice], &barrasEnergiadest[i] );
		//jugador obtener score

		std::string s = std::to_string(scoresCopy[i]);
		surfaceTextoScore = TTF_RenderText_Solid(fontScore, s.c_str(), colorTextoScore);
		texturaTextoScore[i] = SDL_CreateTextureFromSurface(ren, surfaceTextoScore);
		SDL_QueryTexture(texturaTextoScore[i],NULL,NULL,&width,&height);
		rectTextoScore[i].x=200*i + 100;rectTextoScore[i].y=55;rectTextoScore[i].w=width;rectTextoScore[i].h=height;
		SDL_RenderCopy(ren, texturaTextoScore[i],NULL,&rectTextoScore[i]);

	}


	// DIBUJAR ENEMIGOS
	for(int i = info.cantJugadores; i < (info.cantAnimados); i++){
		renderizableActual.textura = texEnemigo[info.animados[i].tipoEnemigo];

		renderizableActual.source = info.animados[i].src;
		reproductorMusica->reproducirSonidoEnemigoSegunSrc(renderizableActual.source);

		renderizableActual.destination = info.animados[i].dest;
		renderizableActual.flip = info.animados[i].flip;
		renderizableActual.soyJugador = false;
		renderizableActual.estaActivo = true;
		renderizableActual.idxJugador = info.animados[i].ID;
		renderizables.push_back(renderizableActual);
	}


	// DIBUJAR CODYS
	for(int i = 0; i < (info.cantJugadores); i++){

		switch(info.animados[i].elementoEnMano) {
		case ELEMENTO_MANO:
			renderizableActual.textura = texCody[i];
			break;
		case ELEMENTO_CUCHILLO:
			renderizableActual.textura = texCodyCuchillo[i];
			break;
		case ELEMENTO_CANIO:
			renderizableActual.textura = texCodyCanio[i];
			break;
		}

		renderizableActual.source = info.animados[i].src;
		reproductorMusica->reproducirSonidoJugadorSegunSrc(renderizableActual.source, info.animados[i].elementoEnMano);

		renderizableActual.destination = info.animados[i].dest;
		renderizableActual.flip = info.animados[i].flip;
		renderizableActual.soyJugador = true;
		renderizableActual.estaActivo = info.animados[i].estaActivo;
		renderizableActual.idxJugador = info.animados[i].ID;
		if(info.animados[i].dibujar){
			renderizablesJugadores.push_back(renderizableActual);
			renderizables.push_back(renderizableActual);
		}
	}


	// DIBUJAR ELEMENTOS
	for(int i = 0; i < (info.cantElementos); i++){
		renderizableActual.textura = texElemento;

		renderizableActual.source = info.elementos[i].src;
		reproductorMusica->reproducirSonidoElementoSegunSrc(renderizableActual.source);

		renderizableActual.destination = info.elementos[i].dest;
		renderizableActual.flip = SDL_FLIP_NONE;
		renderizableActual.soyJugador = false;
		renderizableActual.estaActivo = true;
		renderizableActual.idxJugador = -1;
		renderizables.push_back(renderizableActual);
	}

	std::sort(renderizables.begin(), renderizables.end());


	// OBTENGO EN RENDACTUAL EL QUE ME CORRESPONDE (EL MIO)
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

	texCodyCuchillo[0] = crearTexturaDesdeRuta(sprites.at(11).c_str());
	texCodyCuchillo[1] = crearTexturaDesdeRuta(sprites.at(12).c_str());
	texCodyCuchillo[2] = crearTexturaDesdeRuta(sprites.at(13).c_str());
	texCodyCuchillo[3] = crearTexturaDesdeRuta(sprites.at(14).c_str());

	texCodyCanio[0] = crearTexturaDesdeRuta(sprites.at(15).c_str());
	texCodyCanio[1] = crearTexturaDesdeRuta(sprites.at(16).c_str());
	texCodyCanio[2] = crearTexturaDesdeRuta(sprites.at(17).c_str());
	texCodyCanio[3] = crearTexturaDesdeRuta(sprites.at(18).c_str());

	texCodyInactivo = crearTexturaDesdeRuta(sprites.at(8).c_str());

	texEnemigo[0] = crearTexturaDesdeRuta(sprites.at(5).c_str());
	texEnemigo[1] = crearTexturaDesdeRuta(sprites.at(6).c_str());
	texEnemigo[2] = crearTexturaDesdeRuta(sprites.at(7).c_str());
	texElemento = crearTexturaDesdeRuta(sprites.at(4).c_str());

	texCarasCody = crearTexturaDesdeRuta(sprites.at(10).c_str());
	texBarrasEnergia = crearTexturaDesdeRuta(sprites.at(9).c_str());
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

	surfaceTexto4 = TTF_RenderText_Solid(fontInput, "Press ENTER to login", colorTextoAyuda);
	texturaTextoLogin4 = SDL_CreateTextureFromSurface(ren, surfaceTexto4);
	SDL_QueryTexture(texturaTextoLogin4,NULL,NULL,&width,&height);
	rectTexto4.x=600;rectTexto4.y=575;rectTexto4.w=width;rectTexto4.h=height;

	surfaceTextoError = TTF_RenderText_Solid(fontInput, "Username or password are either invalid or in use. Try again!", colorTextoError);
	texturaTextoLoginError = SDL_CreateTextureFromSurface(ren, surfaceTextoError);
	SDL_QueryTexture(texturaTextoLoginError,NULL,NULL,&width,&height);
	rectTextoError.x=20;rectTextoError.y=570;rectTextoError.w=width;rectTextoError.h=height;

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
			if(keystates[SDL_SCANCODE_RETURN]){
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

		SDL_SetRenderDrawColor(ren, 25, 25,25, 200);
		SDL_Rect banner1;
		banner1.h= 40;
		banner1.w= 190;
		banner1.x= 600;
		banner1.y= 557;
		SDL_RenderFillRect(ren, &banner1);

		SDL_Rect banner2;
		banner2.h= 15;
		banner2.w= 540;
		banner2.x= 20;
		banner2.y= 577;


		SDL_RenderCopy(ren, texturaTextoLogin1,NULL,&rectTexto1);
		SDL_RenderCopy(ren, texturaTextoLogin2,NULL,&rectTexto2);
		SDL_RenderCopy(ren, texturaTextoLogin3,NULL,&rectTexto3);
		SDL_RenderCopy(ren, texturaTextoLogin4,NULL,&rectTexto4);

		SDL_RenderCopy(ren, texturaFlecha,&rectFlechaSource,&rectFlechaDest);
		SDL_RenderCopy(ren, texturaTextoInput1,NULL,&rectTextoInput1);
		SDL_RenderCopy(ren, texturaTextoInput2,NULL,&rectTextoInput2);

		if(errorAlValidar){
			SDL_RenderFillRect(ren, &banner2);
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
	//textoIngresadoContrasenia[textoIngresadoContrasenia.find(' ')] = '\0';

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

void Dibujador::mostrarPantallaEspera(int  myID) {


	SDL_Color colorTextoError;

	int width, height;

	SDL_Surface* surfaceTextoEsperando;
	SDL_Texture* texturaTextoEsperando;
	SDL_Rect rectTextoEsperando;

	SDL_Surface* surfaceTextoPersonaje;
	SDL_Texture* texturaTextoPersonaje;
	SDL_Rect rectTextoPersonaje;

	TTF_Font* fontInput;

	SDL_Texture* texturaFondoLogin1;
	SDL_Surface* surfaceFondoLogin1;
	SDL_Rect rectFondo1source;
	SDL_Rect rectFondo1dest;

	SDL_Rect sourceCody;
	SDL_Rect destCody;

	SDL_RenderClear(ren);

	rectFondo1source.x=0;rectFondo1source.y=0;rectFondo1source.w=620;rectFondo1source.h=390;
	rectFondo1dest.x=0;rectFondo1dest.y=0;rectFondo1dest.w=WINDOW_SIZE_HORIZONTAL;rectFondo1dest.h=WINDOW_SIZE_VERTICAL;
	surfaceFondoLogin1 = IMG_Load("sprites/pantallaCarga.jpg");
	texturaFondoLogin1 = SDL_CreateTextureFromSurface(ren,surfaceFondoLogin1);

	fontInput = TTF_OpenFont("sprites/Sansation-Bold.ttf", 30);
	colorTextoError.r = 180; colorTextoError.g = 30; colorTextoError.b = 10;

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

	SDL_Rect banner2;
	banner2.h= 160;
	banner2.w= 450;
	banner2.x= 150;
	banner2.y= 420;

	sourceCody.x=0;sourceCody.y=0;sourceCody.w=230;sourceCody.h=280;
	destCody.x=420;destCody.y=400;destCody.w=180;destCody.h=180;

	surfaceTextoPersonaje = TTF_RenderText_Solid(fontInput, "Your character is : ", colorTextoError);
	texturaTextoPersonaje = SDL_CreateTextureFromSurface(ren, surfaceTextoPersonaje);
	SDL_QueryTexture(texturaTextoPersonaje,NULL,NULL,&width,&height);
	rectTextoPersonaje.x=180;rectTextoPersonaje.y=470;rectTextoPersonaje.w=width;rectTextoPersonaje.h=height;

	SDL_RenderCopy(ren, texturaFondoLogin1,&rectFondo1source,&rectFondo1dest);
	SDL_RenderFillRect(ren, &banner);
	SDL_RenderFillRect(ren, &banner2);
	SDL_RenderCopy(ren, texturaTextoEsperando,NULL,&rectTextoEsperando);
	SDL_RenderCopy(ren, texturaTextoPersonaje,NULL,&rectTextoPersonaje);

	switch(myID){
		case 0:
			SDL_RenderCopy(ren, texCody[0],&sourceCody,&destCody);
			break;
		case 1:
			SDL_RenderCopy(ren, texCody[1],&sourceCody,&destCody);;
			break;
		case 2:
			SDL_RenderCopy(ren, texCody[2],&sourceCody,&destCody);
			break;
		case 3:
			SDL_RenderCopy(ren, texCody[3],&sourceCody,&destCody);
			break;

	}

	SDL_RenderPresent(ren);

}

void Dibujador::mostrarPantallaErrorConTexto(std::string mensaje){
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
		surfaceFondoLogin1 = IMG_Load("sprites/asfalto.jpg");
		texturaFondoLogin1 = SDL_CreateTextureFromSurface(ren,surfaceFondoLogin1);

	fontInput = TTF_OpenFont("sprites/Sansation-Bold.ttf", 30);
	colorTextoError.r = 190; colorTextoError.g = 30; colorTextoError.b = 10;


	surfaceTextoEsperando = TTF_RenderText_Solid(fontInput, mensaje.c_str(), colorTextoError);
	texturaTextoEsperando = SDL_CreateTextureFromSurface(ren, surfaceTextoEsperando);
	SDL_QueryTexture(texturaTextoEsperando,NULL,NULL,&width,&height);
	rectTextoEsperando.x= WINDOW_SIZE_HORIZONTAL/2 - width/2;
	rectTextoEsperando.y=300;rectTextoEsperando.w=width;rectTextoEsperando.h=height;

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
	SDL_Delay(5000);
}

void Dibujador::mostrarPantallaScores(int scores[MAX_CLIENTES], int cantidadJugadores,bool finDelJuego){
	SDL_Surface* surfaceTextoEsperando;
	SDL_Color colorTextoError;
	SDL_Color colorPuntajes;

	int width, height;
	int maxTempScore;
	int orderedPlayers[MAX_CLIENTES];
	SDL_Texture* texturaTextoScores;
	SDL_Rect rectTextoScores;
	SDL_Texture* texturaPuntaje[MAX_CLIENTES];
	SDL_Rect rectPuntaje[MAX_CLIENTES];
	TTF_Font* fontInput;
	TTF_Font* fontScores;
	SDL_Texture* texturaFondoLogin1;
	SDL_Surface* surfaceFondoLogin1;
	SDL_Rect rectFondo1source;
	SDL_Rect rectFondo1dest;
	SDL_Texture* texturaFondoLogin2;
	SDL_Surface* surfaceFondoLogin2;
	SDL_Rect rectFondo2source;
	SDL_Rect rectFondo2dest;
	SDL_Texture* texturaFondoLogin3;
	SDL_Surface* surfaceFondoLogin3;
	SDL_Rect rectFondo3source;
	SDL_Rect rectFondo3dest;
	SDL_Rect carasCodyScoresDest[MAX_CLIENTES];
	SDL_Rect banner[MAX_CLIENTES];

	SDL_Texture* texPuestos;
	SDL_Surface* surfacePuestos;
	SDL_Rect puestosSrc[MAX_CLIENTES];
	SDL_Rect puestosDest[MAX_CLIENTES];

	SDL_RenderClear(ren);

	rectFondo2source.x=0;rectFondo2source.y=0;rectFondo2source.w=640;rectFondo2source.h=394;
	rectFondo2dest.x=50;rectFondo2dest.y=0;rectFondo2dest.w=450;rectFondo2dest.h=300;
	surfaceFondoLogin2 = IMG_Load("sprites/FinalFight.png");
	texturaFondoLogin2 = SDL_CreateTextureFromSurface(ren,surfaceFondoLogin2);

	rectFondo3source.x=0;rectFondo3source.y=0;rectFondo3source.w=720;rectFondo3source.h=720;
	rectFondo3dest.x=350;rectFondo3dest.y=70;rectFondo3dest.w=350;rectFondo3dest.h=350;
	surfaceFondoLogin3 = IMG_Load("sprites/scores.png");
	texturaFondoLogin3 = SDL_CreateTextureFromSurface(ren,surfaceFondoLogin3);

	rectFondo1source.x=0;rectFondo1source.y=100;rectFondo1source.w=1100;rectFondo1source.h=600;
	rectFondo1dest.x=0;rectFondo1dest.y=0;rectFondo1dest.w=WINDOW_SIZE_HORIZONTAL;rectFondo1dest.h=WINDOW_SIZE_VERTICAL;
	surfaceFondoLogin1 = IMG_Load("sprites/asfalto.jpg");
	texturaFondoLogin1 = SDL_CreateTextureFromSurface(ren,surfaceFondoLogin1);

	surfacePuestos = IMG_Load("sprites/puestos.png");
	texPuestos = SDL_CreateTextureFromSurface(ren,surfacePuestos);

	fontInput = TTF_OpenFont("sprites/Sansation-Bold.ttf", 45);
	fontScores = TTF_OpenFont("sprites/Sansation-Bold.ttf", 30);
	colorTextoError.r = 220; colorTextoError.g = 30; colorTextoError.b = 10;
	colorPuntajes.r = 250; colorPuntajes.g = 210; colorPuntajes.b = 1;

	surfaceTextoEsperando = TTF_RenderText_Solid(fontInput, "Scores", colorTextoError);
	texturaTextoScores = SDL_CreateTextureFromSurface(ren, surfaceTextoEsperando);
	SDL_QueryTexture(texturaTextoScores,NULL,NULL,&width,&height);
	rectTextoScores.x= WINDOW_SIZE_HORIZONTAL/2 - width/2;
	rectTextoScores.y=WINDOW_SIZE_VERTICAL/2 -50 ;rectTextoScores.w=width;rectTextoScores.h=height;

	maxTempScore = 0;
	int scoresCopy [MAX_CLIENTES] = {0,0,0,0};

	for(int i=0; i< (cantidadJugadores); i++){
		scoresCopy[i] = scores[i];
	}

	for(int i=0; i< (cantidadJugadores); i++){
		for(int j=0; j< (cantidadJugadores); j++){
			if(scoresCopy[j] >= maxTempScore){
				maxTempScore = scoresCopy[j];
				orderedPlayers[i] = j;
			}
		}
		maxTempScore = 0;
		scoresCopy[orderedPlayers[i]] = -1;
	}


	for(int i=0; i< (cantidadJugadores); i++){
		//Scores
		std::string s = std::to_string(scores[i]);
		surfaceTextoEsperando = TTF_RenderText_Solid(fontScores, s.c_str(), colorPuntajes);
		texturaPuntaje[i] = SDL_CreateTextureFromSurface(ren, surfaceTextoEsperando);
		rectPuntaje[i].x= 0; rectPuntaje[i].w=0;rectPuntaje[i].h=0;
		rectPuntaje[i].y=WINDOW_SIZE_VERTICAL/2 + 20 + 75*i + (4-cantidadJugadores)*30;
		//Logos Cody
		carasCodyScoresDest[i].x= 330; carasCodyScoresDest[i].w= 70; carasCodyScoresDest[i].h= 70;
		carasCodyScoresDest[i].y = WINDOW_SIZE_VERTICAL/2 + 75*i + (4-cantidadJugadores)*30;
		//Banners rojos
		banner[i].h= 70;banner[i].w= 330;banner[i].x= 230;
		banner[i].y= WINDOW_SIZE_VERTICAL/2 + 75*i + (4-cantidadJugadores)*30;
		//Puestos
		puestosSrc[i].x=115*i;puestosSrc[i].y=0;puestosSrc[i].w=115;puestosSrc[i].h=115;
		puestosDest[i].x=240;puestosDest[i].w=50;puestosDest[i].h=50;
		puestosDest[i].y=WINDOW_SIZE_VERTICAL/2 + 10 + 75*i + (4-cantidadJugadores)*30;
	}

	SDL_SetRenderDrawColor(ren, 170, 10,10, 150);

	SDL_RenderCopy(ren, texturaFondoLogin1,&rectFondo1source,&rectFondo1dest);
	SDL_RenderCopy(ren, texturaFondoLogin2,&rectFondo2source,&rectFondo2dest);
	SDL_RenderCopy(ren, texturaFondoLogin3,&rectFondo3source,&rectFondo3dest);


	for(int i=0; i< (cantidadJugadores); i++){

		SDL_RenderFillRect(ren, &banner[i]);
		SDL_RenderCopy(ren, texCarasCody, &caraCodysrc[orderedPlayers[i]] , &carasCodyScoresDest[i]);
		SDL_RenderCopy(ren, texPuestos, &puestosSrc[i] , &puestosDest[i]);
		SDL_QueryTexture(texturaPuntaje[orderedPlayers[i]],NULL,NULL,&width,&height);
		rectPuntaje[i].x = 475-width/2; rectPuntaje[i].w = width; rectPuntaje[i].h = height ;
		SDL_RenderCopy(ren, texturaPuntaje[orderedPlayers[i]],NULL,&rectPuntaje[i]);
	}

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
	SDL_DestroyTexture(texEnemigo[0]);
	SDL_DestroyTexture(texEnemigo[1]);
	SDL_DestroyTexture(texEnemigo[2]);
	SDL_DestroyTexture(texElemento);

}
