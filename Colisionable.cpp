#include "Colisionable.h"

Colisionable::Colisionable(Objeto3d*modelo,Vector posicion)
{
    //cout << "Colisionable::Colisionable() - constructor"<<endl;
    this->modelo=modelo;
    this->dead=false;
    this->remoto=false;
    if (this->modelo)this->modelo->posicionar(posicion);
    // lo que ?
    ModoJuego::getInstance()->getEscena()->addDibujable(this->modelo);
    MFisica::getInstance()->addColisionable(this);
}

Colisionable::~Colisionable()
{
    MFisica::getInstance()->removeColisionable(this);
    ModoJuego::getInstance()->getEscena()->removeDibujable(this->modelo);
    if (this->modelo)
    {
        //cout << "Colisionable::~Colisionable() - destructor"<<endl;
        //El destructor de objeto3d se encarga de quitarlo de la escena
        //Engine::getInstancia()->getEscena()->removeDibujable(this->modelo);
        delete this->modelo;
    }
}

Vector Colisionable::getPosicion()
{
    if (this->modelo) return this->modelo->getPosicion();
    return Vector();
}

float Colisionable::getRadio()
{
    if (this->modelo)return this->modelo->getEsferaEnvolvente().getRadio();
    return 0;
}
Esfera Colisionable::getEsfera()
{
    if (this->modelo)return this->modelo->getEsferaEnvolvente();
    return Esfera();
}
/*
bool Colisionable::isDead(){
	return this->dead;
}
*/
void Colisionable::setDead(bool dead)
{
    if(!remoto)
        this->dead=dead;
}

void Colisionable::setDeadRemoto()
{
    this->dead=true;
}

void Colisionable::setRemoto()
{
    remoto=true;
}



/*
Objeto3d* Colisionable::getModelo(){
	return this->modelo;
}
*/

void Colisionable::setModelo(Objeto3d*modelo){
	if(this->modelo)delete this->modelo;
	this->modelo=modelo;
}

void Colisionable::posicionar(Vector pos)
{
    if (this->modelo)this->modelo->posicionar(pos);
}
void Colisionable::trasladar(Vector pos)
{
    if (this->modelo)this->modelo->trasladar(pos);
}
void Colisionable::escalar(float factor)
{
    if (this->modelo)this->modelo->escalar(factor);
}
