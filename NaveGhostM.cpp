#include "NaveGhostM.h"

NaveGhostM::NaveGhostM(Objeto3d*obj,Vector posicion):Nave(obj,posicion)
{
//    cout << "Llamo constructor nave Ghost" << endl;
    this->vida=500;
    this->vivo=true;
    this->pondDanio = 1.3;

    this->masa=100;

    this->maxVel=200;
    this->factor=0.9;
    this->danio=0;
    this->counter=0;
    this->velocidad=Vector(0,0,-0.1);
    this->movio=false;
    //this->moviolat= false;
    this->primaria=new FBalaComun(this);
    this->secundaria=new FElectricidad(this);
    this->getModelo()->setOpacidad(0.5);
    this->getModelo()->setCullFace(false);
    this->incrementoVel = 50;

    //obj->setCullFace(false);
    this->espectro=new Objeto3d(obj->getModelo()->getNombre().data(),"espectro.tga");
    this->espectro->setModoTransparencia(Aditivo);
    this->espectro->setModoGeneracionCoordTextura(Enviromental);
    this->espectro->setAfectaLuces(false);
    this->espectro->setOpacidad(0);
    this->espectro->setColor(0.5,0,0.7);

    this->luzFuego=new Luz();
    this->luzFuego->setDistanciaMaxima(50);
    this->luzFuego->setColor(Vector(1,0.8,0.3));

    this->brilloFuego=new Sprite("flare2.tga");
    this->brilloFuego->setModoTransparencia(Aditivo);
    this->brilloFuego->setAfectaZbuffer(false);
    this->brilloFuego->setColor(1,0.5,0.5);

    this->brilloFuegoD=new Sprite("flare2.tga");
    this->brilloFuegoD->setModoTransparencia(Aditivo);
    this->brilloFuegoD->setAfectaZbuffer(false);
    this->brilloFuegoD->setColor(1,0.5,0.5);

    this->brilloFuegoI=new Sprite("flare2.tga");
    this->brilloFuegoI->setModoTransparencia(Aditivo);
    this->brilloFuegoI->setAfectaZbuffer(false);
    this->brilloFuegoI->setColor(1,0.5,0.5);

    //this->Elect->rotable(true);


    this->fuego=new Objeto3d("fuegoG.mdl","fuego.tga");
    this->fuego->setAfectaLuces(false);
    this->fuego->setModoTransparencia(Aditivo);
    //this->fuego->setOpacidad(0.7);
    this->fuego->setCullFace(false);
    this->fuego->setColor(1,0,0);

    this->fuegoD = new Objeto3d("fuegoG.mdl","fuego.tga");
    this->fuegoD->setAfectaLuces(false);

    this->fuegoD->setModoTransparencia(Aditivo);
    this->fuegoD->setCullFace(false);
    this->fuegoD->setColor(1,0,0);

    this->fuegoI = new Objeto3d("fuegoG.mdl","fuego.tga");
    this->fuegoI->setAfectaLuces(false);
    this->fuegoI->setModoTransparencia(Aditivo);
    this->fuegoI->setCullFace(false);
    this->fuegoI->setColor(1,0,0);

    this->targetElectricidad=NULL;

    //this->Central=new Luz();
    //this->Central->setDistanciaMaxima(10);
    //this->Central->setColor(Vector(0,1,0));


    electricidad=new Electricidad();
    electricidad->setCantPartes(5);
    electricidad->setCurvatura(0.4);
    electricidad->setFrecuencia(50);
    electricidad->setPosIni(Vector(-0.7,0.1,-3.4));
    electricidad->setPosFin(Vector(0.7,0.1,-3.4));


    Escena* e=ModoJuego::getInstance()->getEscena();
    e->addDibujable(this->fuego);
    e->addDibujable(this->espectro);
    e->addLuz(this->luzFuego);
    e->addDibujable(this->brilloFuego);
    e->addDibujable(this->fuegoD);
    e->addDibujable(this->fuegoI);
    e->addDibujable(this->brilloFuegoD);
    e->addDibujable(this->brilloFuegoI);


    //	cout << "Termino de crear la nave" << endl;
}

