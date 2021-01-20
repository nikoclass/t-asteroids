
#include "Teclado.h"

Teclado* Teclado::instancia=NULL;

Teclado* Teclado::getInstancia(){
	if(!instancia) instancia=new Teclado;
	return instancia;
}

Teclado::Teclado(){
	this->_shift=false;
	this->_alt=false;
	this->_ctrol=false;
	this->key=0;

	for(int i=0;i<MAX_TECLAS;i++){
		this->presionadas[i]=false;
		this->special[i]=false;
	}


	//Registro de las funciones callback
	glutKeyboardFunc(Teclado::keyFunc);
	glutKeyboardUpFunc(Teclado::keyUpFunc);
	glutSpecialFunc(Teclado::specialFunc);
	glutSpecialUpFunc(Teclado::specialUpFunc);

	glutIgnoreKeyRepeat(true);
}

bool Teclado::shift(){
	return _shift;
}
bool Teclado::control(){
	return _ctrol;
}
bool Teclado::alt(){
	return _alt;
}
bool Teclado::tecla(int key){
	if(key>=0 && key<MAX_TECLAS) return this->presionadas[key];
	else {
		cout << "[ERROR] Teclado:: - tecla invalida: " << key << endl;
		return false;
	}
}

int Teclado::tecla(){
	return this->key;
}

void Teclado::keyFunc(unsigned char key,int x,int y){
	Teclado::instancia->presionadas[key]=true;
	Teclado::instancia->key=key;
	Teclado::teclasEspeciales();
}

void Teclado::keyUpFunc(unsigned char key, int x, int y){
	Teclado::instancia->presionadas[key]=false;
	Teclado::teclasEspeciales();
}


void Teclado::specialFunc(int key,int x,int y){
	Teclado::instancia->special[key]=true;
	Teclado::instancia->key=key;
	Teclado::teclasEspeciales();
}

void Teclado::specialUpFunc(int key,int x,int y){
	Teclado::instancia->special[key]=false;
	Teclado::teclasEspeciales();
}

void Teclado::teclasEspeciales(){
	Teclado::instancia->_shift=GLUT_ACTIVE_SHIFT & glutGetModifiers();
	Teclado::instancia->_alt=GLUT_ACTIVE_ALT & glutGetModifiers();
	Teclado::instancia->_ctrol=GLUT_ACTIVE_CTRL & glutGetModifiers();
}


//Engendro creado para evadir las deficiencias
//del manejador de eventos de windows
void Teclado::actualizar(){
	this->key=0;
	this->_shift=false;
	this->_alt=false;
	this->_ctrol=false;
}

void Teclado::setTecla(int key, bool value){
	if(key>=0 && key<MAX_TECLAS) this->presionadas[key]=value;
	else cout << "[ERROR] Teclado:: - tecla invalida: " << key << endl;
}
bool Teclado::spTecla(int key){
	if(key>=0 && key<MAX_TECLAS) return this->special[key];
	else {
		cout << "[ERROR] Teclado:: - tecla invalida: " << key << endl;
		return false;
	}
}
void Teclado::setSpTecla(int key, bool value){
	if(key>=0 && key<MAX_TECLAS) this->special[key]=value;
	else cout << "[ERROR] Teclado:: - tecla invalida: " << key << endl;
}

