#include "FBalaComun.h"
#include "SoundManager.h"

FBalaComun::FBalaComun(Nave* origen):FMunicion(origen){
	try{
		this->nameModelo="balacomun.mdl";
		//this->nameModelo="esfera.mdl";
		this->textura="envmapnave.tga";
		//this->objeto=new Objeto3d(this->nameModelo.data());
		this->lado=1;
		this->retryTime=0;
		this->retryConst=0.3;
		this->balasRojasc=0;
	}
	catch (exception&e){
		//this->objeto=NULL;
		cout << "[ERROR] "<<e.what()<<endl;
	}

}

FBalaComun::~FBalaComun(){}

void  FBalaComun::disparar(Vector posicion,Vector direccion){
	if(this->retryTime<=0){
	    //cout << "FBalaComun::disparar lado:"<<this->lado<<endl;
		Objeto3d*modelo=new Objeto3d(this->nameModelo.data());
		//modelo->aplicarTransformacion(Nave::getInstance()->getModelo()->getTransformacion());
		modelo->setAfectaLuces(false);

		modelo->setModoTransparencia(Aditivo);
		modelo->setOpacidad(0.7);
		BalaComun*bala=new BalaComun(modelo,posicion,direccion);
		bala->Setorigen(this->origen);
		ModoJuego*mj=ModoJuego::getInstance();//instancio la nave principal creada en modojuego
		bala->getModelo()->aplicarTransformacion(this->origen->getModelo()->getTransformacion());
		bala->posicionar(posicion);
		lado*=-1;
		if(this->balasRojasc>0){
			this->retryConst=0.1;
			bala->setRoja();
			SoundManager::getInstance()->PlayGameSound("blaster1.wav");
		}
		else{
			this->retryConst=0.3;
			SoundManager::getInstance()->PlayGameSound("blaster1.wav");
		}
		this->retryTime=this->retryConst;

	}
}
void FBalaComun::tic(float dt){
	this->retryTime=(this->retryTime>0)?this->retryTime-dt:0;
	this->balasRojasc=(this->balasRojasc>0)?this->balasRojasc-dt:0;
}
int FBalaComun::getLado(){
	return this->lado;
}
void FBalaComun::setLado(int lado){
	this->lado=lado;
}
void FBalaComun::setRetryConst(float r){
	this->retryConst=r;
}
void FBalaComun::balasRojas(){
	this->balasRojasc+=20;
}

bool FBalaComun::isConTarget(){return false;}
void FBalaComun::setConTarget(bool t){}
