#ifndef TEXTO3D_H
#define TEXTO3D_H
class Texto3D;

#include "Dibujable.h"
#include "Multiobjeto3d.h"
#include "Objeto3d.h"


class Texto3D : public Dibujable{
    private:
        Multiobjeto3d *multiobjeto;
        string texto;
        string charset;
        Objeto3d *dummy; //para no devolver nulo cuando no hay caracter

    public:
        Texto3D(string texto="",string charset="charset3d.mdl");

        void cargarCharset(string charset);

        void setTexto(string texto="");

        string getTexto();

        Esfera getEsferaEnvolvente(); //override de dibujable

        Objeto3d* getCaracter(int num);

        void dibujar(); //override de dibujable

        virtual ~Texto3D();
};

#endif // TEXTO3D_H
