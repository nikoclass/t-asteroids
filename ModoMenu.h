#ifndef MODOMENU_H_
#define MODOMENU_H_

/*
#include <iostream>
//references
#include "Teclado.h"
#include "Modo.h"
#include "ModoJuego.h"
#include "CtrlJuego.h"
#include "Texto2D.h"
#include "Opcion.h"

#include <vector>
#include <iostream>

using namespace std;
*/

#include "Modo.h"
#include "Camara.h"
#include "Escena.h"
#include "Texto2D.h"
#include "Engine.h"
#include "Opcion.h"
#include "Multiobjeto3d.h"

#include <vector>
#include <iostream>

using namespace std;

class ModoMenu : public Modo {
protected:
	static ModoMenu*instance;
	int pointer_option;
	Camara* camara;
	vector<Opcion*> opciones;
	Multiobjeto3d* universo;
	Objeto3d* titulo;
	Escena* escena;

public:
	ModoMenu();
	virtual ~ModoMenu();

    void addEscenografia(bool withTitle);
    virtual void inicializarMenus() = 0;
    void addOption(Opcion*);
	void tic(void);
	virtual void mytic() = 0;
	void setPointerOption(int p);
};

#endif /*MODOMENU_H_*/
