#ifndef BOLAEXPLOSIVA_H_
#define BOLAEXPLOSIVA_H_
class BolaExplosiva;

#include "ExplosionParticulas.h"
#include "OrbitaParticulas.h"

class Animable;
class Engine;
class Objeto3d;
class Vector;
class Luz;
class ModoJuego;

#define CANT_DIRS 10

class BolaExplosiva : public Animable {
private:
	Objeto3d*modelo;
	Sprite *explosion;
	Luz*luz;
	Vector dirs[CANT_DIRS];
	Vector pos;
	float retry;

public:
	BolaExplosiva(Vector posicion);
	virtual ~BolaExplosiva();
	void animar(float);
};

#include "Animable.h"
#include "Engine.h"
#include "Objeto3d.h"
#include "Vector.h"
#include "Luz.h"
#include "ModoJuego.h"

#endif /*BOLAEXPLOSIVA_H_*/
