#include "FMisil.h"
#include "SoundManager.h"
#include "Targeteable.h"
#include "CondTargeteable.h"

FMisil::FMisil(Nave* origen):FMunicion(origen){
	this->cantMunicion=10;
	this->nameModelo="misil.mdl";
	this->textura="misil.tga";
	this->retry[0]=0;
	this->retry[1]=0;
	this->retry_global=0;
	this->lado=1;
	this->con_target=true;
}

FMisil::~FMisil(){
}

void FMisil::disparar(Vector posicion,Vector direccion){
	int boca=(this->lado<0)?0:1;
	//cout << "FMisil::disparar() cant:" << cant << " boca:" << boca << " lado:" << lado << " retry_global:"<< retry_global<< endl;
	if(this->cantMunicion<1){
		//cout << "FMisil::disparar() sin misiles GIL"<< endl;
		if(!SoundManager::getInstance()->isPlaying("secundariaVacia.wav"))
            SoundManager::getInstance()->PlayGameSound("secundariaVacia.wav");
		return;
	}
	if(!this->retry_global>0&&!this->retry[boca]>0){
        ModoJuego*mj=ModoJuego::getInstance();
		Objeto3d*modelo=new Objeto3d(this->nameModelo.data());
		Misil* misil=new Misil(modelo,posicion,direccion);
		misil->getModelo()->aplicarTransformacion(this->origen->getModelo()->getTransformacion());
		misil->posicionar(posicion);
		misil->Setorigen(this->origen);

		CondTargeteable* ct = new CondTargeteable(this->origen);
		MFisica::getInstance()->getColisionablesByCondition(ct);
		Targeteable*tg = ct->getTarget();
		delete ct;

		if(tg&&con_target){
		    misil->setTarget(tg);
		}
		this->cantMunicion-=1;
		this->retry_global=0.5;
		this->retry[boca]=1;
		this->lado*=-1;
		//SoundManager::getInstance()->Play("./Sonidos/misil.wav");
		SoundManager::getInstance()->PlayGameSound("./Sonidos/misil.wav");
	}
}

void FMisil::tic(float dt){
	this->retry[0]=(this->retry[0]>0)?this->retry[0]-dt:0;
	this->retry[1]=(this->retry[1]>0)?this->retry[1]-dt:0;
	this->retry_global=(this->retry_global>0)?this->retry_global-dt:0;
}

int FMisil::getLado(){
	return this->lado;
}
void FMisil::setLado(int lado ){
	this->lado=lado;
}
bool FMisil::isConTarget(){
	return this->con_target;
}
void FMisil::setConTarget(bool in){
	this->con_target=in;
}


