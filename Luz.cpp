
#include <stdio.h>
#include <string.h>
#include "Luz.h"


Luz::Luz(){
    this->tipo=puntual;
    this->pos               = Vector(0,0,0);
    this->color             = Vector(1.0,1.0,1.0);
    this->distancia_max     = 0;
    this->atenuacion        = 0;
    this->intensidad        = 1;
    this->escena=NULL;
    this->lensFlare=NULL;
}

Luz::~Luz(){
    if(escena)
        escena->removeLuz(this);

    if(lensFlare)
        delete lensFlare;
}

void Luz::setTipo(TipoLuz tipo){
    this->tipo=tipo;
}
TipoLuz Luz::getTipo(){
    return this->tipo;
}
void Luz::setPos(const Vector &pos){
    this->pos=pos;
}
Vector Luz::getPos(){
    return this->pos;
}
void Luz::setDireccion(const Vector &direccion){
    this->direccion=direccion;
}
Vector Luz::getDireccion(){
    return this->direccion;
}
void Luz::setColor(const Vector &color){
    this->color=color;
}
Vector Luz::getColor(){
    return this->color;
}
void Luz::setDistanciaMaxima(float distancia){
    this->distancia_max=distancia;
    if(distancia>0)
        atenuacion=10.0/distancia/distancia;
    else
        atenuacion=0;
}

void Luz::setLensFlares(bool val){
    if(val){
        if(!lensFlare) lensFlare=new LensFlare(this);
    }else{
        if(lensFlare){
            delete lensFlare;
            lensFlare=NULL;
        }
    }
}

bool Luz::getLensFlares(){
    return lensFlare;
}

float Luz::getDistanciaMaxima(){
    return this->distancia_max;
}
void Luz::setIntensidad(float intensidad){
    this->intensidad=intensidad;
}
float Luz::getIntensidad(){
    return this->intensidad;
}
void Luz::setAngulo(float angulo){
    this->angulo=angulo;
    this->cos_angulo=cos(angulo);
}
float Luz::getAngulo(){
    return this->angulo;
}
float Luz::getCosAngulo(){
    return this->cos_angulo;
}
void Luz::apagar(){
    this->intensidad=0;
}

void Luz::setEscena(Escena* e){
    this->escena=e;
}
Escena* Luz::getEscena(){
    return escena;
}

void Luz::dibujarLensFlares(){
    if(this->lensFlare)
        this->lensFlare->toGL();
}

void Luz::encender(float intensidad=1){
    this->intensidad=intensidad;
}
 void Luz::toGL(int identificador){
    GLenum light=identificador;

    const GLfloat v_pos[4]       = { pos[X], pos[Y], pos[Z], 1.0f };
    const GLfloat v_color[4]   = { color[X]*intensidad, color[Y]*intensidad, color[Z]*intensidad, 1.0f };
    const GLfloat v_nulo[4]   = {0,0,0,1};

    glLightfv(light, GL_POSITION , v_pos);      //Posición

    glLightfv(light, GL_DIFFUSE  , v_color);    //Intensidad de la luz. Define el color
    glLightfv(light, GL_SPECULAR , v_color);

    if(distancia_max==0)//Luz ambiente solo para las luces con alcance infinito
        glLightfv(light, GL_AMBIENT , v_color);
    else
        glLightfv(light, GL_AMBIENT , v_nulo);

    if(tipo==focal){
        const GLfloat v_dir[4] = { direccion[X], direccion[Y], direccion[Z], 1.0f };

        glLightfv(light, GL_SPOT_DIRECTION , v_dir);                    //direccion del foco
        glLightf(light , GL_SPOT_CUTOFF    , angulo);        //apertura del foco

        //glLightf(light,GL_SPOT_EXPONENT,val);                         //Atenuación del foco

    }

    //Atenuación de la luz
    //Pérdida de intensidad de la luz a medida que nos
    //alejamos del foco
    //El factor de atenuación se calcula mediante: 1/(kc+kl.d+kq.d^2)
    //d : distancia entre la posición de la luz y el vértice
    //kc : GL_CONSTANT_ATTENUATION
    //kl : GL_LINEAR_ATTENUATION
    //kq : GL_QUADRATIC_ATTENUATION
    //Una vez establecidas las propiedades de una luz
    //glLightf(light,GL_[CONSTANT|LINEAR|QUADRATIC]_ATTENUATION,val);

    glLightf(light,GL_CONSTANT_ATTENUATION,1);
    glLightf(light,GL_LINEAR_ATTENUATION,0);
    glLightf(light,GL_QUADRATIC_ATTENUATION,atenuacion);

    glEnable(light);    //Habilito la luz

}
