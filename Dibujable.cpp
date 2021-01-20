#include "Dibujable.h"

Dibujable::Dibujable(){
    transformacion=Matriz4x4::Identidad();
    color=Vector(1,1,1);
    visible=true;
    opacidad=1; //100% opaco
    afectaVista=true;
    afectaLuces=true;
    afectaZbuffer=true;
    afectaDetrasCamara=true;
    modoTransparencia=Normal;
    escena=NULL;
    dibujarEsferaEnvolvente=false;
    camara="";
    escala=1;
}
Dibujable::~Dibujable(){
    if(escena)
        escena->removeDibujable(this);
}

void Dibujable::setVisible(bool visible){
    this->visible=visible;
}
bool Dibujable::isVisible(){
    return visible && opacidad>0 && (camara=="" || camara==Engine::getInstancia()->getEscena()->getCamaraActual()->getNombre());
}
void Dibujable::setOpacidad(float opacidad){
    this->opacidad=opacidad;
}
float Dibujable::getOpacidad(){
    return opacidad;
}

void Dibujable::setColor(const Vector &c){
    this->color=c;
}

void Dibujable::setColor(float r,float g, float b){
    this->color=Vector(r,g,b);
}

Vector Dibujable::getColor(){
    return this->color;
}

void Dibujable::setAfectaVista(bool afecta){
    afectaVista=afecta;
}
bool Dibujable::getAfectaVista(){
    return afectaVista;
}
void Dibujable::setAfectaLuces(bool afecta){
    afectaLuces=afecta;
}
bool Dibujable::getAfectaLuces(){
    return afectaLuces;
}
void Dibujable::setAfectaZbuffer(bool afecta){
    afectaZbuffer=afecta;
}
bool Dibujable::getAfectaZbuffer(){
    return afectaZbuffer;
}
void Dibujable::setAfectaDetrasCamara(bool afecta){
    afectaDetrasCamara=afecta;
}
bool Dibujable::getAfectaDetrasCamara(){
    return afectaDetrasCamara;
}

void Dibujable::setEscena(Escena* e){
    this->escena=e;
}
Escena* Dibujable::getEscena(){
    return escena;
}

void Dibujable::setCamara(string c){
    this->camara=c;
}
string Dibujable::getCamara(){
    return this->camara;
}

void Dibujable::setModoTransparencia(ModoTransparencia modo){
    modoTransparencia=modo;
}
ModoTransparencia Dibujable::getModoTransparencia(){
    return modoTransparencia;
}

Matriz4x4 Dibujable::getTransformacion(){
    return transformacion;
}
void Dibujable::setTransformacion(const Matriz4x4 &t){
    transformacion=t;
}

void Dibujable::aplicarTransformacion(const Matriz4x4 &t){
    transformacion=t*transformacion;
}

void Dibujable::identidad(){
    transformacion=Matriz4x4::Identidad();
}
void Dibujable::posicionar(Vector v){
    transformacion(X,3)=v[X];
    transformacion(Y,3)=v[Y];
    transformacion(Z,3)=v[Z];
}

void Dibujable::trasladar(Vector v){
    transformacion(X,3)+=v[X];
    transformacion(Y,3)+=v[Y];
    transformacion(Z,3)+=v[Z];
}
void Dibujable::escalar(Vector v,bool concatenar){
    if(concatenar){
        transformacion=Matriz4x4::Escalado(v)*transformacion;
    }else{
        Vector pos=getPosicion();
        posicionar(Vector(0,0,0));
        transformacion=Matriz4x4::Escalado(v)*transformacion;
        posicionar(pos);
    }
}
void Dibujable::escalar(float f,bool concatenar){
    if(concatenar){
        transformacion=Matriz4x4::Escalado(f,f,f)*transformacion;
    }else{
        Vector pos=getPosicion();
        posicionar(Vector(0,0,0));
        transformacion=Matriz4x4::Escalado(f,f,f)*transformacion;
        posicionar(pos);
    }
}
void Dibujable::rotar(float angulo,Vector v,bool concatenar){
    if(concatenar){
        transformacion=Matriz4x4::Rotacion(angulo,v)*transformacion;
    }else{
        Vector pos=getPosicion();

        transformacion=Matriz4x4::Rotacion(angulo,v)*getRotaciones();

        transformacion(X,3)=pos[X];
        transformacion(Y,3)=pos[Y];
        transformacion(Z,3)=pos[Z];
    }
}

void Dibujable::rotar(Vector origen,Vector destino,bool concatenar){
    origen.normalizar();
    destino.normalizar();

    Vector r=origen^destino;
    float cos=origen*destino;

    if(cos>0)
        rotar(asin(r.norma()),r,concatenar);
    else if(cos<0)
        rotar(PI-asin(r.norma()),r);
    else
        rotar(PI/2,r);
}

