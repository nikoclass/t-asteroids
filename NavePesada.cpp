#include "NavePesada.h"
#include "Logger.h"
#include "Material.h"


class Asteroide;

NavePesada::NavePesada(Objeto3d*obj,Vector posicion): Nave(obj, posicion)
{
    //    cout << "Llamo constructor nave PESADA" << endl;
    this->vida=1000;
    this->vivo=true;
    this->masa=500;

    this->maxVel=100;
    this->factor=0.9;
    this->danio=60;
    this->counter=0;
    this->counterEmb = 0;
    this->pondDanio = 0.6;

    //this->velocidad=Vector(0,0,-0.1);
    this->movio=false;
    //this->moviolat= false;
    this->primaria=new FBalaComun(this);
    this->secundaria=new FEmbestida(this);
    this->getModelo()->setOpacidad(1);
    this->incrementoVel = 20;


    //obj->setCullFace(false);
    this->espectro=new Objeto3d(obj->getModelo()->getNombre().data(),"espectro.tga");
    this->espectro->setModoTransparencia(Aditivo);
    this->espectro->setModoGeneracionCoordTextura(Enviromental);
    this->espectro->setAfectaLuces(false);
    this->espectro->setOpacidad(0);
    this->espectro->setColor(0,1,1);

    this->luzFuego=new Luz();
    this->luzFuego->setDistanciaMaxima(50);
    this->luzFuego->setColor(Vector(0,0.5,0.5));

    this->brilloFuegoP1=new Sprite("flare2.tga");
    this->brilloFuegoP1->setModoTransparencia(Aditivo);
    this->brilloFuegoP1->setAfectaZbuffer(false);

    this->brilloFuegoP2=new Sprite("flare2.tga");
    this->brilloFuegoP2->setModoTransparencia(Aditivo);
    this->brilloFuegoP2->setAfectaZbuffer(false);

    this->brilloFuegoN2=new Sprite("flare2.tga");
    this->brilloFuegoN2->setModoTransparencia(Aditivo);
    this->brilloFuegoN2->setAfectaZbuffer(false);


    this->brilloFuegoN1=new Sprite("flare2.tga");
    this->brilloFuegoN1->setModoTransparencia(Aditivo);
    this->brilloFuegoN1->setAfectaZbuffer(false);

    this->fuego=new Objeto3d("FuegoPesada.mdl","Blue_fire.tga");
    this->fuego->setAfectaLuces(false);
    this->fuego->setModoTransparencia(Aditivo);
    this->fuego->setOpacidad(0.7);
    this->fuego->setCullFace(false);


    this->fuegoP = new Objeto3d("FuegoPesada2.mdl","Blue_fire.tga");
    this->fuegoP->setAfectaLuces(false);
    this->fuegoP->setModoTransparencia(Aditivo);
    this->fuegoP->setOpacidad(0.7);
    this->fuegoP->setCullFace(false);


    Escena* e=ModoJuego::getInstance()->getEscena();
    e->addDibujable(this->espectro);
    e->addLuz(this->luzFuego);
    e->addDibujable(this->fuego);
    e->addDibujable(this->fuegoP);
    e->addDibujable(this->brilloFuegoN2);
    e->addDibujable(this->brilloFuegoN1);
    e->addDibujable(this->brilloFuegoP1);
    e->addDibujable(this->brilloFuegoP2);
}

NavePesada::~NavePesada()
{
    Escena* e=ModoJuego::getInstance()->getEscena();

    e->removeDibujable(this->fuegoP);
    e->removeDibujable(this->brilloFuegoN2);
    e->removeDibujable(this->brilloFuegoN1);
    e->removeDibujable(this->brilloFuegoP1);
    e->removeDibujable(this->brilloFuegoP2);

    delete this->fuegoP;
    delete this->brilloFuegoN1;
    delete this->brilloFuegoN2;
    delete this->brilloFuegoP1;
    delete this->brilloFuegoP2;
}

