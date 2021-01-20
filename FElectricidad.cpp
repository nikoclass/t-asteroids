#include "FElectricidad.h"
#include "Electricidad.h"
#include "ModoJuego.h"
#include "Asteroide.h"
#include "CondTargeteable.h"
#include "Targeteable.h"

#include "NaveGhostM.h"


FElectricidad::FElectricidad(Nave* origen): FMunicion(origen){
    conTarget=true;
    danio=500;
    this->cantMunicion=10;
}

FElectricidad::~FElectricidad(){
}

//Funciones
void FElectricidad::disparar(Vector posicion,Vector direccion){
    if(cantMunicion<=0){
        cantMunicion=0;
        return;
    }
    this->cantMunicion-=dt;

    Vector pos_fin;
    Targeteable*tg = NULL;

    if(conTarget){
        CondTargeteable* ct = new CondTargeteable(this->origen);
		MFisica::getInstance()->getColisionablesByCondition(ct);
		tg = ct->getTarget();
		delete ct;
        if(tg){
            pos_fin=tg->getPosicionTarget();
            if(pos_fin.distancia(posicion)>500){
                pos_fin=posicion+direccion*50;
                tg=NULL;
            }else
                tg->setVidaTarget(tg->getVidaTarget()-danio*dt);
        }else
            pos_fin=posicion+direccion*50;
    }
    else{
        pos_fin=posicion+direccion*50;
    }

    if(tg){
        Electricidad *e=new Electricidad(0.2);
        e->setCantPartes(pos_fin.distancia(posicion)/2);
        e->setPosIni(posicion);
        e->setCurvatura(0.05);
        e->setPosFin(pos_fin);
        //e->setFrecuencia(50);
        e->setFrecuencia(1);
    }

    NaveGhostM *ng=dynamic_cast<NaveGhostM*>(this->origen);
    if(ng)
        ng->setTargetElectricidad(tg);

}

void FElectricidad::tic(float deltate){
    this->dt=deltate;
}

bool FElectricidad::isConTarget(){
    return this->conTarget;
}

void FElectricidad::setConTarget(bool in){
    this->conTarget = in;
}

int FElectricidad::getLado(){
    return this->lado;
}

void FElectricidad::setLado(int lado){
    this->lado = lado;
}

int FElectricidad::getDanio(){
    this->danio;
}

void FElectricidad::setDanio(int danio){
    this->danio = danio;
}
