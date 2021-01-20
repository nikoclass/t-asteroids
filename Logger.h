#ifndef LOGGER_H
#define LOGGER_H

#define DO_DEBUG

#ifdef DO_DEBUG
    // Conveniente para iniciar el Log
    #define LOG_INIT(minLevel, logName, overwrite) Logger::getInstance()->Init(minLevel, logName, overwrite, __PRETTY_FUNCTION__, __LINE__);

    // Usar estas dos
    // An no se que quieras meter el nombre de la funcion a mano :)
    // Loguea el string pasado como parametro
    // Ej: "LOG(LogLevel::Info, "Llamando al tic de pepe")
    #define LOG(level,str) Logger::getInstance()->Log(level, __PRETTY_FUNCTION__, __LINE__, str);

    // Comportamiento parecido al prinf
    // Ej: LOG_MORE(LogLevel::Debug, "A pepe le gusta %s %d veces por dia", "orinar", 3)
    #define LOG_MORE(level, format, ...) Logger::getInstance()->Log(level, __PRETTY_FUNCTION__, __LINE__, format, __VA_ARGS__);

    // Largo maximo en cantidad de caracteres para loguear (con Variadic Function)
    // No se si deba cambiarlo
    #define LOG_MAX_BUFFER_SIZE 1024

#else
    #define LOG_INIT (minLevel, logName, overwrite)
    #define LOG(level,str)
    #define LOG_MORE(level, format, ...)
#endif

#ifdef DO_DEBUG

// Para usar secciones críticas
#include <windows.h>
#include <iostream>
#include <fstream>
using namespace std;

// No tenia ganas de meterlo como enumerado
// Ademas los enumerados me cagan con el cout (me meten un int creo)
class LogLevel
{
private:
    LogLevel(string level, short prio) : m_value(level), m_priority(prio){}
    string m_value;
    short  m_priority;
public:
    string getValue() const{
        return m_value;
    }
    short getPriority() const {
        return m_priority;
    }

    static LogLevel Debug; // Log de bajo nivel, meter parametros, salidas de funciones y otra info pertinente

    static LogLevel Info;
    static LogLevel Warning;
    static LogLevel Error;
    static LogLevel Fatal;
};
ostream& operator<<(ostream &, const LogLevel &);
bool operator>=(const LogLevel &,const LogLevel &);



class Logger // Singleton
{
private:
    static Logger* s_instance;
    Logger();
public:
    static Logger* getInstance();
    ~Logger();


private:
    // El archivo de log
    ofstream m_logStream;

    // La seccion critica para sincronizar
    CRITICAL_SECTION  m_lockLog;

    LogLevel m_minLevel;

    void logDateLevelFunction(const LogLevel & level, const string & functionName, int line);

public:
    void Init(const LogLevel & minLevel, const string & logName, bool overWrite, const string & funcName = " ", int line = 0 );
    void Log(const LogLevel & level, const string & functionName, int line, const string & logInfo);
    void Log(const LogLevel & level, const string & functionName, int line, char* format, ...);
};

#endif // #ifdef DO_DEBUG

#endif // LOGGER_H
