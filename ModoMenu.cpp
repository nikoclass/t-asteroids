#include "ModoMenu.h"
#include "CtrlJuego.h"

ModoMenu::ModoMenu(){
    //cout << "modoMenu" << endl;
}

void ModoMenu::addEscenografia(bool withTitle){
	Engine *engine=Engine::getInstancia();

    this->universo=new Multiobjeto3d("universo.mdl");
	Objeto3d *o;
	o=((Objeto3d *)universo->getObjeto("frente"));
	o->cargarTextura("sp07_FT.tga");
	o->setModoTextura(Reemplazar);
	o->setAfectaDetrasCamara(false);

	o=((Objeto3d *)universo->getObjeto("abajo"));
	o->cargarTextura("sp07_DN.tga");
	o->setModoTextura(Reemplazar);
	o->setAfectaDetrasCamara(false);

	o=((Objeto3d *)universo->getObjeto("arriba"));
	o->cargarTextura("sp07_UP.tga");
	o->setModoTextura(Reemplazar);
	o->setAfectaDetrasCamara(false);

	o=((Objeto3d *)universo->getObjeto("atras"));
	o->cargarTextura("sp07_BK.tga");
	o->setModoTextura(Reemplazar);
	o->setAfectaDetrasCamara(false);

	o=((Objeto3d *)universo->getObjeto("derecha"));
	o->cargarTextura("sp07_RT.tga");
	o->setModoTextura(Reemplazar);
	o->setAfectaDetrasCamara(false);

	o=((Objeto3d *)universo->getObjeto("izquierda"));
	o->cargarTextura("sp07_LF.tga");
	o->setModoTextura(Reemplazar);
	o->setAfectaDetrasCamara(false);

	universo->escalar(10000);
	universo->setAfectaLuces(false);
	universo->setAfectaDetrasCamara(false);

    this->escena->addDibujable(universo);
    if(withTitle){
        this->titulo=new Objeto3d("Asteroids.mdl","envmapnave.tga");
        this->titulo->setModoGeneracionCoordTextura(Enviromental);
        this->titulo->setAfectaVista(false);
        this->titulo->setAfectaLuces(false);
        this->escena->addDibujable(titulo);
    }
    else this->titulo=NULL;
}

ModoMenu::~ModoMenu(){

}

void ModoMenu::addOption(Opcion* o){
    this->opciones.push_back(o);
}

void ModoMenu::tic(){
    mytic();
   	Engine *e=Engine::getInstancia();
   	if(this->titulo != NULL){
        this->titulo->identidad();
        this->titulo->posicionar(Vector(0,1,-2.0));
        this->titulo->rotar(cos(e->getTiempoReal())*0.2,Vector(0,1,0));
   	}
}

void ModoMenu::setPointerOption(int p){
    this->pointer_option = p;
}
