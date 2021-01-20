#include "Mouse.h"

Mouse* Mouse::instancia=NULL;

Mouse* Mouse::getInstancia(){
    if (instancia==NULL)
        instancia=new Mouse;
    return instancia;
}

Mouse::Mouse(){
    this->x=0;
    this->y=0;
    this->xClick=0;
    this->yClick=0;
    this->velX=0;
    this->velY=0;
    //this->estado=GLUT_UP;
    this->_izquierdo=false;
    this->_medio=false;
    this->_derecho=false;

    this->dentro=false;

    //Registro de las funciones callback
    glutPassiveMotionFunc(this->passiveFunc);
    glutMotionFunc(this->activeFunc);
    glutMouseFunc(this->mouseFunc);
    glutEntryFunc(this->entryFunc);
}

int Mouse::getX(){
    return this->x;
}
int Mouse::getY(){
    return this->y;
}

void Mouse::setPos(int x,int y){
    glutWarpPointer(x,y);
    this->x=x;
    this->y=y;
}

int Mouse::getVelX(){
    /*if(this->x<=0 || this->x>=glutGet(GLUT_WINDOW_WIDTH))
        return 0;*/
    //return this->x-this->xAnterior;
    return this->velX;
}
int Mouse::getVelY(){
    /*if(this->y<=0 || this->y>=glutGet(GLUT_WINDOW_HEIGHT))
        return 0;*/
    //return this->y-this->yAnterior;
    return this->velY;
}
int Mouse::getClickX(){
    return this->xClick;
}
int Mouse::getClickY(){
    return this->yClick;
}

/*int Mouse::getEstado(){
    return this->estado;
}
*/
/*int Mouse::getBoton(){
    return this->boton;
}
*/

void Mouse::actualizar(int x,int y){
    Mouse::instancia->xAnterior=Mouse::instancia->x;
    Mouse::instancia->yAnterior=Mouse::instancia->y;
    Mouse::instancia->x=x;
    Mouse::instancia->y=y;
    Mouse::instancia->velX+=instancia->x - instancia->xAnterior;
    Mouse::instancia->velY+=instancia->y - instancia->yAnterior;
}

void Mouse::passiveFunc(int x,int y){
    Mouse::actualizar(x,y);
}
void Mouse::activeFunc(int x,int y){
    Mouse::actualizar(x,y);
}
void Mouse::mouseFunc(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON)
        Mouse::instancia->_izquierdo=(state==GLUT_DOWN);
    else if(button==GLUT_MIDDLE_BUTTON)
        Mouse::instancia->_medio=(state==GLUT_DOWN);
    else if(button==GLUT_RIGHT_BUTTON)
        Mouse::instancia->_derecho=(state==GLUT_DOWN);

    Mouse::actualizar(x,y);
}

void Mouse::entryFunc(int state){
    Mouse::instancia->dentro=state==GLUT_ENTERED;
}

bool Mouse::izquierdo(){
    return this->_izquierdo;
}

bool Mouse::medio(){
    return this->_medio;
}
bool Mouse::derecho(){
    return this->_derecho;
}
bool Mouse::ninguno(){
    return !(this->_izquierdo || this->_medio || this->_derecho);
}

bool Mouse::estaDentro(){
    return this->dentro;
}

void Mouse::setCursor(int cursor){
    glutSetCursor(cursor);
}

//Este invento es por una deficiencia de la actualizacion
//de eventos del sistema de ventanas
void Mouse::actualizar(){
    this->xAnterior=this->x;
    this->yAnterior=this->y;
    Mouse::instancia->velX=0;
    Mouse::instancia->velY=0;
}
