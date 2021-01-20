#include "BalaComun.h"

BalaComun::BalaComun(Objeto3d* modelo,Vector posicion,Vector direccion):Colisionable(modelo,posicion){
	if(modelo){
		this->velocidad=direccion*200;
		this->luz=new Luz();
		this->luz->setColor(Vector(0,1,0));
		this->getModelo()->setColor(0.2,1,0.2);
		this->luz->setDistanciaMaxima(50);
		//this->luz->setLensFlares(true);
		ModoJuego::getInstance()->getEscena()->addLuz(luz);

	}
	this->ttl=4;
	this->porMorir=false;
	this->danio=100;
}

BalaComun::~BalaComun(){
	//cout << "BalaComun::~BalaComun() destructor" << endl;
	//Lo hace el destructor de luz
	//Engine::getInstancia()->getEscena()->removeLuz(this->luz);

	delete this->luz;
}
Vector BalaComun::getVelocidad(){
	return this->velocidad;
}
void BalaComun::setVelocidad(Vector velocidad){
	this->velocidad=velocidad;
}
void BalaComun::tic(float dt){
	this->ttl-=dt;
	this->trasladar(this->velocidad*dt);

	Vector pos=this->getPosicion();
	if(pos.norma()>Settings::getInstance()->getRadioEspacio())
		this->posicionar(-pos+this->velocidad*dt);

	this->luz->setPos(this->getPosicion());
	if (this->ttl<0&&!this->porMorir&&!this->isDead()){
		this->porMorir=true;
		this->ttl=0.5;
	}
	if((this->ttl<0) && this->porMorir){
		this->setDead(true);
	}
	if(this->porMorir&&!this->isDead()){
		this->escalar(this->ttl*dt);
	}
}
float BalaComun::getDanio(){
	return this->danio;
}
void BalaComun::setDanio(float danio){
	this->danio=danio;

}

void BalaComun::bang(Colisionable* who){
	if(Asteroide* asteroide = dynamic_cast<Asteroide*>( who )){
		//cout << "BalaComun::bang() ASTEROIDE"<< endl;
		Objeto3d *m=this->getModelo();
		new ExplosionParticulas(m->getPosicion(),1,m->getColor(),(int)Random(5,10));

	}
	else if (Nave* bala = dynamic_cast<Nave*>( who )){
		//cout << "BalaComun::bang() NAVE"<< endl;

	}

}

void BalaComun::setRoja(){
	this->getModelo()->setColor(Vector(1,0,0));
	this->luz->setColor(Vector(1,0,0));
}

void BalaComun::recibirPaqueteServidor(PaqueteRed* paquete){
    PaqueteRed *p=new PaqueteRed();
	p->addVector(this->getPosicion());
	p->addVector(-this->getModelo()->getEjeZ());
	p->addInt(this->getModelo()->getColor()==Vector(1,0,0));

	this->enviarPaqueteServidor(p);
}

void BalaComun::setTtl(float t){
    ttl=t;
}



string BalaComun::getNombreServidor(){
    return "BalaComun";
}
