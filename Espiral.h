#ifndef ESPIRAL_H_
#define ESPIRAL_H_

#include "Animable.h"
#include "Sprite.h"
#include "ModoJuego.h"

class Espiral : public Animable{

private:
	Sprite*es;
	float opacidad;
public:
	Espiral(Vector pos,float radio);
	virtual ~Espiral();
	void animar(float dt);
};

#endif /*EESPIRAL_H_*/
