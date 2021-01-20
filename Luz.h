#ifndef LUZ_H
#define LUZ_H

#include "GL.h"


#include "Vector.h"
#include "math.h"

enum TipoLuz{
    focal,
    puntual
};

//Forward declaration
class Escena;
class LensFlare;

class Luz
{
    private:
        Vector pos;             //posicion de la luz
        Vector direccion;       //direccion a la que apunta si es focal
        Vector color;
        float distancia_max;    //dist 0 que ilumine todo (infinito) NO SE USA
        float atenuacion;
        float intensidad;
        float angulo;           //si es focal el angulo de apertura
        float cos_angulo;       //NO SE USA
        TipoLuz tipo;

        Escena *escena;

        LensFlare* lensFlare;

    public:
        Luz();
        virtual ~Luz();
        void setTipo(TipoLuz tipo);
        TipoLuz getTipo();

        //setters
        void setPos(const Vector &pos);
        void setDireccion(const Vector &direccion);
        void setColor(const Vector &color);
        void setDistanciaMaxima(float distancia);
        void setIntensidad(float intensidad);
        void setAngulo(float angulo);

        void setLensFlares(bool val);
        bool getLensFlares();

        //getters
        Vector getPos();
        Vector getDireccion();
        Vector getColor();
        float getDistanciaMaxima();
        float getIntensidad();
        float getAngulo();
        float getCosAngulo();

        void setEscena(Escena* e);
        Escena* getEscena();

        void apagar();
        void encender(float intensidad);
        void toGL(int identificador);
        void dibujarLensFlares();
};

#include "Escena.h"
#include "LensFlare.h"

#endif // LUZ_H
