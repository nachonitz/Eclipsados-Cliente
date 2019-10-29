/*
 * ParserXML.cpp
 *
 *  Created on: Sep 17, 2019
 *      Author: franco
 */

#include "ParserXML.h"

using namespace tinyxml2;

/*
Hay distintos tipos de errores:
	- no se puede abrir el archivo custom
	- no existe alguna seccion del xml (FirstChildElement == null)
	- el valor no es del tipo correcto (se esperaba int y dio bool,string,etc)
*/

ParserXML::ParserXML(std::string rutaConfig){

	pConfig = new XMLDocument;

	XMLError eArchivo = pConfig->LoadFile(rutaConfig.c_str());

	//TODO: y si falla en abrir el default?
	XMLError eDefault = configDefault.LoadFile("xmlClienteDefault.xml");

	if (eArchivo != XML_SUCCESS) {

		//TODO: necesitara proveer mas informacion?
		std::string lineaError = pConfig->ErrorLineNum() ? ". Linea de error: " + std::to_string(pConfig->ErrorLineNum()) : ". Linea desconocida";

		Logger::getInstance()->log(ERROR, "Archivo XML personalizado no encontrado o corrupto. Error del tipo: " + std::string(pConfig->ErrorName()) +
									lineaError + ". Se utilizara el defecto.");

		delete pConfig;

		//usar default, en vez de custom
		pConfig = &configDefault;
	}

	pHandler = new XMLHandle(pConfig);

}

void ParserXML::setDebugLevelFromDefault() {
	const char *debugLevel =
			configDefault.FirstChildElement("configuracionServidor")->FirstChildElement(
					"debugLevel")->GetText();
	if (strcmp(debugLevel, "DEBUG") == 0) {
		Logger::getInstance()->setLevel(DEBUG);
	} else if (strcmp(debugLevel, "INFO") == 0) {
		Logger::getInstance()->setLevel(INFO);
	} else if (strcmp(debugLevel, "ERROR") == 0) {
		Logger::getInstance()->setLevel(ERROR);
	}

	Logger::getInstance()->log(ERROR,
			"Opcion invalida o inexistente en <debugLevel>, utilizando la indicada por xmlDefault...");
}


