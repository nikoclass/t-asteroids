#include "CadenaParticulas.h"

CadenaParticulas::CadenaParticulas(int cantParticulas): Animable() {
    sp=new SistemaParticulas(cantParticulas);
    elasticidad=1;
    ModoJuego::getInstance()->getEscena()->addDibujable(sp);
    velocidades=new Vector[cantParticulas];
    distanciaEntreParticulas=0;
}

CadenaParticulas::~CadenaParticulas(){
    delete sp;
}

void CadenaParticulas::setCantParticulas(int cant){
    delete sp;
    delete velocidades;
    sp=new SistemaParticulas(cant);
    ModoJuego::getInstance()->getEscena()->addDibujable(sp);
    velocidades=new Vector[cant];
}

int CadenaParticulas::getCantParticulas(){
    return sp->getCantParticulas();
}

void CadenaParticulas::setPosInicial(const Vector &pos) {
    sp->setParticula(0,pos);
}

Vector CadenaParticulas::getPosInicial(){
    return sp->getParticula(0);
}

void CadenaParticulas::setElasticidad(float k) {
    elasticidad=k;
}

float CadenaParticulas::getElasticidad(){
    return elasticidad;
}

void CadenaParticulas::setDistanciaEntreParticulas(float dist) {
    distanciaEntreParticulas=dist;
}

float CadenaParticulas::getDistanciaEntreParticulas(){
    return distanciaEntreParticulas;
}

void CadenaParticulas::animar(float dt){
    int cant=sp->getCantParticulas();
    float k=dt*elasticidad;
    float dist2=distanciaEntreParticulas*distanciaEntreParticulas;

    for(int i=1; i<cant-1; i++){ //salteo la primera particula y la ultima
        Vector dif=sp->getParticula(i-1) + sp->getParticula(i+1) - 2*sp->getParticula(i);

        if(dif.norma2()>dist2){
            velocidades[i]+=k*dif;
        }else{
            velocidades[i]*=0;
        }
    }
    //ahora que tengo la velocidad de todas las particulas actualizo su posicion
    for(int i=1; i<cant; i++){ //salteo la primera particula
        Vector pos=sp->getParticula(i) + velocidades[i]*dt;
        sp->setParticula(i, pos);
    }
}
