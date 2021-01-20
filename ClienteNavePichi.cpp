#include "ClienteNavePichi.h"
#include "Logger.h"

ClienteNavePichi::ClienteNavePichi():ClienteRed("NavePichi"){
    nave=NULL;
}

ClienteNavePichi::~ClienteNavePichi(){
}


void ClienteNavePichi::onInit(){ //override de ClienteRed
}

void ClienteNavePichi::onDispose(){ //override de ClienteRed
    if(nave)
        nave->setDeadRemoto();
}


ClienteRed* ClienteNavePichi::clonar(){ //Para los prototipos
    return new ClienteNavePichi();
}

void ClienteNavePichi::recibirPaqueteCliente(PaqueteRed* paquete){ //override de ClienteRed

    int numCliente=paquete->getInt();
    int vivo=paquete->getInt();
    Vector posicion=paquete->getVector();
    Vector velocidad=paquete->getVector();
    float vida=paquete->getFloat();
    Vector ejeY=paquete->getVector();
    Vector ejeZ=paquete->getVector();
    float opacidadEspec = paquete->getFloat();
    float cantMunicion = paquete->getFloat();
    float envaiormental = paquete->getFloat(); //envaiormental


    if(!nave){
        nave=new NavePichi(new Objeto3d("nave.mdl","nave.tga"), posicion);
        nave->setRemoto();
        if(numCliente==ControladorRed::getInstancia()->getNumCliente()){
            //TODO: matar a la nave principal y sustituir
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
    if(envaiormental != 0){
        	nave->getModelo()->setModoGeneracionCoordTextura(Enviromental);
            nave->getModelo()->cargarTextura("envmapnave.tga");
            nave->getModelo()->setAfectaLuces(false);
    }
    else{
        nave->getModelo()->setAfectaLuces(true);
		nave->getModelo()->setModoGeneracionCoordTextura(Ninguno);
		nave->getModelo()->cargarTextura("nave.tga");
    }

    if(!vivo && nave->isViva()){
        nave->setViva(false);
        nave->explotar();
    }

}
