#include "Nave.h"
//Nave* Nave::instance=NULL;

const float anguloDeRotacion=2;

Nave::Nave(Objeto3d*obj,Vector posicion):Colisionable(obj,posicion)
{
    numCliente=0;
    primaria = 0;
    secundaria=0;
    fuego=0;
    fuegolat=0;
    luzFuego=0;
    luzFuegolat=0;
    espectro=0;
    brilloFuego=0;
    this->counterEmb=0;
    this->colision = false;

    cadenaParticulas = new CadenaParticulas(20);
    cadenaParticulas->setElasticidad(10);
    cadenaParticulas->setDistanciaEntreParticulas(0.5);

    quieroDispararPrimara=quieroDispararSecundaria=false;
}

Nave::~Nave()
{
    Escena* e=ModoJuego::getInstance()->getEscena();
    e->removeDibujable(this->fuego);
    e->removeDibujable(this->espectro);
    e->removeLuz(this->luzFuego);
    e->removeDibujable(this->brilloFuego);
	e->removeDibujable(this->fuegolat);
	e->removeLuz(this->luzFuegolat);

	delete primaria;
	delete secundaria;
	delete fuego;
	delete fuegolat;
	delete luzFuego;
	delete luzFuegolat;
	delete espectro;
	delete brilloFuego;
	delete cadenaParticulas;
}

void Nave::tic(float dt){
    //if(vivo)
    ticVivo(dt);
    cadenaParticulas->setPosInicial(this->getPosicion());
    quieroDispararPrimara=quieroDispararSecundaria=false;
}
void  Nave::moverAdelante(float dt){
	//cout << "Nave::moverAdelante(float dt) begin" << endl;
	this->velocidad += this->getModelo()->getEjeZ()*dt*-this->incrementoVel;
	if(this->velocidad.norma() > this->maxVel)
		this->velocidad = this->velocidad.direccion()*this->maxVel;
	this->movio=true;
	this->adelante=true;
	//cout << "Nave::moverAdelante(float dt) end" << endl;

}

void  Nave::moverAtraz(float dt){
	this->velocidad += this->getModelo()->getEjeZ()*dt*30;
	if(this->velocidad.norma() > this->maxVel)
		this->velocidad = this->velocidad.direccion()*this->maxVel;
	this->movio=true;
	this->adelante=false;
}


void  Nave::moverDer(float dt){
    this->velocidad += this->getModelo()->getEjeX()*dt*+25;
    if(this->velocidad.norma() > this->maxVel)
		this->velocidad = this->velocidad.direccion()*this->maxVel;
	this->moviolat = true;
	this->lateralD=true;

	/**this->trasladar(4*this->getModelo()->getEjeX()*dtr);
	this->movio=true;**/

}
void  Nave::moverIzq(float dt){
    this->velocidad += this->getModelo()->getEjeX()*dt*-25;
    if(this->velocidad.norma() > this->maxVel)
		this->velocidad = this->velocidad.direccion()*this->maxVel;
	this->moviolat = true;
	this->lateralD=false;
	/**this->trasladar(-4*this->getModelo()->getEjeX()*dtr);
	this->movio=true;**/
}

void  Nave::rotar(float angA,float angB,float dt){
	//Asi es mejor, probando se sacan las cosas...

	this->getModelo()->rotar(MantenerEntre(-2,-angB,2)*dt, this->getModelo()->getEjeX());
	this->getModelo()->rotar(MantenerEntre(-4,-angA,4)*dt, this->getModelo()->getEjeY());
}

void Nave::girarDerecha(float dt){
	this->getModelo()->rotar(anguloDeRotacion*dt,this->getModelo()->getEjeZ());
	this->movio=true;
}

void Nave::girarIzquierda(float dt){
	this->getModelo()->rotar(-anguloDeRotacion*dt,this->getModelo()->getEjeZ());
	this->movio=true;
}

void  Nave::teletransportar(){
    if(!vivo) return;
    Settings *s=Settings::getInstance();
	float aux=s->getRadioEspacio();
	this->getModelo()->posicionar((Random(0,aux*0.9))*Vector(Random(-1,1),Random(-1,1),Random(-1,1)).direccion());
	Engine *e=Engine::getInstancia();
	e->setTimeScale(0);
	e->setTimeScale(s->getVelocidad(),4);
	this->getModelo()->setOpacidad(0);
}

Vector Nave::getVelocidad(){
	return this->velocidad;
}

void Nave::setVelocidad( Vector v){
    this->velocidad = v;
}

void Nave::targeting(){
	this->secundaria->setConTarget(!secundaria->isConTarget());
}

bool Nave::isConTarget(){
	return this->secundaria->isConTarget();
}
float Nave::getDanio(){
	return this->danio;
}
void Nave::setDanio(float danio){
	this->danio=danio;
}
void Nave::setEnvairomental(){
	//esto es para cuando agarra el superpoder pichicateado
	this->danio=5000;
	this->masa=100000;
	this->counter+=15;
	this->getModelo()->setModoGeneracionCoordTextura(Enviromental);
	this->getModelo()->cargarTextura("envmapnave.tga");
	this->getModelo()->setAfectaLuces(false);
}

