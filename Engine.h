#ifndef ENGINE_H
#define ENGINE_H
//includes
//#include <dir.h>
#include <time.h>
#include <iostream>
#include <string>
using namespace std;


//Truquito de cadenizacion para debug
#define DEBUG(x) cout << #x " = " << x << endl
#define TRAZA cout << "Archivo: "<<__FILE__<<" Funcion: "<<__func__<<" Linea: "<<__LINE__<<endl

#include "Escena.h"

class Engine
{

    private:
        static Engine* instancia;
        Engine();

        static void resize(int width, int height);
        static void idle();
        static void display();

        Escena* escena;

        double dt;
        double tiempoTotal;
        double tiempoReal;
        double timescale;
        double timescaleDeseado; //esta, y dTimescale son para el efecto de timescale
        double dTimescale;      //cuando hay transicion con tiempo>0
        double maxFps;

        string ruta;

    public:
        static Engine* getInstancia();

        void inicializar(int argc, char* argv[],const char *nombre);

        void loop(void (*func)());

        void setEscena(Escena* escena);
        Escena* getEscena();

        string getRuta();

        double getDt();
        double getDtReal();

        double getTiempoTotal();
        double getTiempoReal();

        double getTimeScale();
        void setTimeScale(double timescale,double transicion=0);

        double getMaxFps();
        void setMaxFps(double max);

        double getFps();

        virtual ~Engine();

};

//referencias

/*
#include "ManejadorModelos.h"
#include "ManejadorTexturas.h"
#include "Objeto3d.h"
#include "Multiobjeto3d.h"
#include "Sprite.h"
#include "Texto2D.h"
#include "Texto3D.h"
#include "SistemaParticulas.h"
#include "Mouse.h"
#include "Teclado.h"
#include "Matematicas.h"
#include "Material.h"
*/


#endif // ENGINE_H
