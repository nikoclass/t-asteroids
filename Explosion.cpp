#include "Explosion.h"

Explosion::Explosion(Vector pos,float tiempo):Animable(tiempo){
    this->explosion=new Sprite("boom_1.tga");
    this->explosion->posicionar(pos);
    this->explosion->setModoTransparencia(Aditivo);
    this->explosion->escalar(5);
    Engine::getInstancia()->getEscena()->addDibujable(explosion);
}

Explosion::~Explosion(){
    delete this->explosion;
}

void Explosion::animar(float dt){
    float ts=Engine::getInstancia()->getTimeScale()/Settings::getInstance()->getVelocidad();
    float pv=this->porcentajeVida();
    this->explosion->setOpacidad(this->porcentajeVida()/100);
    this->explosion->escalar(1+dt*ts);

    char s[20];
    float c=100;
    int i=0;
    while(c>=pv){
        i++;
        c-=100/8.0;
    }
    sprintf(s,"boom_%d.tga",i);
    this->explosion->cargarTextura(s);

}
