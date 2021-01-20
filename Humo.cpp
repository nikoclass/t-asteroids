#include "Humo.h"

Humo::Humo(Vector pos,float tiempo,float factorOpacidad):Animable(tiempo){
    this->humo=new Sprite("humo.tga");
    this->humo->setRotable(true);
    //this->humo->setModoTransparencia(Aditivo);
    Vector r(Random(-1,1),Random(-1,1),Random(-1,1));
    this->humo->posicionar(pos+r*0.2);
    Engine::getInstancia()->getEscena()->addDibujable(humo);
    this->random=Random(-2,2);
    this->factorOpacidad=factorOpacidad;
}

Humo::~Humo(){
    delete this->humo;
}

void Humo::animar(float dt){
    float ts=Engine::getInstancia()->getTimeScale()/Settings::getInstance()->getVelocidad();
    float p=this->porcentajeVida()/100;
    this->humo->setOpacidad(p*factorOpacidad);
    this->humo->rotar(this->random*dt,this->humo->getEjeZ());
    //this->humo->escalar(1+ts*0.01);
    this->humo->setEscala(1.5/(p+0.1));
}
