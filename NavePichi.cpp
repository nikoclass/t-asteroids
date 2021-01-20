#include "NavePichi.h"

NavePichi::NavePichi(Objeto3d*obj,Vector posicion):Nave(obj,posicion)
{
    this->vida=1000;
	this->vivo=true;
    this->pondDanio = 1;
	this->masa=100;

	this->maxVel=100;
	this->factor=0.9;
	this->danio=0;
	this->counter=0;
	this->velocidad=Vector(0,0,-0.1);
	this->movio=false;
	//this->moviolat= false;
	this->primaria=new FBalaComun(this);
	this->secundaria=new FMisil(this);
	this->incrementoVel = 30;

	//obj->setCullFace(false);
	this->espectro=new Objeto3d(obj->getModelo()->getNombre().data(),"espectro.tga");
	this->espectro->setModoTransparencia(Aditivo);
	this->espectro->setModoGeneracionCoordTextura(Enviromental);
	this->espectro->setAfectaLuces(false);
	this->espectro->setOpacidad(0);
	this->espectro->setColor(0.2,0.2,1);

	this->luz=new Objeto3d("luz2.mdl","luz2.tga",true);
	//this->luz->setOpacidad(0.5);
	this->luz->setAfectaLuces(false);
	this->luz->setModoTransparencia(Aditivo);
	this->luz->setCullFace(false);
	this->luz->setColor(1,1,0.8);

	this->luzFuego=new Luz();
	this->luzFuego->setDistanciaMaxima(50);
	this->luzFuego->setColor(Vector(1,0.8,0.3));

    this->brilloFuego=new Sprite("flare2.tga");
    this->brilloFuego->setModoTransparencia(Aditivo);
    this->brilloFuego->setAfectaZbuffer(false);
    this->brilloFuego->setColor(1,0.9,0.8);

	this->fuego=new Objeto3d("fuegodoble.mdl","fuego.tga");
	this->fuego->setAfectaLuces(false);
	this->fuego->setModoTransparencia(Aditivo);
	//this->fuego->setOpacidad(0.7);
	this->fuego->setCullFace(false);
	this->fuego->setColor(1,0.8,0.3);

	this->fuegolat = new Objeto3d("fuegolateralIzq.mdl","fuego2.tga");
	this->fuegolat->setAfectaLuces(false);
	this->fuegolat->setModoTransparencia(Aditivo);
	this->fuegolat->setCullFace(false);
	this->fuegolat->setColor(1,0.8,0.3);

	this->luzFuegolat=new Luz();
	this->luzFuegolat->setDistanciaMaxima(50);
	this->luzFuegolat->setColor(Vector(1,0.8,0.3));

	Escena* e=ModoJuego::getInstance()->getEscena();
	e->addDibujable(this->luz);
	e->addDibujable(this->fuego);
	e->addDibujable(this->espectro);
	e->addLuz(this->luzFuego);
	e->addDibujable(this->brilloFuego);
	e->addDibujable(this->fuegolat);
	e->addLuz(this->luzFuegolat);

}

NavePichi::~NavePichi()
{
//TRAZA;
    //dtor
    Escena* e=ModoJuego::getInstance()->getEscena();
	e->removeDibujable(this->luz);
	delete this->luz;
}

void NavePichi::explotar(){

    if (!ModoJuego::getInstance()->isModoCliente() && !ModoJuego::getInstance()->isModoSingle())
        return;

    char parte[256];

    Vector pos=this->getModelo()->getPosicion();
    Vector vel=this->getVelocidad();
    Vector velAng=Vector(0,1,0);

    float velNorma=vel.norma();
    float velAngNorma=velAng.norma();


    for(int i=1;i<17;i++){
        sprintf(parte,"parte%d",i);
        Vector vRandom(Random(-1,1),Random(-1,1),Random(-1,1));
        Objeto3d* obj=new Objeto3d("pedazosnave.mdl","nave.tga");
        obj->setNombreMalla(parte);
        obj->setCullFace(false);
        Asteroide* ast=new Asteroide(1,0.9,obj,pos+vRandom);
        Vector w=(velAng^vRandom).direccion();//Perpendicular a la velocidad angular en alguna direccion
        ast->setVelocidad(vRandom*(0.2+vel.norma()));
        ast->setVelocidadAngular(vRandom);
    }
    this->getModelo()->setVisible(false);
    this->espectro->setVisible(false);
    this->luz->setVisible(false);
    //Engine::getInstancia()->setTimeScale(0.1);
    //Engine::getInstancia()->setTimeScale(Settings::getInstance()->getVelocidad(),5);
    new BolaExplosiva(pos);
}

