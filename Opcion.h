#ifndef OPCION_H_
#define OPCION_H_

#include "Texto2D.h"
#include <vector>

using namespace std;

class Opcion{
    private:
        int index;
        Texto2D* nombre;
        Texto2D* descripcion;
        vector<Opcion*> subopciones;
        float valor;
        TipoRenderizado tipo;

    public:
        Opcion(int i,Texto2D* nom,Texto2D* desc);
        virtual ~Opcion();

        //setters
        void setIndex(int value);
        void setNombre(Texto2D* value);
        void setDescripcion(Texto2D* value);
        void setSubOpciones(vector<Opcion*> value);
        void setValor(float value);
        void setTipo(TipoRenderizado value);

        //getters
        int getIndex();
        Texto2D* getNombre();
        Texto2D* getDescripcion();
        vector<Opcion*>& getSubOpciones();
        float getValor();
        TipoRenderizado getTipo();
};

#endif // OPCION_H_INCLUDED
