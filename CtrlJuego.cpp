#include "CtrlJuego.h"

CtrlJuego* CtrlJuego::instance=NULL;

CtrlJuego* CtrlJuego::getInstance()
{
    if (!CtrlJuego::instance)CtrlJuego::instance=new CtrlJuego();
    return CtrlJuego::instance;
}
CtrlJuego::CtrlJuego()
{
    Settings::getInstance();  // Para forzar inicializacion de los settings
    this->modo = NULL;
}

CtrlJuego::~CtrlJuego(){}

void CtrlJuego::inicializar(int argc, char *argv[])
{
    Engine *engine=Engine::getInstancia();
    engine->inicializar(argc,argv,string("Asteroids 3D").data());
    //setModo(MenuIntro::getInstance());
    setModo(ModoMenu3D::getInstance());
    //throw logic_error("err");
    engine->loop(CtrlJuego::tic);
}

void CtrlJuego::tic(void)
{
    SoundManager::getInstance()->update();
    instance->getModo()->tic();
}

Modo* CtrlJuego::getModo()
{
    return this->modo;
}

void CtrlJuego::setModo(Modo*modo)
{
    if (this->modo)
        this->modo->Leave();
    this->modo=modo;
    this->modo->Enter();
}
