#ifndef MOUSE_H
#define MOUSE_H

#include "GL.h"

///Sigue el patron Singleton
class Mouse{
    private:
        static Mouse* instancia;

        int x;
        int y;
        int xAnterior;
        int yAnterior;
        int xClick;
        int yClick;
        int velX;
        int velY;
        //int estado;
        //int boton;
        bool _izquierdo;
        bool _medio;
        bool _derecho;

        bool dentro;

        Mouse();

        //Callbacks
        static void passiveFunc(int x,int y);
        static void activeFunc(int x,int y);
        static void mouseFunc(int button,int state,int x,int y);
        static void entryFunc(int state);

        //Utilitaria
        static void actualizar(int x,int y);

    public:
        static Mouse* getInstancia();
        int getX();
        int getY();

        void setPos(int x,int y);

        int getVelX();
        int getVelY();
        int getClickX();
        int getClickY();
        //int getEstado();
        //int getBoton();
        bool izquierdo();
        bool medio();
        bool derecho();
        bool ninguno();
        bool estaDentro();
        void setCursor(int cursor);
        void actualizar();

};

#endif