Vector Dibujable::getPosicion(){
    return Vector(transformacion(X,3),transformacion(Y,3),transformacion(Z,3));
}
Matriz4x4 Dibujable::getRotaciones(bool normalizar){
    Matriz4x4 m=transformacion;
    m(X,3)=m(Y,3)=m(Z,3)=0;

    if(normalizar){ //Tengo que "normalizar" la matriz de rotacion
        Vector x(m(X,X),m(X,Y),m(X,Z));
        Vector y(m(Y,X),m(Y,Y),m(Y,Z));
        Vector z(m(Z,X),m(Z,Y),m(Z,Z));

        x.normalizar();
        y.normalizar();
        z.normalizar();

        m(X,X)=x[X];m(X,Y)=x[Y];m(X,Z)=x[Z];
        m(Y,X)=y[X];m(Y,Y)=y[Y];m(Y,Z)=y[Z];
        m(Z,X)=z[X];m(Z,Y)=z[Y];m(Z,Z)=z[Z];
    }

    return m;
}

bool Dibujable::esTransparente(){
    return (opacidad<1);
}


Esfera Dibujable::getEsferaEnvolvente(){
    return Esfera();
}

void Dibujable::setDibujarEsferaEnvolvente(bool val){
    this->dibujarEsferaEnvolvente=val;
}
bool Dibujable::getDibujarEsferaEnvolvente(){
    return this->dibujarEsferaEnvolvente;
}

Vector Dibujable::getEjeX(){
    Vector res(transformacion(X,X),transformacion(Y,X),transformacion(Z,X));
    return res.direccion();
}
Vector Dibujable::getEjeY(){
    Vector res(transformacion(X,Y),transformacion(Y,Y),transformacion(Z,Y));
    return res.direccion();
}

Vector Dibujable::getEjeZ(){
    Vector res(transformacion(X,Z),transformacion(Y,Z),transformacion(Z,Z));
    return res.direccion();
}

void Dibujable::setEscala(float escala){
    this->escalar(escala/this->escala);
    this->escala=escala;
}
float Dibujable::getEscala(){
    return this->escala;
}

bool Dibujable::detrasDeCamara(){
    Camara *cam=Engine::getInstancia()->getEscena()->getCamaraActual();
    Vector dir=this->getPosicion()-cam->getPosicion();
    Vector dirCamara=cam->getDireccion();

    return dir*dirCamara<0;
}

void Dibujable::toGL(){
    if(!isVisible()) return;

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

        if(!afectaVista){
            glLoadIdentity();
        }else{ //Le afecta la transformacion de camara
            if(afectaDetrasCamara && detrasDeCamara()){
                glPopMatrix();
                return;
            }
        }

        if(dibujarEsferaEnvolvente){
            glPushMatrix();
                Esfera e=getEsferaEnvolvente();
                glTranslatef(e.getCentro()[X],e.getCentro()[Y],e.getCentro()[Z]);
                glColor4f(1,1,1,1);
                glDisable(GL_LIGHTING);
                glutWireSphere(e.getRadio(),15,15);
            glPopMatrix();
        }

        if(afectaLuces)
            glEnable(GL_LIGHTING);
        else
            glDisable(GL_LIGHTING);


        if(afectaZbuffer)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);


        if(modoTransparencia==Normal)
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        else //Es aditivo
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);


        glColor4f(color[RED],color[GREEN],color[BLUE],opacidad);

        matrizToGL(transformacion);

        dibujar();
    glPopMatrix();

}


//multiplica la matriz actual de la pila de OpenGL por la matriz pasada
void Dibujable::matrizToGL(Matriz4x4 &m){
    float f[16];
    int i,j,cont;
    for(i=cont=0;i<4;i++){
        for(j=0;j<4;j++){
            f[cont]=m(j,i);
            cont++;
        }
    }
    glMultMatrixf(f);
}

//obtiene en la matriz pasada, el valor de la matriz actual de la pila de OpenGL
void Dibujable::glToMatriz(int tipo,Matriz4x4 &m){
    float f[16];

    glGetFloatv(tipo,f);
    int i,j,cont;
    for(i=cont=0;i<4;i++){
        for(j=0;j<4;j++){
            m(j,i)=f[cont];
            cont++;
        }
    }
}

Vector Dibujable::proyectar(Vector v){

    double matrizModelo[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,matrizModelo);

    double matrizProyeccion[16];
    glGetDoublev(GL_PROJECTION_MATRIX,matrizProyeccion);

    int viewPort[4];
    glGetIntegerv(GL_VIEWPORT,viewPort);

    double x,y,z;

    gluProject( v[X], v[Y], v[Z], matrizModelo,matrizProyeccion, viewPort, &x, &y, &z );

    return Vector(x,y,z);
}

Vector Dibujable::desProyectar(Vector w){
    double matrizModelo[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,matrizModelo);

    double matrizProyeccion[16];
    glGetDoublev(GL_PROJECTION_MATRIX,matrizProyeccion);

    int viewPort[4];
    glGetIntegerv(GL_VIEWPORT,viewPort);

    double x,y,z;

    gluUnProject( w[X], w[Y], w[Z], matrizModelo,matrizProyeccion, viewPort, &x, &y, &z );

    return Vector(x,y,z);
}



