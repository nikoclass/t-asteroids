#include "ExplosionParticulas.h"

ExplosionParticulas::ExplosionParticulas(Vector pos,float duracion,Vector color,int cantidad):Animable(duracion){
    this->sp=new SistemaParticulas(cantidad);
    this->sp->posicionar(pos);
    this->sp->setColor(color);
    this->sp->setModoTransparencia(Aditivo);
    //this->sp->setSuave(true);

    ModoJuego::getInstance()->getEscena()->addDibujable(sp);

    this->cantidad=cantidad;
    this->direccion=new Vector[cantidad];
    for(int i=0;i<cantidad;i++)
        direccion[i]=20*Vector(Random(-1,1),Random(-1,1),Random(-1,1)).direccion()*Random();

}

ExplosionParticulas::~ExplosionParticulas(){
    delete sp;
    delete[] direccion;
}

void ExplosionParticulas::animar(float dt){
    Vector nueva_pos;
    for(int i=0;i<cantidad;i++){
        nueva_pos=sp->getParticula(i)+direccion[i]*dt;
        sp->setParticula(i,nueva_pos);
    }

    sp->setOpacidad(this->porcentajeVida()*0.01);
}
