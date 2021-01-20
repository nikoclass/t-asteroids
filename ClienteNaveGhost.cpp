#include "ClienteNaveGhost.h"
#include "Nave.h"
#include "NaveGhostM.h"

ClienteNaveGhost::ClienteNaveGhost():ClienteRed("NaveGhostM")
{
    this->nave = NULL;
}

ClienteNaveGhost::~ClienteNaveGhost()
{

}


void ClienteNaveGhost::recibirPaqueteCliente(PaqueteRed* paquete){

    int numCliente=paquete->getInt();
    int vivo=paquete->getInt();
    Vector posicion=paquete->getVector();
    Vector velocidad=paquete->getVector();
    float vida=paquete->getFloat();
    Vector ejeY=paquete->getVector();
    Vector ejeZ=paquete->getVector();
    float opacidadEspec = paquete->getFloat();
    float cantMunicion = paquete->getFloat();
    float envaiormental = paquete->getFloat();
    int dispararElectricidad=paquete->getInt();
    Vector destinoElectricidad=paquete->getVector();

    if(!nave){
        this->nave = new NaveGhostM(new Objeto3d("GhostM.mdl","Fuselage2.tga"), Vector());
        this->nave->setRemoto();
        if(numCliente==ControladorRed::getInstancia()->getNumCliente()){
            ModoJuego::getInstance()->getNavePrinc()->setDead(true);
            ModoJuego::getInstance()->setNavePrinc(nave);
        }
    }

    Objeto3d *m=nave->getModelo();

    if(ModoJuego::getInstance()->getNavePrinc()!=nave)
        m->posicionar(posicion);

    nave->setVelocidad(velocidad);
    nave->setVida(vida);
    m->rotar(m->getEjeY(),ejeY);
    m->rotar(m->getEjeZ(),ejeZ);
    nave->getEspectro()->setOpacidad(opacidadEspec);
    nave->getSecundaria()->setcantMunicion(cantMunicion);
    if(envaiormental!=0){
        	nave->getModelo()->setModoGeneracionCoordTextura(Enviromental);
            nave->getModelo()->cargarTextura("envmapnave.tga");
            nave->getModelo()->setAfectaLuces(false);
    }
    else{
        nave->getModelo()->setAfectaLuces(true);
		nave->getModelo()->setModoGeneracionCoordTextura(Ninguno);
		nave->getModelo()->cargarTextura("Fuselage2.tga");
    }

    if(dispararElectricidad){
        Electricidad *e=new Electricidad(0.2);

        e->setCantPartes(destinoElectricidad.distancia(posicion)/2);
        e->setPosIni(posicion);
        e->setCurvatura(0.05);
        e->setPosFin(destinoElectricidad);
        //e->setFrecuencia(50);
        e->setFrecuencia(1);
    }

    if(!vivo && nave->isViva()){
        nave->setViva(false);
        nave->explotar();
    }


}
void ClienteNaveGhost::onInit(){


}

void ClienteNaveGhost::onDispose(){
    if(this->nave)
        this->nave->setDeadRemoto();
}

ClienteRed* ClienteNaveGhost::clonar(){
    return new ClienteNaveGhost();
}
