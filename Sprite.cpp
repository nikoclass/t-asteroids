#include "Sprite.h"

Sprite::Sprite():Dibujable(){
    this->afectaLuces=false;
    this->textura=NULL;
    this->x=this->y=0;
    this->ancho=this->alto=1;
    this->modoTextura=Modular;
    this->mirarCamara=true;
    this->rotable=false;
}
Sprite::Sprite(const char* textura,bool forzarTransparencia):Dibujable(){
    this->afectaLuces=false;
    this->textura=ManejadorTexturas::getInstancia()->getTextura(textura,forzarTransparencia);
    this->x=this->y=0;
    this->ancho=this->alto=1;
    this->modoTextura=Modular;
    this->mirarCamara=true;
    this->rotable=false;
}

Sprite::~Sprite(){
}

void Sprite::cargarTextura(const char* textura,bool forzarTransparencia){
    this->textura=ManejadorTexturas::getInstancia()->getTextura(textura,forzarTransparencia);
}

void Sprite::setXY(float x, float y){
    this->x=x;
    this->y=y;
}
void Sprite::setAnchoAlto(float ancho, float alto){
    this->ancho=ancho;
    this->alto=alto;
}
float Sprite::getX(){
    return this->x;
}
float Sprite::getY(){
    return this->y;
}
float Sprite::getAncho(){
    return this->ancho;
}
float Sprite::getAlto(){
    return this->alto;
}

void Sprite::setModoTextura(ModoTextura modo){
    modoTextura=modo;
}
ModoTextura Sprite::getModoTextura(){
    return modoTextura;
}

void Sprite::setMirarCamara(bool val){
    mirarCamara=val;
}
bool Sprite::getMirarCamara(){
    return mirarCamara;
}

void Sprite::setRotable(bool val){
    rotable=val;
}

bool Sprite::isRotable(){
    return rotable;
}

void Sprite::dibujar(){
    if(!textura) return;

    textura->toGL();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, modoTextura);
    glDisable(GL_CULL_FACE);
    //glColor4f(1,1,1,opacidad);


    if(mirarCamara && afectaVista){
        Matriz4x4 m;
        glToMatriz(GL_MODELVIEW_MATRIX,m);
        m(0,3)=m(1,3)=m(2,3)=0;
        m=m.transpuesta();

        if(rotable)
            m=m*this->getRotaciones(true);
        matrizToGL(m);
    }

/**
    if(mirarCamara && afectaVista){
        Matriz4x4 m;
        Vector z=-this->getEjeZ();
        Vector dir=(this->escena->getCamaraActual()->getPosicion() - this->getPosicion()).direccion();
        Vector r=z^dir;
        float cos=z*dir;
        if(cos>0)
            m=Matriz4x4::Rotacion(asin(r.norma()),r);
        else if(cos<0)
            m=Matriz4x4::Rotacion(PI-asin(r.norma()),r);
        else
            m=Matriz4x4::Rotacion(PI/2,r);

        matrizToGL(m);
    }
**/

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glBegin(GL_QUADS);

        //Izquierda abajo
        glTexCoord2f(x,y);
        glVertex3f(-0.5,0.5,0);

        //derecha abajo
        glTexCoord2f(x+ancho,y);
        glVertex3f(0.5,0.5,0);

        //derecha arriba
        glTexCoord2f(x+ancho,y+alto);
        glVertex3f(0.5,-0.5,0);

        //izquierda arriba
        glTexCoord2f(x,y+alto);
        glVertex3f(-0.5,-0.5,0);

    glEnd();
}


bool Sprite::getAfectaLuces(){
    return false;
}

bool Sprite::esTransparente(){
    if(opacidad<1){
        if(!textura) //Si no tengo textura es transparente por la opacidad
            return true;
        else //si tengo textura y es 32 bits, es transparente. Si no es de 32 bits pero el modo no es Reemplazar => es transparente
            return (modoTextura!=Reemplazar || textura->getBpp()==32);
    }else if(textura) //Si tengo textura de 32 bits y no es decal, es transparente
        return (modoTextura!=Decal && textura->getBpp()==32);
    else //No es transparente por la opacidad y porque no tiene textura
        return false;
}