void NavePesada::explotar(){

    if (!ModoJuego::getInstance()->isModoCliente() && !ModoJuego::getInstance()->isModoSingle())
        return;

    char parte[256];

    Vector pos=this->getModelo()->getPosicion();
    Vector vel=this->getVelocidad();
    Vector velAng=Vector(0,1,0);

    float velNorma=vel.norma();
    float velAngNorma=velAng.norma();


    for (int i=1;i<70;i++)
    {
        sprintf(parte,"mesh%d",i);
        Vector vRandom(Random(-1,1),Random(-1,1),Random(-1,1));
        Objeto3d* obj=new Objeto3d("NavePesadaPartes.mdl","NF_Coc02_bk.tga");
        obj->setNombreMalla(parte);
        obj->setCullFace(false);
        Asteroide* ast=new Asteroide(1,0.9,obj,pos+vRandom);
        Vector w=(velAng^vRandom).direccion();//Perpendicular a la velocidad angular en alguna direccion
        ast->setVelocidad(vRandom*(0.2+vel.norma()));
        ast->setVelocidadAngular(vRandom);
    }

    this->getModelo()->setVisible(false);
    //Engine::getInstancia()->setTimeScale(0.1);
    //Engine::getInstancia()->setTimeScale(Settings::getInstance()->getVelocidad(),5);
    new BolaExplosiva(pos);
    this->espectro->setVisible(false);
    this->fuego->setVisible(false);
    this->brilloFuegoN1->setVisible(false);
    this->brilloFuegoN2->setVisible(false);
}


