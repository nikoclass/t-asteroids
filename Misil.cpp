#include "Misil.h"

const float Misil::mult_aceleracion=150;

Misil::Misil(Objeto3d* modelo,Vector posicion,Vector direccion):Colisionable(modelo,posicion){
	this->target=NULL;
	this->arranco=false;
	this->velocidad=Vector();
	this->aceleracion=Vector();
	this->danio=4000;
	this->ttl=6;
	this->inicio=0.3;
	this->luz=new Luz();
	this->luz->setDistanciaMaxima(100);
	this->luz->setColor(Vector(1,1,0.5));
	//this->luz->setLensFlares(true);
	ModoJuego::getInstance()->getEscena()->addLuz(this->luz);
	this->luz->setIntensidad(0);

	this->fuego=new Objeto3d("fuego.mdl","fuego.tga");
	this->fuego->setAfectaLuces(false);
	this->fuego->setModoTransparencia(Aditivo);
	//this->fuego->setOpacidad(0.7);
	this->fuego->setCullFace(false);
	this->fuego->setColor(1,1,0.5);
	ModoJuego::getInstance()->getEscena()->addDibujable(this->fuego);
	this->fuego->setVisible(false);
	this->retryHumo=0;
}

Misil::~Misil(){
	delete this->luz;
	delete this->fuego;
}


void Misil::tic(float dt){

    bool cerca=false;
    Vector resta;

    Nave *origen=this->Getorigen();

    /*if(!origen){
        origen=ModoJuego::getInstance()->getNavePrinc();
    }*/

	if(this->inicio>0 && this->isRemoto()==false){

		Vector v=Vector(0,2*inicio-1,inicio-0.5);

		if(origen)
            v=origen->getModelo()->getTransformacion()*v;

		this->getModelo()->posicionar(v);
		//this->velocidad+=this->abajo*dt;
		this->inicio-=dt;

	}
	else {
		if (!this->arranco) {

			this->arranco=true;
			if(origen  && this->isRemoto()==false)
                this->velocidad=-origen->getModelo()->getEjeZ()*origen->getVelocidad().norma();
			this->aceleracion=this->velocidad.direccion()*mult_aceleracion;
			this->luz->setIntensidad(1);
			this->fuego->setVisible(true);

		}
		else if(this->target&& MFisica::getInstance()->existeColsionable(dynamic_cast<Colisionable*>(this->target))){

			resta=this->target->getPosicionTarget()-this->getPosicion();
			if(resta.norma()<100)
                cerca=true;
			this->aceleracion=resta.direccion()*mult_aceleracion;
		}

		if(cerca)
            this->velocidad=resta.direccion()*(this->velocidad.norma() + this->aceleracion.norma()*dt);
		else
            this->velocidad+=this->aceleracion*dt;

		this->getModelo()->trasladar(velocidad*dt);

        if(this->arranco){
            if(this->retryHumo<=0){
                this->retryHumo=0.025;
                new Humo(this->getModelo()->getPosicion(),Random(2,4));
            }
            this->retryHumo-=dt;
        }

        Vector z=-this->getModelo()->getEjeZ();
		this->getModelo()->rotar(3*dt,z);
        this->getModelo()->rotar(z,this->velocidad);


		Vector luzF(0,0,2);
		luzF=this->getModelo()->getTransformacion()*luzF;
		this->luz->setPos(luzF);

        //this->fuego->posicionar(Vector(0,1,0));
        //this->fuego->aplicarTransformacion(this->getModelo()->getTransformacion());

		//Vector fire(0,0,1);
		//fire=this->getModelo()->getTransformacion()*fire;
        //this->fuego->posicionar(fire);
		this->fuego->identidad();
		this->fuego->escalar(Vector(0.2,0.2,Random(2,4)));
        this->fuego->posicionar(Vector(0,0,1));
        this->fuego->aplicarTransformacion(this->getModelo()->getTransformacion());

        this->ttl-=dt;
		if(this->ttl<0)this->setDead(true);
	}



	Vector pos=this->getPosicion();
    if(pos.norma()>Settings::getInstance()->getRadioEspacio())
    	this->posicionar(-pos+this->velocidad*dt);




    PaqueteRed *p=new PaqueteRed();
    p->addVector(this->getPosicion());
    p->addVector(-this->getModelo()->getEjeZ());
    enviarPaqueteServidor(p);

}
void Misil::bang(Colisionable*who){

}


float Misil::getDanio(){
	return this->danio;
}
void Misil::setDanio(float danio){
	this->danio=danio;
}

void Misil::setTarget(Targeteable*a){
	this->target=a;
}
Targeteable*Misil::getTarget(){
	return this->target;
}

void Misil::setTtl(float t){
    this->ttl=t;
}

void Misil::recibirPaqueteServidor(PaqueteRed *paquete){
    //No hace nada
}

string Misil::getNombreServidor(){
    return "Misil";
}
