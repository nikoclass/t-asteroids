#include "ClienteNavePesada.h"
#include "NavePesada.h"

ClienteNavePesada::ClienteNavePesada():ClienteRed("NavePesada")
{
    this->nave = NULL;
}

ClienteNavePesada::~ClienteNavePesada()
{
    //dtor
}

void ClienteNavePesada::recibirPaqueteCliente(PaqueteRed* paquete){ //override de ClienteRed
    int numCliente=paquete->getInt();
    int vivo=paquete->getInt();
    Vector posicion=paquete->getVector();
    Vector velocidad=paquete->getVector();
    float vida=paquete->getFloat();
    Vector ejeY=paquete->getVector();
    Vector ejeZ=paquete->getVector();
    float counterEmbs = paquete->getFloat();
    float opacidadEspc = paquete->getFloat();
    float cantMunicion = paquete->getFloat();
    float envaiormental = paquete->getFloat();


    if(!nave){
        nave=new NavePesada(new Objeto3d("NavePesada.mdl","NF_Coc02_bk.tga"), posicion);
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
    if(counterEmbs > 0)
        ModoJuego::getInstance()->getCamaraPrincipal()->setFov(70 + (velocidad.norma()<80?velocidad.norma():80));
    nave->setCounterEmb(counterEmbs);
    nave->getEspectro()->setOpacidad(opacidadEspc);
    nave->getSecundaria()->setcantMunicion(cantMunicion);
    if(envaiormental!=0){
        	nave->getModelo()->setModoGeneracionCoordTextura(Enviromental);
            nave->getModelo()->cargarTextura("envmapnave.tga");
            nave->getModelo()->setAfectaLuces(false);
    }
    else{
        nave->getModelo()->setAfectaLuces(true);
		nave->getModelo()->setModoGeneracionCoordTextura(Ninguno);
		nave->getModelo()->cargarTextura("NF_Coc02_bk.tga");
    }

    if(!vivo && nave->isViva()){
        nave->setViva(false);
        nave->explotar();
    }

}
ClienteRed* ClienteNavePesada::clonar(){//Para los prototipos
    return new ClienteNavePesada();
}

void ClienteNavePesada::onInit(){ //override de ClienteRed
    if(nave)
        nave->setDeadRemoto();
}
void ClienteNavePesada::onDispose(){}; //override de ClienteRed
