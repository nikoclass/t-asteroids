#ifndef CAMARA_H
#define CAMARA_H

#include "GL.h"

//#include "Vector.h"
#include "Matriz4x4.h"
#include <string>

using namespace std;

//Forward declaration
class Escena;

class Camara
{
    private:

        Vector posicion;
        Vector direccion;
        Vector up;

        Matriz4x4 matrizVista;

        float fov;
        float znear;
        float zfar;
        float x;
        float y;
        float ancho;
        float alto;

        string nombre;
        int id;

        Escena* escena;

    public:
        Camara(string nombre="");
        ~Camara();

        string getNombre();
        void setId(int id);
        int getId();

        void setEscena(Escena* e);
        Escena* getEscena();

        void setParametros(const Vector& pos,const Vector& direccion,const Vector& up);

        void setUp(const Vector& up);
        void setDireccion(const Vector& direccion);
        void setPosicion(const Vector& pos);

        void setFov(float fov);
        float getFov();

        void setZnearYZfar(float znear,float zfar);
        float getZnear();
        float getZfar();

        void setXY(float x,float y);
        float getX();
        float getY();

        void setAnchoAlto(float ancho,float alto);
        float getAncho();
        float getAlto();

        Vector getUp();
        Vector getDireccion();
        Vector getPosicion();

        void mirarHacia(const Vector &pos);

        void toGL();

        Matriz4x4 getMatrizVista();
};

#include "Escena.h"

#endif // CAMARA_H
