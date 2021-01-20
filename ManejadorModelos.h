#ifndef MANEJADORMODELOS_H
#define MANEJADORMODELOS_H

#include <map>
#include <string>

#include "Modelo.h"
class Modelo;

using namespace std;

///Sigue el patron Singleton
class ManejadorModelos{

    private:

        ManejadorModelos();
        static ManejadorModelos *instancia;
        map<string,Modelo*> modelos;

        string ruta;

    public:
        static ManejadorModelos* getInstancia();

        Modelo* getModelo(const char *modelo,bool optimizarVertices=true);

        void setRuta(string ruta);
        string getRuta();

        virtual ~ManejadorModelos();


};

#endif // MANEJADORMODELOS_H
