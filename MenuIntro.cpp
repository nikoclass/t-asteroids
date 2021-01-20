#include "MenuIntro.h"

MenuIntro* MenuIntro::instance=NULL;

MenuIntro::MenuIntro(){
	//cout << "Menu1 cons" << endl;
	Engine *engine=Engine::getInstancia();
	Escena* esc = new Escena();
	this->escena = esc;
	engine->setEscena(esc);

	this->time = 30;

	this->camara = new Camara("principal");
	this->camara->setPosicion(Vector(0,0,-15));
	this->camara->mirarHacia(Vector(0,0,1.0));

	this->escena->addCamara(this->camara);
	//***********************************/

	this->addEscenografia(false);

	this->pointer_option=0;
	inicializarMenus();

	Texto2D *opcion,*opcion2;
	vector<Opcion*>col = this->opciones;

	for (int i=col.size()-1; i >= 0; i--){
		opcion=col[i]->getNombre();
		opcion->setOpacidad(0.7);
		opcion->posicionar(Vector(0,-15,0));
		this->escena->addDibujable(opcion);
	}
}

MenuIntro* MenuIntro::getInstance(){
	if(!MenuIntro::instance) MenuIntro::instance=new MenuIntro();
	return MenuIntro::instance;
}

MenuIntro::~MenuIntro(){}

void MenuIntro::inicializarMenus(){
	Settings*s=Settings::getInstance();

	Opcion* tex1 = new Opcion(0,new Texto2D("Un satelite de la Nasa enviado\nal espacio en el 2002 ha confirmado la\npresencia de una lluvia de\nasteroides que se acerca peligrosamente al\nplaneta Tierra y pone en riesgo\nla vida de nuestros habitantes.\n\n\nTu mision es salvar a la humanidad\nhe impedir que los asteroides choquen\ncontra nuestro planeta y acaben con\nla vida en ella.\n\n\nAdelante, comienza tu aventura...",0.7,0.7,"charset3.tga"),NULL);
	tex1->getNombre()->setMirarCamara(false);
	tex1->getNombre()->rotar(PI,Vector(0,1,0));
	tex1->getNombre()->rotar(PI/6,Vector(1,0,0));
	addOption(tex1);
    this->opciones[this->pointer_option]->getNombre()->escalar(1.1);
}

void MenuIntro::mytic(void){

	//Mouse::getInstancia()->setCursor(GLUT_CURSOR_DESTROY);
	Engine *e=Engine::getInstancia();
	e->setEscena(this->escena);
	Teclado*t=Teclado::getInstancia();
	double dt = Engine::getInstancia()->getDt();

	// EL ENTER
	if (t->tecla(13)){
		t->setTecla(13,false);
        CtrlJuego::getInstance()->setModo(ModoMenu3D::getInstance());
        return;
	}
    if (t->tecla(27)){
		t->setTecla(27,false);
        CtrlJuego::getInstance()->setModo(ModoMenu3D::getInstance());
		return;
	}
	this->time-=dt;
	if(this->time<=0){
        CtrlJuego::getInstance()->setModo(ModoMenu3D::getInstance());
        return;
	}

	this->opciones[0]->getNombre()->trasladar(Vector(0,dt/2,dt/3));
	this->universo->rotar(dt/15,Vector(1,1,-1),false);
}
