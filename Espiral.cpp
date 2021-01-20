#include "Espiral.h"

Espiral::Espiral(Vector pos,float radio):Animable(3){
	this->es=new Sprite("espiral.tga");
	this->es->posicionar(pos);
	this->es->escalar(radio*0.3);
	this->es->setModoTransparencia(Aditivo);
	this->es->setRotable(true);
    ModoJuego::getInstance()->getEscena()->addDibujable(this->es);
    //this->es->setOpacidad(0.5);
}

Espiral::~Espiral(){
	delete this->es;
}

void Espiral::animar(float dt){
	this->es->rotar(2*PI*dt,this->es->getEjeZ());
    if(this->porcentajeVida()>70)
        this->es->escalar(1+dt*0.1);
    else
        this->es->escalar(1-dt);
    float pv=this->porcentajeVida()/100;
	this->es->setOpacidad(1-pv*pv);
}