NaveGhostM::~NaveGhostM()
{
    //dtor
    Escena* e=ModoJuego::getInstance()->getEscena();

    e->removeDibujable(this->brilloFuegoD);
    e->removeDibujable(this->brilloFuegoI);
    e->removeDibujable(this->fuegoI);
    e->removeDibujable(this->fuegoD);
    MFisica::getInstance()->removeAnimable(this->electricidad);
    delete this->electricidad;

    delete this->fuegoI;
    delete this->brilloFuegoI;
    delete this->fuegoD;
    delete this->brilloFuegoD;
}

void NaveGhostM::explotar()
{
    if (!ModoJuego::getInstance()->isModoCliente() && !ModoJuego::getInstance()->isModoSingle())
        return;

    char parte[256];

    Vector pos=this->getModelo()->getPosicion();
    Vector vel=this->getVelocidad();
    Vector velAng=Vector(0,1,0);

    float velNorma=vel.norma();
    float velAngNorma=velAng.norma();


    for (int i=1;i<30;i++)
    {
        sprintf(parte,"parte%d",i);
        Vector vRandom(Random(-1,1),Random(-1,1),Random(-1,1));
        Objeto3d* obj=new Objeto3d("GhostMpartes.mdl","Fuselage2.tga");
        obj->setNombreMalla(parte);
        obj->setCullFace(false);
        Asteroide* ast=new Asteroide(1,0.9,obj,pos+vRandom);
        Vector w=(velAng^vRandom).direccion();//Perpendicular a la velocidad angular en alguna direccion
        ast->setVelocidad(vRandom*(0.2+vel.norma()));
        ast->setVelocidadAngular(vRandom);
    }
    this->getModelo()->setVisible(false);
    this->electricidad->getMultiobjeto()->setVisible(false);
    //Engine::getInstancia()->setTimeScale(0.1);
    //Engine::getInstancia()->setTimeScale(Settings::getInstance()->getVelocidad(),5);
    new BolaExplosiva(pos);
}


void NaveGhostM::disparar(){

    if(quieroDispararPrimara){
        Vector posicion = this->getPosicion() + this->primaria->getLado()*this->getModelo()->getEjeX();
        this->primaria->disparar(posicion,this->getModelo()->getEjeZ()*-1);
    }

    this->targetElectricidad=NULL;

    if(quieroDispararSecundaria){
        Vector p(0,0,-3.2);
        p=this->getModelo()->getTransformacion()*p;
        this->secundaria->disparar(p,-this->getModelo()->getEjeZ());

        SoundManager *sm=SoundManager::getInstance();

        if(!sm->isPlaying("electricidad.mp3"))
            sm->PlayGameSound("electricidad.mp3");

    }


}