void NavePesada::ticVivo(float dt){

    if(vivo){

        this->trasladar(this->velocidad*dt);
        Vector pos=this->getPosicion();
        if (pos.norma()>Settings::getInstance()->getRadioEspacio())
            this->posicionar(-pos+this->velocidad*dt);

        Objeto3d* miModelo = this->getModelo();
        miModelo->setOpacidad(Min(1,miModelo->getOpacidad()+dt*0.1));

        float op=this->espectro->getOpacidad();
        this->espectro->setOpacidad(Max(0,op-dt*0.2));
        //this->espectro->identidad();
        this->espectro->setTransformacion(miModelo->getTransformacion());
        this->espectro->escalar(1.3);

        if (!this->movio)
        {
            this->velocidad*=0.975;
            luzFuego->apagar();
            this->fuego->setVisible(false);
            this->fuegoP->setVisible(false);
            this->brilloFuegoN1->setVisible(false);
            this->brilloFuegoN2->setVisible(false);
            this->brilloFuegoP1->setVisible(false);
            this->brilloFuegoP2->setVisible(false);

        }
        else
        {
            float r=Random(0.7,1);
            luzFuego->encender(r*r);
            this->fuego->identidad();

            Vector luzF;
            Vector v(0,Random(),Random(0.5,1));
            if (adelante)
            {
                this->fuego->setVisible(true);
                this->brilloFuegoN1->setVisible(true);
                this->brilloFuegoN2->setVisible(true);

                this->fuego->cargarModelo("FuegoPesada.mdl");
                this->fuego->cargarTextura("Blue_fire.tga");
                this->fuego->escalar(Vector(1,1,r*(1 + 0.02*this->velocidad.norma())));
                this->fuego->posicionar(Vector(0,0,1.90));
                this->fuego->setColor(v);

                this->fuegoP->identidad();
                //this->fuegoP->setVisible(this->counterEmb);
                this->fuegoP->escalar(Vector(1,1,r*(1 + 0.01*this->velocidad.norma())));
                this->fuegoP->posicionar(Vector(0,-0.25,2.90));
                this->fuegoP->setColor(v);

                this->luzFuego->setColor(Vector(0,0.5,0.5));

                this->brilloFuegoN1->identidad();
                this->brilloFuegoN1->escalar(Random(2,2.2));
                this->brilloFuegoN1->trasladar(Vector(0,0.3,1.8));
                this->brilloFuegoN1->posicionar(Vector(2.25,-0.60,1.90));
                this->brilloFuegoN1->aplicarTransformacion(miModelo->getTransformacion());
                this->brilloFuegoN1->setColor(v);

                this->brilloFuegoN2->identidad();
                this->brilloFuegoN2->escalar(Random(2,2.2));
                this->brilloFuegoN2->trasladar(Vector(0,0.3,1.8));
                this->brilloFuegoN2->posicionar(Vector(-2.25,-0.60,1.90));
                this->brilloFuegoN2->aplicarTransformacion(miModelo->getTransformacion());
                this->brilloFuegoN2->setColor(v);

                this->brilloFuegoP1->identidad();
                this->brilloFuegoP1->escalar(Random(2.5,3));
                this->brilloFuegoP1->trasladar(Vector(0,0.3,1.8));
                this->brilloFuegoP1->posicionar(Vector(2.25,-0.45,1.90));
                this->brilloFuegoP1->aplicarTransformacion(miModelo->getTransformacion());
                this->brilloFuegoP1->setColor(v);
                this->brilloFuegoP1->setVisible(this->counterEmb);

                this->brilloFuegoP2->identidad();
                this->brilloFuegoP2->escalar(Random(2.5,4));
                this->brilloFuegoP2->trasladar(Vector(0,0.3,1.8));
                this->brilloFuegoP2->posicionar(Vector(-2.25,-0.45,1.90));
                this->brilloFuegoP2->aplicarTransformacion(miModelo->getTransformacion());
                this->brilloFuegoP2->setColor(v);
                this->brilloFuegoP2->setVisible(this->counterEmb);

            }
            else //atras
            {
                this->fuego->cargarModelo("FuegoPesadaD.mdl");
                this->fuego->cargarTextura("blue_fire-med.tga");
                this->fuego->escalar(Vector(1,Random(0.8,1),r*(1+ 0.02*this->velocidad.norma())));
                this->fuego->setColor(v);
                this->luzFuego->setColor(v);
            }

            this->fuego->aplicarTransformacion(miModelo->getTransformacion());
            this->fuegoP->aplicarTransformacion(miModelo->getTransformacion());
            luzF=miModelo->getTransformacion()*luzF;
            luzFuego->setPos(luzF);
        }

        this->movio=false;
        this->primaria->tic(dt);
        float ts=Engine::getInstancia()->getTimeScale()/Settings::getInstance()->getVelocidad();
        if (ts>1)ts=1;

        this->counter=(this->counter>0)?this->counter-dt:0;
        if (this->counterEmb>0){
             this->counterEmb -=dt;
             this->maxVel = 400;
             this->getModelo()->setColor(Random(0.3,1),Random(0.2,0.6),Random(0.5,0.7));
             this->incrementoVel = 300;
             ModoJuego::getInstance()->getCamaraPrincipal()->setFov(70 + (this->velocidad.norma()<80?this->velocidad.norma():80));
        }
        else{
            this->counterEmb = 0;
            ModoJuego::getInstance()->getCamaraPrincipal()->setFov(70);
        }



        if (this->danio>60 && !(this->counter || this->counterEmb))
        {
            this->danio=60;
            this->masa=100;
            this->maxVel = 100;
            this->brilloFuegoP1->setVisible(false);
            this->brilloFuegoP2->setVisible(false);
            this->fuegoP->setVisible(false);
            this->incrementoVel = 20;
            miModelo->setAfectaLuces(true);
            miModelo->setModoGeneracionCoordTextura(Ninguno);
            miModelo->cargarTextura("NF_Coc02_bk.tga");
        }
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
    p->addFloat(this->counterEmb);
    p->addFloat(this->espectro->getOpacidad());
    p->addFloat(this->secundaria->getcantMunicion());
    p->addFloat(this->counter);
    enviarPaqueteServidor(p);

}


void NavePesada::disparar(){
    if(quieroDispararPrimara){
        Vector posicion = this->getPosicion() + this->primaria->getLado()*this->getModelo()->getEjeX();
        this->primaria->disparar(posicion,this->getModelo()->getEjeZ()*-1);
    }

    if(quieroDispararSecundaria){
        this->secundaria->disparar(Vector(),Vector());
        TRAZA;
    }

}



void NavePesada::recibirPaqueteServidor(PaqueteRed* paquete){
}


string NavePesada::getNombreServidor(){
    return "NavePesada";
}
