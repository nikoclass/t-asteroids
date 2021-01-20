#include "Engine.h"

#include "ManejadorModelos.h"
#include "Mouse.h"
#include "Teclado.h"

Engine* Engine::instancia=NULL;

Engine* Engine::getInstancia(){
	if(!instancia) instancia=new Engine();
	return instancia;
}


Engine::Engine(){
	escena=new Escena();

	dt=0;
	tiempoTotal=0;
	tiempoReal=0;
	timescale=timescaleDeseado=1;
	dTimescale=0;
	maxFps=0;
}

Engine::~Engine(){

}

string Engine::getRuta(){
	return ruta;
}

void (*func)();


void Engine::inicializar(int argc, char* argv[],const char *nombre){

	char *temp=argv[0];
	bool linuxx;

	while(*temp){ //fuerzo a que solo sea para windows por cuestiones de debugger
        if(*temp=='/')
            *temp='\\';
        temp++;
	}
	string ruta=argv[0];

	int pos=ruta.rfind("\\",ruta.size())+1;
	linuxx = (pos==0);
	if(linuxx) //no encontro, busco con "/" (caso linux)
		pos=ruta.rfind("\\",ruta.size())+1;

	string s=ruta.substr(0,pos);

	ManejadorModelos::getInstancia()->setRuta(s+"Modelos"+(linuxx?"/":"\\"));
	ManejadorTexturas::getInstancia()->setRuta(s+"Texturas"+(linuxx?"/":"\\"));

	glutInit(&argc, argv);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow(nombre);

	glutFullScreen();

	glutReshapeFunc(Engine::resize);
	glutIdleFunc(Engine::idle);

	Mouse::getInstancia();
	Teclado::getInstancia();

	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth (1000000);

	glEnable(GL_BLEND);

	glCullFace(GL_BACK);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_NORMALIZE);

	///A considerar luego

	//glutTimerFunc...
	//glEnable(GL_FOG);
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH,GL_NICEST);

	InicializarSemilla(); //Para los numeros aleatorios
}

void Engine::loop(void (*funcion)()){
	func=funcion;
	glutDisplayFunc(display);
	glutMainLoop();
}

void Engine::setEscena(Escena* escena){
	this->escena=escena;
}
Escena* Engine::getEscena(){
	return escena;
}

double Engine::getDt(){
	return dt*timescale;
}
double Engine::getDtReal(){
	return dt;
}
double Engine::getTiempoTotal(){
	return tiempoTotal;
}
double Engine::getTiempoReal(){
	return tiempoReal;
}
double Engine::getTimeScale(){
	return timescale;
}
void Engine::setTimeScale(double timescale,double tr){
	if(this->timescale!=timescale){
		this->timescaleDeseado=( timescale < 0 )?0:timescale;
		if(tr>0) this->dTimescale=(this->timescaleDeseado-this->timescale)/tr;
		else this->timescale=this->timescaleDeseado;
	}
}
double Engine::getMaxFps(){
	return maxFps;
}
void Engine::setMaxFps(double max){
	maxFps=max;
}
double Engine::getFps(){
	if(dt>0)
		return 1.0/dt;
	else
		return 0;
}

void Engine::display(){

	clock_t t0=clock();

	// ### La funcion de la logica del juego ###
	//cout << "Engine::display() func(); begin" << endl;
	try {
        func();
	}
	catch(...){
        cout<<"Engine: Excepcion genérica producida en funcion de logica del juego"<<endl;
	}
	//cout << "Engine::display() func(); end" << endl;

	// ### Se dibujan las escenas aqui ###
	//cout << "Engine::display() instancia->escena->toGL(); begin" << endl;
	try {
        instancia->escena->toGL();
	}
	catch(...){
        cout<<"Engine: Excepcion genérica producida en renderizado de escena"<<endl;
	}
	//cout << "Engine::display() instancia->escena->toGL(); end" << endl;
	//----------------------------



	Teclado::getInstancia()->actualizar();
	Mouse::getInstancia()->actualizar();

	//timer, timescale, frames, etc
	clock_t t1=clock();

	double dt;
	dt=(t1-t0)/(double)CLOCKS_PER_SEC;

	//cout << "deltaT=" << dt << endl;

	//Limita los fps
	//if(instancia->maxFps>0){
	//	double d=1.0/instancia->maxFps - dt;
		//if(d>0) Sleep((int)(d*1000));
	//}

	t1=clock();
	instancia->dt=(t1-t0)/(double)CLOCKS_PER_SEC;
	instancia->tiempoReal+=instancia->dt;
	instancia->tiempoTotal+=(instancia->dt*instancia->timescale);

	if(instancia->timescale!=instancia->timescaleDeseado){

		instancia->timescale+=(instancia->dTimescale*instancia->dt);

		if(Sgn(instancia->dTimescale)==1 && (instancia->timescale > instancia->timescaleDeseado))
			instancia->timescale=instancia->timescaleDeseado;
		else if(Sgn(instancia->dTimescale)==-1 && (instancia->timescale < instancia->timescaleDeseado))
			instancia->timescale=instancia->timescaleDeseado;
		else if(Sgn(instancia->dTimescale)==0)
			instancia->timescale=instancia->timescaleDeseado;
	}
}

void Engine::resize(int width, int height){
	//Meter codigo apropiado
}

void Engine::idle(){
	glutPostRedisplay();
}

