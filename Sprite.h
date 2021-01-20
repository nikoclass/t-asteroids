#ifndef SPRITE_H
#define SPRITE_H

#include "Dibujable.h"
#include "ManejadorTexturas.h"


///Posiblemente interese luego setearle el color...
class Sprite : public Dibujable{

    private:

        Textura *textura;
        float x; //Las coordenadas de textura
        float y;
        float ancho;
        float alto;

        ModoTextura modoTextura;

        bool mirarCamara;
        bool rotable;

    public:
        Sprite();
        Sprite(const char* textura,bool forzarTransparencia=true);

        void cargarTextura(const char* textura,bool forzarTransparencia=true);

        void setXY(float x, float y);
        void setAnchoAlto(float ancho, float alto);

        float getX();
        float getY();
        float getAncho();
        float getAlto();

        void setMirarCamara(bool val);
        bool getMirarCamara();

        void setRotable(bool val);
        bool isRotable();

        void setModoTextura(ModoTextura modo);
        ModoTextura getModoTextura();

        void dibujar();

        bool getAfectaLuces(); //override de Dibujable
        bool esTransparente(); //override de Dibujable

        virtual ~Sprite();

};

#endif // SPRITE_H
