#ifndef MULTIOBJETO3D_H
#define MULTIOBJETO3D_H
class Multiobjeto3d;

#include "Dibujable.h"
#include "Objeto3d.h"
#include "Modelo.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

class Multiobjeto3d : public Dibujable
{
    private:
        map<string,Dibujable*> objetos;

    public:
        Multiobjeto3d();
        Multiobjeto3d(const char *mdl,bool optimizarVertices=true);

        Dibujable* getObjeto(string nombre);
        void addObjeto(string nombre,Dibujable *objeto);

        void dibujar();

        bool esTransparente(); //override de dibujable
        bool getAfectaLuces(); //override de dibujable
        Esfera getEsferaEnvolvente(); //override de dibujable

        virtual ~Multiobjeto3d();


};

#endif // MULTIOBJETO3D_H
