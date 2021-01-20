#ifndef MODELO_H
#define MODELO_H

#include "GL.h"
#include "ManejadorModelos.h"
class ManejadorModelos;
#include <vector>
#include <map>
#include <stdio.h>
#include <stdexcept>
#include <string>
#include "stdio.h"

#include "Vector.h"
#include "Malla.h"


using namespace std;

class Modelo
{
    private:
        vector<Malla*> vectorMallas;
        map<string,Malla*> mapMallas;
        string nombre;

        Esfera esferaEnvolvente;

    public:
        Modelo();
        Modelo(const char *modelo,bool optimizarVertices);

        void cargarModelo(const char* modelo,bool optimizarVertices);
        void destruirModelo();

        string getNombre();

        int getCantMallas();

        Malla* getMalla(string nombre);
        Malla* getMalla(int num);

        Esfera getEsferaEnvolvente();
        Esfera getEsferaEnvolvente(int numMalla);
        Esfera getEsferaEnvolvente(string nomMalla);

        void toGL();
        void toGL(int numMalla);
        void toGL(string nomMalla);

        virtual ~Modelo();

};

#endif // MODELO_H
