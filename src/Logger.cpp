#include <ctime>
#include <iostream>
#include <fstream>
#include "Logger.h"

using namespace std;

ofstream logFile;
Logger* Logger::instance = nullptr;

Logger* Logger::getInstance() {

    if (instance == nullptr)
        instance = new Logger;

    return instance;
}

void Logger::setLevel(LogLevel level) {

    Errorlevel = level;
}

void Logger::createLogFile() {

    string logfilename = "logs/logfile_" + datetime();
    logFile.open(logfilename, ios::out);

    if (Errorlevel >= INFO)
        logFile << datetime() << " - [" << LogLevelNames[INFO] << "] - "<< "LOGGER INICIADO: " << LogLevelNames[Errorlevel] << std::endl;
}

Logger::~Logger() {

    if (Errorlevel >= INFO)
        logFile << datetime() << " - [" << LogLevelNames[INFO] << "] - " << "LOGGER DESTRUIDO" << std::endl;

    logFile.close();
}

void Logger::log(LogLevel level, string message) {

    if (Errorlevel >= level)
        logFile << datetime() << " - [" << LogLevelNames[level] << "] - " << message << std::endl;
 }

string Logger::datetime() {

    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%Y-%m-%dT%H-%M-%S",timeinfo);
    return std::string(buffer);
}
