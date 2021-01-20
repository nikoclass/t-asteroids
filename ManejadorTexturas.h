#ifndef MANEJADORTEXTURAS_H
#define MANEJADORTEXTURAS_H

#include "Textura.h"
class Textura;
#include <string>
#include <map>

using namespace std;

class ManejadorTexturas
{

    private:
        static ManejadorTexturas* instancia;
        ManejadorTexturas();
        map<string,Textura*> texturas;

        string ruta;

    public:
        static ManejadorTexturas* getInstancia();

        Textura* getTextura(const char *textura,bool forzarTransparencia=true);

        void setRuta(string ruta);
        string getRuta();

        virtual ~ManejadorTexturas();


};

#endif // MANEJADORTEXTURAS_H
