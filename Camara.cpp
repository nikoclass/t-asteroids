gl#include "Camara.h"

Camara::Camara(string nombre)
{
    posicion=Vector(0,0,0);
    up=Vector(0,1,0);
    direccion=Vector(0,0,-1); //este debe estar normalizado

    fov=70;
    znear=0.1;
    zfar=1000000;
    x=0;
    y=0;
    ancho=1;
    alto=1;

    this->nombre=nombre;
    this->escena=NULL;
}

Camara::~Camara(){
    if(escena)
        escena->removeCamara(this);
}

string Camara::getNombre(){
    return nombre;
}

void Camara::setId(int id){
    this->id=id;
}
int Camara::getId(){
    return id;
}

void Camara::setEscena(Escena* e){
    this->escena=e;
}
Escena* Camara::getEscena(){
    return escena;
}
void Camara::setParametros(const Vector& pos,const Vector& direccion,const Vector& up){
    this->posicion=pos;
    this->direccion=direccion.direccion();
    this->up=up;
}

void Camara::setUp(const Vector& up){
    this->up=up;
}
void Camara::setDireccion(const Vector& direccion){
    this->direccion=direccion.direccion();
}
void Camara::setPosicion(const Vector& pos){
    this->posicion=pos;
}

Vector Camara::getUp(){
    return this->up;
}
Vector Camara::getDireccion(){
    return this->direccion;
}
Vector Camara::getPosicion(){
    return this->posicion;
}

void Camara::mirarHacia(const Vector &pos){
    this->direccion=pos-this->posicion;
    this->direccion.normalizar();
}

void Camara::toGL(){
    int ww=glutGet(GLUT_WINDOW_WIDTH);
    int wh=glutGet(GLUT_WINDOW_HEIGHT);

    int x,y,ancho,alto;
    x=(int)(this->x*ww);
    y=(int)(this->y*wh);
    ancho=(int)(this->ancho*ww);
    alto=(int)(this->alto*wh);

    glViewport (x, y, ancho, alto);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(fov, float(ancho)/(float)(alto), znear, zfar);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    Vector hacia=posicion+direccion;
    gluLookAt(posicion[X],posicion[Y],posicion[Z],hacia[X],hacia[Y],hacia[Z],up[X],up[Y],up[Z]);
}

void Camara::setFov(float fov){
    this->fov=fov;
}
float Camara::getFov(){
    return fov;
}
void Camara::setZnearYZfar(float znear,float zfar){
    this->znear=znear;
    this->zfar=zfar;
}
float Camara::getZnear(){
    return this->znear;
}
float Camara::getZfar(){
    return this->zfar;
}
void Camara::setXY(float x,float y){
    this->x=x;
    this->y=y;
}
float Camara::getX(){
    return x;
}
float Camara::getY(){
    return y;
}
void Camara::setAnchoAlto(float ancho,float alto){
    this->ancho=ancho;
    this->alto=alto;
}
float Camara::getAncho(){
    return ancho;
}
float Camara::getAlto(){
    return alto;
}

Matriz4x4 Camara::getMatrizVista(){
    //Precondicion: direccion unitario

    Vector x,y;

    x=direccion^up;
    x.normalizar();

    y=x^direccion; //como son ortogonales y ambos normalizados, el resultado es unitario

    Matriz4x4 m;
    for(int i=0;i<3;i++){
        m(0,i)=x[i];
        m(1,i)=y[i];
        m(2,i)=direccion[i];
    }
    m(3,3)=1;

    return m*Matriz4x4::Traslacion(-posicion);
}
