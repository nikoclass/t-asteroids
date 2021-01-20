#ifndef TECLADO_H
#define TECLADO_H

#include "GL.h"

#include <iostream>
#include <string>
using namespace std;


///Sigue el patron de diseño Singleton
class Teclado {
private:
	static Teclado *instancia;
	Teclado();
	static const int MAX_TECLAS=256;
	//Callbacks
	static void keyUpFunc(unsigned char key, int x, int y);
	static void keyFunc(unsigned char key, int x, int y);
	static void specialUpFunc(int key, int x, int y);
	static void specialFunc(int key, int x, int y);
	static void teclasEspeciales();

	bool _shift;
	bool _ctrol;
	bool _alt;
	int key;

	bool presionadas[MAX_TECLAS];
	bool special[MAX_TECLAS];

public:
	static Teclado* getInstancia();
	bool tecla(int key);
	void setTecla(int key, bool value);

	bool spTecla(int key);
	void setSpTecla(int key, bool value);

	int tecla();

	bool shift();
	bool control();
	bool alt();

	//Engendro creado para evadir las deficiencias
	//del manejador de eventos de windows
	void actualizar();
};

#endif