void  NavePichi::ticVivo(float dt){

    if(vivo){
        //cout << "Nave::tic(float dt) begin" << endl;
        this->trasladar(this->velocidad*dt);
        Vector pos=this->getPosicion();
        if(pos.norma()>Settings::getInstance()->getRadioEspacio())
            this->posicionar(-pos+this->velocidad*dt);

        this->getModelo()->setOpacidad(Min(1,this->getModelo()->getOpacidad()+dt*0.1));

        float op=this->espectro->getOpacidad();
        this->espectro->setOpacidad(Max(0,op-dt*0.2));
        //this->espectro->identidad();
        this->espectro->setTransformacion(this->getModelo()->getTransformacion());
        this->espectro->escalar(1.3);


        if (!this->movio){
            this->velocidad*=0.975;
            luzFuego->apagar();
            this->fuego->setVisible(false);
            this->brilloFuego->setVisible(false);
        }
        else{
            float r=Random(0.7,1);
            luzFuego->encender(r*r);

            this->fuego->setVisible(true);
            this->fuego->identidad(); //Esto de aca mas o menos a ojo

            Vector luzF;

            if(adelante){
                this->fuego->cargarModelo("fuegodoble.mdl");
                this->fuego->cargarTextura("fuego.tga");
                this->fuego->escalar(Vector(0.6,0.5,r*(1+ 0.05*this->velocidad.norma())));
                this->fuego->posicionar(Vector(0,0.25,1.7));
                luzF=Vector(0,2,2);
                this->luzFuego->setColor(Vector(1,0.8,0.3));

                this->brilloFuego->setVisible(true);
                this->brilloFuego->identidad();
                //this->brilloFuego->escalar(Vector(2,1.5,1));
                this->brilloFuego->escalar(Random(2,2.2));
                this->brilloFuego->trasladar(Vector(0,0.3,1.8));
                this->brilloFuego->aplicarTransformacion(this->getModelo()->getTransformacion());

            }else{//atras
                this->fuego->cargarModelo("fuegodelante.mdl");
                this->fuego->cargarTextura("fuego2.tga");
                this->fuego->escalar(Vector(1,1,r*(0.5+ 0.05*this->velocidad.norma())));
                //this->fuego->posicionar(Vector(0,0.5,-1.2));
                luzF=Vector(0,2,-2);
                this->luzFuego->setColor(Vector(1,0.5,0.5));

                this->brilloFuego->setVisible(false);
            }

            this->fuego->aplicarTransformacion(this->getModelo()->getTransformacion());

            luzF=this->getModelo()->getTransformacion()*luzF;
            luzFuego->setPos(luzF);
        }

        if (!this->moviolat){
            //this->velocidad*=0.975;
            luzFuegolat->apagar();
            this->fuegolat->setVisible(false);

        }
        else{
            float r=Random(0.7,1);
            luzFuegolat->encender(r*r);
            this->fuegolat->setVisible(true);
            this->fuegolat->identidad(); //Esto de aca mas o menos a ojo

            Vector luzF;
            if(lateralD){
                this->fuegolat->cargarModelo("fuegolateralIzq.mdl");
                this->fuegolat->cargarTextura("fuego2.tga");
                //this->fuego->posicionar(Vector(0,0.5,-1.2));
                luzF=Vector(2,2,-2);
                this->luzFuegolat->setColor(Vector(1,0.5,0.5));

            }
            else{
                this->fuegolat->cargarModelo("fuegolateralDer.mdl");
                this->fuegolat->cargarTextura("fuego2.tga");
                //this->fuego->posicionar(Vector(0,0.5,-1.2));
                luzF=Vector(-2,2,-2);
                this->luzFuegolat->setColor(Vector(1,0.5,0.5));


            }

            this->fuegolat->escalar(Vector(1.2*r,1,1));
            this->fuegolat->aplicarTransformacion(this->getModelo()->getTransformacion());
            luzF=this->getModelo()->getTransformacion()*luzF;
            luzFuegolat->setPos(luzF);
        }

        this->movio=false;this->moviolat=false;
        this->primaria->tic(dt);
        this->secundaria->tic(dt);

        float ts=Engine::getInstancia()->getTimeScale()/Settings::getInstance()->getVelocidad();
        if(ts>1)ts=1;
        ts*=ts*ts*ts*ts;
        ts*=ts;
        this->luz->identidad();
        this->luz->setOpacidad(ts*0.8);
        this->luz->escalar(Vector(2.5*ts,2*ts,6));
        this->luz->trasladar(Vector(0,0.2,-1.5));
        this->luz->aplicarTransformacion(this->getModelo()->getTransformacion());

        this->counter=(this->counter>0)?this->counter-dt:0;
        if(this->danio && !this->counter){
            this->danio=0;
            this->masa=100;
            this->getModelo()->setAfectaLuces(true);
            this->getModelo()->setModoGeneracionCoordTextura(Ninguno);
            this->getModelo()->cargarTextura("nave.tga");
        }
        //cout << "Nave::tic(float dt) end" << endl;
        ///Esto es para experimentar luego
        ///ModoJuego::getInstance()->getCamaraPrincipal()->setFov(70 + this->velocidad.norma()*1.09);
    }


    PaqueteRed *p=new PaqueteRed();
    Objeto3d *m=this->getModelo();

    p->addInt(this->getNumCliente());
    p->addInt(this->vivo);
    p->addVector(m->getPosicion());
    p->addVector(this->velocidad);
    p->addFloat(this->vida);
    p->addVector(m->getEjeY());
    p->addVector(m->getEjeZ());
    p->addFloat(this->espectro->getOpacidad());
    p->addFloat(this->secundaria->getcantMunicion());
    p->addFloat(this->counter);

    enviarPaqueteServidor(p);

}


void NavePichi::recibirPaqueteServidor(PaqueteRed* paquete){
}

void NavePichi::disparar(){

    if(quieroDispararPrimara){
        Vector posicion=this->getPosicion()+this->primaria->getLado()*this->getModelo()->getEjeX();
        this->primaria->disparar(posicion,-this->getModelo()->getEjeZ());
    }

    if(quieroDispararSecundaria){
        Vector posicion=this->getPosicion()+this->secundaria->getLado()*this->getModelo()->getEjeX();
        Vector direccion=this->getModelo()->getEjeZ()*-1+this->getModelo()->getEjeZ()*-this->velocidad.norma();
        this->secundaria->disparar(posicion,direccion);
    }

}


string NavePichi::getNombreServidor(){
    return "NavePichi";
}