void ParserXML::parsearConfig(std::vector<std::string> &nivel1, std::vector<std::string> &nivel2, std::vector<std::string> &sprites) {

	Logger::getInstance()->log(INFO, "Iniciando parseo de XML.");

	if (pConfig == &configDefault) {
		//TODO: estoy parseando el default, no debo chequear errores
		//parsearConfigDefault(cantEnemigos, cantCuchillos, cantCajas, cantCanios, cantBarriles, nivel1, nivel2);
	}

	XMLElement* pEscenarioDEFAULT = configDefault.FirstChildElement("configuracionCliente")->FirstChildElement("escenario");

	XMLHandle hRaiz = pHandler->FirstChildElement("configuracionCliente");

	XMLElement* elementDebug;

	Logger::getInstance()->log(DEBUG, "Parseando debugLevel...");

	if ((elementDebug = hRaiz.FirstChildElement("debugLevel").ToElement()) != nullptr) {

		const char* debugLevel = elementDebug->GetText();

		if (strcmp(debugLevel, "DEBUG") == 0) {
			Logger::getInstance()->setLevel(DEBUG);
			Logger::getInstance()->log(INFO, "Cambiando debugLevel al indicado por xml: DEBUG");

		}
		else if (strcmp(debugLevel, "INFO") == 0) {
			Logger::getInstance()->setLevel(INFO);
			Logger::getInstance()->log(INFO, "Cambiando debugLevel al indicado por xml: INFO");

		}
		else if (strcmp(debugLevel, "ERROR") == 0) {
			Logger::getInstance()->setLevel(ERROR);
			Logger::getInstance()->log(INFO, "Cambiando debugLevel al indicado por xml: ERROR");
		}
		else {
			setDebugLevelFromDefault();
		}
	}

	else {
		setDebugLevelFromDefault();
	}
	XMLHandle hEscenario = hRaiz.FirstChildElement("escenario");

	XMLHandle hNiveles = hEscenario.FirstChildElement("niveles");

	XMLElement* eNivel1 = hNiveles.FirstChildElement("nivel1").ToElement();

	Logger::getInstance()->log(DEBUG, "Parseando capas para nivel 1...");

	if (eNivel1 != nullptr)
		asignarLista(nivel1, eNivel1, "fondo");

	if (nivel1.size() != CANT_CAPAS) {
		//TODO: no existe <nivel1>, o se pasaron fondos de menos o de mas! avisar por log
		Logger::getInstance()->log(ERROR, "Cantidad insuficiente de items en <nivel1> (se esperaban " + std::to_string(CANT_CAPAS) + "), o etiqueta no existente. Se utilizaran fondos predeterminados.");
		nivel1.clear();
		asignarLista(nivel1, pEscenarioDEFAULT->FirstChildElement("niveles")->FirstChildElement("nivel1"), "fondo");
	}


	XMLElement* eNivel2 = hNiveles.FirstChildElement("nivel2").ToElement();

	Logger::getInstance()->log(DEBUG, "Parseando capas para nivel 2...");

	if (eNivel2 != nullptr)
		asignarLista(nivel2, eNivel2, "fondo");

	if (nivel2.size() != CANT_CAPAS) {
		//TODO: no existe <nivel2>, o se pasaron fondos de menos o de mas! avisar por log
		Logger::getInstance()->log(ERROR, "Cantidad insuficiente de items en <nivel2> (se esperaban " + std::to_string(CANT_CAPAS) + "), o etiqueta no existente. Se utilizaran fondos predeterminados.");
		nivel2.clear();
		asignarLista(nivel2, pEscenarioDEFAULT->FirstChildElement("niveles")->FirstChildElement("nivel2"), "fondo");
	}


	XMLElement* eSprites = hRaiz.FirstChildElement("sprites").ToElement();

	Logger::getInstance()->log(DEBUG, "Parseando sprites...");

	if (eSprites != nullptr) {
		//asume que los sprites estan en orden! ni le da bola a las tags
		asignarLista(sprites, eSprites, "sprite");
	}
	if (sprites.size() != CANT_SPRITES) {
		//TODO: no existe <sprites>, o se pasaron sprites de menos o de mas! avisar por log
		Logger::getInstance()->log(ERROR, "Cantidad insuficiente de items en <sprites> (se esperaban " + std::to_string(CANT_SPRITES) + "), o etiqueta no existente. Se utilizaran sprites predeterminados.");
		sprites.clear();
		asignarLista(sprites, configDefault.FirstChildElement("configuracion")->FirstChildElement("sprites"), "sprite");
	}

	Logger::getInstance()->log(INFO, "Finalizado el parseo del XML.");

}

ParserXML::~ParserXML() {

	delete pHandler;

	if (pConfig != &configDefault)
		delete pConfig;

}

void ParserXML::asignarValor(int* variable, const char* nombre, XMLHandle base, XMLElement* backup) {

	XMLElement* pElemento = base.FirstChildElement(nombre).ToElement();

	XMLError error = XML_SUCCESS;

	if (pElemento)
		error = pElemento->QueryIntText(variable);

	if (!pElemento || error != XML_SUCCESS) {

		Logger::getInstance()->log(ERROR, "Etiqueta <" + std::string(nombre) + "> inexistente o con datos erroneos/nulos. Utilizando valor predeterminado.");
		backup->FirstChildElement(nombre)->QueryIntText(variable);
	}


}

void ParserXML::asignarLista(std::vector<std::string> &lista, XMLElement* eBase, const char * nombreItems) {
	//asume que los elementos estan en orden
  Logger::getInstance()->log(DEBUG, "Leyendo rutas: " + std::string(nombreItems) + "...");
	for(XMLElement* e = eBase->FirstChildElement(nombreItems); e != NULL; e = e->NextSiblingElement(nombreItems)) {
      Logger::getInstance()->log(DEBUG, "Sprite a cargar: " + std::string(e->GetText()));
	    lista.push_back(e->GetText());
	}

	Logger::getInstance()->log(DEBUG, "Cargada lista de " + std::string(nombreItems) + " con longitud " + std::to_string(lista.size()));
}
