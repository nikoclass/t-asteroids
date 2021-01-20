#include "Animable.h"

Animable::Animable(){
	//cout << "Animable::Animable()" << endl;
	this->dead=false;
	this->tini=0;
	this->trest=0;
	MFisica::getInstance()->addAnimable(this);
}
Animable::Animable(float tini){
	//cout << "Animable::Animable(float tini)" << endl;
	this->dead=false;
	this->tini=tini;
	this->trest=tini;
	MFisica::getInstance()->addAnimable(this);
}
Animable::~Animable(){
	MFisica::getInstance()->removeAnimable(this);
}

bool Animable::isDead(){
	return this->dead;
}
void Animable::setDead(bool dead){
	this->dead=dead;
}

void Animable::tic(float dt){
	this->trest-=dt;
	if(this->tini!=0 && this->trest<0)this->dead=true;
	this->animar(dt);
}

float Animable::porcentajeVida(){
	//cout << "Animable::porcentajeVida() porcentaje=" << trest*100/tini<< endl;
	if(this->tini!=0) return trest*100/tini;

	return 100;
}
