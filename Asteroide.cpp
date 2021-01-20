#include "Asteroide.h"
#include "Animable.h"

Asteroide::Asteroide(int id, float nivel,Objeto3d*modelo,Vector posicion):Colisionable(modelo,posicion){
	//cout << "Asteroide::Asteroide() - constructor"<<endl;
	this->id=id;
	this->nivel=nivel;
	this->vida=100*this->nivel;
	this->ya_choco=false;
	this->maxVel=85; //No seas amarrete Chalai, dale mas velocidad jeje

	if(nivel>1)
        this->masa=100*this->nivel*this->nivel*this->nivel;
    else
        this->masa=0.001;

	this->dt=0;
	if(modelo){
		modelo->escalar(this->nivel*10);
		//modelo->getMaterial().setSpecular(Vector(0,0,0));
	}
	this->velocidadAngular=Vector(Random(-1,1),Random(-1,1),Random(-1,1));
	//cout << "Asteroide::Asteroide() - end"<<endl;
	if(!this->nivel<1)ModoJuego::getInstance()->addAsteroide(this);

}

Asteroide::~Asteroide(){
    ModoJuego::getInstance()->removeAsteroide(this);
//TRAZA;
}

void Asteroide::setModeloAsteroide(Objeto3d*modelo){
	if(modelo){
		if(this->getModelo())delete this->getModelo();
		this->setModelo(modelo);
		this->getModelo()->escalar(this->nivel*10);
		this->getModelo()->getMaterial().setSpecular(Vector(0,0,0));
	}
}


void Asteroide::setVelocidad(Vector vel){
	//cout << "Asteroide::setVelocidad()" << endl;
	this->velocidad=vel;
	//cout << "Asteroide::setVelocidad() end" << endl;
}
void Asteroide::setVelocidadAngular(Vector vel){
	this->velocidadAngular=vel;
}
Vector Asteroide::getVelocidad(){
	return this->velocidad;
}
Vector Asteroide::getVelocidadAngular(){
	return this->velocidadAngular;
}

void Asteroide::tic(float dt){
	this->dt=dt;

	if(this->getModelo()){
		if(this->velocidad.norma()>this->maxVel)this->velocidad=this->velocidad.direccion()*this->maxVel;
		this->getModelo()->trasladar(dt*this->velocidad);
		this->getModelo()->rotar(dt*this->velocidadAngular.norma(),this->velocidadAngular);

		if(this->nivel<1){
		    this->vida-=dt*0.1;
		    if(this->getModelo()->getEscala()>0)
                this->getModelo()->setEscala(this->getModelo()->getEscala()-dt*0.1);
		}

		Vector pos=this->getPosicion();
		if(pos.norma()>Settings::getInstance()->getRadioEspacio()){
		    float radio=this->getRadio(); ///
			new Espiral(this->getPosicion(),radio); ///
			if(this->nivel<1)
                this->vida=0;
            else
                this->posicionar(-pos*0.9);
			new Espiral(this->getPosicion(),radio); ///
		}
	}


	if(this->vida<=0){
		this->setDead(true);
		if(this->nivel>1){ //Se fragmenta

			float poder=Random(1);
			if(poder<0.25)new Poder(new Objeto3d("esfera.mdl"),this->getModelo()->getPosicion());

			string modName=this->getModelo()->getModelo()->getNombre();
			string textName=this->getModelo()->getTextura()->getNombre();

			Objeto3d *obj;
			Asteroide *ast;

			Vector pos=this->getModelo()->getPosicion();
			Vector vel=this->getVelocidad();
			Vector velAng=this->getVelocidadAngular();

			float velNorma=vel.norma();
			float velAngNorma=velAng.norma();

            char master[20];
			int cant=(int)Random(2,4);
			for(int i=0;i<cant;i++){
                sprintf(master,"asteroide_%d.mdl",(int)Random(12)+1);
			    Vector vRandom(Random(-1,1),Random(-1,1),Random(-1,1));
			    obj=new Objeto3d(master,textName.data());
			    //ast=new Asteroide(1,this->nivel-1,obj,pos+vRandom);
			    ast=new Asteroide(1,this->nivel/(cant-0.5),obj,pos+vRandom);
			    Vector w=(velAng^vRandom).direccion();//Perpendicular a la velocidad angular en alguna direccion
                ast->setVelocidad(vel+w*velAngNorma*(1+velNorma));
                ast->setVelocidadAngular(vRandom);
			}

			//Los fragmentos de nivel <1
			ModoJuego* mj=ModoJuego::getInstance();
			if( mj->getNavePrinc()->isViva()) //Nave::getInstance()->isViva())
                cant=(int)Random(8,15);
            else
                cant=(int)Random(10,20);

			for(int i=0;i<cant;i++){
			    sprintf(master,"asteroide_%d.mdl",(int)Random(12)+1);
			    Vector vRandom(Random(-1,1),Random(-1,1),Random(-1,1));
			    obj=new Objeto3d(master,textName.data());
			    ast=new Asteroide(1,Random(0.1,0.9),obj,pos+vRandom);
			    Vector w=(velAng^vRandom).direccion();//Perpendicular a la velocidad angular en alguna direccion
                ast->setVelocidad(2*vRandom*(1+vel.norma()));
                ast->setVelocidadAngular(vRandom);
			}
		}
	}


}

