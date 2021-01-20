#include "Embestida.h"
#include "Nave.h"
#include "ModoJuego.h"
#include "Asteroide.h"

Embestida::Embestida(Objeto3d* obj, Vector pos):Colisionable(obj,pos){
    this->Ttl = 4;
    this->esActivo = false;
    this->masa = 100000;
    this->danio = 10000;

    this->fuego = new Objeto3d();
    this->fuego->setAfectaLuces(false);
    this->fuego->setModoTransparencia(Aditivo);
    this->fuego->setOpacidad(0.7);
    this->fuego->setCullFace(false);
    this->fuego->escalar(1.5);

    obj->setModoTransparencia(Aditivo);
    obj->setModoGeneracionCoordTextura(Enviromental);
    obj->setAfectaLuces(false);
    obj->setOpacidad(0);
    obj->setColor(1,0.5,1);


    Escena* e = ModoJuego::getInstance()->getEscena();
    e->addDibujable(this->fuego);
}

Embestida::~Embestida()
{
    delete this->fuego;
}

void Embestida::tic(float dt){
    //TODO: verifique si esta activo , y anime el fuego de ser encesario
    float r=Random(0.7,1);
    Nave* n = ModoJuego::getInstance()->getNavePrinc();
    Vector v(0,Random(),Random(0.5,1));
    if(this->esActivo){
            this->getModelo()->setVisible(true);
            this->getModelo()->setOpacidad(1);
            this->getModelo()->setColor(Vector(1,Random(),1));

            this->fuego->cargarModelo(n->getFuego()->getModelo()->getNombre().data());
            this->fuego->cargarTextura("VioletFlames.tga");
            this->fuego->escalar(Vector(1,Random(0.8,1),r*(1+ 0.02*n->getVelocidad().norma())));
            this->fuego->posicionar(n->getFuego()->getPosicion() + Vector(0,0.20,0));//TODO: Hacer un dynamic cast para saber a que nave hay que colocarlo
            this->fuego->setColor(Vector(Random(),1,1));
            if(this->Ttl > 0)
                this->Ttl -= dt;
            else{
                this->esActivo = false;
                this->Ttl = 4;
            }

            ModoJuego::getInstance()->getCamaraPrincipal()->setFov(150);

    }
    else{
        ModoJuego::getInstance()->getCamaraPrincipal()->setFov(70);
        this->getModelo()->setOpacidad(0);
    }
    if(n->getMovio()){
        TRAZA;
        this->fuego->aplicarTransformacion(n->getModelo()->getTransformacion()); //TODO: sincronizar el movimiento bien
        this->getModelo()->aplicarTransformacion(n->getModelo()->getTransformacion());
    }

}


void Embestida::bang(Colisionable* who){
    //TODO: maneja la colision con el resto de los objetos
    if(Asteroide* asteroide = dynamic_cast<Asteroide*>( who )){
        Nave* n = ModoJuego::getInstance()->getNavePrinc();
        Vector v=asteroide->getVelocidad()-n->getVelocidad();
        float m1= this->masa;
        float m2=asteroide->getMasa();
        float c1=m1/(m1+m2);
        float c2=m2/(m1+m2);
        Esfera suEsfera=asteroide->getEsfera();
        Esfera miEsfera=this->getEsfera();
        Vector dir=miEsfera.getCentro()-suEsfera.getCentro();
        float normaDir=dir.normalizar();

        Vector proyeccion=ProyeccionOrtogonal(v,dir);
        asteroide->setVelocidad(asteroide->getVelocidad()-proyeccion*2*c1);
        float dt=Engine::getInstancia()->getDt();

        if (dir==Vector(0,0,0))
              dir=Vector(dt,dt,dt);

        float dif=miEsfera.getRadio()+suEsfera.getRadio()-normaDir;
        asteroide->trasladar(-dif*dir*(c2+dt));
        }
}
