#include "CondTargeteable.h"
#include "Targeteable.h"

CondTargeteable::CondTargeteable(Nave* n)
{
    this->nave = n;
    this->target = NULL;
    this->distTarget = 999999;
}

CondTargeteable::~CondTargeteable(){
}

bool CondTargeteable::cumple(Condicionable* obj){
    if(!nave) return false;

    Targeteable* tg = dynamic_cast<Targeteable*>(obj);
    if( tg && obj != this->nave ){

        float dist = (this->nave->getPosicion() - tg->getPosicionTarget()).norma();
        Vector v1=-this->nave->getModelo()->getEjeZ();
		Vector v2=(tg->getPosicionTarget() - this->nave->getPosicion()).direccion();
		Asteroide* a = dynamic_cast<Asteroide*>(tg);
		if((v1*v2 > 0.9 && dist < this->distTarget && (a?a->getNivel()>1:true))){
		//if(v1*v2 > 0.9 && dist < this->distTarget){
		    this->target = tg;
		    this->distTarget = dist;
		}
    }
    return false;
}
