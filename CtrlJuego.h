#ifndef CTRLJUEGO_H_
#define CTRLJUEGO_H_
#include <iostream>
#include <stdexcept>
#include <vector>
//references
#include "Modo.h"
#include "ModoJuego.h"
//#include "ModoMenu3D.h"
#include "MenuIntro.h"

#include "Nave.h"
#include "Engine.h"
#include "ControladorRed.h"
#include "Settings.h"
#include "Colisionable.h"
#include "SoundManager.h"

//namespace
using namespace std;
//class definition
class CtrlJuego{
private:
	static CtrlJuego*instance;
	CtrlJuego();
	Modo*modo;
public:
	static CtrlJuego*getInstance();
	virtual ~CtrlJuego();
	//operaciones
	void inicializar(int argc, char *argv[]);
	static void tic(void);
	//getters & setters
	Modo*getModo();
	void setModo(Modo*);

};



#endif /*CTRLJUEGO_H_*/
