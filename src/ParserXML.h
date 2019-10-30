/*
 * ParserXML.h
 *
 *  Created on: Sep 17, 2019
 *      Author: franco
 */



#ifndef SRC_PARSERXML_H_
#define SRC_PARSERXML_H_

#define CANT_CAPAS 3
#define CANT_SPRITES 9

#include <string>

#include <vector>

#include "tinyxml2/tinyxml2.h"

#include "Logger.h"

class ParserXML {
public:
	ParserXML(std::string rutaConfig);

	void parsearConfig(std::vector<std::string> &nivel1, std::vector<std::string> &nivel2, std::vector<std::string> &sprites);

	void parsearConfigDefault(int *cantEnemigos, int *cantCuchillos, int *cantCajas, int *cantCanios, int *cantBarriles);

	virtual ~ParserXML();

	void asignarValor(int* variable, const char* nombre, tinyxml2::XMLHandle base, tinyxml2::XMLElement* backup);

	void asignarLista(std::vector<std::string> &lista, tinyxml2::XMLElement* eBase, const char * nombreItems);


	void setDebugLevelFromDefault();


private:
	tinyxml2::XMLDocument configDefault;

	tinyxml2::XMLDocument* pConfig;

	tinyxml2::XMLHandle* pHandler;


};

#endif /* SRC_PARSERXML_H_ */
