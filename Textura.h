#ifndef TEXTURA_H
#define TEXTURA_H
class Textura;

#include "GL.h"
#include "ManejadorTexturas.h"
#include "Thread.h"


class ManejadorTexturas;
class Engine;

enum ModoTextura{Reemplazar=GL_REPLACE,Decal=GL_DECAL, Modular=GL_MODULATE,Blend=GL_BLEND};

class Textura
{
    private:
        int bpp;  // bpp significa bits per pixel (bits por punto) es la calidad en palabras sensillas
        int largo;  // Largo de la textura
        int ancho;  // Ancho de la textura
        GLuint id;  // ID de la textura, es como su nombre para opengl
        string nombre;

        GLubyte *dibujo; //buffer

        int estado;

    public:
        Textura(const char *ruta,bool forzarTransparencia=true);
        virtual ~Textura();

        void cargarTGA(const char *ruta,bool forzarTransparencia);
        string getNombre();
        int getBpp();
        int getLargo();
        int getAncho();

        void toGL();

};

///Pensar bien el tema de los llamados a OpenGL con los threads ya que al parecer no dan buenos resultados
class CargadorTexturaAsincrono: public Thread{
    public:
        CargadorTexturaAsincrono(Textura* t,const char *ruta,bool forzar):ruta_(ruta){
            this->t=t;
            this->forzarTransparencia=forzar;
        }

        void run(){
            t->cargarTGA(ruta_,forzarTransparencia);
        }

    private:
        Textura* t;
        const char* ruta_;
        bool forzarTransparencia;
};



#include "Engine.h"

#endif // TEXTURA_H
