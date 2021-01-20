#include "BolaExplosiva.h"

#define RETRY_TIME 0.05

BolaExplosiva::BolaExplosiva(Vector posicion):Animable(1){
	Escena* e=Engine::getInstancia()->getEscena();
/*
	this->modelo=new Objeto3d("esfera.mdl");
	this->modelo->posicionar(posicion);
	this->modelo->setAfectaLuces(false);
	this->modelo->setModoTransparencia(Aditivo);
*/

	this->explosion=new Sprite("explosion.tga");
	this->explosion->setMirarCamara(false);
	this->explosion->setModoTransparencia(Aditivo);
	this->explosion->rotar(Random(-0.5,0.5),e->getCamaraActual()->getDireccion());
	this->explosion->escalar(10);
	this->explosion->posicionar(posicion);

	this->luz=new Luz;
	this->luz->setPos(posicion);
	this->luz->setColor(Vector(1,1,0.8));
	this->luz->setDistanciaMaxima(250);

    e->addDibujable(this->explosion);
	e->addLuz(this->luz);
    //e->addDibujable(this->modelo);

    pos=posicion;
    for(int i=0;i<CANT_DIRS;i++)
        dirs[i]=Vector(Random(-1,1),Random(-1,1),Random(-1,1)).direccion();

    retry=RETRY_TIME;
}

BolaExplosiva::~BolaExplosiva(){
	//delete this->modelo;
	delete this->luz;
	delete this->explosion;
}
void BolaExplosiva::animar(float dt){
	float p=this->porcentajeVida()/100;
    float ts=Engine::getInstancia()->getTimeScale()/Settings::getInstance()->getVelocidad();
/*
	Vector pos=this->modelo->getPosicion();
	this->modelo->identidad();
	this->modelo->escalar(1/(p*p*p+0.01));
	this->modelo->posicionar(pos);
	this->modelo->setOpacidad(p);
*/
    //this->explosion->escalar(1.1);
    this->explosion->setEscala(1.0/(p*p*p));
	this->explosion->setOpacidad(p);

	this->luz->setIntensidad(p);

    retry = retry - dt;
    if(retry<=0){
        retry = RETRY_TIME;
        for(int i=0;i<CANT_DIRS;i++)
            new Humo(pos+dirs[i]*(1-sqrt(p))*100,Random(1,2),0.7);
    }
}