void NaveGhostM::ticVivo(float dt)
{
    if(vivo){

        //cout << "NaveGhost::tic(float dt) begin" << endl;
        this->trasladar(this->velocidad*dt);
        Vector pos=this->getPosicion();
        if (pos.norma()>Settings::getInstance()->getRadioEspacio())
            this->posicionar(-pos+this->velocidad*dt);

        Objeto3d* miModelo = this->getModelo();
        miModelo->setOpacidad(Min(0.5,miModelo->getOpacidad()+dt*0.1));

        float op=this->espectro->getOpacidad();
        this->espectro->setOpacidad(Max(0,op-dt*0.2));
        //this->espectro->identidad();
        this->espectro->setTransformacion(miModelo->getTransformacion());
        this->espectro->escalar(1.3);

        //this->primaria->balasRojas(); // dos veces balas rojas? esta bien?

        if (!this->movio)
        {
            this->velocidad*=0.975;
            luzFuego->apagar();
            this->fuego->setVisible(false);
            this->fuegoD->setVisible(false);
            this->fuegoI->setVisible(false);
            this->brilloFuego->setVisible(false);
            this->brilloFuegoD->setVisible(false);
            this->brilloFuegoI->setVisible(false);
        }
        else
        {
            float r=Random(0.7,1);
            luzFuego->encender(r*r);
                this->fuego->setVisible(true);
                this->fuegoD->setVisible(true);
                this->fuegoI->setVisible(true);
            this->fuegoD->identidad();
            this->fuegoI->identidad();
            this->fuego->identidad(); //Esto de aca mas o menos a ojo

            Vector luzF;

            if (adelante)
            {
                //TODO: Sacar la triple declaracion y usar solo un modelo PUEDE ESPERAR!!!
                this->fuego->cargarModelo("fuegoG.mdl");
                this->fuego->cargarTextura("fuego.tga");

                this->fuegoD->cargarModelo("fuegoG.mdl");
                this->fuegoD->cargarTextura("fuego.tga");

                this->fuegoI->cargarModelo("fuegoG.mdl");
                this->fuegoI->cargarTextura("fuego.tga");

                this->brilloFuego->setVisible(true);
                this->brilloFuegoI->setVisible(true);
                this->brilloFuegoD->setVisible(true);
                this->fuego->escalar(Vector(0.6,0.5,r*(1+ 0.05*this->velocidad.norma())));
                this->fuego->posicionar(Vector(0,0.1,3.40));
                //luzF=Vector(0,2,2);
                this->luzFuego->setColor(Vector(1,0.8,0.3));

                this->fuegoD->escalar(Vector(0.6,0.5,r*(1+ 0.05*this->velocidad.norma())));
                this->fuegoD->posicionar(Vector(0.37,-0.24,2.8));

                this->fuegoI->escalar(Vector(0.6,0.5,r*(1+ 0.05*this->velocidad.norma())));
                this->fuegoI->posicionar(Vector(-0.37,-0.24,2.8));
                luzF=Vector(0,2,2);

                this->brilloFuego->identidad();
                this->brilloFuego->escalar(Random(2,2.2));
                this->brilloFuego->trasladar(Vector(0,0.3,1.8));
                this->brilloFuego->posicionar(Vector(0,0.1,3.40));
                this->brilloFuego->aplicarTransformacion(miModelo->getTransformacion());

                this->brilloFuegoI->identidad();
                this->brilloFuegoI->escalar(Random(2,2.2));
                this->brilloFuegoI->trasladar(Vector(0,0.3,1.8));
                this->brilloFuegoI->posicionar(Vector(-0.37,-0.24,2.8));
                this->brilloFuegoI->aplicarTransformacion(miModelo->getTransformacion());

                this->brilloFuegoD->identidad();
                this->brilloFuegoD->escalar(Random(2,2.2));
                this->brilloFuegoD->trasladar(Vector(0,0.3,1.8));
                this->brilloFuegoD->posicionar(Vector(0.37,-0.24,2.8));
                this->brilloFuegoD->aplicarTransformacion(miModelo->getTransformacion());

            }
            else //atras
            {
                this->fuego->cargarModelo("FuegoGhostTrasero.mdl");
                this->fuego->cargarTextura("fuegoTraseroG.tga");
                //this->fuego->rotar(PI,this->fuego->getEjeX()- Vector(0,0,0.5));
                this->fuego->posicionar(Vector(0,-0.05,2));
                this->fuego->escalar(Vector(1,1,r*(1+ 0.05*this->velocidad.norma())));
                this->luzFuego->setColor(Vector(1,0.5,0.5));
                this->fuegoD->setVisible(false);
                this->fuegoI->setVisible(false);
            }

            this->fuego->aplicarTransformacion(miModelo->getTransformacion());
            this->fuegoI->aplicarTransformacion(miModelo->getTransformacion());
            this->fuegoD->aplicarTransformacion(miModelo->getTransformacion());
            luzF=miModelo->getTransformacion()*luzF;
            luzFuego->setPos(luzF);
        }

        this->movio=false;
        this->primaria->tic(dt);
        this->secundaria->tic(dt);
        float ts=Engine::getInstancia()->getTimeScale()/Settings::getInstance()->getVelocidad();
        if (ts>1)ts=1;
        //ts*=ts*ts*ts*ts;
        //ts*=ts;
        this->counter=(this->counter>0)?this->counter-dt:0;
        if (this->danio && !this->counter)
        {
            this->danio=0;
            this->masa=100;
            miModelo->setAfectaLuces(true);
            miModelo->setModoGeneracionCoordTextura(Ninguno);
            miModelo->cargarTextura("Fuselage2.tga");
        }

        this->electricidad->getMultiobjeto()->setTransformacion(this->getModelo()->getTransformacion());
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

    if(this->quieroDispararSecundaria && this->targetElectricidad){
        p->addInt(this->quieroDispararSecundaria);
        p->addVector(this->targetElectricidad->getPosicionTarget());
    }else{
        p->addInt(0);
        p->addVector(Vector());
    }
    enviarPaqueteServidor(p);

    this->targetElectricidad=NULL;

}

void NaveGhostM::recibirPaqueteServidor(PaqueteRed* paquete){};
string NaveGhostM::getNombreServidor(){
    return "NaveGhostM";
}