void Asteroide::bang(Colisionable*who){

	if (BalaComun* bala = dynamic_cast<BalaComun*>( who )){
			//cout << "Asteroide::bang() BALA_COMUN"<< endl;
			//Modifico la velocidad angular
			Vector dir=(this->getModelo()->getPosicion() - bala->getModelo()->getPosicion()).direccion();
            this->velocidadAngular+=(bala->getVelocidad()^dir)/300;

            bala->setDead(true);
			this->vida-=bala->getDanio();
	}
	else if (Misil* misil = dynamic_cast<Misil*>( who )){
		//cout << "Asteroide::bang() MISIL"<< endl;
		misil->setDead(true);
		this->vida-=misil->getDanio();
		if(this->vida<0){
            new BolaExplosiva(this->getPosicion());
            //new Explosion(this->getPosicion(),1);
		}
	}
	else if (Nave* nave = dynamic_cast<Nave*>( who )){
		//cout << "Asteroide::bang() NAVE"<< endl;
		this->vida-=nave->getDanio();
	}
	else if (Asteroide* aster = dynamic_cast<Asteroide*>( who )){
			//cout << "Asteroide::bang() ASTEROIDE"<< endl;
			if(ya_choco)ya_choco=false;
			else {

				Vector v=aster->velocidad-this->velocidad;
				float m1=this->masa;
				float m2=aster->masa;
				float c1=m1/(m1+m2);
                float c2=m2/(m1+m2);
                Esfera suEsfera=aster->getEsfera();
				Esfera miEsfera=this->getEsfera();
				Vector dir=miEsfera.getCentro()-suEsfera.getCentro();
				float normaDir=dir.normalizar();

                Vector proyeccion=ProyeccionOrtogonal(v,dir);

				this->velocidad+=proyeccion*2*c2;
				aster->velocidad-=proyeccion*2*c1;

				aster->ya_choco=true;
                if(dir==Vector(0,0,0))
                    dir=Vector(dt,dt,dt);

                float dif=miEsfera.getRadio()+suEsfera.getRadio()-normaDir;

                if(dif>0){
                    this->trasladar(dif*dir*(c1+dt));
                    aster->trasladar(-dif*dir*(c2+dt));
                }
			}
	}
	if (this->vida < 0){
	    SoundManager::getInstance()->PlayGameSound("./Sonidos/explosion.wav");
	}


	PaqueteRed *p=new PaqueteRed();

    Objeto3d *m=this->getModelo();

    p->addVector(m->getPosicion());
    p->addVector(this->getVelocidad());
    p->addVector(this->getVelocidadAngular());
    p->addFloat(this->nivel);
    p->addString(m->getModelo()->getNombre());
    p->addString(m->getTextura()->getNombre());

    enviarPaqueteServidor(p);

}

float Asteroide::getDanio(){
	if (!this->nivel<1) return this->nivel*20;
	return 0;
}

float Asteroide::getMasa(){
    return this->masa;
}

void Asteroide::setVida(float v){
    this->vida=v;
}

float Asteroide::getVida(){
    return vida;
}


void Asteroide::recibirPaqueteServidor(PaqueteRed *paquete){

    PaqueteRed *p=new PaqueteRed();

    Objeto3d *m=this->getModelo();

    p->addVector(m->getPosicion());
    p->addVector(this->getVelocidad());
    p->addVector(this->getVelocidadAngular());
    p->addFloat(this->nivel);
    p->addString(m->getModelo()->getNombre());
    p->addString(m->getTextura()->getNombre());

    enviarPaqueteServidor(p);

}

string Asteroide::getNombreServidor(){
    return "Asteroide";
}
