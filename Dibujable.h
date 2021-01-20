#ifndef DIBUJABLE_H
#define DIBUJABLE_H

#include "GL.h"

#include "Matriz4x4.h"
#include "Esfera.h"

class Engine;
class Escena;

enum ModoTransparencia{Normal=1,Aditivo=2};

class Dibujable
{

    protected:
        Matriz4x4 transformacion;
        bool visible;
        float opacidad;
        Vector color;
        bool afectaVista;
        bool afectaLuces;
        bool afectaZbuffer;
        bool afectaDetrasCamara;
        bool dibujarEsferaEnvolvente;
        ModoTransparencia modoTransparencia;
        Escena *escena;

        float escala;
        string camara;
        bool detrasDeCamara();

    public:
        Dibujable();
        virtual ~Dibujable();

        void setVisible(bool visible);
        bool isVisible();
        void setOpacidad(float opacidad);
        float getOpacidad();
        void setAfectaVista(bool afecta);
        bool getAfectaVista();
        void setAfectaLuces(bool afecta);
        virtual bool getAfectaLuces();
        void setAfectaZbuffer(bool afecta);
        bool getAfectaZbuffer();
        void setAfectaDetrasCamara(bool afecta);
        bool getAfectaDetrasCamara();

        void setCamara(string c);
        string getCamara();

        void setDibujarEsferaEnvolvente(bool val);
        bool getDibujarEsferaEnvolvente();

        void setEscena(Escena* e);
        Escena* getEscena();

        void setModoTransparencia(ModoTransparencia modo);
        ModoTransparencia getModoTransparencia();

        virtual bool esTransparente();

        virtual Esfera getEsferaEnvolvente();

        void setColor(const Vector &c);
        void setColor(float r,float g, float b);
        Vector getColor();

        Matriz4x4 getTransformacion();
        void setTransformacion(const Matriz4x4 &t);

        void aplicarTransformacion(const Matriz4x4 &t);

        void identidad();
        void posicionar(Vector v);

        void trasladar(Vector v);
        void escalar(Vector v,bool concatenar=false);
        void escalar(float f,bool concatenar=false);
        void rotar(float angulo,Vector v,bool concatenar=false);
        void rotar(Vector origen,Vector destino,bool concatenar=false);

        void setEscala(float escala);
        float getEscala();

        Vector getPosicion();
        Matriz4x4 getRotaciones(bool normalizar=false);

        Vector getEjeX();
        Vector getEjeY();
        Vector getEjeZ();

        //multiplica la matriz actual de la pila de OpenGL por la matriz pasada
        static void matrizToGL(Matriz4x4 &m);
        //obtiene en la matriz pasada, el valor de la matriz actual de la pila de OpenGL
        static void glToMatriz(int tipo,Matriz4x4 &m);

        static Vector proyectar(Vector v);
        static Vector desProyectar(Vector w);

        void toGL();

        virtual void dibujar()=0; ///para override
};

#include "Escena.h"
#include "Engine.h"

#endif // DIBUJABLE_H
