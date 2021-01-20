#ifndef TEXTO2D_H
#define TEXTO2D_H


#include "Dibujable.h"
#include "ManejadorTexturas.h"

class Texto2D : public Dibujable{

    private:

        Textura *textura;
        string texto;
        bool mirarCamara;

        int idList;
        float centroX;
        float centroY;

    public:
        Texto2D(string texto="",float espacioHorizontal=0.7,float espacioVertical=0.7,string textura="charset.tga");

        void cargarTextura(string textura);

        void setTexto(string texto,float espacioHorizontal=0.7,float espacioVertical=0.7);
        string getTexto();

        void setMirarCamara(bool val);
        bool getMirarCamara();


        void dibujar();

        bool getAfectaLuces(); //override de Dibujable
        bool esTransparente(); //override de Dibujable

        virtual ~Texto2D();

};

#endif // TEXTO2D_H