void Nave::setEmbistiendo(){
    if(this->secundaria->getcantMunicion() != 0 && !this->counterEmb){
        this->secundaria->setcantMunicion(this->secundaria->getcantMunicion() - 1);
        this->danio=5000;
        this->masa=100000;
        this->counterEmb =1;
        this->getModelo()->setModoGeneracionCoordTextura(Enviromental);
        this->getModelo()->cargarTextura("espectro.tga");
        this->getModelo()->setAfectaLuces(false);
    }
}
float Nave::getVida(){
	return this->vida;
}
void Nave::setVida(float vida){
	this->vida=vida;
}
//int Nave::getMisiles(){
//	if(this->secundaria){
//		if (FMisil* fm = dynamic_cast<FMisil*>( this->secundaria )){
//			return fm->getCantMisiles();
//		}
//	}
//	return 0;
//}
float Nave::getNormaVelocidad(){
	return this->velocidad.norma();
	//return 1;
}
void Nave::recargarSecundaria(){
	if(this->secundaria)
		  this->secundaria->setcantMunicion(this->secundaria->getcantMunicion() + (int)Random(5,10));






}

void Nave::recargarVida(){
	this->vida+=Random(20,50);
}

void Nave::balasRojas(){
	this->primaria->balasRojas();
}

bool Nave::isViva(){
    return this->vivo;
}

Objeto3d* Nave::getFuego(){
    return this->fuego;
}

Sprite* Nave::getBrillo(){
    return this->brilloFuego;
}

bool Nave::getMovio(){
    return this->movio;
}
void Nave::setMovio( bool val){
    this->movio = val;
}
float Nave::getpondDanio(){
    return this->pondDanio;
}
void Nave::setpondDanio( float val){
    this->pondDanio = val;
}
void Nave::bang(Colisionable* who){
     //cout << "Nave::bang() ASTEROIDE"<< endl;
     if (BalaComun* bala = dynamic_cast<BalaComun*>( who )){
            if(!bala->mismoOrigen(this)){
                this->getModelo()->rotar(Random(0, 0.3),Vector(Random(-1,1),Random(-1,1),Random(-1,1)));
                bala->setDead(true);
                this->vida-=bala->getDanio()*this->pondDanio;
                this->colision = true;
            }
	}
	else if (Misil* misil = dynamic_cast<Misil*>( who )){
		if(!misil->mismoOrigen(this)){
		    misil->setDead(true);
            this->vida-=misil->getDanio()*this->pondDanio;
            new BolaExplosiva(this->getPosicion());
            this->colision = true;
		}
	}
	else if (Nave* nave = dynamic_cast<Nave*>( who )){
		if(this->danio <= 60){
		    this->colision = true;
            this->vida-=nave->getDanio()*this->pondDanio;
            Vector v=nave->getVelocidad()-this->velocidad;
            float m1= this->masa;
            float m2=nave->getMasa();
            float c1=m1/(m1+m2);
            float c2=m2/(m1+m2);
            Esfera suEsfera=nave->getEsfera();
            Esfera miEsfera=this->getEsfera();
            Vector dir=miEsfera.getCentro()-suEsfera.getCentro();
            float normaDir=dir.normalizar();

            Vector proyeccion=ProyeccionOrtogonal(v,dir);
            //para cuando embiste
            this->velocidad+=proyeccion*c2;
            nave->setVelocidad(nave->getVelocidad()-proyeccion*2*c1);

            float dt=Engine::getInstancia()->getDt();
            if (dir==Vector(0,0,0))
                dir=Vector(dt,dt,dt);

            float dif=miEsfera.getRadio()+suEsfera.getRadio()-normaDir;

            if (dif>0)
            {
                this->trasladar(dif*dir*(c1+dt));
            }
            nave->trasladar(-dif*dir*(c2+dt));

            //if (vivo) por ahora no
            //    this->getModelo()->rotar(Random(-1,2),getModelo()->getEjeY());
		}
	}
    else if (Asteroide* asteroide = dynamic_cast<Asteroide*>( who ))
    {
        this->colision = true;
        if (this->danio <= 60)
        {

            if (this->getModelo()->getOpacidad()>=0.5)
                this->vida-=asteroide->getDanio()*this->pondDanio;

            Vector v=asteroide->getVelocidad()-this->velocidad;
            float m1= this->masa;
            float m2=asteroide->getMasa();
            float c1=m1/(m1+m2);
            float c2=m2/(m1+m2);
            Esfera suEsfera=asteroide->getEsfera();
            Esfera miEsfera=this->getEsfera();
            Vector dir=miEsfera.getCentro()-suEsfera.getCentro();
            float normaDir=dir.normalizar();

            Vector proyeccion=ProyeccionOrtogonal(v,dir);
            //para cuando embiste
            this->velocidad+=proyeccion*c2;
            asteroide->setVelocidad(asteroide->getVelocidad()-proyeccion*2*c1);

            float dt=Engine::getInstancia()->getDt();
            //asteroide->ya_choco=true;
            if (dir==Vector(0,0,0))
                dir=Vector(dt,dt,dt);

            float dif=miEsfera.getRadio()+suEsfera.getRadio()-normaDir;

            if (dif>0)
            {
                this->trasladar(dif*dir*(c1+dt));
            }
            asteroide->trasladar(-dif*dir*(c2+dt));
            if (vivo)
                this->getModelo()->rotar(Random(-1,2),getModelo()->getEjeY());
        }
    }

    if (this->vida<=0 && vivo)
    {
         vivo=false;
         explotar();
         new BolaExplosiva(this->getPosicion());

    }
    else if (this->counterEmb==0 && vivo && this->colision)
    {
        this->espectro->setOpacidad(1);
    }
    this->colision = false;
}
