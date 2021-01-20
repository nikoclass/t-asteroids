#include "OrbitaParticulas.h"

OrbitaParticulas::OrbitaParticulas(Vector pos,float duracion,Vector color,int cantidad,float dist,float dispersion,float masa):Animable(duracion){
    this->sp=new SistemaParticulas(cantidad);
    this->sp->posicionar(pos);
    this->sp->setColor(color);
    this->sp->setModoTransparencia(Aditivo);
    this->sp->setTamanio(2);

    ModoJuego::getInstance()->getEscena()->addDibujable(sp);

    this->cantidad=cantidad;
    this->velocidad=new Vector[cantidad];
    this->masa=masa;

    //De estas 2 formulas de fisica 1 saco la magia
    //a=omega^2*r
    //a=1/r^2
    Vector posicion;
    //float vel=sqrt(masa)*dist*sqrt(1.0/(dist*dist*dist));
    float vel=sqrt(masa/dist); //esta es la simplificacion!

    for(int i=0;i<cantidad;i++){
        posicion=Vector(Random(-1,1),Random(-1,1),Random(-1,1)).direccion()*dist;
        this->sp->setParticula(i,posicion);
        velocidad[i]=vel*((posicion^Vector(Random(-1,1),Random(-1,1),Random(-1,1))).direccion()) + dispersion*Vector(Random(-1,1),Random(-1,1),Random(-1,1));
    }
}

OrbitaParticulas::~OrbitaParticulas(){
    delete sp;
    delete[] velocidad;
}

void OrbitaParticulas::setMasa(float masa){
    this->masa=masa;
}

float OrbitaParticulas::getMasa(){
    return this->masa;
}

void OrbitaParticulas::animar(float dt){
    Vector nueva_pos;
    float norma;

    for(int i=0;i<cantidad;i++){
        nueva_pos=sp->getParticula(i)+velocidad[i]*dt;
        sp->setParticula(i,nueva_pos);

        norma=nueva_pos.normalizar();
        velocidad[i]-=masa*dt*nueva_pos/(norma*norma);
    }

    sp->setOpacidad(this->porcentajeVida()*0.01);
}

