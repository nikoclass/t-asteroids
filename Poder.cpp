#include "Poder.h"

Poder::Poder(Objeto3d*modelo,Vector posicion):Colisionable(modelo,posicion){
	this->ttl=Random(6,10);
	this->pow=(int)Random(0,MAX_PODER);
	//this->pow=POW_MISIL;

	//modelo->setOpacidad(0.5);
	modelo->escalar(7);
	modelo->setModoTransparencia(Aditivo);
	modelo->setAfectaLuces(false);
	modelo->cargarTextura("espectro.tga");
	modelo->setModoGeneracionCoordTextura(Enviromental);


	this->luz=new Luz();
	this->luz->setDistanciaMaxima(100);
	this->luz->setIntensidad(1);
	this->luz->setPos(this->getModelo()->getPosicion());
	ModoJuego::getInstance()->getEscena()->addLuz(this->luz);

	this->secundario=NULL;

    op=NULL;
}

Poder::~Poder(){
	delete this->luz;
	if(this->secundario)delete this->secundario;
	op->setMasa(0); //esto obliga a la dispersion

}

void Poder::tic(float dt){
    if(!op)
        op=new OrbitaParticulas(this->getPosicion(),ttl*2,Vector(1,1,1),40,5,0,1000);

    if(!secundario){
            switch (this->pow){
            case POW_VIDA:
                this->luz->setColor(Vector(1,0,0));
                this->secundario=new Objeto3d("podervida.mdl");
                this->secundario->escalar(5);
                this->secundario->setColor(1,0,0);
                //this->secundario->getMaterial().setDiffuse(Vector(1,0,0));
                break;
            case POW_MISIL:
                this->luz->setColor(Vector(0,0,1));
                this->secundario=new Objeto3d("podermisiles.mdl");
                this->secundario->escalar(5);
                break;
            case POW_ENV:
                    this->luz->setColor(Vector(1,1,0.6));
                    this->secundario=new Objeto3d("poderescudo.mdl","envmapnave.tga");
                    this->secundario->escalar(5);
                    this->secundario->setModoGeneracionCoordTextura(Enviromental);
                break;
            case POW_BALAS:
                this->luz->setColor(Vector(1,0,1));
                this->secundario=new Objeto3d("poderbalasrojas.mdl");
                this->secundario->setColor(Vector(1,0,0));
                this->secundario->escalar(5);
                break;
            }

            if(this->secundario){
                ModoJuego::getInstance()->getEscena()->addDibujable(this->secundario);
                this->secundario->posicionar(this->getPosicion());
                //this->secundario->setAfectaLuces(false);
                //this->secundario->setOpacidad(0.5);
            }
    }


	this->ttl-=dt;
	if(this->ttl<0)this->setDead(true);
	this->getModelo()->rotar(2*dt,this->getModelo()->getEjeY());
	if(this->secundario)this->secundario->rotar(2*dt,this->secundario->getEjeY());

}

void Poder::bang(Colisionable*who){
	if(Nave* nave = dynamic_cast<Nave*>( who )){
		cout << "Poder::bang() NAVE"<< endl;
		this->setDead(true);
		this->op->setDead(true);
		switch (this->pow){
		case POW_VIDA:
			nave->recargarVida();
			break;
		case POW_MISIL:
			nave->recargarSecundaria();
			break;
		case POW_ENV:
			nave->setEnvairomental();
			break;
		case POW_BALAS:
			nave->balasRojas();
			break;
		}

		SoundManager::getInstance()->PlayGameSound("recargar.wav");
	}
}

void Poder::recibirPaqueteServidor(PaqueteRed* paquete){

    PaqueteRed *p=new PaqueteRed();

    p->addVector(this->getPosicion());
    p->addInt(this->pow);

    enviarPaqueteServidor(p);

}

string Poder::getNombreServidor(){
    return "Poder";
}
