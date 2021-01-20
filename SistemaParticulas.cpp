#include "SistemaParticulas.h"

SistemaParticulas::SistemaParticulas(int cant, string particula){
    cantParticulas=cant;

    particulas=new Sprite*[cant];
    for(int i=0;i<cant;i++){
        Sprite *sp=new Sprite(particula.c_str());
        sp->setModoTransparencia(Aditivo);
        sp->setAfectaDetrasCamara(false);
        particulas[i]=sp;
    }

    tam=1;
    this->afectaLuces=false;
    this->afectaDetrasCamara=false;
    this->setModoTransparencia(Aditivo);
}

SistemaParticulas::~SistemaParticulas(){
    for(int i=0; i<cantParticulas; i++)
        delete particulas[i];
    delete[] particulas;
}

void SistemaParticulas::setParticula(int n,const Vector &particula){
    if(n<0 || n>=cantParticulas) return;

    particulas[n]->posicionar(particula);
}

Vector SistemaParticulas::getParticula(int n){
    if(n<0 || n>=cantParticulas) return Vector();

    return particulas[n]->getPosicion();
}

int SistemaParticulas::getCantParticulas(){
    return cantParticulas;
}

void SistemaParticulas::setTamanio(float tamanio){
    tam=tamanio;
     for(int i=0;i<cantParticulas;i++){
        particulas[i]->setEscala(tam);
    }
}

float SistemaParticulas::getTamanio(){
    return tam;
}

//Overrides
bool SistemaParticulas::esTransparente(){
    return true;
}
bool SistemaParticulas::getAfectaLuces(){
    return false;
}


Esfera SistemaParticulas::getEsferaEnvolvente(){
    Vector centro;
    for(int i=0;i<cantParticulas;i++)
        centro+=particulas[i]->getPosicion();

    centro/=cantParticulas;
    float dist=99999999;

    for(int i=0;i<cantParticulas;i++)
        dist=Min(dist,centro.distancia2(particulas[i]->getPosicion()));

    dist=sqrt(dist);
    centro+=getPosicion();

    return Esfera(centro,dist);
}

void SistemaParticulas::dibujar(){

    for(int i=0;i<cantParticulas;i++){
        particulas[i]->setColor(this->color);
        particulas[i]->setOpacidad(this->opacidad);
        particulas[i]->toGL();
    }
}
